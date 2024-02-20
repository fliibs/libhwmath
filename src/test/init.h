#ifndef init_h
#define init_h
#include "fp32.h"

#define POINTER_SET(pointer,function) \
    float Ini_input::function(int argc, char **argv, const std::string&prefix){ \
        if(processArguments(argc,argv,(prefix+"INF"))) \
        {   \
            pointer= &Ini_input::get_rand_inf; \
        }   \
        if(processArguments(argc,argv,(prefix+"ZERO"))) \
        {   \
            pointer= &Ini_input::get_rand_zero; \
        }   \
        if(processArguments(argc,argv,(prefix+"REG"))) \
        {   \
            pointer= &Ini_input::get_rand_reg; \
        }   \
        if(processArguments(argc,argv,(prefix+"NAN"))) \
        {   \
            pointer= &Ini_input::get_rand_nan; \
        }   \
    }
    
class Ini_input{
public:
    typedef float (Ini_input::*RandFunc_Ptr)();
    Ini_input();
    float get_rand_reg();
    float get_rand_nan();
    float get_rand_zero();
    float get_rand_inf();
    float get_accurate_float(const Fp32 input);
    float a_get_rand();
    float b_get_rand();
    float c_get_rand();

    uint32_t parseArgument_u32(int argc,char **argv,const std::string&prefix);
    bool processArguments(int argc, char **argv, const std::string&prefix);
    float init_a_func(int argc, char **argv,const std::string&prefix);
    float init_b_func(int argc, char **argv,const std::string&prefix);
    float init_c_func(int argc, char **argv,const std::string&prefix);

private:
    RandFunc_Ptr a_get_rand_ptr;
    RandFunc_Ptr b_get_rand_ptr;
    RandFunc_Ptr c_get_rand_ptr;
};


#endif //init_h