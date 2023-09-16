#ifndef SHELL_H
#define SHELL_H
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

/**
 * struct pathMeta - a structure to store path and it length
 * @path: A path to were executable can be found on the system
 * @len: The length of the path
 * @nextpath: a pointer to the node containing another pathMeta
 */
typedef struct pathMeta
{
	unsigned short len;
	char *path;
	struct pathMeta *nextpath;
} pathMeta_t;

extern char **environ;
int executor(char **argv, char **env, pathMeta_t *paths);
char **tokenizer(char *lineptr);
int comligner(char **lineptr);
char *_getenv(char *value);
char *pathfinder(char **cmd, pathMeta_t *pathead);
char *_strdup(char *s);
int pathgen(char *path, pathMeta_t **pathead);
void pathdegen(pathMeta_t *pathead);
#endif
