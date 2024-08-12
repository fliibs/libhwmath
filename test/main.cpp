
#include "../tool/model/struct.h"
#include "../tool/if/if.h"
#include "../tool/model/ref.h"
#include <algorithm>
int main(){
    FpBase a("fp32");
    FpBase b("fp32");
    a.sign=0;
    a.expo=244;
    a.mant=6;
    b.sign=0;
    b.expo=255;
    b.mant=20;
    IF if1;
    float a_flt= if1.Fp32toFloat(a);
    float b_flt= if1.Fp32toFloat(b);
    std::feclearexcept(FE_ALL_EXCEPT);
    std::array<int,5> arr={0,0,0,0,0};

    float c_flt= std::min(a_flt,b_flt);

    FpBase c("fp32");
    c=if1.FloattoFp32(c_flt);
    arr[4]=std::fetestexcept(FE_INVALID)  ?1:0; 
    arr[3]=std::fetestexcept(FE_DIVBYZERO)?1:0; 
    arr[2]=std::fetestexcept(FE_OVERFLOW) ?1:0; 
    arr[1]=std::fetestexcept(FE_UNDERFLOW)?1:0; 
    arr[0]=std::fetestexcept(FE_INEXACT)  ?1:0; 

    for(int i=0;i<5;i++){
        std::cout<<"arr"<<i<<":"<<arr[i]<<std::endl;
    }
    std::cout<<"a_print:"<<std::endl;
    a.print();
    std::cout<<"b_print:"<<std::endl;
    b.print();
    std::cout<<"c_print:"<<std::endl;
    c.print();
    return 0;
}