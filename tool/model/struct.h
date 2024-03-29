#ifndef struct_h
#define struct_h
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/cpp_bin_float.hpp>
#include <half.hpp>
namespace mp = boost::multiprecision;
using half_float::half;
using half_float::detail::uint16;
#include <memory>
#include <variant>

struct FpBase{
    bool sign;
    uint32_t expo;
    uint64_t mant;
    int expo_w;
    int mant_w;
    FpBase(){}
    FpBase(std::string fptype){
        if(fptype=="fp16"){
            expo_w=5;
            mant_w=10;
        }
        else if(fptype=="fp32"){
            expo_w=8;
            mant_w=23;
        }
        else if(fptype=="fp64"){
            expo_w=11;
            mant_w=52;
        }
        else{
            expo_w=8;
            mant_w=23;
            std::cout <<"Fail to set "<<std::left<<std::setw(20)<<"Fptype";  
            std::cout << std::left<<std::setw(20)<<",default set is"<<"fp32"<<std::endl;     
        }

    }
    bool operator==(const FpBase& other) const {
        return (sign == other.sign) && (expo == other.expo) && (mant == other.mant);
    }
    void print() const{
        if(expo_w==5 && mant_w==10){
            uint16 bits=(sign<<(expo_w+mant_w)) + (expo<<mant_w) + mant;
            std::cout<<"value      :"<<std::setw(12)<<(*(half*)&bits)<<std::endl;
            printf(" - sign    : %12ld\n", sign);
            if(expo==0){            
                printf(" - exponent: %12ld,%64lb, 2^(%d)\n", expo, expo, (expo +1 - static_cast<uint32_t>(std::pow(2, expo_w - 1))));
                printf(" - mantissa: %12ld,%64lb, %lf\n", mant, mant, (half)(mant / (1 << (mant_w))));

            }
            else{
                printf(" - exponent: %12ld,%64lb, 2^(%d)\n", expo, expo, (expo - static_cast<uint32_t>(std::pow(2, expo_w - 1))));
                printf(" - mantissa: %12ld,%64lb, %lf\n", mant, mant + (1 << (mant_w)), (half)(mant + (1 << (mant_w))) / (1 << (mant_w)));
            }
            printf("\n");
        }

        else if (expo_w==8 && mant_w==23)
        {
            uint32_t bits=(sign<<(expo_w+mant_w)) + (expo<<mant_w) + mant;
            printf("value      : %12lf\n", *(float*)&bits);
            printf(" - sign    : %12ld\n", sign);
            if(expo==0){            
                printf(" - exponent: %12ld,%64lb, 2^(%d)\n", expo, expo, (expo +1 - static_cast<uint32_t>(std::pow(2, expo_w - 1))));
                printf(" - mantissa: %12ld,%64lb, %lf\n", mant, mant, (float)(mant / (1 << (mant_w))));

            }
            else{
                printf(" - exponent: %12ld,%64lb, 2^(%d)\n", expo, expo, (expo - static_cast<uint32_t>(std::pow(2, expo_w - 1))));
                printf(" - mantissa: %12ld,%64lb, %lf\n", mant, mant + (1 << (mant_w)), (float)(mant + (1 << (mant_w))) / (1 << (mant_w)));
            }
            printf("\n");
        }
        
        else if(expo_w==11 && mant_w==52)
        {
            uint64_t bits=(sign<<(expo_w+mant_w)) + (expo<<mant_w) + mant;
            printf("value      : %12lf\n", *(double*)&bits);
            printf(" - sign    : %12ld\n", sign);
            if(expo==0){            
                printf(" - exponent: %12ld,%64lb, 2^(%d)\n", expo, expo, (expo +1 - static_cast<uint32_t>(std::pow(2, expo_w - 1))));
                printf(" - mantissa: %12ld,%64lb, %lf\n", mant, mant, (double)(mant / (1 << (mant_w))));

            }
            else{
                printf(" - exponent: %12ld,%64lb, 2^(%d)\n", expo, expo, (expo - static_cast<uint32_t>(std::pow(2, expo_w - 1))));
                printf(" - mantissa: %12ld,%64lb, %lf\n", mant, mant + (1 << (mant_w)), (double)(mant + (1 << (mant_w))) / (1 << (mant_w)));
            }
            printf("\n");
        }
    }
};

#endif //struct_h