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
/**
 * _setenv - Initialize a new environment variable, or modify an existing one
 * @argv: pointer the command line argument containing VARIABLE & VALUE
 *
 * Return: zero on success && print something on stderr on failure
 */
int _setenv(size_t argc, char **argv, pathMeta_t **pathead)
{
	char **copenv, *copyarg, *copyarg2;
	size_t var_len, tot_len;
	(void)pathead;

	if (argc != 3)
	{
		printfd("usage: setenv VARIABLE VALUE\n", 2);
		return (-1);
	}
	copyarg = argv[1];
	for (var_len = tot_len =  0; copyarg[var_len]; var_len++)
	{/*loop to get length*/
		if (!(copyarg[var_len + 1]) && copyarg != argv[2])
		{
			var_len++;/*using '\0' space for '=' in env*/
			copyarg = argv[2];
			tot_len += var_len;
			var_len = -1;/*for next '++' to make it zero*/
		}
	}
	tot_len += var_len;
	copyarg = malloc(sizeof(char) * tot_len);
	if (copyarg == NULL)
	{
		printfd("Space could not be allocated for new env\n", 2);
		return (-1);
	}
	copyarg[tot_len - 1] = '\0';
	copyarg2 = argv[1];
	for (tot_len = var_len = 0; copyarg2[var_len]; tot_len++, var_len++)
	{/*loop synthesizes the variable*/
		copyarg[tot_len] = copyarg2[var_len];
		if (!(copyarg2[var_len + 1]) && copyarg2 == argv[1])
		{
			copyarg[++tot_len] = '=';
			var_len = -1;
			copyarg2 = argv[2];
		}
	}
	copenv = _getenv(argv[1]);/*get the address of the Variable*/
	*copenv == NULL ? *(copenv + 1) = NULL : 0;
	*copenv = copyarg;/*modify or and environmental variable*/
	return (0);
}
/**
 * _unsetenv - removes an environmental variable
 * @name: the name of the variable
 *
 * Return: zero on success -1 otherwise
 */
int _unsetenv(size_t argc, char **argv, pathMeta_t **pathead)
{
	(void) pathead;
	char **copenv;
	int idx;

	if (argc != 2)
	{
		printfd("Usage: usetenv var\n", 2);
		return (-1);
	}
	copenv = _getenv(argv[1]);
	if (*copenv == NULL)
	{
		printfd("The variable doesn't exit\n", 2);
		return (-1);
	}
	idx = 0;
	while (copenv[idx])
	{
		copenv[idx] = copenv[idx + 1];
		idx++;
	}
	return (0);
}
