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
#include <stdlib.h>
#include <string.h>
#include <boost/utility.hpp>
#include <boost/process.hpp>
#include <sstream>
#include "test.h"
#include <chrono>


using namespace std;
namespace bp = boost::process;
uint32_t round_mode=0;                                                        //0:zero 1:negative 2:positive 3:nearest                        


#ifndef TEST_TIMES
    #define TEST_TIMES 10
#endif

#ifndef SPECIAL_TEST_TIMES
    #define SPECIAL_TEST_TIMES 10
#endif

#ifndef TEST_MONTE
    #define TEST_MONTE 0
#endif

#ifndef DEBUG_ENABLE
    #define DEBUG_ENABLE 0
#endif

#ifndef TEST_BUG
    #define TEST_BUG 0
#endif

#ifndef DEBUG_SV
    #define DEBUG_SV 0
#endif

#ifndef TEST_ONE
    #define TEST_ONE 0
#endif

#ifndef TEST_INF
    #define TEST_INF 0
#endif

#ifndef TEST_ZERO_INF
    #define TEST_ZERO_INF 0
#endif

#ifndef TEST_1_NAN
    #define TEST_ONE_NAN 0
#endif

#ifndef TEST_2_NAN
    #define TEST_2_NAN 0
#endif

#ifndef TEST_REG
    #define TEST_REG 0
#endif

#ifdef DEBUG_SV
    bp::opstream to_sv;
    bp::ipstream from_sv;
    bp::child mul_sv("./simv",bp::std_in < to_sv,bp::std_out > from_sv);
    std::string line;
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
    int test_regular_num();
    void print(uint32_t int_in);
    uint32_t verif_inout(uint32_t a,uint32_t b);
};

uint32_t Tester::verif_inout(uint32_t a,uint32_t b){
    to_sv<< a << " " << b <<" "<<round_mode<<std::endl;
    uint32_t value;
    // printf("input putin");
    while(mul_sv.running()&&std::getline(from_sv,line)){
        // std::cout<<line<<std::endl;
        // printf("!!!!!!!!!\n");
        if(line.compare(0,4,"res=")==0){
            std::string value_str=line.substr(4);
            std::istringstream(value_str) >> value;
            // std::cout << "Extracted value: " << value << std::endl;
            break;
        }
    }
    return value;
}

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
    Fp32 fp32_a(num1);
    Fp32 fp32_b(num2);
    uint32_t int_a=fp32_a.to_uint32();
    uint32_t int_b=fp32_b.to_uint32();
    uint32_t tmp1 =*(uint32_t*)&res;
    uint32_t tmp2;
    #ifndef DEBUG_SV
        Fp32 fp32_res;
        fp32_res = fp32_a * fp32_b;
        tmp2 = fp32_res.to_uint32;
    #endif  
    #ifdef DEBUG_SV
        tmp2=verif_inout(int_a,int_b);
    #endif
    bool a_is_nan;
    bool b_is_nan;
    a_is_nan= ((int_a&0x7f800000)>>23==0xff)&&(int_a&0x007fffff);
    b_is_nan= ((int_b&0x7f800000)>>23==0xff)&&(int_b&0x007fffff);
    if(a_is_nan&&b_is_nan){
        tmp2=tmp1;
    }
    if(tmp1!=tmp2){
        printf("--------a is: ");
        fp32_a.print();
        printf("--------b is: ");
        fp32_b.print();
        printf("--------correct res is: ");
        print(tmp1);
        printf("--------your res is: ");
        print(tmp2);
        fail=1;
    }
    else{
        fail=0;
    }
    return fail;
}

int Tester::test_special_values(){
    int fail;
    #if TEST_ONE
        fail=test_zeros();
    #endif
    #if TEST_INF
        fail=test_infs();
    #endif
    #if TEST_ZERO_INF
        fail=test_zero_times_inf();
    #endif
    #if TEST_1_NAN
        fail=test_one_nan();
    #endif
    #if TEST_2_NAN
        fail=test_two_nan();
    #endif
    return fail;
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
    for (int i = 0; i < SPECIAL_TEST_TIMES; i++)
    {
        num1=get_rand_float();
        fail=fail||test_once(num1,num2);
    }
    num1 = -00.0000;
    for (int i = 0; i < SPECIAL_TEST_TIMES; i++)
    {
        num2=get_rand_float();
        fail=fail||test_once(num1,num2);
    }
    if(!fail){
        printf("zero times any regular pass!\n");
    }
    else{
        printf("zero times any regular fail!\n");
    }
    return fail;
}

