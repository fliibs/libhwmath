#include <iostream>
#include <cmath>
using namespace std;

class Fp32 {
    public:
        bool sign; // sign bit
        uint32_t exponent; // exponent bit
        uint64_t fraction; // fraction bit

        // constructor, convert a float to Fp32 type
        Fp32(float x) {
            uint32_t bits = *(uint32_t*)&x; // get the binary representation of the float
            sign = bits >> 31; // get the sign bit
            exponent = (bits >> 23) & 0xff; // get the exponent bit
            fraction = bits & 0x7fffff; // get the fraction bit
        }

        // conversion function, convert a Fp32 type to float
        operator float() {
            uint32_t bits = 0; // initialize the binary representation of the float
            bits |= sign << 31; // set the sign bit
            bits |= exponent << 23; // set the exponent bit
            bits |= fraction; // set the fraction bit
            float x = *(float*)&bits; // get the float value
            return x;
        }

        // multiplication operation, multiply two Fp32 type objects, return a Fp32 type object
        Fp32 operator*(const Fp32& rhs) {
            Fp32 result(0.0f); // initialize the result to 0
            result.sign = sign ^ rhs.sign; // calculate the sign of the result
            result.exponent = exponent + rhs.exponent - 127; // calculate the exponent of the result
            result.fraction = (fraction | 0x800000) * (rhs.fraction | 0x800000); // calculate the significand of the result
            if (result.fraction & 0x1000000000000) { // if the significand of the result exceeds 24 bits
                result.fraction >>= 1; // shift right by one bit
                result.exponent++; // increment the exponent
            }
            if (result.exponent > 254) { // if the exponent of the result exceeds the maximum value
                result.exponent = 255; // set the exponent to infinity
                result.fraction = 0; // set the significand to 0
            } else if (result.exponent < 1) { // if the exponent of the result is less than the minimum value
                result.exponent = 0; // set the exponent to 0
                result.fraction = 0; // set the significand to 0
            } else { // if the exponent of the result is within the valid range
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
};

// test code
int main() {
    Fp32 a(1.2f); // create a Fp32 type object, value is 1.2
    Fp32 b(3.4f); // create a Fp32 type object, value is 3.4
    Fp32 c = a * b; // call the multiplication operation of Fp32 class
    cout << c << endl; // output 4.08
    return 0;
}
