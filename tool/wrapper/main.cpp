#include "../model/struct.h"
#include "tester.h"
#include <type_traits>
#include <string>
#include <typeinfo>

int main(int argc, char **argv) {
    Tester t1;
    int fail=t1.run(argc,argv);
    return fail;
}

