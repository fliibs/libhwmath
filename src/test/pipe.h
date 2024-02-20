#ifndef pipe_h
#define pipe_h
#include "reflib.h"
#include <boost/utility.hpp>
#include <boost/process.hpp>
namespace bp = boost::process;                      
class Pipe { //pipe can be place outside now
public:
    bp::opstream to_sv  ;
    bp::ipstream from_sv;
    std::string line    ;
    bp::child mul_sv    ;
    Pointer P1          ;
    bool test_rtl       ;
    uint32_t round_mode ;  //0:zero 1:negative 2:positive 3:nearest                        
    Pipe(const std::string& executable) : mul_sv(executable, bp::std_in < to_sv, bp::std_out > from_sv){
    }
    int test_once(float num1,float num2,float num3);
    void print(uint32_t int_in);
    uint32_t verif_inout(uint32_t a,uint32_t b);    
    Fp32 c_model_res(Fp32 a,Fp32 b,Fp32 c);
};
#endif