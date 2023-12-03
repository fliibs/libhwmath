

#include <iostream>
#include <bitset>

#include <random>
#include <limits>

using namespace std;



class Fp32{

    public:
        bool     sign;
        uint32_t exponent;
        uint64_t fraction;

    void display(){
        cout << sign << endl;
        cout << exponent << endl;
        cout << fraction << endl;

    }

    Fp32() {
        sign = 0;
        exponent = 0;
        fraction = 0;
    }

    // constructor, convert a float to Fp32 type
    Fp32(float x) {
        uint32_t bits = *(uint32_t*)&x; // get the binary representation of the float
        sign     = bits >> 31;          // get the sign bit
        exponent = (bits >> 23) & 0xff; // get the exponent bit
        fraction = bits & 0x7fffff;     // get the fraction bit
    }


    // conversion function, convert a Fp32 type to float
    operator float() {
        uint32_t bits = 0;          // initialize the binary representation of the float
        bits |= sign << 31;         // set the sign bit
        bits |= exponent << 23;     // set the exponent bit
        bits |= fraction;           // set the fraction bit
        float x = *(float*)&bits;   // get the float value
        return x;
    }

    operator uint32_t() {
        uint32_t bits = 0;          // initialize the binary representation of the float
        bits |= sign << 31;         // set the sign bit
        bits |= exponent << 23;     // set the exponent bit
        bits |= fraction;           // set the fraction bit
        return bits;
    }

    void print() {
        printf("value      : %12f\n", (float)*this);
        printf(" - sign    : %12ld\n",sign);
        printf(" - exponent: %12ld,%64lb, 2^(%d)\n",exponent,exponent,exponent-127);
        printf(" - fraction: %12ld,%64lb, %f\n",fraction,fraction+ (1 << 23),(float)(fraction + (1 << 23))/(1<<23));
        printf("\n");
    }



    Fp32 mul(const Fp32 &rhs) {

        Fp32 result(0.0f); // initialize the result to 0
        result.sign = sign ^ rhs.sign; // calculate the sign of the result
        result.exponent = exponent + rhs.exponent - 127; // calculate the exponent of the result

        result.fraction = (fraction | 0x800000) * (rhs.fraction | 0x800000); // calculate the significand of the result

        //printf("%lb\n",result.fraction);
        //printf("%lb\n",0x800000000000);
        //printf("?????????\n");
        if (result.fraction & 0x800000000000) { // if the significand of the result exceeds 24 bits
            //printf("%lb\n",result.fraction);
            result.fraction >>= 1; // shift right by one bit
            //printf("%lb\n",result.fraction);
            result.exponent++; // increment the exponent
        }
        if (result.exponent > 254) { // if the exponent of the result exceeds the maximum value
            result.exponent = 255; // set the exponent to infinity
            result.fraction = 0; // set the significand to 0
        } else if (result.exponent < 1) { // if the exponent of the result is less than the minimum value
            result.exponent = 0; // set the exponent to 0
            result.fraction = 0; // set the significand to 0
        } else { // if the exponent of the result is within the valid range
            result.fraction = result.fraction >> 23;
            result.fraction &= 0x7fffff; // remove the hidden 1
            uint32_t g = result.fraction & 0x40; // get the guard bit
            uint32_t r = result.fraction & 0x20; // get the round bit
            uint32_t s = result.fraction & 0x1f; // get the sticky bit
            if (g && (r || s)) { // if rounding is needed
                result.fraction++; // increment the least significant bit
                if (result.fraction & 0x800000) { // if rounding causes overflow
                    result.fraction >>= 1; // shift right by one bit
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


int main(int argc, char **argv)
{

    float num1,num2,res;

    num1 = 1.1f;
    num2 = 1.3f;

    res = num1*num2;
    
    
    Fp32 hw_num1(num1);
    Fp32 hw_num2(num2);
    Fp32 hw_res;


    hw_res = hw_num1 * hw_num2;

    cout << bitset<32>(*(uint32_t*)&res) << endl;
    cout << bitset<32>((uint32_t)hw_res) << endl;


    cout << bitset<32>(*(uint32_t*)&num1) << endl;

    hw_num1.print();
    hw_num2.print();
    hw_res.print();

    constexpr int FLOAT_MIN = 10;
    constexpr int FLOAT_MAX = 100;

    cout << numeric_limits<float>::min() << endl;
    cout << numeric_limits<float>::max() << endl;

    cout << endl;

    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_real_distribution<float> distr(numeric_limits<float>::min(),numeric_limits<float>::max() );


    for (int n = 0; n < 20; ++n) {
        cout << distr(eng) << "\n";
    }
    


    return 0;
}