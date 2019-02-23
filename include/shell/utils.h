#ifndef _SHELL_UTILS_H
#define _SHELL_UTILS_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void shell_utils_cutstr(char *str, size_t n);

size_t getline(char **lineptr, size_t *n, FILE *stream);

#endif