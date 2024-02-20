#include "pipe.h"

uint32_t Pipe::verif_inout(uint32_t a,uint32_t b){
    std::cout<<"into verif_inout "<<std::endl;
    to_sv<< a << " " << b <<" "<<round_mode<<std::endl;
    uint32_t value;
    while(mul_sv.running()&&std::getline(from_sv,line)){
        // std::cout<<line<<std::endl;
        if(line.compare(0,4,"res=")==0){
            std::string value_str=line.substr(4);
            std::istringstream(value_str) >> value;
            break;
        }
    }
    return value;
}

Fp32 Pipe::c_model_res(Fp32 a,Fp32 b,Fp32 c){
    Fp32 res;
    res=P1.c_op(a,b,c);
    return res;
}

int Pipe::test_once(float num1,float num2,float num3) {
    // Implementation of the testing logic
    int fail=0;
    float res;
    //the result res is 
    res = P1.res_op(num1,num2,num3);

    Fp32 fp32_a(num1);
    Fp32 fp32_b(num2);
    Fp32 fp32_c(num3);
    printf("--------a is: ");
    fp32_a.print();
    printf("--------b is: ");
    fp32_b.print();
    printf("--------c is: ");
    fp32_c.print();
    uint32_t int_a=fp32_a.to_uint32();
    uint32_t int_b=fp32_b.to_uint32();
    uint32_t int_c=fp32_c.to_uint32();

    uint32_t tmp1 =*(uint32_t*)&res;
    uint32_t tmp2 =0; 
    std::cout<<"test_rtl:"<<test_rtl<<std::endl;

    if (test_rtl) //rtl model test
    { 
        tmp2=verif_inout(int_a,int_b);
    }
    else  //cmodel test
    {
        Fp32 fp32_res;
        fp32_res = c_model_res(fp32_a,fp32_b,fp32_c);
        tmp2     = fp32_res.to_uint32();
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

void Pipe::print(uint32_t int_in) {
    int sign = (int_in&0x80000000)>>31;
    int exponent =(int_in&0x7f800000)>>23;
    int mantissa =int_in&0x007fffff;
    printf("value      : %64lb\n", int_in);
    printf(" - sign    : %12ld\n",sign);
    printf(" - exponent: %12ld,%64lb, 2^(%d)\n",exponent,exponent,exponent-127);
    printf(" - mantissa: %12ld,%64lb, %f\n",mantissa,mantissa);
    printf("\n");
}