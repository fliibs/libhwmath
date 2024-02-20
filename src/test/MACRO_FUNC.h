#ifndef MACRO_FUNC_H
#define MACRO_FUNC_H
#include "init.h"
#include <string>
#include <fenv.h>

#define SET_REFMODEL(operation, function) \
    if (init1.processArguments(argc, argv, operation)) { \
        pipe1.P1.function(); \
    }

#define RTL_COMMAND(model,option) \
    model = " +" #option; \
    if (init1.processArguments(argc,argv, #option)) \
        command = command + model; 

#define SET_RND_MODE(test_rnd_mode,num,c_rnd_mode) \
    if(init1.processArguments(argc,argv,test_rnd_mode)){ \
        std::cout<<"---------------------------------------"<<std::endl; \
        std::cout<<"--------"<<test_rnd_mode<<"--------"<<std::endl; \
        std::cout<<"---------------------------------------"<<std::endl; \
        round_mode=num; \
        pipe1.round_mode=round_mode; \
        pipe1.P1.round_mode=round_mode; \
        fesetround(c_rnd_mode); \
    }

#endif
