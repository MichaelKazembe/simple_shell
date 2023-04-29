#include "shell.h"

/**
 * locate_array_element - retrieves the specified element from an array
 *
 * @array: pointer to the array to search
 * @element_name: pointer to the name of the element to locate
 *
 * This function searches the given array for an element with the specified name.
 *
 * Return: pointer to the element if found, or NULL if it is not found in the array
 */
char *get_array_element(char **array, char *element_name)
{
	while (*array != NULL)
	{
		if (str_comp(*array, element_name, PRFX) == TRUE)
			return (*array);

		array++;
	}

	return (NULL);
}

/**
 * make_array - creates a list from a buffer string
 *
 * @str: pointer to the buffer string to parse
 * @delim: the delimiter character that separates list entries
 * @ifsep: pointer to update with location after delimiter, or NULL
 *
 * Returns pointer to resulting list, or NULL if allocation fails
 */
char **make_array(char *str, char delim, char **ifsep)
{
	char *str_ptr = str;
	unsigned int i = 2;
	char **array = NULL;

	while (*str_ptr != '\0')
	{
		if (*str_ptr == ';')
			break;
		if (*str_ptr == delim && *(str_ptr + 1) != '\0')
			i++;

		str_ptr++;
	}

	array = malloc(i * sizeof(char **));
	if (array == NULL)
		exit(EXIT_FAILURE);

	array[0] = str;
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
				array[i] = NULL;
				if (*(str_ptr + 1) != '\0' && *(str_ptr + 2) != '\0')
					*ifsep = str_ptr + 2;
				break;
			}
			if (*str_ptr != '\0')
			{
				array[i] = str_ptr;
				i++;
			}
		}
		str_ptr++;
	}
	array[i] = NULL;

	return (array);
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
 * array_cpy - creates a copy of an array
 *
 * @old_array: pointer to array to copy
 * @new_s: size of new array
 *
 * Returns pointer to new array, or NULL if allocation fails
 */
char **array_cpy(char **old_array, int new_s)
{
	char **new_array = NULL;
	char **ptr_array;

	new_array = malloc(sizeof(char **) * new_s);

	ptr_array = new_array;
	while (*old_array != NULL)
	{
		*ptr_array = _strdup(*old_array);
		ptr_array++;
		old_array++;
	}
	*ptr_array = NULL;

	return (new_array);
}

/**
 * free_array - frees a 2D array
 *
 * @args: pointer to the array to free
 *
 * Frees the memory allocated for the given 2D array.
 *
 * Returns: TRUE
 */
int free_array(char **args)
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
