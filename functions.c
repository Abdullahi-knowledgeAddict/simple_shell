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
	if (argv[0] == NULL)
		return (-1);
	path = pathfinder(argv, paths);
	if (path)/*check for commands existence*/
	{
		pid = fork();/*duplicating parent process*/
		if (pid == 0)/* ensuring only child process*/
		{
			/*Handling execution failure*/
			if (execve(path, argv, env) == -1)
				printfd("command exec failed\n", 2);
			exit(-1);
		}
		else if (pid > 0)/*making parent process wait*/
		{
			wait(&status);
			if (path != argv[0])
				free(path);/*prevent 2x free*/
			path = NULL;/*since, a function exist to do ^ it*/
		}
		else /*what happens when process duplication fails*/
		{
			printfd("calling process was not duplicated\n", 2);
			return (-1);
		}
	}
	else/* if command doesn't exist*/
		perror(argv[0]);
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
 * @lptr: the address buffer container the command line
 *
 * Return: a buffer containing the address of each token
 */
char **tokenizer(char **lptr)
{
	char **argv, *toks, *chr;
	size_t argc, tok_len, idx, ctok_len;

	for (argc = comligner(lptr), tok_len = idx = 0; (*lptr)[idx]; idx++)
	{/*getting  length and removing unnecessary spaces*/
		chr = *lptr + idx;
		if (chr[0] >= '!')
		{
			tok_len++;
			(chr[1] == 32 || chr[1] == 9) ? tok_len++ : 0;
		}
	} /*now allocating spaces*/
	argv = malloc(sizeof(char *) * (argc + 1));
	toks = malloc(sizeof(char) * tok_len);
	if (argv == NULL || toks == NULL)
	{
		free(argv);
		free(toks);
		printfd("command could not be tokenized\n", 2);
		return (NULL);
	}
	ctok_len = tok_len;/*copy tok_len*/
	for (tok_len = idx = 0; (*lptr)[idx]; idx++)
	{
		chr = *lptr + idx;/*holds index address*/
		if (chr[0] >= '!')
		{
			toks[tok_len++] = chr[0];
			if (chr[1] == 32 || chr[1] == 9 || chr[1] == 10)
				toks[tok_len++] = '\0';
		}
	}
	free(*lptr);	/*free the now dormant lineptr*/
	argv[0] = *lptr = toks;	/*initializing argv*/
	for (tok_len = 1, idx = 0; idx < ctok_len; idx++)
	{
		if (toks[idx] == 0 && toks[idx + 1] != 0)
			argv[tok_len++] = toks + idx + 1;
	}
	argv[tok_len] = NULL;
	return (argv);
}
/**
 * _getenv - Searches the environmental variable for
 * @name: variable name
 *
 * Return: a pointer to the corresponding variable or NULL if it doesn't exist
 */
char **_getenv(char *name)
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
				return (ev);
			sd++;
		}
		else
		{
			ev++;
			sd = 0;
		}
	}
	return (ev);
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
