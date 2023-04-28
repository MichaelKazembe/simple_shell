#include "shell.h"


/**
 * sanitize_input - Cleanses input from the command line to prevent unwanted behavior.
 *
 * @param old_buf Pointer to the buffer to be cleansed.
 * @param old_size Size of the original buffer.
 *
 * @return A new buffer containing the sanitized input.
 */
char *input_sanitizer(char *old_buf, size_t *old_size)
{
	char *new_buf = malloc(*old_size * 3);
	char *new_ptr = new_buf;
	char *old_ptr = old_buf;

	while (*old_ptr != '\0')
	{
		while (*old_ptr == ' ')
			old_ptr++;
		while (*old_ptr	!= ' ' && *old_ptr != ';' && *old_ptr != '|'
		       && *old_ptr != '&' && *old_ptr != '\0')
		{
			*new_ptr = *old_ptr;
			new_ptr++;
			old_ptr++;
		}
		while (*old_ptr == ' ')
			old_ptr++;
		if (new_ptr != new_buf && *old_ptr != '\0')
		{
			*new_ptr = ' ';
			new_ptr++;
		}

		if (*old_ptr == ';' || *old_ptr == '|' || *old_ptr == '&')
		{
			if (input_err_checking(old_ptr) == FALSE)
			{
				*old_size = 0;
				break;
			}
			*new_ptr = *old_ptr;
			if (*old_ptr == ';')
			{
				new_ptr++;
				*new_ptr = ' ';
			}
			else if (*(old_ptr + 1) == *old_ptr)
			{
				if (new_ptr == new_buf)
				{
					err_msg(old_ptr, NULL);
					*old_size = 0;
					break;
				}
				new_ptr++;
				*new_ptr = *old_ptr;
				new_ptr++;
				*new_ptr = ' ';
				old_ptr++;
			}
			new_ptr++;
			old_ptr++;
		}
	}
	*new_ptr = '\0';
	free(old_buf);
	return (new_buf);
}

/**
 * input_err_checking - A helper function for the input sanitizer that checks for unexpected characters.
 *
 * @param ptr Pointer to the area that needs to be checked.
 *
 * @return TRUE if there are no errors, FALSE if there are errors.
 */
int input_err_checking(char *ptr)
{
	char *iter = ptr;

	iter++;
	if (*ptr == ';' && *iter == *ptr)
	{
		err_msg(ptr, NULL);
		return (FALSE);
	}
	if (*iter == *ptr)
		iter++;

	while (*iter == ' ')
		iter++;

	if (*iter == ';' || *iter == '|' || *iter == '&')
	{
		err_msg(iter, NULL);
		return (FALSE);
	}

	return (TRUE);
}

/**
 * err_msg -  Prints an error message and sets the status accordingly.
 *
 * @param arg0 The command that is causing the error.
 * @param arg1 The first argument to the command.
 */
void err_msg(char *arg0, char *arg1)
{
	char *err_str_num = ito_str(line_nm);

	write(STDERR_FILENO, SMshell, _strlen(SMshell));
	write(STDERR_FILENO, ": ", 2);
	write(STDERR_FILENO, err_str_num, _strlen(err_str_num));
	free(err_str_num);

	if (str_comp("cd", arg0, MARK) == TRUE)
	{
		status = 2;
		write(STDERR_FILENO, ": cd: can't cd to", 17);
		write(STDERR_FILENO, arg1, _strlen(arg1));
		write(STDERR_FILENO, "\n", 1);
		return;
	}

	if (str_comp("exit", arg0, MARK) == TRUE)
	{
		write(STDERR_FILENO, ": exit: Illegal number: ", 24);
		write(STDERR_FILENO, arg1, _strlen(arg1));
		write(STDERR_FILENO, "\n", 1);
		return;
	}
	if (*arg0 == ';' || *arg0 == '|' || *arg0 == '&')
	{
		status = 2;
		write(STDERR_FILENO, ": Syntax error: \"", 17);
		write(STDERR_FILENO, arg0, 1);
		if (*arg0 == *(arg0 + 1))
			write(STDERR_FILENO, arg0, 1);
		write(STDERR_FILENO, "\" unexpected\n", 14);
		return;
	}

	status = 127;
	write(STDERR_FILENO, ": ", 2);
	write(STDERR_FILENO, arg0, _strlen(arg0));
	write(STDERR_FILENO, ": not found\n", 12);
}

/**
 * check_for_vars - Checks if the argument is a variable and if so, replaces it with its value.
 *
 * @param arg The argument to be checked.
 *
 * @return A pointer to the evaluated argument (either new or an altered clone).
 */
char *var_check(char *arg)
{
	char *clone = NULL;
	char *ptr = arg;
	char *next;
	char *tmp;
	int is_var;
	int i;

	while (*ptr != '\0')
	{
		if (*ptr == '$')
		{
			if (clone == NULL)
			{
				clone = _strdup(arg);
				i = ptr - arg;
				ptr = clone + i;
			}
			next = ptr + 1;
			while (*next != '\0' && *next != '$' && *next != '#')
				next++;
			if (*next == '$' && next > ptr + 1)
				is_var = TRUE;
			else if (*next == '#')
				is_var = NETHR;
			else
				is_var = FALSE;

			*next = '\0';

			if (str_comp("$?", ptr, MARK) == TRUE)
				tmp = ito_str(status);
			else if (str_comp("$0", ptr, MARK) == TRUE)
				tmp = _strdup(SMshell);
			else if (get_arr_element(environ, ptr + 1) != NULL)
				tmp = _strdup(get_arr_element
					      (environ, ptr + 1)
					      + _strlen(ptr));
			else
				tmp = _strdup("");

			*ptr = '\0';
			ptr = str_concat(clone, tmp);
			free(tmp);
			if (is_var == FALSE)
			{
				free(clone);
				clone = ptr;
				break;
			}
			if (is_var == TRUE)
				*next = '$';
			else if (is_var == NETHR)
				*next = '#';
			tmp = str_concat(ptr, next);
			free(ptr);
			ptr = tmp;
			free(clone);
			clone = ptr;
			if (is_var == NETHR)
			{
				while (*ptr != '#')
					ptr++;
			}
		}
		ptr++;
	}
	if (clone != NULL)
		return (clone);

	return (arg);
}