int Tester::test_infs(){
        float num1;
        float num2;
        int fail;
        //-------test inf times any regular num
        printf("-------test inf times any regular num-----\n");
        fail=0;
        num2=INFINITY;
        for (int i = 0; i < SPECIAL_TEST_TIMES; i++)
        {
            num1=get_rand_float();
            fail=fail||test_once(num1,num2);
        }
        num1=-INFINITY;
        for (int i = 0; i < SPECIAL_TEST_TIMES; i++)
        {
            num2=get_rand_float();
            fail=fail||test_once(num1,num2);    
        }
        if(!fail){
            printf("inf times any regular pass!\n");
        }
        else{
            printf("inf times any regular fail!\n");
        }
        return fail;
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
        fail=fail||test_once(num1,num2);    
        num2=INFINITY;
        num1=-00.0000;
        fail=fail||test_once(num1,num2);    
        num2=00.0000;
        num1=-INFINITY;
        fail=fail||test_once(num1,num2);    
        num2=-00.0000;
        num1=-INFINITY;
        fail=fail||test_once(num1,num2);    
        if(!fail){
            printf("zero times inf pass!\n");
        }
        else{
            printf("zero times inf fail!\n");
        }
        return fail;
        }

int Tester::test_one_nan(){
    float num1;
    float num2;
    int fail;
    //-------test nan times any regular num
    printf("-------test nan times any regular num-----\n");
    fail=0;
    for (int i = 0; i < SPECIAL_TEST_TIMES; i++)
    {
        num2=get_rand_nan();
        num1=get_rand_float();
        fail=fail||test_once(num1,num2);    
    }
    for (int i = 0; i < SPECIAL_TEST_TIMES; i++)
    {
        num2=get_rand_float();
        num1=get_rand_nan();
        fail=fail||test_once(num1,num2);
    }
    if(!fail){
        printf("nan times any regular pass!\n");
    }
    else{
        printf("nan times any regular fail!\n");
    }
    return fail;
}

int Tester::test_two_nan(){
    float num1;
    float num2;
    int fail;
    //-------test nan times nan
    printf("-------test nan times nan-----\n");
    fail=0;
    for (int i = 0; i < SPECIAL_TEST_TIMES; i++){
        num1=get_rand_nan();
        num2=get_rand_nan();
        fail=fail||test_once(num1,num2);    }
    if(!fail){
        printf("nan times nan pass!\n");
    }
    else{
        printf("nan times nan fail!\n");
    }
    return fail;
}

int Tester::test_regular_num(){
    float num1;
    float num2;
    int fail=0;
    //-------test nan times nan
    #if TEST_REG
    printf("-------test regular mul -----\n");
    fail=0;
    for (int i = 0; i < TEST_TIMES; i++){
        num1=get_rand_float();
        num2=get_rand_float();
        fail=fail||test_once(num1,num2);
    }
    if(!fail){
        printf("regular mul test pass!\n");
    }
    else{
        printf("regular mul test fail!\n");
    }
    #endif
    return fail;
}

void Tester::print(uint32_t int_in) {
    int sign = (int_in&0x80000000)>>31;
    int exponent =(int_in&0x7f800000)>>23;
    int mantissa =int_in&0x007fffff;
    printf("value      : %64lb\n", int_in);
    printf(" - sign    : %12ld\n",sign);
    printf(" - exponent: %12ld,%64lb, 2^(%d)\n",exponent,exponent,exponent-127);
    printf(" - mantissa: %12ld,%64lb, %f\n",mantissa,mantissa);
    printf("\n");
    }

int main(int argc, char **argv) {
    auto start_time = std::chrono::high_resolution_clock::now();
    Tester t1;
    int fail;
    //----------------test regular values--------------------
#if TEST_MONTE
    printf("test nearest::\n");
    round_mode=3;
    fesetround(FE_TONEAREST);
    fail=t1.test_special_values();
    fail=t1.test_regular_num();
    printf("\n");

    printf("test zero::\n");
    round_mode=0;
    fesetround(FE_TOWARDZERO);
    fail=t1.test_special_values();
    fail=t1.test_regular_num();
    printf("\n");

    printf("test positive::\n");
    round_mode=2;
    fesetround(FE_UPWARD);
    fail=t1.test_special_values();
    fail=t1.test_regular_num();
    printf("\n");

    printf("test negative::\n");
    round_mode=1;
    fesetround(FE_DOWNWARD);
    fail=t1.test_special_values();
    fail=t1.test_regular_num();
#endif
    //----------------debugging--------------------
#if TEST_BUG
    // round_mode=03;
    // fesetround(FE_TONEAREST);
    round_mode=2;
    fesetround(FE_UPWARD);
    // round_mode=0;
    // fesetround(FE_TOWARDZERO);
    // round_mode=1;
    // fesetround(FE_DOWNWARD);
    Fp32 a;
    Fp32 b;
    a.sign = 1;
    a.exponent = 255;
    a.mantissa = 4646337;
    b.sign = 0;
    b.exponent = 255;
    b.mantissa = 0;
    float num1=t1.get_accurate_float(a);
    float num2=t1.get_accurate_float(b);
    t1.test_once(num1,num2);
    mul_sv.wait();
#endif
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    std::cout << "Time taken by function: " << duration.count() << " microseconds" << std::endl;
    return fail;
}


