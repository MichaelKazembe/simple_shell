#include "shell.h"

/**
 * locate_arr_element - retrieves the specified element from an arr
 *
 * @arr: pointer to the arr to search
 * @el_name: pointer to the name of the element to locate
 *
 * This function searches the given arr for an element with the specified name.
 *
 * Return: pointer to the element if found, or NULL if it is not found in the arr
 */
char *get_arr_element(char **arr, char *el_name)
{
	while (*arr != NULL)
	{
		if (str_comp(*arr, el_name, PRFX) == TRUE)
			return (*arr);

		arr++;
	}

	return (NULL);
}

/**
 * make_arr - creates a list from a buffer string
 *
 * @str: pointer to the buffer string to parse
 * @delim: the delimiter character that separates list entries
 * @ifsep: pointer to update with location after delimiter, or NULL
 *
 * Returns pointer to resulting list, or NULL if allocation fails
 */
char **make_arr(char *str, char delim, char **ifsep)
{
	char *str_ptr = str;
	unsigned int i = 2;
	char **arr = NULL;

	while (*str_ptr != '\0')
	{
		if (*str_ptr == ';')
			break;
		if (*str_ptr == delim && *(str_ptr + 1) != '\0')
			i++;

		str_ptr++;
	}

	arr = malloc(i * sizeof(char **));
	if (arr == NULL)
		exit(EXIT_FAILURE);

	arr[0] = str;
	str_ptr = str;
	i = 1;

	while (*str_ptr != '\0')
	{
		if (*str_ptr == delim)
		{
			*str_ptr = '\0';
			str_ptr++;
			if (*str_ptr == ';')
			{
				arr[i] = NULL;
				if (*(str_ptr + 1) != '\0' && *(str_ptr + 2) != '\0')
					*ifsep = str_ptr + 2;
				break;
			}
			if (*str_ptr != '\0')
			{
				arr[i] = str_ptr;
				i++;
			}
		}
		str_ptr++;
	}
	arr[i] = NULL;

	return (arr);
}

/**
 * list_length - returns length or index of a list intro
 *
 * @list: pointer to the list to evaluate
 * @intro: pointer to the intro to index
 *
 * Returns length or index of the intro if successful, or -1 if failed
 */
int list_length(char **list, char *intro)
{
	int i = 0;

	if (intro == NULL)
	{
		while (*list != NULL)
		{
			i++;
			list++;
		}
		i++;
		return (i);
	}
	else
	{
		while (*list != NULL)
		{
			if (str_comp(*list, intro, PRFX) == TRUE)
				return (i);

			i++;
			list++;
		}
	}

	return (-1);
}

/**
 * arr_cpy - creates a copy of an arr
 *
 * @old_arr: pointer to arr to copy
 * @new_s: size of new arr
 *
 * Returns pointer to new arr, or NULL if allocation fails
 */
char **arr_cpy(char **old_arr, int new_s)
{
	char **new_arr = NULL;
	char **ptr_arr;

	new_arr = malloc(sizeof(char **) * new_s);

	ptr_arr = new_arr;
	while (*old_arr != NULL)
	{
		*ptr_arr = _strdup(*old_arr);
		ptr_arr++;
		old_arr++;
	}
	*ptr_arr = NULL;

	return (new_arr);
}

/**
 * free_arr - frees a 2D arr
 *
 * @args: pointer to the arr to free
 *
 * Frees the memory allocated for the given 2D arr.
 *
 * Returns: TRUE
 */
int free_arr(char **args)
{
	char **ptr = args;

	while (*ptr != NULL)
	{
		free(*ptr);
		ptr++;
	}

	free(args);

	return (TRUE);
}
