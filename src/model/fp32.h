
#ifndef fp32_h
#define fp32_h

#include <stdint.h>
#include <stdbool.h>
#include <iostream>
#include <random>
#include <limits>
#include <cmath>
#include "../test/debug.h"

#define mantissa_64_bit         0x8000000000000000
#define mantissa_50_bit         0x2000000000000
#define mantissa_49_bit         0x1000000000000
#define mantissa_48_bit         0x800000000000
#define mantissa_sign_bit       0xffff000000000000
#define mantissa_47_bit         0x400000000000
#define mantissa_75_bit         0x8000000000000000000
#define mantissa_26_bit         0x400000000

#define mantissa_get_64_bit     0xffffffffffffffff
#define mantissa_get_50_bit     0x3ffffffffffff
#define mantissa_get_47_bit     0x7fffffffffff
#define mantissa_get_48_bit     0xffffffffffff

#define mantissa_l_75           0x800  //the upper 64 bits of mantissa

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

#define BIT_CAL(function_name,cal_eq)   \
    void Fp32::function_name(int array_length,mp::cpp_int input_a,mp::cpp_int input_b,int * out){ \
        for (int i = 0; i < array_length; i++) \
        { \
            int a_last  =   static_cast<int>(input_a&0x01); \
            int b_last  =   static_cast<int>(input_b&0x01); \
            out[i]      =   cal_eq; \   
            input_a     =   input_a>>1; \
            input_b     =   input_b>>1; \ 
        } \
    }
class Fp32{
    public:
        bool       sign;
        uint32_t   exponent;
        uint64_t   mantissa;
        uint32_t   round_mode;
        Debug      D1;
        Fp32();
        Fp32(float x);
        operator float();
        operator uint32_t();
        uint32_t to_uint32();
        void print();
        //fp32 c model
        Fp32 mul(const Fp32 &a,const Fp32 &rhs,const Fp32 &dont_care);
        Fp32 add(const Fp32 &a,const Fp32 &rhs,const Fp32 &dont_care);
        Fp32 fma(const Fp32 &a,const Fp32 &b,  const Fp32 &c);
        //algri
        uint32_t get_the_zero_nums(uint64_t *mantissa_in);
        uint32_t detect_one(uint64_t *mantissa_in,uint32_t width);
        uint32_t get_the_zero_nums_add(uint64_t mantissa_in);
        uint32_t get_the_zero_nums_add_long(mp::cpp_int mantissa_in);
        uint32_t detect_one_long(mp::cpp_int *mantissa_in,uint32_t width);
        // void debug_printf(const char* cmd, ...);
        uint64_t cpp_int_last_bit(const mp::cpp_int a,int cut_length);
        uint32_t cpp_int_cut_bit(const mp::cpp_int a, int cut_bit,int shift_bit);
        void freeArray(int *arr);
        void get_p(int array_length,mp::cpp_int input_a,mp::cpp_int input_b,int * out);
        void get_g(int array_length,mp::cpp_int input_a,mp::cpp_int input_b,int * out);
        void get_a(int array_length,mp::cpp_int input_a,mp::cpp_int input_b,int * out);
        uint32_t loa(int array_length,mp::cpp_int input_a,mp::cpp_int input_b);
        // void print_cpp_int(const mp::cpp_int& num,const std::string& str);
};

#endif//FP32_H