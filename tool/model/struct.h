#ifndef struct_h
#define struct_h
#include <boost/multiprecision/cpp_int.hpp>
namespace mp = boost::multiprecision;

#include <memory>
#include <variant>

struct Bf16{
    bool      sign;
    uint32_t  expo;
    uint32_t  mant;
    int       expo_w=8;
    int       mant_w=7;
    bool operator==(const Bf16& other) const{
        return(sign == other.sign) && (expo == other.expo) && (mant == other.mant);
    }
};

struct Fp32 {
    bool     sign;
    uint32_t expo;
    uint32_t mant;
    int expo_w = 8;
    int mant_w = 23;
    bool operator==(const Fp32& other) const{
        return(sign == other.sign) && (expo == other.expo) && (mant == other.mant);
    }
};

struct Fp16{
    bool      sign;
    uint32_t  expo;
    uint32_t  mant;
    int       expo_w=5;
    int       mant_w=10;
    bool operator==(const Fp16& other) const{
        return(sign == other.sign) && (expo == other.expo) && (mant == other.mant);
    }
};

struct Fp64{
    bool        sign;
    uint32_t    expo;
    uint64_t    mant;
    int         expo_w=11;
    int         mant_w=52;
    bool operator==(const Fp64& other) const{
        return(sign == other.sign) && (expo == other.expo) && (mant == other.mant);
    }
};

#endif //struct_h