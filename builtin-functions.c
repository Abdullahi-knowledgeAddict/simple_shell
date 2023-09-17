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
	(void)argc;
/*freeing malloc'ed spaces before existing*/
	free(argv[0]);
	free(argv);
	pathdegen(pathead);
	_exit(0);
}
