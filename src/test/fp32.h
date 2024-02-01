#include <stdint.h>
#include <stdbool.h>
#include <iostream>
#include <bitset>
#include <random>
#include <limits>
#include <boost/multiprecision/cpp_int.hpp>
namespace mp = boost::multiprecision;

#ifndef fp32_h
#define fp32_h

#define mantissa_49_bit         0x1000000000000
#define mantissa_48_bit         0x800000000000
#define mantissa_sign_bit       0xffff000000000000
#define mantissa_47_bit         0x400000000000

#define mantissa_get_47_bit     0x7fffffffffff
#define mantissa_get_48_bit     0xffffffffffff
#define mantissa_get_71_bit     0x7fffffffffffffffff
#define mantissa_get_the_71_bit 0x400000000000000000

#define exponent_sign           0x200
#define exponent_num            0x1ff
#define mantissa_with_g_r       0x7fffffc00000              //47-23          
#define last_22_binary          0x03fffff
#define the_25_binary           0x1000000 
#define the_24_binary           0x0800000
#define the_23_binary           0x0400000

#define last_23_binary          0x07fffff
#define g_bit                   0x0800000
#define r_bit                   0x0400000


#define underflow_downrange ((static_cast<uint32_t>(1) << 32) - 127)
#define underflow_uprange ((static_cast<uint32_t>(1) << 32) - 1)
#define FP32_INF 0x7f800000 // Inf，指数为0xff，有效位为0
#define FP32_NAN 0xffc00000 // NaN，指数为0xff，有效位不为0
#define FP32_MAX 0x7f7fffff // 单精度浮点表示的最大值
#define FP32_REG_MIN 0x800000
extern uint32_t round_mode; 
class Fp32{
    public:
        bool     sign;
        uint32_t exponent;
        uint64_t mantissa;
        bool     debug;
        Fp32();
        Fp32(float x);
        operator float();
        operator uint32_t();
        uint32_t to_uint32();
        void print();
        Fp32 mul(const Fp32 &rhs);
        Fp32 add(const Fp32 &rhs);
        Fp32 fma(const Fp32 &a,const Fp32 &b,const Fp32 &c);
        Fp32 operator* (const Fp32 &rhs);
        Fp32 operator+ (const Fp32 &rhs);
        uint32_t get_the_zero_nums(uint64_t *mantissa_in);
        uint32_t detect_one(uint64_t *mantissa_in,uint32_t width);
        uint32_t get_the_zero_nums_add(uint64_t mantissa_in);
        uint32_t get_the_zero_nums_add_long(mp::cpp_int mantissa_in);
        uint32_t detect_one_long(mp::cpp_int *mantissa_in,uint32_t width);
        void debug_printf(const char* cmd, ...);
        uint64_t cpp_int_last_bit(const mp::cpp_int a,int cut_length);
        uint32_t cpp_int_cut_bit(const mp::cpp_int a, int cut_bit,int shift_bit)
};
#endif//FP32_H