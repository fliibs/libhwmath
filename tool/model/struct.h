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
    bool operator==(const FpBase& other) const {
        return (sign == other.sign) && (expo == other.expo) && (mant == other.mant);
    }
    void print() const{
        if(expo_w==5 && mant_w==10){
            uint16 bits=(sign<<(expo_w+mant_w)) + (expo<<mant_w) + mant;
            printf("value      : %12lf\n", *(half*)&bits);
            printf(" - sign    : %12ld\n", sign);
            printf(" - exponent: %12ld,%64lb, 2^(%d)\n", expo, expo, (expo - static_cast<uint32_t>(std::pow(2, expo_w - 1))));
            printf(" - mantissa: %12ld,%64lb, %lf\n", mant, mant + (1 << (mant_w)), (half)(mant + (1 << (mant_w))) / (1 << (mant_w)));
            printf("\n");
        }

        else if (expo_w==8 && mant_w==23)
        {
            uint32_t bits=(sign<<(expo_w+mant_w)) + (expo<<mant_w) + mant;
            printf("value      : %12lf\n", *(float*)&bits);
            printf(" - sign    : %12ld\n", sign);
            printf(" - exponent: %12ld,%64lb, 2^(%d)\n", expo, expo, (expo - static_cast<uint32_t>(std::pow(2, expo_w - 1))));
            printf(" - mantissa: %12ld,%64lb, %lf\n", mant, mant + (1 << (mant_w)), (float)(mant + (1 << (mant_w))) / (1 << (mant_w)));
            printf("\n");
        }
        
        else if(expo_w==11 && mant_w==52)
        {
            uint64_t bits=(sign<<(expo_w+mant_w)) + (expo<<mant_w) + mant;
            printf("value      : %12lf\n", *(double*)&bits);
            printf(" - sign    : %12ld\n", sign);
            printf(" - exponent: %12ld,%64lb, 2^(%d)\n", expo, expo, (expo - static_cast<uint32_t>(std::pow(2, expo_w - 1))));
            printf(" - mantissa: %12ld,%64lb, %lf\n", mant, mant + (1 << (mant_w)), (double)(mant + (1 << (mant_w))) / (1 << (mant_w)));
            printf("\n");
        }
    }
};

#endif //struct_h