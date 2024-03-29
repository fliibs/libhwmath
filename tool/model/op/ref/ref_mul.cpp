#include "ref_mul.h"
std::array<int,5> ref_mul::mul(const FpBase& a, const FpBase& b,const FpBase& c, int rnd_mode, FpBase* res){
    SET_RND_MODE(rnd_mode);
    std::array<int,5> arr={0,0,0,0,0};
    std::feclearexcept(FE_ALL_EXCEPT);
    feclearexcept(FE_ALL_EXCEPT);
    if((*res).expo_w==5){     //fp16
        half a_h;
        half b_h;
        a_h=convert2flt<half>(a);
        b_h=convert2flt<half>(b);
        half res_h;
        res_h = a_h * b_h;
        half_arr_excps;  
        printf("arr[4] is %d\n",arr[4]);
        *res   = if1.HalftoFp16(res_h);
    }
    else if((*res).expo_w==8){ //fp32
        float a_f;
        float b_f;
        a_f=convert2flt<float>(a);
        b_f=convert2flt<float>(b);
        float res_f;
        res_f = a_f * b_f;
        arr_excps;    
        *res  = if1.FloattoFp32(res_f);
    }
    else if((*res).expo_w==11){ //fp64
        double a_d;
        double b_d;
        a_d=convert2flt<double>(a);
        b_d=convert2flt<double>(b);
        double res_d;
        res_d = a_d * b_d;
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