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
	char *lineptr;

	if (argc > 1)/* giving shell ability to  interprete command line argument*/
	{
		executor(&argv[1], env);
		return (0);
	}

	buff_size = 0;
	lineptr = NULL;/*space to keep command line args*/
	write(STDOUT_FILENO, "$ ", 2);/*first prompt*/
	while ((line_length = getline(&lineptr, &buff_size, stdin)) != -1)
	{
		if (line_length > 1)
			argv = tokenizer(lineptr);
		if (argv != NULL)
			executor(argv, env);
		write(STDOUT_FILENO, "$ ", 2);
		free(argv);/*freeing argv*/
	}
	free(lineptr);/*freeing lineptr*/
	return (0);
}
