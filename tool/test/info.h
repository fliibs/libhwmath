#ifndef info_h
#define info_h
#include <iostream>
#include <cstdarg>
#include <bitset>
#include <cstdio>
#include <boost/multiprecision/cpp_int.hpp>

namespace mp = boost::multiprecision;

#define mantissa_get_48_bit 0xffffffffffff
#define mantissa_get_50_bit 0x3ffffffffffff
#define mantissa_get_64_bit 0xffffffffffffffff

#define mantissa_75_bit_h   0x800
#define mantissa_48_bit_l   0x800000000000

#define mant_get_75_bit_h   0x7ff
#define mant_get_50_bit_l   0xfffc000000000000


class Info {
public:
    bool debug;
    void debug_printf(const char* cmd, ...);
    void print_cpp_int(const mp::cpp_int& num,const std::string& str);
    void print_array(int* array, int array_length);
};

#endif