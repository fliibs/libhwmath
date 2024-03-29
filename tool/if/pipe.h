#ifndef pipe_h
#define pipe_h
#include <boost/utility.hpp>
#include <boost/process.hpp>
#include "../model/struct.h"
#include "../model/cal.h"
#include <map>
namespace bp = boost::process;

class Pipe{
public:
    bp::opstream to_sv  ;
    bp::ipstream from_sv;
    std::string line    ;
    bp::child model_rtl ;

    //table
    std::map<std::string, mp::cpp_int> VariablesTable;
    void addVariable(const std::string &variableName,mp::cpp_int value);
    void removeVariable(const std::string &variableName);
    
    //func
    std::array<int,5> verif_inout(FpBase a,FpBase b,FpBase c,int rnd_mode,FpBase* result);
    
    //
    Pipe(const std::string& executable): model_rtl(executable,bp::std_in < to_sv,bp::std_out > from_sv){
    }
    
    //default
    Pipe(){
    }
};

#endif