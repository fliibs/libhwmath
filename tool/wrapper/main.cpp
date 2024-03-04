#include "../model/struct.h"
#include "tester.h"
#include <type_traits>
#include <string>
#include <typeinfo>

template <typename T>
std::string getTypeNameWithoutPrefix() {
    std::string typeName = typeid(T).name();
    size_t pos = typeName.find_first_not_of("0123456789");

    if (pos != std::string::npos) {
        typeName = typeName.substr(pos);
    }

    return typeName;
}

template <typename A,typename B,typename C,typename res,typename flt>
int runTester(int argc,char* argv[],int * meet){
    Tester<A,B,C,res,flt> t;
    int fail;
    std::string a_struct = t.argc_string(argc,argv,"in1_struct=");
    std::string b_struct = t.argc_string(argc,argv,"in2_struct=");
    std::string c_struct = t.argc_string(argc,argv,"in3_struct=");
    std::string o_struct = t.argc_string(argc,argv,"out_struct=");

    std::string A_s  =getTypeNameWithoutPrefix<A>();
    std::string B_s  =getTypeNameWithoutPrefix<B>();
    std::string C_s  =getTypeNameWithoutPrefix<C>();
    std::string res_s=getTypeNameWithoutPrefix<res>();

    if (a_struct == A_s && b_struct == B_s&&
        c_struct == C_s && o_struct == res_s) {
        *meet=1;
        fail=t.run(argc,argv);
    }
    else
        fail=0;
    return fail;
}

int main(int argc, char **argv) {
    int fail=0;
    int meet=0;
        fail = runTester<Fp32, Fp32, Fp32, Fp32, float>(argc, argv,&meet);
    if (meet == 0)
        fail = runTester<Fp32, Fp32, Fp64, Fp32, float>(argc, argv,&meet);
    if (meet == 0)
        fail = runTester<Fp32, Fp64, Fp32, Fp32, float>(argc, argv,&meet);
    if (meet == 0)
        fail = runTester<Fp64, Fp32, Fp32, Fp32, float>(argc, argv,&meet);
    if (meet == 0)
        fail = runTester<Fp32, Fp64, Fp64, Fp32, float>(argc, argv,&meet);
    if (meet == 0)
        fail = runTester<Fp64, Fp64, Fp32, Fp32, float>(argc, argv,&meet);
    if (meet == 0)
        fail = runTester<Fp64, Fp32, Fp64, Fp32, float>(argc, argv,&meet);
    if (meet == 0)
        fail = runTester<Fp64, Fp64, Fp64, Fp32, float>(argc, argv,&meet);
   
    if (meet == 0)
        fail = runTester<Fp32, Fp32, Fp32, Fp64, float>(argc, argv,&meet);
    if (meet == 0)
        fail = runTester<Fp32, Fp32, Fp64, Fp64, float>(argc, argv,&meet);
    if (meet == 0)
        fail = runTester<Fp32, Fp64, Fp32, Fp64, float>(argc, argv,&meet);
    if (meet == 0)
        fail = runTester<Fp64, Fp32, Fp32, Fp64, float>(argc, argv,&meet);
    if (meet == 0)
        fail = runTester<Fp32, Fp64, Fp64, Fp64, float>(argc, argv,&meet);
    if (meet == 0)
        fail = runTester<Fp64, Fp64, Fp32, Fp64, float>(argc, argv,&meet);
    if (meet == 0)
        fail = runTester<Fp64, Fp32, Fp64, Fp64, float>(argc, argv,&meet);
    if (meet == 0)
        fail = runTester<Fp64, Fp64, Fp64, Fp64, float>(argc, argv,&meet);

    return fail;
}

