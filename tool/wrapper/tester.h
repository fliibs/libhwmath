#ifndef TESTER_H
#define TESTER_H
#include <string>

#define SET_RND_MODE(test_rnd_mode,num,c_rnd_mode) \
    if(argc_bool(argc,argv,test_rnd_mode)){ \
        std::cout<<"---------------------------------------"<<std::endl; \
        std::cout<<"--------"<<test_rnd_mode<<"--------"<<std::endl; \
        std::cout<<"---------------------------------------"<<std::endl; \
        round_mode=num; \
        fesetround(c_rnd_mode); \
    }

class Tester{
public:    
    bool run(int argc, char **argv);
    bool argc_bool(int argc, char **argv, const std::string&prefix);
    std::string argc_string(int argc,char **argv,const std::string&prefix);
    std::string argc_str_same(int argc,char **argv,const std::string&prefix); 
};

#endif 