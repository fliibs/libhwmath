#include "fp32.h"
#include <iostream>
#include <bitset>
#include <random>
#include <limits>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <cstdint>
#include <fenv.h>
#include <cstdarg>
#include <cstdio>

#ifndef test_times
    #define test_times 100000
#endif

#ifndef special_test_time
    #define special_test_time 100000
#endif

#ifndef NEGATIVE_ENABLE
    #define NEGATIVE_ENABLE 0
#endif

#ifndef POSITIVE_ENABLE
    #define POSITIVE_ENABLE 0
#endif

#ifndef NEAREST_ENABLE
    #define NEAREST_ENABLE 0
#endif

#ifndef ZERO_ENABLE
    #define ZERO_ENABLE 0
#endif

#ifndef TEST_MONTE
    #define TEST_MONTE 0
#endif

#ifndef DEBUG_ENABLE
    #define DEBUG_ENABLE 0
#endif

#ifndef TEST_DEBUG
    #define TEST_DEBUG 0
#endif

class Tester {
public:
    int test_once(float num1,float num2);
    int test_special_values();
    int test_zeros();
    int test_infs();
    int test_zero_times_inf();
    int test_one_nan();
    int test_two_nan();
    float get_rand_float();
    float get_accurate_float(const Fp32 input);
    uint32_t get_rand_uint32();
    float get_rand_nan();
    int test_a_regular_num(float a,float b);
    int test_regular_num();

};

float Tester::get_rand_float() {
    float res;
    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_int_distribution<uint32_t> distr(std::numeric_limits<uint32_t>::min(), std::numeric_limits<uint32_t>::max());
    uint32_t tmp1 = distr(eng);
    res = *(float*)&tmp1;
    return res;
}

uint32_t Tester::get_rand_uint32(){
    uint32_t res;
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
        return static_cast<uint32_t>(rand());
}

int Tester::test_once(float num1,float num2) {
    // Implementation of the testing logic
            int fail=0;
            float res = num1*num2;
            Fp32 hw_num1(num1);
            Fp32 hw_num2(num2);
            uint32_t tmp_num1=hw_num1.to_uint32();
            uint32_t tmp_num2=hw_num2.to_uint32();
            Fp32 hw_res;
            hw_res = hw_num1 * hw_num2;
            uint32_t tmp1 = *(uint32_t*)&res;
            uint32_t tmp2 = hw_res.to_uint32();
            if(tmp1!=tmp2){
                printf("------input num--------\n");
                printf("num1=%lf\n",num1); 
                hw_num1.print();
                printf("num2=%lf\n",num2);
                hw_num2.print();
                printf("------output num--------\n");
                printf("D::uint32_t result is %d\n",tmp1);
                printf("D::my mul result is %d\n",tmp2);
                printf("B::uint32_t result is %b\n",tmp1);
                printf("B::my mul result is   %b\n",tmp2);
                fail=1;
            }
    return fail;
}

int Tester::test_special_values(){
        // Implementation of the testing logic
        test_zeros();
        test_infs();
        test_zero_times_inf();
        test_one_nan();
        test_two_nan();
    return 0;
}

float Tester::get_rand_nan(){
    uint32_t rand_mantissa=get_rand_uint32();
    uint32_t intValue=0xff<<23|0x400000|(rand_mantissa&0x3fffff);
    float floatvalue;
    std::memcpy(&floatvalue, &intValue, sizeof(float));
    return floatvalue;
}

float Tester::get_accurate_float(const Fp32 input){
    uint32_t intValue=(uint32_t(input.sign<<31))|(uint32_t(input.exponent)<<23)|(uint32_t(input.mantissa&0x7fffff));

    float floatvalue;
    std::memcpy(&floatvalue, &intValue, sizeof(float));
    return floatvalue;
}

int Tester::test_zeros(){
    float num1;
    float num2;
    int fail;
    //-------test zeros times any regular num
    printf("-------test zeros times any regular num-----\n");
    fail=0;
    num2= 00.0000;
    for (int i = 0; i < special_test_time; i++)
    {
        num1=get_rand_float();
        fail=test_once(num1,num2)?1:fail;
    }
    num1 = -00.0000;
    for (int i = 0; i < special_test_time; i++)
    {
        num2=get_rand_float();
        fail=test_once(num1,num2)?1:fail;
    }
    if(!fail){
        printf("zero times any regular pass!\n");
    }
    else{
        printf("zero times any regular fail!\n");
    }
    return 0;
}

