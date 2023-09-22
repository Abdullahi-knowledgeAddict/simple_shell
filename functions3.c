#include "shell.h"

/**
 * prompt - displays prompt if necessary
 */

void prompt(void)
{
	if (isatty(0) == 1)
		printfd("$ ", 1);
}
