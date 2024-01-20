#include "test.h"
int get_length(uint64_t num){
    int length=0;
    while(num){
        num>>=1;
        ++length;
    }
    return length;
}

uint32_t get_last_22_bit(uint64_t num){
    uint32_t last_22_bit=num&0x3fffff;
    return last_22_bit;
}
