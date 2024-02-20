#include "fp32.h"
#include "reflib.h"
#include "MACRO_FUNC.h"
#include "pipe.h"
#include <iostream>
#include <bitset>
#include <random>
#include <limits>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <cstdint>
#include <cstdarg>
#include <cstdio>
#include <stdlib.h>
#include <string.h>

#include <sstream>
#include <chrono>
#include <unistd.h>
#include <unordered_map>

using namespace std;

int main(int argc, char **argv) {
    int  fail_1              =0     ;
    int  fail_2              =0     ;
    bool a_sign              =false ;
    bool b_sign              =false ;
    uint32_t a_expo          =0     ;
    uint32_t b_expo          =0     ;
    uint32_t a_mant          =0     ;
    uint32_t b_mant          =0     ;
    uint32_t round_mode      =0     ;
    float a                  =0     ;
    float b                  =0     ;
    float c                  =0     ;
    //----------------oponents assignment functions -------------------
    Ini_input init1;
    init1.init_a_func(argc,argv,"in1=");
    init1.init_b_func(argc,argv,"in2=");
    init1.init_c_func(argc,argv,"in3=");
    
    //---------------test times------------------------------------
    uint32_t test_times;
    test_times = init1.parseArgument_u32(argc, argv,"TEST_TIMES=");
    std::cout<<"test_times:"<<test_times<<std::endl;

    //---------------start sv or not--------------------------------
    std::string simv_executable=SIMV_EXECUTABLE_PATH;
    std::string quiet   = " -q";
    std::string debug   = " +RTL_DEBUG";
    std::string command;
    
    if(init1.processArguments(argc,argv,"DEBUG_RTL"))
        command = simv_executable + quiet + debug;
    else
        command = simv_executable + quiet;

    //rtl model choose 
    std::string model_mul;
    std::string model_add;
    std::string model_fma;

    RTL_COMMAND(model_mul,MUL);
    RTL_COMMAND(model_add,ADD);
    // RTL_COMMAND(model_fma,FMA);

    std::cout<<command<<std::endl;
    Pipe pipe1(command);

    if(init1.processArguments(argc,argv,"TEST_RTL"))
        pipe1.test_rtl=true;
    else
        pipe1.test_rtl=false;
    if(init1.processArguments(argc,argv,"DEBUG_C"))
        pipe1.P1.debug_c=true;
    else
        pipe1.P1.debug_c=false;
    std::cout<<"pipe1.test_rtl:"<<pipe1.test_rtl<<std::endl;
    std::cout<<"pipe1.P1.debug_c:"<<pipe1.P1.debug_c<<std::endl;

    //----------------test round mode build-------------------
    SET_RND_MODE("TEST_RND_NEAREST",3,FE_TONEAREST);
    SET_RND_MODE("TEST_RND_ZERO",0,FE_TOWARDZERO);
    SET_RND_MODE("TEST_RND_POS",2,FE_UPWARD);
    SET_RND_MODE("TEST_RND_NEG",1,FE_DOWNWARD);

    //c model and reference model choose
    SET_REFMODEL("MUL",setMulOperation);
    SET_REFMODEL("ADD",setAddOperation);
    SET_REFMODEL("FMA",setFMAOperation);

    //------------Monte Carlo testing
    if(init1.processArguments(argc,argv,"TEST_MONTE")){
        //---------------start testing;
        std::cout<<"into monte carlo testing"<<std::endl;
        for (int i = 0; i < test_times; i++)
        {   
            std::cout<<"debug_c:"<<pipe1.P1.debug_c<<std::endl;
            std::cout<<"into for loop"<<std::endl;
            fail_1=0;
            a = init1.a_get_rand();
            b = init1.b_get_rand();
            c = init1.c_get_rand();

            printf("a is %f\n",a);
            printf("b is %f\n",b);
            printf("c is %f\n",c);
            fail_1=fail_1||pipe1.test_once(a,b,c);
        }
    }
    if(init1.processArguments(argc,argv,"TEST_BUG")){

        Fp32 a;
        Fp32 b;
        Fp32 c;
        // a.sign = init1.parseArgument_u32(argc, argv,"a_sign=");
        // a.exponent = init1.parseArgument_u32(argc, argv,"a_expo=");;
        // a.mantissa = init1.parseArgument_u32(argc, argv,"a_mant=");
        // b.sign = init1.parseArgument_u32(argc, argv,"b_sign=");
        // b.exponent = init1.parseArgument_u32(argc, argv,"b_expo=");
        // b.mantissa = init1.parseArgument_u32(argc, argv,"b_mant=");
        // c.sign = init1.parseArgument_u32(argc, argv,"c_sign=");
        // c.exponent = init1.parseArgument_u32(argc, argv,"c_expo=");
        // c.mantissa = init1.parseArgument_u32(argc, argv,"c_mant=");
        a.sign     = 1;
        a.exponent = 107;
        a.mantissa = 3328832;
        b.sign     = 0;
        b.exponent = 215;
        b.mantissa = 7274097;
        c.sign     = 1;
        c.exponent = 14;
        c.mantissa = 962360;
        float num1=init1.get_accurate_float(a);
        float num2=init1.get_accurate_float(b);
        float num3=init1.get_accurate_float(c);
        printf("a is %f\n",a);
        printf("b is %f\n",b);
        printf("c is %f\n",c);
        fail_2=fail_2||pipe1.test_once(num1,num2,num3);

        if(init1.processArguments(argc,argv,"TEST_RTL")){
            pipe1.mul_sv.wait();}
    }
    int fail=fail_1||fail_2;
    return fail;
}