int Tester::test_infs(){
        float num1;
        float num2;
        int fail;
        //-------test inf times any regular num
        printf("-------test inf times any regular num-----\n");
        fail=0;
        num2=INFINITY;
        for (int i = 0; i < special_test_time; i++)
        {
            num1=get_rand_float();
            fail=test_once(num1,num2)?1:fail;    
        }
        num1=-INFINITY;
        for (int i = 0; i < special_test_time; i++)
        {
            num2=get_rand_float();
            fail=test_once(num1,num2)?1:fail;    
        }
        if(!fail){
            printf("inf times any regular pass!\n");
        }
        else{
            printf("inf times any regular fail!\n");
        }
        return 0;
}

int Tester::test_zero_times_inf(){
        float num1;
        float num2;
        int fail;
        //-------test zero times inf
        printf("-------test zero times inf-----\n");
        fail=0;
        num2=INFINITY;
        num1=00.0000;
        fail=test_once(num1,num2)?1:fail;    
        num2=INFINITY;
        num1=-00.0000;
        fail=test_once(num1,num2)?1:fail;    
        num2=00.0000;
        num1=-INFINITY;
        fail=test_once(num1,num2)?1:fail;    
        num2=-00.0000;
        num1=-INFINITY;
        fail=test_once(num1,num2)?1:fail;    
        if(!fail){
            printf("zero times inf pass!\n");
        }
        else{
            printf("zero times inf fail!\n");
        }
        return 0;
        }

int Tester::test_one_nan(){
    float num1;
    float num2;
    int fail;
    //-------test nan times any regular num
    printf("-------test nan times any regular num-----\n");
    fail=0;
    for (int i = 0; i < special_test_time; i++)
    {
        num2=get_rand_nan();
        num1=get_rand_float();
        fail=test_once(num1,num2)?1:fail;    
    }
    for (int i = 0; i < special_test_time; i++)
    {
        num2=get_rand_float();
        num1=get_rand_nan();
        fail=test_once(num1,num2);
    }
    if(!fail){
        printf("nan times any regular pass!\n");
    }
    else{
        printf("nan times any regular fail!\n");
    }
    return 0;
}

int Tester::test_two_nan(){
    float num1;
    float num2;
    int fail;
    //-------test nan times nan
    printf("-------test nan times nan-----\n");
    fail=0;
    for (int i = 0; i < special_test_time; i++){
        num1=get_rand_nan();
        num2=get_rand_nan();
        fail=test_once(num1,num2)?1:fail;    }
    if(!fail){
        printf("nan times nan pass!\n");
    }
    else{
        printf("nan times nan fail!\n");
    }
    return 0;
}

int Tester::test_regular_num(){
    float num1;
    float num2;
    int fail;
    //-------test nan times nan
    printf("-------test regular mul -----\n");
    fail=0;
    for (int i = 0; i < test_times; i++){
        num1=get_rand_float();
        num2=get_rand_float();
        fail=test_once(num1,num2)?1:fail;
    }
    if(!fail){
        printf("regular mul test pass!\n");
    }
    else{
        printf("regular mul test fail!\n");
    }
    return 0;
}

int Tester::test_a_regular_num(float a,float b){
    float num1;
    float num2;
    int fail;
    //-------test nan times nan
    printf("-------test regular mul -----\n");
    fail=0;
    num1=a;
    num2=b;
    fail=test_once(num1,num2);
    if(!fail){
        printf("regular mul test pass!\n");
    }
    else{
        printf("regular mul test fail!\n");
    }
    return 0;
}

int main(int argc, char **argv) {
        Tester t1;
    #if NEGATIVE_ENABLE
        printf("------------------------------------------\n");
        printf("--------test round to the negative--------\n");
        printf("------------------------------------------\n");
        fesetround(FE_DOWNWARD);
    #endif
    #if POSITIVE_ENABLE
        printf("------------------------------------------\n");
        printf("--------test round to the positive--------\n");
        printf("------------------------------------------\n");
        fesetround(FE_UPWARD);
    #endif
    #if NEAREST_ENABLE
        printf("------------------------------------------\n");
        printf("--------test round to the nearest--------\n");
        printf("------------------------------------------\n");
        fesetround(FE_TONEAREST);
    #endif
    #if ZERO_ENABLE
        printf("------------------------------------------\n");
        printf("--------test round to the zero--------\n");
        printf("------------------------------------------\n");
        fesetround(FE_TOWARDZERO);
    #endif
    //----------------test special values--------------------
#if TEST_MONTE
    t1.test_special_values();
#endif

    //----------------test regular values--------------------
#if TEST_MONTE
    t1.test_regular_num();
#endif

#if TEST_DEBUG
    Fp32 a;
    Fp32 b;
    a.sign=1;
    a.exponent=0;
    a.mantissa=469134;

    b.sign=0;
    b.exponent=130;
    b.mantissa=5600648;

    float a_float=t1.get_accurate_float(a);
    float b_float=t1.get_accurate_float(b);
    t1.test_a_regular_num(a_float,b_float);
#endif
    return 0;
}