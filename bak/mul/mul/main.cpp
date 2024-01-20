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

using namespace std;
namespace bp = boost::process;
uint32_t round_mode=0;                                                        //0:zero 1:negative 2:positive 3:nearest                        


#ifndef test_times
    #define test_times 10
#endif

#ifndef special_test_time
    #define special_test_time 10
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
    int test_a_regular_num(float a,float b);
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

    #if DEBUG_SV
        tmp2=verif_inout(int_a,int_b);
    #else
        Fp32 fp32_res;
        fp32_res = fp32_a * fp32_b;
        tmp2 = fp32_res.to_uint32();    
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
        exit(1);
    }{
        fail=0;
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
        fail=test_once(num1,num2);
    }
    num1 = -00.0000;
    for (int i = 0; i < special_test_time; i++)
    {
        num2=get_rand_float();
        fail=test_once(num1,num2);
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
            fail=test_once(num1,num2);
        }
        num1=-INFINITY;
        for (int i = 0; i < special_test_time; i++)
        {
            num2=get_rand_float();
            fail=test_once(num1,num2);    
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
        fail=test_once(num1,num2);    
        num2=INFINITY;
        num1=-00.0000;
        fail=test_once(num1,num2);    
        num2=00.0000;
        num1=-INFINITY;
        fail=test_once(num1,num2);    
        num2=-00.0000;
        num1=-INFINITY;
        fail=test_once(num1,num2);    
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
        fail=test_once(num1,num2);    
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
        fail=test_once(num1,num2);    }
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

        fail=test_once(num1,num2);
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
        Tester t1;
        fesetround(FE_TOWARDZERO);
        round_mode=03;
        // printf("round_mode is %u\n",round_mode);
        fesetround(FE_TONEAREST);

    //----------------test special values--------------------
#if TEST_MONTE
    // t1.test_special_values();
#endif

    //----------------test regular values--------------------
#if TEST_MONTE
    printf("test nearest\n");
    t1.test_regular_num();
    printf("test zero\n");
    round_mode=0;
    fesetround(FE_TOWARDZERO);
    t1.test_regular_num();

    printf("test positive\n");
    round_mode=2;
    fesetround(FE_UPWARD);
    t1.test_regular_num();

    printf("test negative\n");
    round_mode=1;
    fesetround(FE_DOWNWARD);
    t1.test_regular_num();
#endif
#if TEST_BUG
    fesetround(FE_TONEAREST);
    Fp32 a;
    Fp32 b;
    a.sign = 1;
    a.exponent = 95;
    a.mantissa = 954781;
    b.sign = 0;
    b.exponent = 221;
    b.mantissa = 3770043;
    float num1=t1.get_accurate_float(a);
    float num2=t1.get_accurate_float(b);
    test_once(num1,num2);
#endif
}

