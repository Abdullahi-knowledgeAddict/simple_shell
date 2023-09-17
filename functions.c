#include "shell.h"

/**
 * executor - of the shell's command line arguement
 * @argv: an array containing the tokens of the command line (command, args)
 * @env: array of environmental variables.
 * @paths: pointer to linked list of paths, value of environmental paths
 *
 * Return: 0 indicating that the command exits, successful fork, -1 otherwise
 */
int executor(char **argv, char **env, pathMeta_t *paths)
{
	pid_t pid;
	int status;
	char *path;

	path = NULL;
	/*checking if argv[0] is NULL to stop execution*/
	if (!argv[0])
		return (0);
	path = pathfinder(argv, paths);
	if (path)/*check for commands existence*/
	{
		pid = fork();/*duplicating parent process*/
		if (pid == 0)/* ensuring only child process*/
		{
			/*Handling execution failure*/
			if (execve(path, argv, env) == -1)
				perror("command execution failed\n");
			exit(-1);
		}
		else if (pid > 0)/*making parent process wait*/
			wait(&status);
		else /*what happens when process duplication fails*/
		{
			perror("calling process was not duplicated\n");
			return (-1);
		}
		free(path);
	}
	else/* if command doesn't exist*/
	{
		perror(argv[0]);
	}
	return (0);
}
/**
 * comligner - aligns a command line to rm unnecessary spaces ahead of it
 * @lineptr: a pointer to command line (string)
 *
 * Return: the numb of memory to store the address of each word to make tokens
 */
int comligner(char **lineptr)
{
	int len;
	size_t arg_len;

	arg_len = 1;/*counted for first arg and NULL*/
	len = 0;
	while ((*lineptr)[len] != '\0')
	{
		if (**lineptr < '!')/*eliminate unnecessary spaces in front*/
			(*lineptr)++;
		else if (**lineptr >= '!' && len == 0)
		{
			arg_len++;
			len++;
		}
		else
		{/*len only start incrementing at this stage*/
			if ((*lineptr)[len] == 32 || (*lineptr)[len] == 9)
				(*lineptr)[len + 1] >= '!' ? arg_len++ : arg_len;
			len++;
		}
	}
	return (arg_len);
}

/**
 * tokenizer - takes a buffer container a command line and tokenizes it
 * @lineptr: the buffer container the command line
 *
 * Return: a buffer container the address of each token
 */
char **tokenizer(char *lineptr)
{/* 9 = ' ', 32 = '	', 10 = '\n', 0 = '\0'*/
	int argc, andex, lndex;
	char **argv, *c;

	argc = comligner(&lineptr);/* align command and get req spac for args*/
	argv = malloc(sizeof(char *) * argc);
	if (argv == NULL)/* error handling*/
		perror(NULL);
	for (andex = lndex = 0; lineptr[lndex] != '\0' && argv; lndex++)
	{
		c = lineptr + lndex;/* use c to shorten code using char addr*/
		if (lndex == 0)/* first arg*//* assigning args to argv*/
			argv[andex++] = c;
		if (*c >= '!')/*seperate each word in lineptr*/
		{
			if (*(c + 1) == 9 || *(c + 1) == 32 || *(c + 1) == 10)
			{
				*(c + 1) = 0;/*seperating with '\0'*/
				lndex++;
				if (*(c + 2) >= '!')
				{/* when a char is after the space*/
					++lndex;
					argv[andex++] = lineptr + lndex;
				}
			}
		}
		else if (*c == 32 || *c == 9)
		{/*remaining args*/
			if (*(c + 1) == 10)
			{/*taking care of newline*/
				*(c + 1) = 0;
				lndex++;
			}
			else if (*(c + 1) >= '!')
				argv[andex++] = c + 1;
		}
	}
	argv[andex] = NULL;/*adding NULL at the end of argv*/
	return (argv);
}

/**
 * _getenv - Searches the environmental variable for
 * @name: variable name
 *
 * Return: a pointer to the corresponding value or NULL if it doesn't exist
 */
char *_getenv(char *name)
{
	char **ev;
	int sd;/* sd-> string index*/

	sd = 0;
	ev = environ;/* global variable in the header file */
	while (*ev)
	{
		if (name[sd] == (*ev)[sd])
		{
			if (name[sd + 1] == '\0' && (*ev)[sd + 1] == '=')
				return ((*ev) + sd + 2);
			sd++;
		}
		else
		{
			ev++;
			sd = 0;
		}
	}
	return (*ev);
}
/**
 * pathfinder - find the specific path to command or confirm it existence
 * @cmd: The first element of argv (command)
 * @pathead: linked list of paths in PATH
 *
 * Return: pointer to the command directory or NULL if not found
 */
char *pathfinder(char **cmd, pathMeta_t *pathead)
{
	unsigned short cml, lpatl, index, index1;
	pathMeta_t *copy;
	char *cmdpath;
	struct stat st;

	if (cmd[0][0] == '/' || cmd[0][0] == '.')/*checking cmd with abs path*/
		return (stat(cmd[0], &st) == 0 ? cmd[0] : NULL);
	cmdpath = NULL;/*taking care of cmds^ already specified with abs path*/
	copy = pathead;
	for (lpatl = cml = 0; cmd[0][cml] || copy;)
	{/*getting longest path length*/
		if (copy)
		{/*if the new len > oldlen assign new*/
			(copy->len > lpatl) ? lpatl = copy->len : 0;
			copy = copy->nextpath;
		}
		cmd[0][cml] ? cml++ : 0;/*getting cmd length*/
	} cmdpath = malloc(sizeof(char) * (lpatl + cml + 1)); /*reserve space*/
	if (cmdpath)/*one more for '/' to be appended ^ after path*/
	{/*Entered only cmdpath is not NULL*/
		for (index = index1 = 0; pathead; index++)
		{/*now writing full path into cmdpath*/
			(index == pathead->len) ? cmdpath[index++] = '/' : 0;
			if (pathead->len > index)
				cmdpath[index] = pathead->path[index];
			else if (index <= (pathead->len + cml))/*the cmd*/
				cmdpath[index] = cmd[0][index1++];
			else
			{
				cmdpath[index] = cmd[0][index1];
				if (!(stat(cmdpath, &st)))
					break;
				index = -1;/*index++ makes it zero*/
				index1 = 0;
				pathead = pathead->nextpath;
			}
		}
	}
	/* now returning cmdpath if pathead not NULL confirming path found*/
	return (pathead ? cmdpath : NULL);
}
