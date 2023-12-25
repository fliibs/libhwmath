#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <iostream>
#include <bitset>
#include <random>
#include <limits>
#include "fp32.h"

#ifndef calculate_h
#define calculate_h

class Calculate{
    public:
    void     generate_s(uint32_t* current_s, uint64_t* input_num,int loop);
    bool     is_zero(uint32_t num);
    bool     is_inf(uint32_t num);
    bool     is_nan(uint32_t num);
    bool     is_special(uint32_t num);
    uint64_t get_last_n_bit(uint64_t num,int n);
    uint64_t get_the_n_bit(uint64_t num,int n);

};

#endif//calculate_h