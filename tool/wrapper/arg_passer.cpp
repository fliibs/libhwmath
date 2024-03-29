#include "arg_passer.h"

void strip_quotes(char* str) {
    if (str[0] == '"' && str[strlen(str) - 1] == '"') {
        memmove(str, str + 1, strlen(str)); 
        str[strlen(str) - 1] = '\0';        
    }
}

Arg arg_passer::set_arg(int argc,char **argv){
    Arg arg1;
    char **argv_new = new char*[argc];
    for(int i=0;i<argc;i++){
        int len    = strlen(argv[i]);
        argv_new[i] = new char[len + 1];
        strcpy(argv_new[i], argv[i]);
        strip_quotes(argv_new[i]);
    }

    po::options_description desc("Allowed options");
    desc.add_options()
        ("corner"    , po::value<std::vector<std::string>>()->multitoken(), "set Floating point corner")
        ("type"      , po::value<std::vector<std::string>>()->multitoken(), "set Floating point type")
        ("checker"   , po::value<std::vector<std::string>>()->multitoken(), "set Checker type and two input models")
        ("op"        , po::value<std::string>()                           , "set operator")
        ("debug"     , po::value<std::vector<int>>()->multitoken()        , "enable rtl debug[0] and c debug[1]")
        ("rnd_mode"  , po::value<int>()                                   , "set rnd_mode")
        ("test_times", po::value<int>()                                   , "set test_times")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
    
    set_arg_obj(std::vector<std::string>, arg1.corner    , "corner"    , 3, "reg"   );
    set_arg_obj(std::vector<std::string>, arg1.type      , "type"      , 4, "fp32"  );
    set_arg_obj(std::vector<std::string>, arg1.checker   , "checker"   , 3, "cmodel");
    set_arg_obj(std::vector<int>        , arg1.debug     , "debug"     , 2, 0       );

    set_arg_single(int          , arg1.rnd_mode  , "rnd_mode"  , 1);
    set_arg_single(int          , arg1.test_times, "test_times", 1);
    set_arg_single(std::string  , arg1.op        , "op"        , "mul");
    return arg1;
}

