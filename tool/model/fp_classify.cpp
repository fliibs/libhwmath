#include "fp_classify.h"
#include "cal.h"
#include "../test/info.h"

// basic
bool isZeroExpo (const FpBase& f) {
    bool res;
    return static_cast<bool>(f.expo==0);
}

bool isMaxExpo (const FpBase& f) {
    // return  expo_whe_max(f.expo,f.expo_w);
    // int expo_mask = (1 << f.expo_w) - 1;
    // return (f.expo & expo_mask) == expo_mask;
    bool res;
    if(f.expo_w==5 && f.mant_w==10) {
        res = (f.expo == 0x1F);
    } 
    else if(f.expo_w==8 && f.mant_w==23) {
        res =  (f.expo == 0xFF);
    }
    else if(f.expo_w==11 && f.mant_w==52) {
        res = (f.expo == 0x7FF) ;
    } 
    return res;
}

bool isZeroMant (const FpBase& f) {
    // mp::cpp_int zero_cpp=0;
    // return  static_cast<bool>(f.mant==zero_cpp);
    bool res;
    res = (f.mant == 0);
    return res;
}

//Part2 : ten ops based on fparith.md
bool        isNegInf(const FpBase& f) {
    // if(f.expo_w==5 && f.mant_w==10) {
    //     return (f.sign==1) && (f.expo == 0x1F) && (f.mant==0);
    // } 
    // else if(f.expo_w==8 && f.mant_w==23) {
    //     return (f.sign==1) && (f.expo == 0xFF) && (f.mant==0);
    // }
    // else if(f.expo_w==11 && f.mant_w==52) {
    //     return (f.sign==1) && (f.expo == 0x7FF) && (f.mant==0);
    // }
    if (f.sign==1)
    {
        return (isMaxExpo(f) && isZeroMant(f));
    } else {
        return false;
    }
};

bool        isNegNor(const FpBase& f) {
    bool res;
    if(f.sign==1) {
        return res = (!isNegInf) && (!isNegZero) && (!isNan);
        // return (!isInf) && (!isZero) && (!isNan);
    } else {
        return false;
    }
};
bool        isNegSubnor(const FpBase& f) {
    if(f.sign==1) {
        return (static_cast<bool>(f.expo==0) && !static_cast<bool>(f.mant==0));
    } else {
        return false;
    }
};

bool        isNegZero(const FpBase& f) {
    bool res;
    if(f.sign==1) {
        return (static_cast<bool>(f.expo==0) && static_cast<bool>(f.mant==0));
        // return res = (isZeroExpo(f) && isZeroMant(f));
    } else {
        return false;
    }
};

bool        isPosZero(const FpBase& f) {
    bool res;
    if(f.sign==0) {
        // return (static_cast<bool>(f.expo==0) && static_cast<bool>(f.mant==0));
        return res = (isZeroExpo(f) && isZeroMant(f));
    } else {
        return false;
    }
};

bool        isPosNor(const FpBase& f) {
    bool res;
    if(f.sign==0) {
        // return (!isNegInf && !isPosInf) && (!isNegZero && !isPosZero) && (!isSNan && !isQNan);
        // return (!isPosInf) && (!isPosZero) && (!isNan);
        return res = (!isInf) && (!isZero) && (!isNan);
    } else {
        return false;
    }
};

bool        isPosSubnor(const FpBase& f) {
    if(f.sign==0) {
        return (static_cast<bool>(f.expo==0) && !static_cast<bool>(f.mant==0));
    }
};

bool        isPosInf(const FpBase& f) {
    bool res;
    if (f.sign==0)
    {
        return (isMaxExpo(f) && isZeroMant(f));
    }
    
    // if(f.expo_w==5 && f.mant_w==10) {
    //     res =(f.sign==0) && (f.expo == 0x1F) && (f.mant==0) ;
        
    // } 
    // else if(f.expo_w==8 && f.mant_w==23) {
    //     res = (f.sign==0) && (f.expo == 0xFF) && (f.mant==0) ;
    // }
    // else if(f.expo_w==11 && f.mant_w==52) {
    //      res= (f.sign==0) && (f.expo == 0x7FF) && (f.mant==0) ;
    // }
    return res;
};


bool        isSNan(const FpBase& f) {
    bool res;
    res = expo_whe_max(f.expo,f.expo_w) && !static_cast<bool>(f.mant==0) && !static_cast<bool>(f.mant>>(f.mant_w-1));
    return res;
};

bool        isQNan(const FpBase& f) {
    return (expo_whe_max(f.expo,f.expo_w) && !static_cast<bool>(f.mant==0) && static_cast<bool>(f.mant>>(f.mant_w-1)));
};


//Part 3: advance op based on part2
bool        isInf(const FpBase& f) {
    bool res = isNegInf(f) || isPosInf(f);
    return res;
};

bool        isSubNor(const FpBase& f) {
    bool res = isNegSubnor(f) || isPosSubnor(f);
    return res;
};

bool        isZero(const FpBase& f) {
    bool res = isNegZero(f) || isPosZero(f);
    return res;
};

bool        isNor(const FpBase& f) {
    bool res = isNegNor(f) || isPosNor(f);
    return res;
};

bool        isNan(const FpBase& f) {
    bool res = isSNan(f) || isQNan(f);
    return res;
};