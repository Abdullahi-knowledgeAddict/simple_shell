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

/**
 * struct builtin - a data containing, builtin command with their function
 * @cmd: the commmand
 * @cmd_f: the function that executes the command
 */
typedef struct builtin
{
	char *cmd;
	int (*cmd_f)(size_t argc, char **argv, pathMeta_t **paths);
} builtin_t; /*type to declare this struct type*/
extern char **environ;
int executor(char **argv, char **env, pathMeta_t *paths);
char **tokenizer(char *lineptr);
int comligner(char **lineptr);
char *_getenv(char *value);
char *pathfinder(char **cmd, pathMeta_t *pathead);
char *_strdup(char *s);
int pathgen(char *path, pathMeta_t **pathead);
void pathdegen(pathMeta_t **pathead);
/* functions implementing shell builtin commands*/
int findib(char **argv, pathMeta_t **pathead);
int ext(size_t argc, char **argv, pathMeta_t **pathead);
int cdr(size_t argc, char **argv, pathMeta_t **pathead);
void printfd(char *s, int fd);
int printenv(size_t argc, char **argv, pathMeta_t **pathead);
#endif
