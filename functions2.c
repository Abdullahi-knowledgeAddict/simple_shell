#include "shell.h"
/**
 * _strdup - duplicates a string
 * @s: pointer to the string to be duplicated
 *
 * Return: pointer to duplicate on success, null on failure
 */
char *_strdup(char *s)
{
	char *duplicate;
	int len;
	for (len = 0; s[len];)
		len++;
	duplicate = malloc(sizeof(char) * (len + 1));
	while (duplicate != NULL && --len >= 0)
		duplicate[len] = s[len];
	return (duplicate);
}
