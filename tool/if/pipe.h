#ifndef pipe_h
#define pipe_h
#include <boost/utility.hpp>
#include <boost/process.hpp>
#include "../model/struct.h"
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

    uint64_t get_mask(const int&width){
        uint64_t res=0;
        for(int i=0;i<width;i++){
            res = 1+(res<<1);
        }   
        return res;
    }

    template<typename T>
    T verif_inout(T a,T b,int rnd_mode){
        T res;
        uint64_t value;
        printf("into verif_inout\n");
        to_sv<<a.sign<<a.expo<<a.mant<<" "<<b.sign<<b.expo<<b.mant<<" "<<rnd_mode<<std::endl;
        if(line.compare(0,4,"res=")==0){
            std::string value_str=line.substr(4);
            std::istringstream(value_str) >> value;
            break;
        }
        res.sign  = value >> (res.expo_w + res.mant_w);
        uint64_t expo_mask;
        uint64_t mant_mask;
        expo_mask = get_mask(res.expo_w);
        mant_mask = get_mask(res.mant_w);
        res.expo  = (value >> res.mant) & expo_mask;
        res.mant  = (value >> res.expo) & mant_mask;
        
    return res;
    }
};

#endif