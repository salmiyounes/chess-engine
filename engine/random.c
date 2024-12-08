#include "random.h"

uint64_t keys[2];

inline  uint64_t rotate(uint64_t v, uint8_t s) {
    return (v >> s) | (v << (64 - s));
}

inline uint64_t radnom_64_int() {
    uint64_t tmp = keys[0];
    keys[0] += rotate(keys[1] ^ 0xc5462216u ^ ((uint64_t) 0xcf14f4ebu << 32), 1);
    return keys[1] + rotate(tmp ^ 0x75ecfc58u ^ ((uint64_t) 0x9576080cu << 32), 9);
}

void seed_random(uint64_t seed) {
    keys[0] = keys[1] = seed;

    for (int i = 0; i < 64; i++) {
        radnom_64_int();
    }
}

uint64_t random_magic() {
    return radnom_64_int() & radnom_64_int() & radnom_64_int();
}