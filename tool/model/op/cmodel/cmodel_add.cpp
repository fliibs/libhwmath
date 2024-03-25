#include "cmodel_add.h"

std::array<int,5> cmodel_add::add(const FpBase& a, const FpBase& b, const FpBase& c, const int& rnd_mode, FpBase *res){
    std::array<int,5> status;
    (*res).sign = a.sign;
    (*res).expo = a.expo;
    (*res).mant = a.mant;
    return status ;
}
