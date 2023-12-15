

#include <iostream>
#include <bitset>
#include <random>
#include <limits>

using namespace std;


class Fp32{

    public:
        bool     sign;
        uint32_t exponent;
        uint64_t mantissa;


    Fp32() {
        sign = 0;
        exponent = 0;
        mantissa = 0;
    }

    // constructor, convert a float to Fp32 type
    Fp32(float x) {
        uint32_t bits = *(uint32_t*)&x; // get the binary representation of the float
        sign     = bits >> 31;          // get the sign bit
        exponent = (bits >> 23) & 0xff; // get the exponent bit
        mantissa = bits & 0x7fffff;     // get the mantissa bit
    }


    // conversion function, convert a Fp32 type to float
    operator float() {
        uint32_t bits = 0;          // initialize the binary representation of the float
        bits |= sign << 31;         // set the sign bit
        bits |= exponent << 23;     // set the exponent bit
        bits |= mantissa;           // set the mantissa bit
        float x = *(float*)&bits;   // get the float value
        return x;
    }

    operator uint32_t() {
        uint32_t bits = 0;          // initialize the binary representation of the float
        bits |= sign << 31;         // set the sign bit
        bits |= exponent << 23;     // set the exponent bit
        bits |= mantissa;           // set the mantissa bit
        return bits;
    }

    uint32_t to_uint32() {
        uint32_t bits = 0;          // initialize the binary representation of the float
        bits |= sign << 31;         // set the sign bit
        bits |= exponent << 23;     // set the exponent bit
        bits |= mantissa;           // set the mantissa bit
        return bits;
    }


    void print() {
        printf("value      : %12f\n", (float)*this);
        printf(" - sign    : %12ld\n",sign);
        printf(" - exponent: %12ld,%64lb, 2^(%d)\n",exponent,exponent,exponent-127);
        printf(" - mantissa: %12ld,%64lb, %f\n",mantissa,mantissa+ (1 << 23),(float)(mantissa + (1 << 23))/(1<<23));
        printf("\n");
    }



    Fp32 mul(const Fp32 &rhs) {

        Fp32 result(0.0f); // initialize the result to 0
        result.sign = sign ^ rhs.sign; // calculate the sign of the result
        result.exponent = exponent + rhs.exponent - 127; // calculate the exponent of the result

        result.mantissa = (mantissa | 0x800000) * (rhs.mantissa | 0x800000); // calculate the significand of the result

        //printf("%lb\n",result.mantissa);
        //printf("%lb\n",0x800000000000);
        //printf("?????????\n");
        if (result.mantissa & 0x800000000000) { // if the significand of the result exceeds 24 bits
            //printf("%lb\n",result.mantissa);
            result.mantissa >>= 1; // shift right by one bit
            //printf("%lb\n",result.mantissa);
            result.exponent++; // increment the exponent
        }
        if (result.exponent > 254) { // if the exponent of the result exceeds the maximum value
            result.exponent = 255; // set the exponent to infinity
            result.mantissa = 0; // set the significand to 0
        } else if (result.exponent < 1) { // if the exponent of the result is less than the minimum value
            result.exponent = 0; // set the exponent to 0
            result.mantissa = 0; // set the significand to 0
        } else { // if the exponent of the result is within the valid range
            result.mantissa = result.mantissa >> 23;
            result.mantissa &= 0x7fffff; // remove the hidden 1
            uint32_t g = result.mantissa & 0x40; // get the guard bit
            uint32_t r = result.mantissa & 0x20; // get the round bit
            uint32_t s = result.mantissa & 0x1f; // get the sticky bit
            if (g && (r || s)) { // if rounding is needed
                result.mantissa++; // increment the least significant bit
                if (result.mantissa & 0x800000) { // if rounding causes overflow
                    result.mantissa >>= 1; // shift right by one bit
                    result.exponent++; // increment the exponent
                }
            }
        }
        return result;
    }




    Fp32 operator* (const Fp32 &rhs) {
        return mul(rhs);
    }


};



class Tester{

    public:

    float get_rand_float() {
        float res;

        std::random_device rd;
        std::default_random_engine eng(rd());
        std::uniform_int_distribution<uint32_t> distr(numeric_limits<uint32_t>::min(),numeric_limits<uint32_t>::max());

        uint32_t tmp1 = distr(eng);
        res = *(float*)&tmp1;
        return res;
    }

    int test_once() {

        float num1 = get_rand_float();
        float num2 = get_rand_float();
        float res = num1 * num2;

        Fp32 hw_num1(num1);
        Fp32 hw_num2(num2);
        Fp32 hw_res;
        hw_res = hw_num1 * hw_num2;

        uint32_t tmp1 = *(uint32_t*)&res;
        uint32_t tmp2 = hw_res.to_uint32();

        if (tmp1 != tmp2) {
            printf("%d\n",tmp1);
            printf("%d\n",tmp2);
            printf("%b\n",tmp1);
            printf("%b\n",tmp2);
            printf("?????\n");
        }

    }

    int test_equal() {
        for (int n = 0; n < 10; n++) {
            test_once();
        }
    }

};



int main(int argc, char **argv)
{
    Tester t1;
    t1.test_equal();

    return 0;
}


