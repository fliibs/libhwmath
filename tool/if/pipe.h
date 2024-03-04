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

    template<typename T_FP_a,typename T_FP_b, typename T_FP_c,typename T_FP_res>
    T_FP_res verif_inout(T_FP_a a,T_FP_b b,T_FP_c c,int rnd_mode){
        T_FP_res res;
        uint64_t value;
        uint64_t value_a;
        uint64_t value_b;
        uint64_t value_c;
        value_a=((a.sign<<(a.expo_w+a.mant_w))+(a.expo<<a.mant_w)+a.mant);
        value_b=((b.sign<<(b.expo_w+b.mant_w))+(b.expo<<b.mant_w)+b.mant);
        value_c=((c.sign<<(c.expo_w+c.mant_w))+(c.expo<<c.mant_w)+c.mant);

        // printf("into verif_inout\n");
        // std::cout<<"into rtl "<<value_a<<" "<<value_b<<" "<<value_c<<" "<<rnd_mode<<std::endl;
        // printf("value_a  is %lb\n",value_a );
        // printf("value_b  is %lb\n",value_b );
        // printf("value_c  is %lb\n",value_c );

        to_sv<<value_a<<" "<<value_b<<" "<<value_c<<" "<<rnd_mode<<std::endl;
        while(model_rtl.running()&&std::getline(from_sv,line)){
            std::cout<<line<<std::endl;
            if(line.compare(0,4,"res=")==0){
                std::string value_str=line.substr(4);
                std::istringstream(value_str) >> value;
                break;
            }
        }
        // printf("value is %lb\n",value);
        // printf("value is %lu\n",value);
        res.sign  = value >> (res.expo_w + res.mant_w);
        uint64_t expo_mask;
        uint64_t mant_mask;
        expo_mask = get_mask(res.expo_w);
        mant_mask = get_mask(res.mant_w);
        // printf("expo_mask is %d\n",expo_mask);
        // printf("mant_mask is %lb\n",mant_mask);
        res.expo  = (value >> res.mant_w) & expo_mask;
        // printf("value shift is %lb\n",value >> res.mant_w);
        res.mant  = value&mant_mask; 
        // printf("value     is %lb\n",value);
        // printf("mant_mask is %lb\n",mant_mask);
        // printf("res.mant  is %lb\n",res.mant );
        return res;
    }
};

#endif