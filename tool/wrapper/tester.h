#ifndef TESTER_H
#define TESTER_H

template<typename T_FP_a,typename T_FP_b, typename T_FP_c,typename T_FP_res, typename T_flt>
class Tester{
public:    
    bool        run(int argc, char **argv);
    bool        argc_bool(int argc, char **argv, const std::string&prefix);
    std::string argc_string(int argc,char **argv,const std::string&prefix);
    int         argc_int(int argc,char **argv,const std::string&prefix);
};
#include "tester_impl.h"

#endif 