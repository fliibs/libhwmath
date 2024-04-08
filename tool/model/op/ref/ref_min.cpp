#include "ref_min.h"
std::array<int,5> ref_min::min(const FpBase& a, const FpBase& b,const FpBase& c, int rnd_mode, FpBase* res){

    std::array<int,5> arr={0,0,0,0,0};
    std::feclearexcept(FE_ALL_EXCEPT);
    feclearexcept(FE_ALL_EXCEPT);
    // std::cout << "valuea a is: "<< std::endl;
    // (a).print();
    // std::cout << "valuea b is: "<< std::endl;
    // (b).print();
    arr_excps
    for(int i=0;i<5;i++){
        std::cout<<"ref arr b4 convert:"<<i<<":"<<arr[i]<<std::endl;
    }
    if((*res).expo_w==5){ //fp16
        // std::feclearexcept(FE_ALL_EXCEPT);
        std::cout << "enter ref_min fp16" << std::endl;
        half a_h;
        half b_h;
        arr_excps
        for(int i=0;i<5;i++){
            std::cout<<"ref arr b4 convert:"<<i<<":"<<arr[i]<<std::endl;
        }
        a_h=convert2flt<half>(a);
        b_h=convert2flt<half>(b);
        std::cout << "valuea a_h is: "<< a_h << std::endl;
        std::cout << "valuea b_h is: "<< b_h << std::endl;
        // *res = if1.HalftoFp16(std::min(a_h, b_h));
        // half_float::half a_half;
        // half_float::half b_half;
        // a_half = static_cast<half_float::half>(a_f);
        // b_half = static_cast<half_float::half>(b_f);
        // *res = if1.HalftoFp16(half_float::fmin(a_half,b_half));
        // *res = if1.HalftoFp16(half_float::fmin(a_h,b_h));
        arr_excps
        for(int i=0;i<5;i++){
            std::cout<<"ref arr b4:"<<i<<":"<<arr[i]<<std::endl;
        }
        *res = if1.HalftoFp16(std::min<half>(a_h,b_h));
        arr_excps
        // arr[4]=std::fetestexcept(FE_INVALID) ?1:0; 
        // arr[3]=std::fetestexcept(FE_DIVBYZERO)?1:0; 
        // arr[2]=std::fetestexcept(FE_OVERFLOW) ?1:0; 
        // arr[1]=std::fetestexcept(FE_UNDERFLOW)?1:0; 
        // arr[0]=std::fetestexcept(FE_INEXACT) ?1:0; 
        for(int i=0;i<5;i++){
            std::cout<<"ref arr"<<i<<":"<<arr[i]<<std::endl;
        }
        // (*res).print();
    }
    else if((*res).expo_w==8){ //fp32
        std::cout << "enter ref_min fp32" << std::endl;
        float a_f;
        float b_f;
        // a_f=convert2flt<float>(a);
        // b_f=convert2flt<float>(b);
        a_f = if1.Fp32toFloat(a);
        b_f = if1.Fp32toFloat(b);
        // std::feclearexcept(FE_ALL_EXCEPT);
        std::cout << "valuea a_f is: "<< a_f << std::endl;
        std::cout << "valuea b_f is: "<< b_f << std::endl;
        *res = if1.FloattoFp32(std::min(a_f, b_f));
        // arr_excps
        arr[4]=std::fetestexcept(FE_INVALID) ?1:0; 
        arr[3]=std::fetestexcept(FE_DIVBYZERO)?1:0; 
        arr[2]=std::fetestexcept(FE_OVERFLOW) ?1:0; 
        arr[1]=std::fetestexcept(FE_UNDERFLOW)?1:0; 
        arr[0]=std::fetestexcept(FE_INEXACT) ?1:0; 

        for(int i=0;i<5;i++){
            std::cout<<"ref arr"<<i<<":"<<arr[i]<<std::endl;
        }
        // *res = if1.FloattoFp32(res_f);
        std::cout << "print ref_min model res" << std::endl;
        (*res).print();
    }
    else if((*res).expo_w==11){ //fp64
        std::cout << "enter ref_min fp64" << std::endl;
        // std::feclearexcept(FE_ALL_EXCEPT);
        double a_d;
        double b_d;
        IF if1;
        a_d= if1.Fp64toDouble(a);
        b_d= if1.Fp64toDouble(b);
        // a_d=convert2flt<double>(a);
        // b_d=convert2flt<double>(b);
        
        arr_excps
        for(int i=0;i<5;i++){
            std::cout<<"ref arr b4:"<<i<<":"<<arr[i]<<std::endl;
        }
        std::cout << "valuea a_d is: "<< a_d << std::endl;
        std::cout << "valuea b_d is: "<< b_d << std::endl;

        double c_d = std::min<double>(a_d, b_d);
        // arr_excps
        // for(int i=0;i<5;i++){
        //     std::cout<<"ref arr"<<i<<":"<<arr[i]<<std::endl;
        // }
        // arr_excps
        // for(int i=0;i<5;i++){
        //     std::cout<<"ref arr"<<i<<":"<<arr[i]<<std::endl;
        // }
        
        *res = if1.DoubletoFp64(c_d);
        // arr_excps
        // for(int i=0;i<5;i++){
        //     std::cout<<"ref arr"<<i<<":"<<arr[i]<<std::endl;
        // }
        // *res = if1.DoubletoFp64(std::min(a_d, b_d));

        
        // arr[4]=std::fetestexcept(FE_INVALID) ?1:0; 
        // arr[3]=std::fetestexcept(FE_DIVBYZERO)?1:0; 
        // arr[2]=std::fetestexcept(FE_OVERFLOW) ?1:0; 
        // arr[1]=std::fetestexcept(FE_UNDERFLOW)?1:0; 
        // arr[0]=std::fetestexcept(FE_INEXACT) ?1:0; 
        arr_excps
        for(int i=0;i<5;i++){
            std::cout<<"ref arr"<<i<<":"<<arr[i]<<std::endl;
        }
        // (*res).print();

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