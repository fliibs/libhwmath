#include "../model/struct.h"
#include "tester.h"
#include <type_traits>
#include <string>
#include <typeinfo>

int main(int argc, char **argv) {
    Tester t1(argc,argv);
    int fail=t1.run();
    return fail;
}

