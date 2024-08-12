#include "ref_min.h"
std::array<int,5> ref_min::min(const FpBase& a, const FpBase& b,const FpBase& c, int rnd_mode, FpBase* res){

    std::array<int,5> arr={0,0,0,0,0};
    std::feclearexcept(FE_ALL_EXCEPT);
    feclearexcept(FE_ALL_EXCEPT);
    // std::cout << "valuea a is: "<< std::endl;
    // (a).print();
    // std::cout << "valuea b is: "<< std::endl;
    // (b).print();
    double tol = std::numeric_limits<double>::epsilon() * 1000;
    if((*res).expo_w==5){ //fp16
        std::cout << "enter ref_min fp16" << std::endl;
        half a_h;
        half b_h;
        a_h=convert2flt<half>(a);
        b_h=convert2flt<half>(b);
        std::cout << "valuea a_h is: "<< a_h << std::endl;
        std::cout << "valuea b_h is: "<< b_h << std::endl;
        bool ref_a_is_nan = std::isnan(a_h);
        bool ref_b_is_nan = std::isnan(b_h);
        std::cout << "ref_a_is_nan is: "<< ref_a_is_nan << std::endl;
        std::cout << "ref_b_is_nan is: "<< ref_b_is_nan << std::endl;
        if (ref_a_is_nan && ref_b_is_nan) {
            *res = if1.HalftoFp16(a_h);
        } else if (ref_a_is_nan) {
            *res = if1.HalftoFp16(b_h);
        } else if (ref_b_is_nan) {
            *res = if1.HalftoFp16(a_h);
        } else {
            *res = if1.HalftoFp16(std::min(a_h, b_h));
        }
        // *res = if1.HalftoFp16(half_float::fmin(a_h,b_h));
        arr_excps
        arr[4]=std::fetestexcept(FE_INVALID) ?1:0; 
        arr[3]=std::fetestexcept(FE_DIVBYZERO)?1:0; 
        arr[2]=std::fetestexcept(FE_OVERFLOW) ?1:0; 
        arr[1]=std::fetestexcept(FE_UNDERFLOW)?1:0; 
        arr[0]=std::fetestexcept(FE_INEXACT) ?1:0; 
        for(int i=5;i<1;i--){
            std::cout<<"ref arr"<<i<<":"<<arr[i]<<std::endl;
        }
    }
    else if((*res).expo_w==8){ //fp32
        std::cout << "enter ref_min fp32" << std::endl;
        float a_f;
        float b_f;
        // a_f=convert2flt<float>(a);
        // b_f=convert2flt<float>(b);
        a_f = if1.Fp32toFloat(a);
        b_f = if1.Fp32toFloat(b);
        std::feclearexcept(FE_ALL_EXCEPT);
        std::cout << "valuea a_f is: "<< a_f << std::endl;
        std::cout << "valuea b_f is: "<< b_f << std::endl;
        bool ref_a_is_nan = std::isnan(a_f);
        bool ref_b_is_nan = std::isnan(b_f);
        std::cout << "ref_a_is_nan is: "<< ref_a_is_nan << std::endl;
        std::cout << "ref_b_is_nan is: "<< ref_b_is_nan << std::endl;
        if (ref_a_is_nan && ref_b_is_nan) {
            *res = if1.FloattoFp32(a_f);
        } else if (ref_a_is_nan) {
            *res = if1.FloattoFp32(b_f);
        } else if (ref_b_is_nan) {
            *res = if1.FloattoFp32(a_f);
        } else {
            *res = if1.FloattoFp32(std::min(a_f, b_f));
        }
        // *res = if1.FloattoFp32(std::min(a_f, b_f));
        // arr_excps
        arr[4]=std::fetestexcept(FE_INVALID) ?1:0; 
        arr[3]=std::fetestexcept(FE_DIVBYZERO)?1:0; 
        arr[2]=std::fetestexcept(FE_OVERFLOW) ?1:0; 
        arr[1]=std::fetestexcept(FE_UNDERFLOW)?1:0; 
        arr[0]=std::fetestexcept(FE_INEXACT) ?1:0; 

        for(int i=5;i<1;i--){
            std::cout<<"ref arr"<<i<<":"<<arr[i]<<std::endl;
        }
        // *res = if1.FloattoFp32(res_f);
        std::cout << "print ref_min model res" << std::endl;
        (*res).print();
    }
    else if((*res).expo_w==11){ //fp64
        std::cout << "enter ref_min fp64" << std::endl;
        double a_d;
        double b_d;
        a_d=convert2flt<double>(a);
        b_d=convert2flt<double>(b);
        std::cout << "valuea a_d is: "<< a_d << std::endl;
        std::cout << "valuea b_d is: "<< b_d << std::endl;
        bool ref_a_is_nan = std::isnan(a_d);
        bool ref_b_is_nan = std::isnan(b_d);
        std::cout << "ref_a_is_nan is: "<< ref_a_is_nan << std::endl;
        std::cout << "ref_b_is_nan is: "<< ref_b_is_nan << std::endl;
        if (ref_a_is_nan && ref_b_is_nan) {
            *res = if1.DoubletoFp64(a_d);
        } else if (ref_a_is_nan) {
            *res = if1.DoubletoFp64(b_d);
        } else if (ref_b_is_nan) {
            *res = if1.DoubletoFp64(a_d);
        } else {
            *res = if1.DoubletoFp64(std::min(a_d, b_d));
        }
        // *res = if1.DoubletoFp64(std::min(a_d, b_d));
        // *res = if1.DoubletoFp64((a_d = b_d) ? a_d :(a_d < b_d) ? a_d : b_d);
        arr_excps
        arr[4]=std::fetestexcept(FE_INVALID) ?1:0; 
        arr[3]=std::fetestexcept(FE_DIVBYZERO)?1:0; 
        arr[2]=std::fetestexcept(FE_OVERFLOW) ?1:0; 
        arr[1]=std::fetestexcept(FE_UNDERFLOW)?1:0; 
        arr[0]=std::fetestexcept(FE_INEXACT) ?1:0; 
        for(int i=5;i<1;i--){
            std::cout<<"ref arr"<<i<<":"<<arr[i]<<std::endl;
        }
    }
    else{
        printf("input not belongs to Fpbase");
        (*res).print();
        std::terminate();   
    }

    // std::cout << "print ref_min model res" << std::endl;
    // (*res).print();
    return arr;
}