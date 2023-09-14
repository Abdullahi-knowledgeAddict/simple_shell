#ifndef SHELL_H
#define SHELL_H
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
extern char **environ;
int executor(char **argv, char **env);
char **tokenizer(char *lineptr);
int comligner(char **lineptr);
char *_getenv(char *value);
char *pathfinder(char *cmd);
char *_strdup(char *s);
#endif
