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
/**
 * pathgen - generates a linked list of paths to find executables
 * @path: pointer to value of the PATH(environmental variable) name=value
 * @pathead: pointer to the head of the linked list
 *
 * Return: 0 on success and -1 on failure to generate the complete list
 */
int pathgen(char *path, pathMeta_t **pathead)
{
	pathMeta_t *copy;

	*pathead = malloc(sizeof(pathMeta_t));/*the first path*/
	if (*pathead == NULL)
		return (-1);
	copy = *pathead;
	copy->path = path;
	copy->len = 0;
	while (path[0])
	{
		if (path[0] == ':')/*establishing new nodes*/
		{
			copy->nextpath = malloc(sizeof(pathMeta_t));
			if (copy->nextpath == NULL)
				return (-1);/*NULL*/
			copy = copy->nextpath;
			copy->path = ++path;
			copy->len = 0;
		}
		else/*initializing present node*/
		{
			path++;
			copy->len++;
		}
	}
	copy->nextpath = NULL;
	return (0);
}

/**
 * pathdegen - free all allocated blocks for paths list
 * @pathead: pointer to the first node of the linked list
 */
void pathdegen(pathMeta_t **pathead)
{
	pathMeta_t *copy;

	copy = NULL;
	while (*pathead != NULL)
	{
		copy = (*pathead)->nextpath;
		free(*pathead);
		*pathead = copy;
	}
}

/**
 * findib - checks if a given command is inbuilt and executes it
 * @argv: argument vector of command line
 * @pathead: pointer to the address of the head of path linked list
 *
 * Return: 0 to indicate success and -1 if not found
 * -2 if matching funtion complains
 */
int findib(char **argv, pathMeta_t **pathead)
{
	size_t idx1, idx2, argc; /* 1 for array 2 for .cmd*/
	builtin_t cp;
	builtin_t build[] = {{"exit", ext}, {NULL, NULL}};

	cp = build[0];/*cp holds the array value keeping code short*/
	for (argc = 0; argv[argc] != NULL;)/*getting argc*/
		argc++;
	for (idx1 = idx2 = 0; cp.cmd && argv[0];)
	{
		if (cp.cmd[idx2] == argv[0][idx2])
		{/*comparing command and  cmd in the struct*/
			if (cp.cmd[idx2 + 1] == '\0' && argv[0][idx2 + 1] == '\0')
				return (cp.cmd_f(argc, argv, pathead));/*found a match*/
			idx2++;
		}
		else/*no match yet*/
		{
			idx2 = 0;
			cp = build[++idx1];
		}
	}
	return (-1);/*no match at all*/
}
