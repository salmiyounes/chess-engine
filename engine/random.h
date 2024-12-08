#ifndef RANDOM_H
#define RANDOM_H

#include <inttypes.h>

uint64_t rotate(uint64_t v, uint8_t s);
uint64_t radnom_64_int();
void seed_random(uint64_t seed);
uint64_t random_magic();

#endif