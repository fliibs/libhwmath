#include "../model/struct.h"
// #include "../if/pipe.h"
#include "../test/stimus.h"
#include "tester.h"
#include <fenv.h>
#include "../model/my_ref.h"

bool Tester::run(int argc, char **argv){
//get stimus
    Stimus<Fp32> stimu1;
    std::string in1_stimuls = argc_string(argc,argv,"in1=");
    std::string in2_stimuls = argc_string(argc,argv,"in2=");
    std::string in3_stimuls = argc_string(argc,argv,"in3=");
    auto in1_func=stimu1.stimus_table.find(in1_stimuls);
    auto in2_func=stimu1.stimus_table.find(in2_stimuls);
    auto in3_func=stimu1.stimus_table.find(in3_stimuls);

//build round mode 
    int round_mode=0;
    SET_RND_MODE("TEST_RND_NEAREST",3,FE_TONEAREST);
    SET_RND_MODE("TEST_RND_ZERO",0,FE_TOWARDZERO);
    SET_RND_MODE("TEST_RND_POS",2,FE_UPWARD);
    SET_RND_MODE("TEST_RND_NEG",1,FE_DOWNWARD);

//if to ref：from ip to ref

//get ref
    My_ref<float> my_ref1;
    std::string ref_string;
    auto op_func = my_ref1.functionTable.end();
    for(int i=0;i<argc;i++){
        ref_string = argv[i];
        op_func = my_ref1.functionTable.find(ref_string);
        if(op_func != my_ref1.functionTable.end())
        {
            break;
        }
        else{
            op_func = my_ref1.functionTable.find("mul");
        }
    }
    float res=op_func->second(2.0,3.0,1.0);
    printf("res is %f\n",res);

}

bool Tester::argc_bool(int argc, char **argv, const std::string&prefix){
    bool value = false;
    for (int i = 0; i < argc; ++i) {
        std::string arg = argv[i];
        if(arg.compare(0,prefix.size(),prefix)==0){
            value=true;
        }
    }
    return value;
}

std::string Tester::argc_string(int argc,char **argv,const std::string&prefix){
    std::string res="zero";
    for (int i =0 ; i<argc ; i++){
        std::string arg = argv[i];
        if(arg.compare(0,prefix.size(),prefix)==0)
            res = arg.substr(prefix.size());
    }
    return res;
}

std::string Tester::argc_str_same(int argc,char **argv,const std::string&prefix){
    std::string res="mul";
    for (int i =0 ; i<argc ; i++){
        std::string arg = argv[i];
        if(arg.compare(0,prefix.size(),prefix)==0)
            res = arg;
    }
    return res;

}
