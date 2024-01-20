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
#include <iostream>
#include <boost/process.hpp>
#include <sstream>


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
    void compare();
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
    char command[120];
    uint32_t c    = *(uint32_t*)&res;
    snprintf(command, sizeof(command), "./simv +A=%b +B=%b +C=%b", tmp_num1,tmp_num2,c);
    // printf("command is %s\n",command);
    // printf("a_int is %u\n",tmp_num1);
    // printf("a_int is %b\n",tmp_num1);
    // printf("b_int is %u\n",tmp_num2);
    // printf("b_int is %b\n",tmp_num2);
    // printf("c_int is %u\n",c);
    // printf("c_int is %b\n",c);
    // print(tmp_num1);
    // print(tmp_num2);
    // print(c);
    FILE *file = fopen("c_out.txt", "w");
    if (file != NULL) {
        // Read the value from the file using %u format
        fprintf(file, "%b\n", c);
        // printf("Result read from file: %b\n", c); 
        fclose(file);
    } else {
        printf("Error: Unable to open file\n");
    }
    system(command); 
    compare();
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

uint32_t round_mode=0;                                                        //0:zero 1:negative 2:positive 3:nearest                        

void Tester::compare(){
    int fail=0;
    int n_fail=0;
    FILE *file_1 = fopen("c_out.txt", "r");
    FILE *file_2 = fopen("mul_out.txt", "r");
    FILE *file_3 = fopen("mul_in.txt", "r");
    char line_1[50];
    char line_2[50];
    char line_3[100];

    char *token_1;
    char *ptr_1;
    char *token_2;
    char *ptr_2;
    uint32_t out_c;
    uint32_t out_mul;

    uint32_t mul_in;
    char *token_3;

    if ((file_1 != NULL) &&(file_2 != NULL)  &&(file_3 != NULL)) 
    {
        while((fgets(line_1,sizeof(line_1),file_1)&&(fgets(line_2,sizeof(line_2),file_2))&&(fgets(line_3,sizeof(line_3),file_3)))){
            token_1 = strtok(line_1,"\n");
            token_2 = strtok(line_2,"\n");
            token_3 = strtok(line_3,"\n");

            while ((token_1 != NULL)&&(token_2 != NULL)&&(token_3 != NULL)) {
                // printf("token_1 is %s\n", token_1);
                out_c=strtoul(token_1,&ptr_1,2);
                // printf("token_1 is %u\n", out_c);
                // printf("token_2 is %s\n",  token_2);
                out_mul=strtoul(token_2,&ptr_2,2);
                // printf("token_2 is %u\n", out_mul);
                // printf("token_3 is %s\n", token_3);
                char *token_mul_in=strtok(token_3," ");
                char *ptr_in;
                if(out_c!=out_mul){
                    fail=1;
                    n_fail=1;
                    printf("-----------correct mul is:");
                    print(out_c);
                    printf("-----------mul is:");

                    print(out_mul);
                }
                else{
                    n_fail=0;
                }
                while (token_mul_in != NULL) {
                    // printf("%s\n", token_mul_in);
                    mul_in=strtoul(token_mul_in,&ptr_in,2);
                    if(n_fail){
                        printf("input is:");
                        print(mul_in);
                    }
                    token_mul_in = strtok(NULL, " =");
                }
                token_1 = strtok(NULL, "\n");
                token_2 = strtok(NULL, "\n");
                token_3 = strtok(NULL, "\n");
            }
        }
    }
    if(fail){
        printf("test fail\n");
        exit(0);
    }
    else{
        printf("test success\n");
    }
    fclose(file_1);
    fclose(file_2);
    fclose(file_3);
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
    t1.test_regular_num();
#endif
#if TEST_BUG
    // t1.test_regular_num();
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
    float res = num1*num2;
    Fp32 hw_num1(num1);
    Fp32 hw_num2(num2);
    uint32_t tmp_num1=hw_num1.to_uint32();
    uint32_t tmp_num2=hw_num2.to_uint32();
    char command[120];
    uint32_t c    = *(uint32_t*)&res;
    printf("c is %b\n",c);
    snprintf(command, sizeof(command), "./simv +A=%b +B=%b +C=%b", tmp_num1,tmp_num2,c);
    t1.print(tmp_num1);
    t1.print(tmp_num2);
    t1.print(c);
    FILE *file = fopen("c_out.txt", "w");
    if (file != NULL) {
        fprintf(file, "%b\n", c);
        // printf("Result read from file: %b\n", c);
        fclose(file);
    } else {
        printf("Error: Unable to open file\n");
    }
    system(command); 
    t1.compare();
#endif
}


