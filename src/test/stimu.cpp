#include"stimu.h"

Ini_input::Ini_input(){
    a_get_rand_ptr=&Ini_input::get_rand_reg;
    b_get_rand_ptr=&Ini_input::get_rand_reg;
    c_get_rand_ptr=&Ini_input::get_rand_reg;
}

float Ini_input::get_rand_reg() {
    float res;
    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_int_distribution<uint32_t> distr(std::numeric_limits<uint32_t>::min(), std::numeric_limits<uint32_t>::max());
    uint32_t tmp1 = distr(eng);
    res = *(float*)&tmp1;
    return res;
}

float Ini_input::get_rand_nan(){
    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_int_distribution<int> distr(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());    float floatvalue;
    int sign = distr(eng);
    int mantissa=distr(eng);
    uint32_t intvalue=(sign<<31)+(0xff<<23)+(mantissa&0x7fffff);
    printf("intvalue is %b\n",intvalue);
    std::memcpy(&floatvalue, &intvalue, sizeof(float));
    return floatvalue;
}

float Ini_input::get_rand_zero(){
    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_int_distribution<int> distr(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
    int sign = distr(eng);
    uint32_t intvalue = (static_cast<uint32_t>(sign)<<31);
    float floatvalue;
    std::memcpy(&floatvalue,&intvalue,sizeof(float));
    return floatvalue;
}

float Ini_input::get_rand_inf(){
    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_int_distribution<int> distr(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
    int sign = distr(eng);
    uint32_t intvalue = (static_cast<uint32_t>(sign)<<31)+(0xff<<23);
    float floatvalue;
    std::memcpy(&floatvalue,&intvalue,sizeof(float));
    return floatvalue;
}

float Ini_input::get_accurate_float(const Fp32 input){
    uint32_t intValue=(uint32_t(input.sign<<31))|(uint32_t(input.exponent)<<23)|(uint32_t(input.mantissa&0x7fffff));
    float floatvalue;
    std::memcpy(&floatvalue, &intValue, sizeof(float));
    return floatvalue;
}

float Ini_input::a_get_rand(){
    float res = (this->*a_get_rand_ptr)();
    return res;
}

float Ini_input::b_get_rand(){
    float res = (this->*b_get_rand_ptr)();
    return res;
}

float Ini_input::c_get_rand(){
    float res = (this->*c_get_rand_ptr)();
    return res;
}

uint32_t Ini_input::parseArgument_u32(int argc,char **argv,const std::string&prefix){
    uint32_t variable=0;
    for (int i = 0; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg.compare(0, prefix.size(), prefix) == 0) {
            std::cout << arg << std::endl;
            std::string value_str = arg.substr(prefix.size());
            std::istringstream(value_str) >> variable;
        }
    }
    return variable;
}

bool Ini_input::processArguments(int argc, char **argv, const std::string&prefix){
    bool value = false;
    for (int i = 0; i < argc; ++i) {
        std::string arg = argv[i];
        if(arg.compare(0,prefix.size(),prefix)==0){
            value=true;
        }
    }
    return value;
}

POINTER_SET(a_get_rand_ptr,init_a_func);

POINTER_SET(b_get_rand_ptr,init_b_func);

POINTER_SET(c_get_rand_ptr,init_c_func);