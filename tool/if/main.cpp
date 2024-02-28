#include "if.h"
#include "pipe.h"
int main(){
    //---------testing if
    // IF if1;
    // float res1=123.456;
    // float res2=123.456;
    // Fp32 a_old;
    // Fp32 a_new;
    // a_old = if1.floatTOFp32(res1);
    // std::cout << "a_old:------- " << std::endl;
    // std::cout << "Sign: " << a_old.sign << std::endl;
    // printf("Exponent is        %b\n",a_old.expo);
    // printf("Mantissa is        %b\n",a_old.mant);
    // a_new = if1.float2fp<Fp32,float>(res1);
    // // 打印 Fp32 结构体的内容
    // std::cout << "a_new:------- " << std::endl;
    // std::cout << "Sign: "     << a_new.sign << std::endl;
    // printf("Exponent is        %b\n",a_new.expo);
    // printf("Mantissa is        %b\n",a_new.mant);
    // float f1;
    // float f2;
    // f1=if1.fp32tofloat(a_old);
    // f2=if1.fp2float<Fp32,float>(a_new);
    // printf("f1 is %f\n",f1);
    // printf("f2 is %f\n",f2);
    
    //----------testing out
    std::string out="out";
    Pipe P1;
    float res=P1.verif_inout<float>(3.5f,2.0f);
    return 0;
}