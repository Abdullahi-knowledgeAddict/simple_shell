#include "shell.h"

/**
 * executor - of the shell's command line arguement
 * @argv: an array containing the tokens of the command line (command, args)
 * @env: array of environmental variables.
 *
 * Return: 0 indicating that the command exits, successful fork, -1 otherwise
 */
int executor(char **argv, char **env)
{
	pid_t pid;
	int status, errno __attribute__((unused));
	struct stat st;

	if (stat(argv[0], &st) == 0)/*check for commands existence*/
	{
		pid = fork();/*duplicating parent process*/
		if (pid == 0)/* ensuring only child process executes command*/
		{
			/*Handling execution failure*/
			if (execve(argv[0], argv, env) == -1)
				perror("command execution failed\n");
			exit(98);
		}
		else if (pid > 0)/*making parent process wait*/
			wait(&status);
		else /*what happens when process duplication fails*/
		{
			perror("calling process could not be duplicated\n");
			exit(99);
		}
	}
	else/* if command doesn't exist*/
	{
		perror(argv[0]);
	}
	return (0);
}

