#include "shell.h"

/**
 * main - entry point for the command line interpreter
 * @argc: number of command line argument
 * @argv: array containing command and arguments
 * @env: environmental variable
 *
 * Return: zero (0) to indicate success
 */

int main(int argc, char **argv, char **env)
{
	int line_length;
	size_t buff_size;
	char *lineptr, *PATH;
	pathMeta_t *paths;

	paths = NULL;
	PATH = _getenv("PATH");
	if (pathgen(PATH, &paths))
	{/*error handling PATH list generation*/
		perror("unable to generate PATH list\n");
		return (-1);
	}
	if (argc > 1)
	{/* giving shell ability to interprete command line args*/
	/*cmd index is 1 in non interactive mode, and the shell is argv[0]*/
		executor(&argv[1], env, paths);
		pathdegen(paths);
		return (0);
	}

	buff_size = 0;
	lineptr = NULL;/*space to keep command line args*/
	write(STDOUT_FILENO, "$ ", 2);/*first prompt*/
	while ((line_length = getline(&lineptr, &buff_size, stdin)) >= 0)
	{
		if (line_length > 1)
			argv = tokenizer(lineptr);
		if (argv != NULL)
			executor(argv, env, paths);
		write(STDOUT_FILENO, "$ ", 2);
		free(argv);/*freeing argv*/
	}
	free(lineptr);/*freeing lineptr*/
	pathdegen(paths);
	return (0);
}
