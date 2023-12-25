#include <stdint.h>
#include <stdbool.h>
#include <iostream>
#include <bitset>
#include <random>
#include <limits>

#ifndef fp32_h
#define fp32_h

#define underflow_downrange ((static_cast<uint32_t>(1) << 32) - 127)
#define underflow_uprange ((static_cast<uint32_t>(1) << 32) - 1)
#define FP32_INF 0x7f800000 // Inf，指数为0xff，有效位为0
#define FP32_NAN 0xffc00000 // NaN，指数为0xff，有效位不为0
#define FP32_MAX 0x7f7fffff // 单精度浮点表示的最大值
#define FP32_REG_MIN 0x800000
// #define FP32_Whole_MAX 0x7fffffff
// #define FP32_WithoutSign_MAX 0x3fffffff
class Fp32{
    public:
        bool     sign;
        uint32_t exponent;
        uint64_t mantissa;
        Fp32();
        Fp32(float x);
        operator float();
        operator uint32_t();
        uint32_t to_uint32();
        void print();
        Fp32 mul(const Fp32 &rhs);
        Fp32 operator* (const Fp32 &rhs);
        uint64_t mantissa_msb(uint64_t input_man);
        void normalization(uint32_t* exponent,uint64_t* mantissa,uint32_t* s);
        void round_to_the_negative(bool* sign,uint32_t* exponent,uint64_t* mantissa,uint32_t g,uint32_t r,uint32_t s);
        void round_to_the_positive(bool* sign,uint32_t* exponent,uint64_t* mantissa,uint32_t g,uint32_t r,uint32_t s);
        void round_to_the_nearest(bool* sign,uint32_t* exponent,uint64_t* mantissa,uint32_t g,uint32_t r,uint32_t s);
        void round_to_the_zero(bool* sign,uint32_t* exponent,uint64_t* mantissa,uint32_t g,uint32_t r,uint32_t s);

};
#endif//FP32_H