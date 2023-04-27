#include "shell.h"

int status;

char *SMshell;

/**
 * cmd_manager - manages the process a command goes through to get executed
 * @args: command and arguments
 *
 * Return: TRUE if success, FALSE if failure
 */
int cmd_manager(char **args)
{
	char **args_ptr = args;
	int prev_eval = NETHR;
	int no_err = TRUE;
	char prev_op = 'c';
	char next_op = 'c';
	int what_do;

	while (*args != NULL && prev_eval != EXT_SHELL)
	{
		while (*args_ptr != NULL && **args_ptr != '&'
		       && **args_ptr != '|')
			args_ptr++;

		if (str_comp(*args_ptr, "||", MARK) == TRUE)
		{
			*args_ptr = NULL;
			args_ptr++;
			next_op = '|';
		}
		if (str_comp(*args_ptr, "&&", MARK) == TRUE)
		{
			*args_ptr = NULL;
			args_ptr++;
			next_op = '&';
		}
		if (next_op == 'c')
			break;

		prev_eval = andor(args, prev_op, prev_eval);
		if (prev_eval == FALSE)
			no_err = FALSE;
		prev_op = next_op;
		args = args_ptr;
	}

	if (next_op == 'c')
	{
		what_do = execute_cmd(args);

		if (what_do == EXT_SHELL)
			return (EXT_SHELL);
	}

	if (no_err == FALSE || what_do == FALSE)
		return (FALSE);

	return (TRUE);
}

/**
 * builtins - checks if a command is a built in
 * @args: command and arguments
 *
 * Return: SKP_FORK if built in, _EXECVE if not a built in, EXT_SHELL if
 * exit shell, EXT_SHELL_CODE if exiting with a particular code
 */
int builtins(char **args)
{
	char **args_ptr = args;
	int i;

	while (*args_ptr != NULL)
	{
		if (**args_ptr == '#')
		{
			*args_ptr = NULL;
			break;
		}
		*args_ptr = var_check(*args_ptr);

		args_ptr++;
	}
	if (*args == NULL)
		return (SKP_FORK);

	i = alias_fun(args, FALSE);
	if (i == _EXECVE || i == SKP_FORK)
		return (i);

	if (str_comp("exit", *args, MARK) == TRUE && args[1] != NULL)
	{
		status = _atoi(args[1]);
		if (status < 0)
		{
			status = 2;
			err_msg(args[0], args[1]);
			return (SKP_FORK);
		}
	}
	if (str_comp("exit", *args, MARK) == TRUE)
		return (EXT_SHELL);
	else if (str_comp("setenv", *args, MARK) == TRUE && args[1] != NULL)
		return (_setenv(args[1], args[2]));
	else if (str_comp("unsetenv", *args, MARK) == TRUE
		 && args[1] != NULL)
		return (_unsetenv(args[1]));
	else if (str_comp("cd", *args, MARK) == TRUE)
		return (change_directory(args[1]));
	else if (str_comp("env", *args, MARK) == TRUE)
		return (print_environ());

	return (_EXECVE);
}

/**
 * andor - deals with command line logical operators
 * @args: command and arguments
 * @operator: first char of logical operator
 * @last_compare: if last command in logic evaluated to true or false
 *
 * Return: if this command evaluates to true or false
 */
int andor(char **args, char operator, int last_compare)
{
	int i;

	if (last_compare == NETHR)
	{
		i = execute_cmd(args);
		if (i == EXT_SHELL)
			return (EXT_SHELL);
		if (i == TRUE)
			return (TRUE);

		return (FALSE);
	}
	if (last_compare == TRUE && operator == '&')
	{
		i = execute_cmd(args);
		if (i == EXT_SHELL)
			return (EXT_SHELL);
		if (i == TRUE)
			return (TRUE);

		return (FALSE);
	}

	if (last_compare == FALSE && operator == '|')
	{
		i = execute_cmd(args);
		if (i == EXT_SHELL)
			return (EXT_SHELL);
		if (i == TRUE)
			return (TRUE);

		return (FALSE);
	}

	if (last_compare == TRUE && operator == '|')
		return (TRUE);

	return (FALSE);
}

/**
 * check_cmd - checks if a non-built-in exists
 * @args: argument and commands
 *
 * Return: TRUE if valid command, FALSE if not
 */
char *check_cmd(char **args)
{
	char *command_buf;
	char *full_buf;
	char *path_str = NULL;
	char *path_ptr;
	char *path_tmp;
	char **path_var;
	char **path_var_ptr;

	if (access(*args, X_OK) == 0)
		return (_strdup(*args));

	if (get_arr_element(environ, "PATH") != NULL)
		path_str = _strdup(get_arr_element(environ, "PATH") + 5);

	path_ptr = path_str;

	if (path_str != NULL)
	{
		if (*path_str == ':')
		{
			command_buf = str_concat("./", *args);
			if (access(command_buf, X_OK) == 0)
			{
				free(path_str);
				return (command_buf);
			}
			else
			{
				free(command_buf);
				path_ptr = _strdup(path_str + 1);
				free(path_str);
				path_str = path_ptr;
			}
		}
		while (*path_ptr != '\0')
		{
			if (*path_ptr == ':' && *(path_ptr + 1) == ':')
			{
				*(path_ptr + 1) = '\0';
				path_tmp = (str_concat(path_str, ".:"));
				path_ptr = str_concat(path_tmp, path_ptr + 2);
				free(path_str);
				free(path_tmp);
				path_str = path_ptr;
			}
			path_ptr++;
		}
	}

	path_var = make_arr(path_str, ':', NULL);
	path_var_ptr = path_var;

	command_buf = str_concat("/", *args);

	full_buf = _strdup(command_buf);

	while (*path_var_ptr != NULL && access(full_buf, X_OK) != 0)
	{
		free(full_buf);
		full_buf = str_concat(*path_var_ptr, command_buf);
		path_var_ptr++;
	}

	free(command_buf);
	free(path_str);
	free(path_var);

	if (access(full_buf, X_OK) != 0)
	{
		err_msg(args[0], NULL);
		free(full_buf);
		return (NULL);
	}

	return (full_buf);
}

/**
 * execute_cmd - executes a command
 * @args: command and arguments
 *
 * Return: TRUE or EXT_SHELL
 */
int execute_cmd(char **args)
{
	char *buf_ptr = *args;
	char *command_name;
	pid_t pid;
	int what_do = builtins(args);

	if (what_do == _EXECVE)
	{
		command_name = check_cmd(args);
		if (command_name == NULL)
			return (FALSE);

		pid = fork();
		if (pid == -1)
		{
			exit(EXT_SHELL);
		}
		if (pid == 0)
		{
			execve(command_name, args, environ);
			exit(EXT_SHELL);
		}
		wait(&status);
		free(command_name);
		fflush(stdin);
	}

	if (str_comp("false", *args, MARK) == TRUE)
		status = 1;

	if (*args != buf_ptr)
		free(*args);
	args++;

	while (*args != NULL)
	{
		while (*buf_ptr != '\0')
		{
			buf_ptr++;
		}
		buf_ptr++;

		if (*args != buf_ptr)
			free(*args);

		args++;
	}

	if (what_do == EXT_SHELL)
		return (EXT_SHELL);

	if (status != 0)
		return (FALSE);

	return (TRUE);
}
