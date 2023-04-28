#include "shell.h"


/**
 * free_alias - frees all aliases
 *
 * @alias_ptr: pointer to the head of the alias list
 *
 * Frees all aliases in the list starting from `alias_ptr`.
 *
 * Returns: TRUE
 */
int free_alias(alias *alias_ptr)
{
	alias *tmp;

	while (alias_ptr != NULL)
	{
		tmp = alias_ptr;
		alias_ptr = alias_ptr->next;
		free(tmp->name);
		free(tmp->value);
		free(tmp);
	}

	return (TRUE);
}

/**
 * if_alias - checks if a command is an alias and replaces it with its value
 *
 * @args: argument to be checked
 * @alias_ptr: pointer to the head of the alias list
 *
 * If the command in `args` is an alias, replaces it with its value
 * from the list starting from `alias_ptr`.
 *
 * Returns: TRUE
 */
int if_alias(char **args, alias *alias_ptr)
{
	while (alias_ptr != NULL)
	{
		if (str_comp(*args, alias_ptr->name, MARK) == TRUE)
		{
			*args = _strdup(alias_ptr->value);
			return (_EXECVE);
		}
		alias_ptr = alias_ptr->next;
	}
	return (TRUE);
}

/**
 * alias_print - prints all aliases in list
 *
 * @alias_ptr: pointer to the head of the alias list
 *
 * Prints all the aliases in the list starting from `alias_ptr`.
 *
 * Returns: SKP_FORK
 */
int alias_print(alias *alias_ptr)
{
	while (alias_ptr != NULL)
	{
		write(STDOUT_FILENO, alias_ptr->name, _strlen(alias_ptr->name));
		write(STDOUT_FILENO, "=\'", 2);
		write(STDOUT_FILENO, alias_ptr->value,
		      _strlen(alias_ptr->value));
		write(STDOUT_FILENO, "\'\n", 2);
		alias_ptr = alias_ptr->next;
	}
	return (SKP_FORK);
}

/**
 * alias_value_print - prints the value of a particular alias
 *
 * @arg: name of the alias
 * @alias_ptr: pointer to the head of the alias list
 *
 * Searches for the alias with the specified name in the list starting from
 * `alias_ptr`, and prints its value to stdout if found.
 *
 * Returns: TRUE if the alias is found, FALSE otherwise.
 */
int alias_value_print(char *arg, alias *alias_ptr)
{
	while (alias_ptr != NULL)
	{
		fflush(stdin);
		if (str_comp(arg, alias_ptr->name, MARK) == TRUE)
		{
			write(STDOUT_FILENO, arg, _strlen(arg));
			write(STDOUT_FILENO, "=\'", 2);
			write(STDOUT_FILENO, alias_ptr->value,
			      _strlen(alias_ptr->value));
			write(STDOUT_FILENO, "\'\n", 2);
			return (TRUE);
		}
		alias_ptr = alias_ptr->next;
	}

	status = 1;
	write(STDERR_FILENO, "alias: ", 7);
	write(STDERR_FILENO, arg, _strlen(arg));
	write(STDERR_FILENO, " not found\n", 11);

	return (FALSE);
}

/**

*Initializes or resets an alias value.
*@param alias_ptr Pointer to the alias list.
*@param arg Name of the alias.
*@param new_value Value of the alias to set.
*@return TRUE if the alias is successfully initialized or reset.
*/
int alias_value_set(char *arg, alias *alias_ptr, char *new_value)
{
	while (alias_ptr->next != NULL
	       && str_comp(alias_ptr->name, arg, MARK) != TRUE)
	{
		alias_ptr = alias_ptr->next;
	}

	if (str_comp(alias_ptr->name, arg, MARK) == TRUE)
	{
		free(alias_ptr->value);
	}
	else
	{
		alias_ptr->next = malloc(sizeof(alias *));
		alias_ptr = alias_ptr->next;
		if (alias_ptr == NULL)
			exit(EXIT_FAILURE);

		alias_ptr->name = _strdup(arg);
		alias_ptr->next = NULL;
	}
	alias_ptr->value = _strdup(new_value);

	return (TRUE);
}
