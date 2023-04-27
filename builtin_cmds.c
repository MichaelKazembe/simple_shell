#include "shell.h"

int status;

/**
 * _setenv - sets and environmental variable
 * @name: name of the variable
 * @value: value to set the variable to
 *
 * Return: 0 on success
 */
int _setenv(const char *name, const char *value)
{
	char **new_environ;
	char *buffer;
	char *buf_tmp;
	char *element_ptr = get_arr_element(environ, (char *) name);
	int len;

	if (value == NULL)
	{
		write(STDERR_FILENO, "setenv: no value given\n", 23);
		status = 2;
		return (SKP_FORK);
	}

	buffer = str_concat((char *)name, "=");
	buf_tmp = str_concat(buffer, (char *)value);
	free(buffer);
	buffer = buf_tmp;

	if (element_ptr == NULL)
	{
		len = list_length(environ, NULL);
		new_environ = arr_cpy(environ, len + 1);
		new_environ[len - 1] = buffer;
		new_environ[len] = NULL;
		free_arr(environ);
		environ = new_environ;
		return (SKP_FORK);
	}

	len = list_length(environ, (char *)name);
	free(environ[len]);
	environ[len] = buffer;

	status = 0;

	return (SKP_FORK);
}

/**
 * _unsetenv - deletes an environmental variable
 * @name: name of variable
 *
 * Return: 0 if successful
 */
int _unsetenv(const char *name)
{
	char **env_ptr;
	int len = list_length(environ, (char *)name);

	if (len == -1)
	{
		write(STDERR_FILENO, "unsetenv: variable not found\n", 29);
		status = 2;
		return (SKP_FORK);
	}

	env_ptr = environ + len;
	free(*env_ptr);
	while (*(env_ptr + 1) != NULL)
	{
		*env_ptr = *(env_ptr + 1);
		env_ptr++;
	}
	*env_ptr = NULL;
	status = 0;

	return (SKP_FORK);
}

/**
 * change_directory - changes the current working directory
 * @name: name of directory to change to
 *
 * Return: 0 if successful
 */
int change_directory(char *name)
{
	char *home;
	char *pwd;
	char path_buffer[PATH_MAX];
	size_t buf_size = PATH_MAX;
	int i;

	getcwd(path_buffer, buf_size);

	if (name == NULL || str_comp("~", name, PRFX) == TRUE
	    || str_comp("$HOME", name, MARK) == TRUE)
	{
		if (name != NULL && *name == '~' && *(name + 1) != '\0'
		    && *(name + 1) != '/')
		{
			status = 2;
			err_msg("cd", name);
			return (SKP_FORK);
		}

		home = get_arr_element(environ, "HOME");
		if (home == NULL)
		{
			status = 2;
			err_msg("cd", name);
			return (SKP_FORK);
		}

		while (*home != '=')
			home++;

		home++;
		i = chdir((const char *)home);
		if (i != -1)
			_setenv("PWD", (const char *)home);

		if (name != NULL && str_comp("~/", name, PRFX) == TRUE)
			name += 2;
	}
	if (str_comp("-", name, MARK) == TRUE)
	{
		pwd = get_arr_element(environ, "OLDPWD");
		if (pwd == NULL)
			return (2);

		while (*pwd != '=')
		{
			pwd++;
		}
		pwd++;

		i = chdir((const char *)pwd);
		if (i != -1)
		{
			write(STDOUT_FILENO, pwd, _strlen(pwd));
			write(STDOUT_FILENO, "\n", 1);
			_setenv("PWD", (const char *)pwd);
		}
	}
	if (name != NULL && str_comp("~", name, MARK) == FALSE
	    && str_comp("$HOME", name, MARK) == FALSE && i != -1
	    && *name != '\0' && str_comp("-", name, MARK) != TRUE)
	{
		i = chdir((const char *)name);
		if (i != -1)
			_setenv("PWD", (const char *)name);
	}
	if (i == -1)
	{
		status = 2;
		err_msg("cd", name);
		return (SKP_FORK);
	}

	status = 0;
	_setenv("OLDPWD", (const char *)path_buffer);

	return (SKP_FORK);
}

/**
 * alias_fun - deals with command aliases
 * @args: arguments from command line
 * @to_free: indicated if aliases need to be freed (exiting shell);
 *
 * Return: TRUE if exiting, FALSE if the command is not "alias" or an alias,
 * SKP_FORK if success
 */
int alias_fun(char **args, int to_free)
{
	static alias head = {NULL, NULL, NULL};
	char *char_ptr;
	int no_error = TRUE;

	if (to_free == TRUE)
		return (free_alias(head.next));

	if (str_comp("alias", *args, MARK) != TRUE)
		return (if_alias(args, head.next));

	args++;

	if (*args == NULL)
		return (alias_print(head.next));

	while (*args != NULL)
	{
		char_ptr = *args;
		while (*char_ptr != '\0' && *char_ptr != '=')
			char_ptr++;

		if (*char_ptr == '\0' || char_ptr == *args)
		{
			if (alias_value_print(*args, &head) == FALSE)
				no_error = FALSE;
		}
		else
		{
			*char_ptr = '\0';
			char_ptr++;
			alias_value_set(*args, &head, char_ptr);
			*(char_ptr - 1) = '=';
		}
		args++;
	}

	if (no_error == FALSE)
		return (SKP_FORK);

	status = 0;
	return (SKP_FORK);
}

/**
 * print_environ - prints the environment
 *
 * Return: TRUE
 */
int print_environ(void)
{
	char **ptr = environ;

	while (*ptr != NULL)
	{
		write(STDOUT_FILENO, *ptr, _strlen(*ptr));
		write(STDOUT_FILENO, "\n", 1);
		ptr++;
	}

	status = 0;

	return (SKP_FORK);
}
