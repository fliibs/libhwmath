#ifndef pipe_h
#define pipe_h
#include <boost/utility.hpp>
#include <boost/process.hpp>
#include "../model/struct.h"
#include "../model/cal.h"
namespace bp = boost::process;

class Pipe{
public:
    bp::opstream to_sv  ;
    bp::ipstream from_sv;
    std::string line    ;
    bp::child model_rtl ;
    bool test_rtl       ;
    Pipe(const std::string& executable): model_rtl(executable,bp::std_in < to_sv,bp::std_out > from_sv){
    }
    FpBase verif_inout(FpBase a,FpBase b,FpBase c,int rnd_mode);
};

#endif