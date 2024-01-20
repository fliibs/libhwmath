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
#include <unistd.h>
#include <unordered_map>
using namespace std;
namespace bp = boost::process;
uint32_t round_mode=0;                                                        //0:zero 1:negative 2:positive 3:nearest                        

#ifndef TEST_TIMES
    #define TEST_TIMES 0
#endif

#ifndef SPECIAL_TEST_TIMES
    #define SPECIAL_TEST_TIMES 0
#endif

class Tester {
public:
    bp::opstream to_sv  ;
    bp::ipstream from_sv;
    std::string line    ;
    bp::child mul_sv    ;
    bool test_one       ;
    bool test_inf       ;
    bool test_zero_inf  ;
    bool test_1_nan     ;
    bool test_2_nan     ;
    bool test_reg       ;
    bool test_rtl       ;      
    bool test_bug       ;
    Tester(const std::string& executable) : mul_sv(executable, bp::std_in < to_sv, bp::std_out > from_sv){
    }
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
    int i=0;
    while(mul_sv.running()&&std::getline(from_sv,line)){
        if(line.compare(0,4,"res=")==0){
            std::string value_str=line.substr(4);
            std::istringstream(value_str) >> value;
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
    if (test_rtl)
    {
        tmp2=verif_inout(int_a,int_b);
    }
    else
    {
        // printf("into test_c_modle\n");
        Fp32 fp32_res;
        if(test_bug){
            fp32_res.debug=true;
            // printf("fp32_res_debug is %d\n",fp32_res.debug);
        }
        fp32_res = fp32_a * fp32_b;
        tmp2 = fp32_res.to_uint32();
    }
    
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
    int fail=0;
    if(test_one){
        fail=test_zeros();
    }
    if(test_inf)
    {
        fail=test_infs();
    }
    if(test_zero_inf){
        fail=test_zero_times_inf();
    }
    if(test_1_nan){
        fail=test_one_nan();
    }
    if(test_2_nan){
        fail=test_two_nan();
    }
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
    if (test_reg)
    {
        printf("-------test regular mul -----\n");
        // printf("test_times is %d",TEST_TIMES);
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
    }
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


void parseArgument_bool(const std::string& arg, const std::string& prefix, bool& variable) {
    if (arg.compare(0, prefix.size(), prefix) == 0) {
        std::cout << arg << std::endl;
        std::string value_str = arg.substr(prefix.size());
        std::istringstream(value_str) >> variable;
    }
}
void parseArgument_u32(const std::string& arg, const std::string& prefix, uint32_t& variable) {
    if (arg.compare(0, prefix.size(), prefix) == 0) {
        std::cout << arg << std::endl;
        std::string value_str = arg.substr(prefix.size());
        std::istringstream(value_str) >> variable;
    }
}

int main(int argc, char **argv) {
    bool test_monte          =false ;
    bool test_bug            =false ;
    bool test_rnd_nearest    =false ;  
    bool test_rnd_zero       =false ;
    bool test_rnd_pos        =false ;
    bool test_rnd_neg        =false ;
    bool test_rtl            =false ;
    bool test_reg            =false ;
    bool test_one            =false ;
    bool test_inf            =false ;
    bool test_zero_inf       =false ;
    bool test_1_nan          =false ;
    bool test_2_nan          =false ;
    int  fail                =0     ;

    bool a_sign              =false ;
    bool b_sign              =false ;
    uint32_t a_expo          =0     ;
    uint32_t b_expo          =0     ;
    uint32_t a_mant          =0     ;
    uint32_t b_mant         =0     ;
    std::unordered_map<std::string, bool*> testVariables = {
        {"TEST_MONTE", &test_monte},
        {"TEST_BUG", &test_bug},
        {"TEST_RND_NEAREST", &test_rnd_nearest},
        {"TEST_RND_ZERO", &test_rnd_zero},
        {"TEST_RND_POS", &test_rnd_pos},
        {"TEST_RND_NEG", &test_rnd_neg},
        {"TEST_RTL", &test_rtl},
        {"TEST_REG", &test_reg},
        {"TEST_ONE", &test_one},
        {"TEST_INF", &test_inf},
        {"TEST_ZERO_INF", &test_zero_inf},
        {"TEST_1_NAN", &test_1_nan},
        {"TEST_2_NAN", &test_2_nan},
    };

    for (int i = 0; i < argc; ++i) {
        auto it = testVariables.find(argv[i]);
        if (it != testVariables.end()) {
            *(it->second) = true;
        }
        parseArgument_bool(argv[i], "a_sign=", a_sign);
        parseArgument_bool(argv[i], "b_sign=", b_sign);
        parseArgument_u32(argv[i], "a_expo=", a_expo);
        parseArgument_u32(argv[i], "b_expo=", b_expo);
        parseArgument_u32(argv[i], "a_mant=", a_mant);
        parseArgument_u32(argv[i], "b_mant=", b_mant);          
    }

    //---------------start sv or not--------------------------------
    std::string simv_executable=SIMV_EXECUTABLE_PATH;
    std::string quiet   = " -q";
    std::string debug   = " +RTL_DEBUG";
    std::string command;
    if(test_bug)
        command = simv_executable + quiet + debug;
    else
        command = simv_executable + quiet;

    std::cout<<command<<std::endl;
    Tester t1(command);
    t1.test_rtl=test_rtl;
    t1.test_inf=test_inf;
    t1.test_zero_inf=test_zero_inf;
    t1.test_1_nan=test_1_nan;
    t1.test_2_nan=test_2_nan;
    t1.test_reg  =test_reg;
    t1.test_one  =test_one;
    t1.test_bug  =test_bug;

    //----------------test regular values--------------------
    printf("test_monte is %d\n",test_monte);
    if(test_monte){
        if(test_rnd_nearest){
        printf("test nearest::\n");
        round_mode=3;
        fesetround(FE_TONEAREST);
        fail=t1.test_special_values();
        fail=t1.test_regular_num();
        printf("\n");
        }


        if(test_rnd_zero){
        printf("test zero::\n");
        round_mode=0;
        fesetround(FE_TOWARDZERO);
        fail=t1.test_special_values();
        fail=t1.test_regular_num();
        printf("\n");
        }

        if(test_rnd_pos){
        printf("test positive::\n");
        round_mode=2;
        fesetround(FE_UPWARD);
        fail=t1.test_special_values();
        fail=t1.test_regular_num();
        printf("\n");
        }

        if(test_rnd_neg){
        printf("test negative::\n");
        round_mode=1;
        fesetround(FE_DOWNWARD);
        fail=t1.test_special_values();
        fail=t1.test_regular_num();
        }
    }
        //----------------debugging--------------------
    if(test_bug){
        printf("into test_bug\n");
        if(test_rnd_nearest){
        round_mode=3;
        fesetround(FE_TONEAREST);
        }

        if(test_rnd_zero){
        round_mode=0;
        fesetround(FE_TOWARDZERO);
        }

        if(test_rnd_pos){
        round_mode=2;
        fesetround(FE_UPWARD);
        }

        if(test_rnd_neg){
        printf("test negative::\n");
        round_mode=1;
        fesetround(FE_DOWNWARD);
        }

        Fp32 a;
        Fp32 b;
        a.sign = a_sign;
        a.exponent = a_expo;
        a.mantissa = a_mant;
        b.sign = b_sign;
        b.exponent = b_expo;
        b.mantissa = b_mant;
        float num1=t1.get_accurate_float(a);
        float num2=t1.get_accurate_float(b);
        t1.test_once(num1,num2);
        if(test_rtl){
        t1.mul_sv.wait();}
        
    }
    return fail;
}


