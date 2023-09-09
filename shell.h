#ifndef SHELL_H
#define SHELL_H
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

int executor(char **argv, char **env);
#endif
