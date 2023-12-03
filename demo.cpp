#include <iostream>
#include <cmath>
using namespace std;

class Fp32 {
    public:
        bool sign; // 符号位
        uint32_t exponent; // 指数位
        uint64_t fraction; // 小数位

        // 构造函数，将一个浮点数转换为 Fp32 类型
        Fp32(float x) {
            uint32_t bits = *(uint32_t*)&x; // 获取浮点数的二进制表示
            sign = bits >> 31; // 获取符号位
            exponent = (bits >> 23) & 0xff; // 获取指数位
            fraction = bits & 0x7fffff; // 获取小数位
        }

        // 转换函数，将一个 Fp32 类型转换为浮点数
        operator float() {
            uint32_t bits = 0; // 初始化浮点数的二进制表示
            bits |= sign << 31; // 设置符号位
            bits |= exponent << 23; // 设置指数位
            bits |= fraction; // 设置小数位
            float x = *(float*)&bits; // 获取浮点数的值
            return x;
        }

        // 加法运算，将两个 Fp32 类型的对象相加，返回一个 Fp32 类型的对象
        Fp32 operator+(const Fp32& rhs) {
            float x = *this; // 将左操作数转换为浮点数
            float y = rhs; // 将右操作数转换为浮点数
            float z = x + y; // 用浮点数的加法运算符相加
            Fp32 result(z); // 将结果转换为 Fp32 类型
            return result;
        }

        // 减法运算，将两个 Fp32 类型的对象相减，返回一个 Fp32 类型的对象
        Fp32 operator-(const Fp32& rhs) {
            float x = *this; // 将左操作数转换为浮点数
            float y = rhs; // 将右操作数转换为浮点数
            float z = x - y; // 用浮点数的减法运算符相减
            Fp32 result(z); // 将结果转换为 Fp32 类型
            return result;
        }

        // 乘法运算，将两个 Fp32 类型的对象相乘，返回一个 Fp32 类型的对象
        Fp32 operator*(const Fp32& rhs) {
            float x = *this; // 将左操作数转换为浮点数
            float y = rhs; // 将右操作数转换为浮点数
            float z = x * y; // 用浮点数的乘法运算符相乘
            Fp32 result(z); // 将结果转换为 Fp32 类型
            return result;
        }

        // 除法运算，将两个 Fp32 类型的对象相除，返回一个 Fp32 类型的对象
        Fp32 operator/(const Fp32& rhs) {
            float x = *this; // 将左操作数转换为浮点数
            float y = rhs; // 将右操作数转换为浮点数
            float z = x / y; // 用浮点数的除法运算符相除
            Fp32 result(z); // 将结果转换为 Fp32 类型
            return result;
        }
};

// 测试代码
int main() {
    Fp32 a(1.2f); // 创建一个 Fp32 类型的对象，值为 1.2
    Fp32 b(3.4f); // 创建一个 Fp32 类型的对象，值为 3.4
    Fp32 c = a + b; // 调用 Fp32 类的加法运算
    Fp32 d = a - b; // 调用 Fp32 类的减法运算
    Fp32 e = a * b; // 调用 Fp32 类的乘法运算
    Fp32 f = a / b; // 调用 Fp32 类的除法运算
    cout << c << endl; // 输出 4.6
    cout << d << endl; // 输出 -2.2
    cout << e << endl; // 输出 4.08
    cout << f << endl; // 输出 0.352941
    return 0;
}
