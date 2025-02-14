#ifndef TABLE_H
#define TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "types.h"
#include "board.h"
#include "move.h"

#define	EXACT 1
#define ALPHA 2
#define BETA  3

int table_alloc(Table *table, 
                int bits);

void table_free(Table *table);

Entry *table_entry(Table *table, 
                   bb key);

Move table_get_move(Table *table, 
                    bb key);

void table_set_move(Table *table, 
                    bb key, 
                    int depth, 
                    Move move);

void table_set(Table *table, 
               bb key, 
               int depth, 
               int value, 
               int flag);

int table_get(Table *table, 
              bb key, 
              int depth, 
              int alpha, 
              int beta, 
              int *value);

#endif