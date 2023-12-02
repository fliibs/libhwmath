

#include <iostream>

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

    Fp32 mul(const Fp32 &rhs) {

        Fp32 res;

        res.sign = !(sign ^ rhs.sign);

        
        res.exponent = exponent + rhs.exponent;
        
        return res;
    }




    Fp32 operator* (const Fp32 &rhs) {
        return mul(rhs);
    }


};


int main(int argc, char **argv)
{
    Fp32 num1,num2,res;

    num1.sign = 1;
    num2.sign = 1;

    res = num1 * num2;
    res.display();

    //num1.display();

    cout << "hello world" << endl;
    cout << "hello world" << endl;
    cout << "hello world" << endl;
    cout << "hello world" << endl;

    return 0;
}