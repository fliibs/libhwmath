#ifndef struct_h
#define struct_h
#include <boost/multiprecision/cpp_int.hpp>
namespace mp = boost::multiprecision;

#include <memory>
#include <variant>

struct FpBase{
    bool sign;
    uint32_t expo;
    uint64_t mant;
    bool operator==(const FpBase& other) const {
        return (sign == other.sign) && (expo == other.expo) && (mant == other.mant);
    }
    virtual void print() const = 0;
};

struct Bf16 :public FpBase{
    int       expo_w=8;
    int       mant_w=7;
    bool operator==(const Bf16& other) const{
        return(sign == other.sign) && (expo == other.expo) && (mant == other.mant);
    }
    void print() const override {
        uint32_t bits = (sign << 31) + (expo << 23) + (mant);
        printf("value      : %12lf\n", *(double*)&bits);
        printf(" - sign    : %12ld\n", sign);
        printf(" - exponent: %12ld,%64lb, 2^(%d)\n", expo, expo, (expo - static_cast<int>(std::pow(2, expo_w - 1))));
        printf(" - mantissa: %12ld,%64lb, %lf\n", mant, mant + (1 << (mant_w)), (double)(mant + (1 << (mant_w))) / (1 << (mant_w)));
        printf("\n");
    }
    
};

struct Fp16: public FpBase{
    int       expo_w=5;
    int       mant_w=10;
    bool operator==(const Fp16& other) const{
        return(sign == other.sign) && (expo == other.expo) && (mant == other.mant);
    }
};

struct Fp64 : public FpBase {
    int expo_w = 11;
    int mant_w = 52;

    void print() const override {
        uint64_t bits = (sign << (expo_w + mant_w)) + (expo << (mant_w)) + (mant);
        printf("value      : %12lf\n", *(double*)&bits);
        printf(" - sign    : %12ld\n", sign);
        printf(" - exponent: %12ld,%64lb, 2^(%d)\n", expo, expo, (expo - static_cast<int>(std::pow(2, expo_w - 1))));
        printf(" - mantissa: %12ld,%64lb, %lf\n", mant, mant + (1 << (mant_w)), (double)(mant + (1 << (mant_w))) / (1 << (mant_w)));
        printf("\n");
    }
};

struct Fp32 : public FpBase {
    int expo_w = 8;
    int mant_w = 23;

    void print() const override {
        uint64_t bits = (sign << (expo_w + mant_w)) + (expo << (mant_w)) + (mant);
        printf("value      : %12lf\n", *(double*)&bits);
        printf(" - sign    : %12ld\n", sign);
        printf(" - exponent: %12ld,%64lb, 2^(%d)\n", expo, expo, (expo - static_cast<int>(std::pow(2, expo_w - 1))));
        printf(" - mantissa: %12ld,%64lb, %lf\n", mant, mant + (1 << (mant_w)), (double)(mant + (1 << (mant_w))) / (1 << (mant_w)));
        printf("\n");
    }
};
#endif //struct_h