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

bool processArgumens(int argc, char **argv, const std::string&prefix);

class Tester {
public:
    bp::opstream to_sv  ;
    bp::ipstream from_sv;
    std::string line    ;
    bp::child mul_sv    ;
    // bool test_one       ;
    int argc            ;
    char **argv         ;
    Tester(const std::string& executable,int argc,char ** argv) : mul_sv(executable, bp::std_in < to_sv, bp::std_out > from_sv),argc(argc),argv(argv){
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
    float res;
    if(processArgumens(argc,argv,"TEST_MUL"))
    {
        // printf("into test_mul");    
        res = num1*num2;
    }
    if(processArgumens(argc,argv,"TEST_ADD"))
    {
        // printf("into test_add\n");
        res = num1+num2;
    }
    Fp32 fp32_a(num1);
    Fp32 fp32_b(num2);
    uint32_t int_a=fp32_a.to_uint32();
    uint32_t int_b=fp32_b.to_uint32();
    uint32_t tmp1 =*(uint32_t*)&res;
    uint32_t tmp2;  
    if (processArgumens(argc,argv,"TEST_RTL"))
    {
        tmp2=verif_inout(int_a,int_b);
    }
    else
    {
        // printf("into test_c_modle\n");
        Fp32 fp32_res;
        if(processArgumens(argc,argv,"TEST_BUG")){
            fp32_res.debug=true;
            fp32_a.debug=true;
            fp32_b.debug=true;
            // printf("fp32_res_debug is %d\n",fp32_res.debug);
            }
        else{
            fp32_res.debug=false;
            fp32_a.debug=false;
            fp32_b.debug=false;

            // printf("fp32_res_debug is %d\n",fp32_res.debug);

        }
        if(processArgumens(argc,argv,"TEST_MUL"))
        {
            // printf("into test_mul\n");    
            fp32_res = fp32_a * fp32_b;
        }
        if(processArgumens(argc,argv,"TEST_ADD"))
        {
            // printf("into test_add\n");
            // printf("fp32_res_debug is %d\n",fp32_res.debug);
            fp32_res = fp32_a + fp32_b;
            // printf("fp32_res_debug is %d\n",fp32_res.debug);

        }
        tmp2 = fp32_res.to_uint32();
    }
    
    // bool a_is_nan;
    // bool b_is_nan;
    // a_is_nan= ((int_a&0x7f800000)>>23==0xff)&&(int_a&0x007fffff);
    // b_is_nan= ((int_b&0x7f800000)>>23==0xff)&&(int_b&0x007fffff);
    // if(a_is_nan&&b_is_nan){
    //     tmp2=tmp1;
    // }
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
    if(processArgumens(argc,argv,"TEST_ONE")){
        fail=test_zeros();
    }
    if(processArgumens(argc,argv,"TEST_INF"))
    {
        fail=test_infs();
    }
    if(processArgumens(argc,argv,"TEST_ZERO_INF")){
        fail=test_zero_times_inf();
    }
    if(processArgumens(argc,argv,"TEST_1_NAN")){
        fail=test_one_nan();
    }
    if(processArgumens(argc,argv,"TEST_2_NAN")){
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
        // printf("fail is %d\n",fail);
        num1=get_rand_float();
        fail=fail||test_once(num1,num2);
        // printf("fail is %d\n",fail);
    }
    num1 = -00.0000;
    for (int i = 0; i < SPECIAL_TEST_TIMES; i++)
    {
        num2=get_rand_float();
        fail=fail||test_once(num1,num2);
        // printf("fail is %d\n",fail);
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
    if (processArgumens(argc,argv,"TEST_REG"))
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


uint32_t parseArgument_u32(int argc,char **argv,const std::string&prefix) {
    uint32_t variable=0;
    for (int i = 0; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg.compare(0, prefix.size(), prefix) == 0) {
            std::cout << arg << std::endl;
            std::string value_str = arg.substr(prefix.size());
            std::istringstream(value_str) >> variable;
        }
    }
    return variable;
}
bool processArgumens(int argc, char **argv, const std::string&prefix){
    bool value = false;
    for (int i = 0; i < argc; ++i) {
        std::string arg = argv[i];
        if(arg.compare(0,prefix.size(),prefix)==0){
            value=true;
        }
    }
    return value;
}
int main(int argc, char **argv) {
    int  fail_1              =0     ;
    int  fail_2              =0     ;
    bool a_sign              =false ;
    bool b_sign              =false ;
    uint32_t a_expo          =0     ;
    uint32_t b_expo          =0     ;
    uint32_t a_mant          =0     ;
    uint32_t b_mant          =0     ;
    //---------------start sv or not--------------------------------
    std::string simv_executable=SIMV_EXECUTABLE_PATH;
    std::string quiet   = " -q";
    std::string debug   = " +RTL_DEBUG";
    std::string command;
    if(processArgumens(argc,argv,"TEST_BUG"))
        command = simv_executable + quiet + debug;
    else
        command = simv_executable + quiet;

    std::cout<<command<<std::endl;
    Tester t1(command,argc,argv);
    //----------------test regular values--------------------
    if(processArgumens(argc,argv,"TEST_MONTE")){
        if(processArgumens(argc,argv,"TEST_RND_NEAREST")){
        printf("test nearest::\n");
        round_mode=3;
        fesetround(FE_TONEAREST);
        printf("test_special_values::\n");

        fail_1=t1.test_special_values();
        fail_2=t1.test_regular_num();
        printf("\n");
        }

        if(processArgumens(argc,argv,"TEST_RND_ZERO")){
        printf("test zero::\n");
        round_mode=0;
        fesetround(FE_TOWARDZERO);
        fail_1=t1.test_special_values();
        fail_2=t1.test_regular_num();
        printf("\n");
        }

        if(processArgumens(argc,argv,"TEST_RND_POS")){
        printf("test positive::\n");
        round_mode=2;
        fesetround(FE_UPWARD);
        fail_1=t1.test_special_values();
        fail_2=t1.test_regular_num();
        printf("\n");
        }

        if(processArgumens(argc,argv,"TEST_RND_NEG")){
        printf("test negative::\n");
        round_mode=1;
        fesetround(FE_DOWNWARD);
        fail_1=t1.test_special_values();
        fail_2=t1.test_regular_num();
        }
    }
        //----------------debugging--------------------
    if(processArgumens(argc,argv,"TEST_BUG")){
        printf("into test_bug\n");
        if(processArgumens(argc,argv,"TEST_RNG_NEAREST")){
        round_mode=3;
        fesetround(FE_TONEAREST);
        }

        if(processArgumens(argc,argv,"TEST_RND_ZERO")){
        round_mode=0;
        fesetround(FE_TOWARDZERO);
        }

        if(processArgumens(argc,argv,"TEST_RND_POS")){
        round_mode=2;
        fesetround(FE_UPWARD);
        }

        if(processArgumens(argc,argv,"TEST_RND_NEG")){
        printf("test negative::\n");
        round_mode=1;
        fesetround(FE_DOWNWARD);
        }

        Fp32 a;
        Fp32 b;
        a.sign = parseArgument_u32(argc, argv,"a_sign=");
        a.exponent = parseArgument_u32(argc, argv,"a_expo=");;
        a.mantissa = parseArgument_u32(argc, argv,"a_mant=");
        b.sign = parseArgument_u32(argc, argv,"b_sign=");
        b.exponent = parseArgument_u32(argc, argv,"b_expo=");
        b.mantissa = parseArgument_u32(argc, argv,"b_mant=");;
        float num1=t1.get_accurate_float(a);
        float num2=t1.get_accurate_float(b);
        fail_2=t1.test_once(num1,num2);
        if(processArgumens(argc,argv,"TEST_RTLS")){
        t1.mul_sv.wait();}
    }
    int fail=fail_1||fail_2;
    return fail;
}


