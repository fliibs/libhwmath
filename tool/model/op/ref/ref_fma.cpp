#include "ref_fma.h"
std::array<int,5> ref_fma::fma(const FpBase& a, const FpBase& b,const FpBase& c, int rnd_mode, FpBase* res){
SET_RND_MODE(rnd_mode);
    std::array<int,5> arr={0,0,0,0,0};
    std::feclearexcept(FE_ALL_EXCEPT);
    if((*res).expo_w==5){ //fp16
        half a_h;
        half b_h;
        half c_h;
        a_h=convert2flt<half>(a);
        b_h=convert2flt<half>(b);
        c_h=convert2flt<half>(c);
        half res_h;
        res_h = a_h * b_h + c_h;
        arr_excps; 
        *res   = if1.HalftoFp16(res_h);
    }
    else if((*res).expo_w==8){ //fp32
        float a_f;
        float b_f;
        float c_f;
        a_f=convert2flt<float>(a);
        b_f=convert2flt<float>(b);
        c_f=convert2flt<float>(c);
        float res_f;
        res_f = a_f * b_f + c_f;
        arr_excps; 
        *res  = if1.FloattoFp32(res_f);
    }
    else if((*res).expo_w==11){ //fp64
        double a_d;
        double b_d;
        double c_d;
        a_d=convert2flt<double>(a);
        b_d=convert2flt<double>(b);
        c_d=convert2flt<double>(c);
        double res_d;
        res_d = a_d * b_d +c_d;
        arr_excps; 
        *res  = if1.DoubletoFp64(res_d);
    }
    else{
        printf("intput is not among Fpbase");
        (*res).print();
        std::terminate();   
    }
    return arr;
}