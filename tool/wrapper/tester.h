#ifndef TESTER_H
#define TESTER_H

class Tester{
public:    
    bool        run(int argc, char **argv);
    bool        argc_bool(int argc, char **argv, const std::string&prefix);
    std::string argc_string(int argc,char **argv,const std::string&prefix);
    int         argc_int(int argc,char **argv,const std::string&prefix);
    void        set_type(std::string type_str,FpBase* a);
};

#endif 