#include "cmodel_min.h"
#include "../../fp_classify.h"
// bool isNAN(const FpBase& f) {
//     bool res;
//     if ((f.expo == 0xFF) && (f.mant != 0)) {
//         return res=true;
//     } else {
//         return res=false;
//     }
// }

// bool isDenormal(const FpBase& f) {
//     bool res;
//     if ((f.expo == 0) && (f.mant != 0)) {
//         return res=true;
//     } else {
//         return res=false;
//     }
// }

// bool isInfinity(const FpBase& f) {
//     bool res;
//     if ((f.expo == 0xFF) && (f.mant == 0)) {
//         return res=true;
//     } else {
//         return res=false;
//     }
// }

// bool isZero(const FpBase& f) {
//     bool res;
//     if ((f.expo == 0) && (f.mant == 0)) {
//         return res=true;
//     } else {
//         return res=false;
//     }
// }

//=============comments==============//
//nan exception:
//1.if a b are both +nan or -nan,return first
//2.if one of inputs is nan and another is -nan,return first
//3.if onn of inputs is nan but another is not,return another

std::array<int,5> cmodel_min::min(const FpBase& a,const FpBase& b,const FpBase& c,const int& rnd_mode,FpBase *result)
{
{
    VariablesTable.clear();
    std::array<int,5> arr={0,0,0,0,0}; 
    uint32_t    a_expo        = a.expo;
    uint32_t    b_expo        = b.expo;
    mp::cpp_int a_mant        = a.mant;
    mp::cpp_int b_mant        = b.mant;
    bool        a_sign        = a.sign;
    bool        b_sign        = b.sign;
    
    std::cout << "a expo w is: "<< a.expo_w << std::endl;
    std::cout << "a expo  is: "<< a.expo << std::endl;
    std::cout << "b expo w is: "<< b.expo_w << std::endl;
    std::cout << "b expo is: "<< b.expo << std::endl;

    info1.debug_printf("--------------------------------");
    info1.debug_printf("-----------into c model---------");
    info1.debug_printf("--------------------------------");

    bool a_min;
    bool b_min;
    bool in_equal;
    bool a_expo_is_0;
    bool b_expo_is_0;
    a_expo_is_0  = static_cast<bool>(a_expo==0);
    b_expo_is_0  = static_cast<bool>(b_expo==0);
    
    bool a_expo_is_max;
    bool b_expo_is_max;
    a_expo_is_max = expo_whe_max(a_expo,a.expo_w);
    b_expo_is_max = expo_whe_max(b_expo,b.expo_w);
    
    bool a_mant_is_0;
    bool b_mant_is_0;
    mp::cpp_int zero_cpp=0;
    a_mant_is_0   = static_cast<bool>(a_mant==zero_cpp);
    b_mant_is_0   = static_cast<bool>(b_mant==zero_cpp);

    bool a_is_0;
    bool b_is_0;
    bool a_is_nan;
    bool b_is_nan;
    bool a_is_inf;
    bool b_is_inf;
    bool a_is_pos_inf;
    bool b_is_pos_inf;
    bool a_is_neg_inf;
    bool b_is_neg_inf;
    a_is_0       = (a_expo_is_0)  &&(a_mant_is_0);
    b_is_0       = (b_expo_is_0)  &&(b_mant_is_0);
    a_is_inf     = (a_expo_is_max)&&(a_mant_is_0);
    b_is_inf     = (b_expo_is_max)&&(b_mant_is_0);
    a_is_nan     = (a_expo_is_max)&&(!a_mant_is_0);
    b_is_nan     = (b_expo_is_max)&&(!b_mant_is_0);
    a_is_pos_inf = (a_sign==-1) && a_is_inf;
    b_is_pos_inf = (b_sign==-1) && b_is_inf;
    a_is_neg_inf = (a_sign== 1) && a_is_inf;
    b_is_neg_inf = (b_sign== 1) && b_is_inf;

    bool a_is_q;
    bool b_is_q;
    a_is_q             = static_cast<bool>(a.mant>>(a.mant_w-1));
    // a_is_q = static_cast<bool>(a.mant & (1 << (a.mant_w - 1)));

    std::cout << "a mant w is:" << a.mant_w<< std::endl;
    std::cout << "a_is_q:" << a_is_q<< std::endl;
    // b_is_q = static_cast<bool>(b.mant & (1 << (b.mant_w - 1)));

    b_is_q             = static_cast<bool>(b.mant>>(b.mant_w-1));

    bool a_is_s_nan;
    bool b_is_s_nan;
    bool in_is_s_nan;
    a_is_s_nan      = (!a_is_q) && a_is_nan;
    
    std::cout << "a_is_nan is:" << a_is_nan<< std::endl;
    std::cout << "a_is_s_nan is:" << a_is_s_nan<< std::endl;

    b_is_s_nan      = (!b_is_q) && b_is_nan;
    in_is_s_nan     = a_is_s_nan || b_is_s_nan;

    bool r_is_nan;
    bool in_is_nan;
    bool is_inf_nan;
    bool r_is_0nan;
    FpBase temp_a;
    FpBase temp_b;
    temp_a = a;
    temp_b = b;
    // temp_a.sign = a.sign;
    // temp_a.expo = a.expo;
    // temp_a.mant = a.mant;

    // temp_b.sign = b.sign;
    // temp_b.expo = b.expo;
    // temp_b.mant = b.mant;

    in_is_nan       = a_is_nan  || b_is_nan;
    is_inf_nan      = in_is_nan || a_is_inf || b_is_inf;
    
    bool  a_is_nor;
    bool  b_is_nor;
    a_is_nor        = !a_is_inf && !a_is_0 && !a_is_nan;
    b_is_nor        = !b_is_inf && !b_is_0 && !b_is_nan;

    bool a_is_n0;
    bool b_is_n0;
    a_is_n0=!a_is_0;
    b_is_n0=!b_is_0;

    echo(a_expo);
    echo(b_expo);
    echo(a_mant);
    echo(b_mant);
    echo(a_is_n0);
    echo(b_is_n0);
    echo(a_is_nan);
    echo(b_is_nan);
    echo(a_is_q);
    echo(b_is_q);

    echo(in_is_nan);

    echo(is_inf_nan);
    echo(a_is_nor);
    echo(b_is_nor);

    echo(a_is_s_nan);
    echo(b_is_s_nan);

    bool in_both_pos_inf;
    bool in_both_neg_inf;
    in_both_pos_inf = a_is_pos_inf && b_is_pos_inf;
    in_both_neg_inf = a_is_neg_inf && b_is_neg_inf;

    a_min = false;
    // b_min = false;
    in_equal = false;


    // if (isNAN(a) && isNAN(b)) {
    //     std::cout << "a & b are both NAN" << std::endl;
    //     a_min = true;
    //     goto res_output;
    // } else if (isNAN(a) && !isNAN(b)) {
    //     std::cout << "a is NAN" << std::endl;
    //     a_min = true;
    //     goto res_output;
    // } else if (!isNAN(a) && isNAN(b)) {
    //     std::cout << "b is NAN" << std::endl;
    //     a_min = false;
    //     goto res_output;
    // }
    if (in_is_nan) {
        std::cout << "at least one input is NAN,return 1st input" << std::endl;
        if (a_is_nan && b_is_nan) {
            a_min = true;
        } else if (a_is_nan) {
            a_min = false;
        } else if (b_is_nan) {
            a_min = true;
        }
        goto res_output;
    } 
        
    if (isInf(a) && isInf(b)) {
        if (temp_a.sign == 0 && temp_b.sign == 0) { 
            a_min = true;  //a inf  b inf
        } else if (a.sign == 0 && b.sign == 1) {
            a_min = false; //a inf  b -inf
        } else if (a.sign == 1 && b.sign == 0) {
            a_min = true;  //a -inf b inf
        } else if (a.sign == 1 && b.sign == 1) {
            a_min = false; //a -inf b inf
        }
        goto res_output;
    } else if (isInf(a) && !isInf(b)) {
        if (a.sign == 0) { 
            a_min = false; // a inf
        } else if (a.sign == 1){
            a_min = true; // a -inf
        }
        goto res_output;
    } else if (!isInf(a) && isInf(b)) {
        if (b.sign == 0) { 
            a_min = true; // b inf
        } else if (b.sign == 1){
            a_min = false; // b -inf
        }
        goto res_output;
    } 

    // std::cout << "temp_a w(before) is: "<< a.expo_w << std::endl;
    // std::cout << "temp_a(before): is "<< a.expo << std::endl;
    // if (isDenormal(a)) {
    //     std::cout << "a is Denormal" << std::endl;
    //     temp_a.sign = a.sign;
    //     temp_a.expo = 0 ; 
    //     temp_a.mant = 0 ;
    // } else {
    //     std::cout << "a is not Denormal" << std::endl;
    //     temp_a.sign = a.sign;
    //     temp_a.expo = a.expo; 
    //     temp_a.mant = a.mant;
    // }
    // std::cout << "xxxxxxxxxxxxxx"<< std::endl;
    // std::cout << "temp_a w is: "<< a.expo_w << std::endl;
    // std::cout << "temp_a: is "<< a.expo << std::endl;

    // if (isDenormal(b)) {
    //     std::cout << "b is Denormal" << std::endl;
    //     temp_b.sign = b.sign;
    //     temp_b.expo = 0 ; 
    //     temp_b.mant = 0 ;
    // } else {
    //     temp_b.sign = b.sign;
    //     temp_b.expo = b.expo; 
    //     temp_b.mant = b.mant;
    // }
    

    if (isZero(temp_a) && isZero(temp_b)) {
        std::cout << "a&b are both 0" << std::endl;
        in_equal = 1;
        goto res_output;
    } 

    // uint32_t diff_expo = temp_a.expo - temp_b.expo;
    // if (diff_expo > 0) {
    //     temp_b.expo += diff_expo;
    //     temp_b.mant /= pow(2, diff_expo);
    // } else {
    //     temp_a.expo -= diff_expo;
    //     temp_a.mant /= pow(2, -diff_expo);
    // }

    // 比较a和b的大小
    std::cout << "compare values" << std::endl;
    if (temp_a.sign != temp_b.sign) {
        if (temp_a.sign < temp_b.sign) {
            std::cout << "a.sign < b.sign" << std::endl;
            a_min = false;
        } else {
            std::cout << "a.sign > b.sign" << std::endl;
            a_min = true;
        }
    } else {
        if (temp_a.expo != temp_b.expo) {
            if ((temp_a.sign==0) && (temp_b.sign==0)) {
                std::cout << "a&b are pos" << std::endl;
                if (temp_a.expo < temp_b.expo) {
                    std::cout << "a.expo < b.expo" << std::endl;
                    a_min = true;
                } else {
                    std::cout << "a.expo > b.expo" << std::endl;
                    a_min = false;
                }
            } else if ((temp_a.sign==1) && (temp_b.sign==1)) {
                std::cout << "a&b are neg" << std::endl;
                if (temp_a.expo < temp_b.expo) {
                    std::cout << "a.expo < b.expo" << std::endl;
                    a_min = false;
                } else {
                    std::cout << "a.expo > b.expo" << std::endl;
                    a_min = true;
                }
            }
            
        } else {
            std::cout << "a.expo == b.expo" << std::endl;
            if ((temp_a.sign==0) && (temp_b.sign==0)) {
                if (temp_a.mant < temp_b.mant) {
                    a_min = true;
                }
                else if(temp_a.mant > temp_b.mant) {
                    a_min = false;
                }
            } else if ((temp_a.sign==1) && (temp_b.sign==1)) {
                if (temp_a.mant < temp_b.mant) {
                    a_min = false;
                }
                else if(temp_a.mant > temp_b.mant) {
                    a_min = true;
                }
            }

            
        }
    }

    res_output:
    std::cout << "a_min is: "<< a_min << std::endl;
    std::cout << "b_min is: "<< b_min << std::endl;
    if (in_equal) {
        *result = temp_a;
        std::cout << "temp_a expo is: "<< temp_a.expo << std::endl;
        std::cout << "temp_a expow is: "<< temp_a.expo_w << std::endl;
    } else if (a_min) {
        std::cout << "temp_a expo is: "<< temp_a.expo << std::endl;
        std::cout << "temp_a expow is: "<< temp_a.expo_w << std::endl;
        *result = temp_a;
    } else if (!a_min) {
        std::cout << "temp_b expo is: "<< temp_b.expo << std::endl;
        std::cout << "temp_b expow is: "<< temp_b.expo_w << std::endl;
        *result = temp_b;
    }

    int expo_of;
    // expo_of = 31;
    expo_of = std::ceil(std::pow(2,(*result).expo_w))-1;
    if (((*result).expo_w == 5)) {
        expo_of = 31;
    } else if (((*result).expo_w == 7)) {
        expo_of = 127;
    } else if (((*result).expo_w == 11)) {
        expo_of = 2047;
    } 
    std::cout << "result's expo_w is: "<< (*result).expo_w << std::endl;
    std::cout << "expo_of is: "<< expo_of << std::endl;
    bool overflow;
    overflow = (a.expo >= expo_of) || (b.expo >= expo_of); 
    arr[4]          = a_is_s_nan || b_is_s_nan;
    // arr[4]          = a_is_nan || b_is_nan;
    std::cout << "a_is_snan is: "<< a_is_s_nan << std::endl;
    std::cout << "b_is_snan is: "<< b_is_s_nan << std::endl;
    arr[3]          = 0;
    std::cout << "overflow is: "<< overflow << std::endl;
    // arr[2]          = overflow && (!is_inf_nan);
    arr[2]          = 0;
    // arr[1]          = underflow && (!a_is_0) && (!b_is_0);
    arr[1]          = 0;
    bool  inexact;
    inexact         = 0;
    bool  inexact_of;
    inexact_of      = 0;
    // inexact_of      = a_is_nor && b_is_nor && overflow;
    // inexact_of      = a_is_nan || b_is_nan || a_is_inf || b_is_inf;
    
    std::cout << "Result sign is : " << result->sign << std::endl;
    std::cout << "Result expo is : " << result->expo << std::endl;
    std::cout << "Result mant is : " << result->mant << std::endl;

    
    std::cout << "inexact_of is: "<< inexact_of << std::endl;
    arr[0]          = 0;

    std::cout << "my arr[0] NX is: "<< arr[0] << std::endl;
    std::cout << "my arr[1] UF is: "<< arr[1] << std::endl;
    std::cout << "my arr[2] OF is: "<< arr[2] << std::endl;
    std::cout << "my arr[3] DZ is: "<< arr[3] << std::endl;
    std::cout << "my arr[4] NV is: "<< arr[4] << std::endl;
    
    std::array<int,5> status;
    status=arr;
    echo(status[4]); 
    echo(status[3]); 
    echo(status[2]); 
    echo(status[1]); 
    echo(status[0]); 
    return arr;
}




}






