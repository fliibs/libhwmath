#include <stdint.h>
#include <cstdio>
#include <iostream>
#include <stdbool.h>
#include <cstdarg>
#include <bitset>
#ifndef test_h
#define test_h
int get_length(uint64_t num);

uint32_t get_last_22_bit(uint64_t num);

void debug_printf(const char* cmd, ...);

#endif