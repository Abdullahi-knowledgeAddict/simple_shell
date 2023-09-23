#include "shell.h"

/**
 * ext - causes shell to terminate normally.
 * @argv: vector containing exit command, and arguements
 * @argc: contains the number of element in argv
 * @pathead: address of the pointer to the head of a linked list
 *
 * Return: 0, But -2 to signifying failure
 */
int ext(size_t argc, char **argv, pathMeta_t **pathead)
{
	unsigned long n;
	int idx;

	 idx = n = 0;
/*freeing malloc'ed spaces before existing*/
	if (argc == 1)
	{
		free(argv[0]);
		free(argv);
		pathdegen(pathead);
		_exit(0);
	}
	else if (argc > 1)
	{
		while (argv[1][idx])/*extracting the status, from argv[1]*/
		{
			if (argv[1][idx] >= 48 && argv[1][idx] <= 57)
			{
				n *= 10;
				n += argv[1][idx] - '0';
			}
			else
			{
				printfd(argv[0], STDERR_FILENO);
				printfd(": ", STDERR_FILENO);
				printfd(argv[1], STDERR_FILENO);
				printfd(" : ", STDERR_FILENO);
				printfd("Illegal Argument\n", STDERR_FILENO);
				return (0);
			}
			idx++;
		}
		free(argv[0]);
		free(argv);
		pathdegen(pathead);
		_exit(n & 0xFF);
	}
	return (0);
}

/**
 * printenv - prints the current environment
 * @argv: Not used by this function.
 * @argc: Not used by this function.
 * @pathead: Not used by this function.
 *
 * Return: zero (0) indicating success
 */
int printenv(size_t argc, char **argv, pathMeta_t **pathead)
{
	char **copyenv;
	(void)argc;
	(void)argv;
	(void)pathead;

	copyenv = environ;
	while (*copyenv)
	{
		printfd(*copyenv, 1);
		printfd("\n", 1);
		copyenv++;
	}
	return (0);

}
