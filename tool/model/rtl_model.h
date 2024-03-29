#ifndef RTL_MODEL_H
#define RTL_MODEL_H
#include "../if/pipe.h"
class rtl_model{
public:
    //function pointer
    using Rtl_Ptr = std::function<std::array<int,5>(const FpBase&,const FpBase&,const FpBase&,const int&,FpBase*)>;
    //command
    std::string command;
    //pipe
    Pipe pipe1;
    //
    Rtl_Ptr rtl_func;

    rtl_model(int rtl_debug){
        std::string quiet = " -q";
        std::string debug = " +RTL_DEBUG";
        std::string simv_exe=SIMV_EXECUTABLE_PATH;
        if(rtl_debug)
            command = simv_exe + quiet + debug;
        else
            command = simv_exe + quiet ;
        pipe1=Pipe(command);
        rtl_func = std::bind(&Pipe::verif_inout, &pipe1, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
    }
    
    //default
    rtl_model(){}
};

#endif