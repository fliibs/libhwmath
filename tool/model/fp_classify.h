#ifndef classify_h
#define classify_h
#include "struct.h"
#include <cmath>
//basic
bool        isZeroExpo(const FpBase& f);
bool        isMaxExpo(const FpBase& f);
bool        isZeroMant(const FpBase& f);


//fp 10 ops
bool        isNegInf(const FpBase& f);
bool        isNegNor(const FpBase& f);
bool        isNegSubnor(const FpBase& f);
bool        isNegZero(const FpBase& f);
bool        isPosZero(const FpBase& f);
bool        isPosNor(const FpBase& f);
bool        isPosSubnor(const FpBase& f);
bool        isPosInf(const FpBase& f);
bool        isSNan(const FpBase& f);
bool        isQNan(const FpBase& f);

//advance op
bool        isInf(const FpBase& f);
bool        isSubNor(const FpBase& f);
bool        isZero(const FpBase& f);
bool        isNor(const FpBase& f);
bool        isNan(const FpBase& f);

#endif //classify_h