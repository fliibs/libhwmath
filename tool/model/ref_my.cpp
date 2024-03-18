#include "ref_my.h"
void My_ref::min(const FpBase& a, const FpBase& b,const FpBase& c, int rnd_mode, FpBase* res){
    SET_RND_MODE(rnd_mode);
    if((*res).expo_w==5){ //fp16
        half a_h;
        half b_h;
        a_h=convert2flt<half>(a);
        b_h=convert2flt<half>(b);
        half res_h;
        res_h = a_h<=b_h?a_h:b_h;
        *res   = if1.HalftoFp16(res_h);
    }
    else if((*res).expo_w==8){ //fp32
        float a_f;
        float b_f;
        a_f=convert2flt<float>(a);
        b_f=convert2flt<float>(b);
        float res_f;
        res_f = a_f<=b_f?a_f:b_f;
        *res  = if1.FloattoFp32(res_f);
    }
    else if((*res).expo_w==11){ //fp64
        double a_d;
        double b_d;
        a_d=convert2flt<double>(a);
        b_d=convert2flt<double>(b);
        double res_d;
        res_d = a_d<=b_d?a_d:b_d;
        *res  = if1.DoubletoFp64(res_d);
    }
    else{
        printf("intput is not among Fpbase");
        (*res).print();
        std::terminate();   
    }
}