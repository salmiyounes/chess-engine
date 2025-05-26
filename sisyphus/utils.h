#ifndef UTILS_H
#define UTILS_H

#include "types.h"
#include <stdlib.h>
#include <string.h>

void swap_any(void *a, void *b, size_t s);

char *strdup(const char *src);

bb xorshift64();
#endif