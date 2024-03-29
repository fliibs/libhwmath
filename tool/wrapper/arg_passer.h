#ifndef PAR_PASSER_H
#define PAR_PASSER_H
#include <iostream>
#include <boost/program_options.hpp>
#include <string>
#include <sstream>
#include <iomanip>

#define set_arg_obj(arg_type,arg_vector,arg_name,num,arg_dft) \
{   \
    if(vm.count(arg_name)){     \
        arg_vector = vm[arg_name].as<arg_type>();  \
    }  \
    if(arg_vector.empty()){  \
        arg_vector = arg_type(num, arg_dft);  \
        std::cout <<"Fail to set "<<std::left<<std::setw(20)<<arg_name;  \
        std::cout << std::left<<std::setw(20)<<",default set is"<<arg_dft<<std::endl;  \
    }  \
    else{   \
        while(arg_vector.size()<num){  \
            arg_vector.push_back(arg_vector.back());  \
        }  \
        for(int i=0;i<num;i++){  \
            std::cout <<"Successfully set "<<std::left<<std::setw(15)<<arg_name<<i+1<<"::"<<std::setw(10)<<arg_vector[i]<<std::endl;  \
        }  \
    } \
} 

#define set_arg_single(arg_type,arg_single,arg_name,arg_dft) \
{   \
    if(vm.count(arg_name)){     \
        arg_single = vm[arg_name].as<arg_type>();  \
        std::cout <<"Successfully set "<<std::left<<std::setw(15)<<arg_name<<"::"<<std::setw(10)<<arg_single<<std::endl;  \
    }  \
    else    \
    {   \
        std::cout <<"Fail to set "<<std::left<<std::setw(20)<<arg_name;  \
        std::cout << std::left<<std::setw(20)<<",default set is"<<arg_dft<<std::endl;  \
    }   \
} 

namespace po = boost::program_options;
struct Arg{
    std::vector<std::string> corner    ;//corner1   corner2    corner3
    std::vector<std::string> type      ;//type1     type2      type3     type4   type5
    std::vector<std::string> checker   ;//checktp   checkmdl1  checkmdl2
    std::string              op        ;//op
    std::vector<int>         debug     ;//c_debug   rtl_debug  
    int                      test_times;
    int                      rnd_mode  ;
};

class arg_passer{
public:
    Arg  set_arg(int argc,char **argv);
};
#endif