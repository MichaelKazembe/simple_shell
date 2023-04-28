#include "shell.h"

/**
 * _strlen - Computes the length of a given string.
 *
 * @param str The string to be evaluated.
 *
 * @return The length of the string.
 */
int _strlen(char *str)
{
	int i = 0;

	if (str == NULL)
		return (0);

	while (*str != '\0')
	{
		i++;
		str++;
	}

	return (i);
}

/**
 * _strdup - Allocates memory for a new string and copies the contents of the given string into it.
 *
 * @param src The string to be duplicated.
 *
 * @return A pointer to the newly allocated memory, or NULL if there was a failure.
 */
char *_strdup(char *src)
{
	int len = _strlen(src);
	char *dest = malloc(len + 1);
	char *ptr;

	if (dest == NULL)
		exit(EXIT_FAILURE);

	ptr = dest;

	while (*src != '\0')
	{
		*ptr = *src;
		ptr++;
		src++;
	}

	*ptr = '\0';

	return (dest);
}

/**
 * str_concat - Concatenates two strings into a new, dynamically-allocated string.
 *
 * @param s1 The first string.
 * @param s2 The second string.
 *
 * @return A pointer to the newly-allocated string, or NULL if there was a failure.
 */
char *str_concat(char *s1, char *s2)
{
	int len = _strlen(s1) + _strlen(s2);
	char *dest = malloc(len + 1);
	char *ptr = dest;

	if (s1 != NULL)
	{
		while (*s1 != '\0')
		{
			*ptr = *s1;
			ptr++;
			s1++;
		}
	}

	if (s2 != NULL)
	{
		while (*s2 != '\0')
		{
			*ptr = *s2;
			ptr++;
			s2++;
		}
	}

	*ptr = '\0';

	return (dest);
}

/**
 * str_comp - Compares two strings based on a specified match preference.
 *
 * @param s1 The first string to compare.
 * @param s2 The second string to compare.
 * @param pref_or_match A flag indicating if the strings should be compared exactly
 *                      or if only a prefix match is required.
 *
 * @return An integer indicating the difference between the two strings.
 *         A negative value indicates that s1 is less than s2.
 *         A positive value indicates that s1 is greater than s2.
 *         A value of 0 indicates that the strings are equal.
 */
int str_comp(char *s1, char *s2, int pref_or_match)
{
	if (s1 == NULL || s2 == NULL)
		return (FALSE);

	while (*s1 != '\0' && *s2 != '\0')
	{
		if (*s1 != *s2)
			return (FALSE);

		s1++;
		s2++;
	}

	if (pref_or_match == PRFX)
		return (TRUE);

	if (*s1 == *s2)
		return (TRUE);

	return (FALSE);
}
