#include "cal.h"
#include "fp32.h"

Fp32::Fp32() {
        sign = 0;
        exponent = 0;
        mantissa = 0;
    }
    // constructor, convert a float to Fp32 type
Fp32::Fp32(float x) {
        uint32_t bits = *(uint32_t*)&x; // get the binary representation of the float
        sign     = bits >> 31;          // get the sign bit
        exponent = (bits >> 23) & 0xff; // get the exponent bit
        mantissa = bits & 0x7fffff;     // get the mantissa bit
    }
    // conversion function, convert a Fp32 type to float
Fp32::operator float() {
        uint32_t bits = 0;          // initialize the binary representation of the float
        bits |= sign << 31;         // set the sign bit
        bits |= exponent << 23;     // set the exponent bit
        bits |= mantissa;           // set the mantissa bit
        float x = *(float*)&bits;   // get the float value
        return x;
    }

Fp32::operator uint32_t() {
        uint32_t bits = 0;          // initialize the binary representation of the float
        bits |= sign << 31;         // set the sign bit
        bits |= exponent << 23;     // set the exponent bit
        bits |= mantissa;           // set the mantissa bit
        return bits;
    }

uint32_t Fp32::to_uint32() {
        uint32_t bits = 0;          // initialize the binary representation of the float
        bits |= sign << 31;         // set the sign bit
        bits |= exponent << 23;     // set the exponent bit
        bits |= mantissa;           // set the mantissa bit
        return bits;
    }

void Fp32::print() {
    printf("value      : %12f\n", (float)*this);
    printf(" - sign    : %12ld\n",sign);
    printf(" - exponent: %12ld,%64lb, 2^(%d)\n",exponent,exponent,exponent-127);
    printf(" - mantissa: %12ld,%64lb, %f\n",mantissa,mantissa+ (1 << 23),(float)(mantissa + (1 << 23))/(1<<23));
    printf("\n");
    }

Fp32 Fp32::add(const Fp32 &a,const Fp32 &rhs,const Fp32 &dont_care){
    Fp32 result(0.0f);
    //--------------to determine 0, NAN and INF
    
    //-----------expand mant_a and mant_b to 48 bit
    bool a_expo_or   = a.exponent!=0               ;
    bool b_expo_or   = rhs.exponent!=0           ; 
    bool a_is_n0;
    bool b_is_n0;
    a_is_n0=(a.exponent!=0)||(a.mantissa!=0);
    b_is_n0=(rhs.exponent!=0)||(rhs.mantissa!=0);
    uint64_t mant_a_l;
    uint64_t mant_b_l;
    mant_a_l         = (((static_cast<uint32_t>(a_expo_or)) << 23) +a.mantissa)    <<23    ;
    mant_b_l         = (((static_cast<uint32_t>(b_expo_or)) << 23) +rhs.mantissa)<<23    ;
    D1.debug_printf("mant_a_l is %lb",mant_a_l);
    D1.debug_printf("mant_b_l is %lb",mant_b_l);

    //------------expo_1
    D1.debug_printf("---------------------------");
    D1.debug_printf("------into expo_1----------");
    D1.debug_printf("---------------------------");
    bool a_big;
    uint32_t a_31    = (a.exponent     << 23) + a.mantissa                   ;
    uint32_t b_31    = (rhs.exponent << 23) + rhs.mantissa               ; 
    a_big            = (a_31>=b_31)   ? 1              : 0               ;
    D1.debug_printf("a_big is %d",static_cast<int>(a_big))                  ;

    //------------sign_pre
    D1.debug_printf("---------------------------");
    D1.debug_printf("------into sign_pre--------");
    D1.debug_printf("---------------------------");
    bool     same_sign  ;
    bool     sign_with_m;
    bool     sign_neg   ;


    same_sign   =   (rhs.sign == a.sign)                ;

    sign_with_m =   (!same_sign) && (!(a_big ^ a.sign)) ; //sign is the same as minu-subtr;
    
    sign_neg    =   (!same_sign) && (a_big ^ a.sign)    ; //sign is the opposite as minu-subtr;

    // sign_1      =   a_big ? sign : rhs.sign           ;

    D1.debug_printf("same_sign is %d",static_cast<int>(same_sign))                   ;
    D1.debug_printf("sign_with_m is %u",static_cast<int>(sign_with_m))               ;
    D1.debug_printf("sign_neg is %u",static_cast<int>(sign_neg))                     ;
    
    //-----------mant_c and mant_s
    D1.debug_printf("---------------------------");
    D1.debug_printf("------into mant_c and mant_s---------");
    D1.debug_printf("---------------------------");
    uint64_t mant_s      ;
    uint64_t mant_c      ;
    uint32_t minu_e      ;
    uint32_t subtr_e     ;
    bool     minu_exp_or ;
    bool     subtr_exp_or;
    
    minu_e      = a_big ?  a.exponent     : rhs.exponent      ;
    minu_exp_or = a_big ? a_expo_or     : b_expo_or         ;
    subtr_e     = a_big ?  rhs.exponent : a.exponent          ;
    subtr_exp_or= a_big ? b_expo_or     : a_expo_or         ;

    uint32_t expo_1  = minu_e                               ;
    uint32_t shift_l = minu_e - subtr_e + static_cast<uint32_t>(!minu_exp_or) - static_cast<uint32_t>(!subtr_exp_or) ;

    D1.debug_printf("minu_expo_is_0 is %d",static_cast<uint32_t>(!minu_exp_or));
    D1.debug_printf("subtr_expo_is_0 is %d",static_cast<uint32_t>(!minu_exp_or));

    D1.debug_printf("expo_1 is %u",expo_1)  ;
    D1.debug_printf("subr_e is %u",subtr_e) ; 
    D1.debug_printf("shift_l is %u",shift_l);
    bool shift_more_than_48 ;
    shift_more_than_48= shift_l >= 47 ? 1:0;
    shift_l     = shift_l > 48 ? 48                     : shift_l                ; 
    mant_s      = a_big        ? (mant_b_l >> shift_l)  : (mant_a_l >> shift_l)  ;
    if(a_big){
        if((!mant_s)&&(mant_b_l))
            mant_s=1;
        else
            mant_s=mant_b_l>>shift_l;
    }
    else{
        if(!mant_s&&mant_a_l)
            mant_s=1;
        else
            mant_s=mant_a_l>>shift_l;
    }
    if(a_big){
        D1.debug_printf("mant_shift is b");
    }
    else{
        D1.debug_printf("mant_shift is a");
    }

    if(a_big && (a.sign||sign_neg)){
        mant_c = 1-mant_a_l ;
        D1.debug_printf("mant_comp is 1-a");
    } 
    else{
        if(a_big && (!a.sign))
        {
            mant_c = mant_a_l;
            D1.debug_printf("mant_comp is a");
        }
        else{
            if((!a_big)&&((rhs.sign)||sign_neg)){
                mant_c = 1 - mant_b_l;
                D1.debug_printf("mant_comp is 1-b");
            }
            else{
                mant_c = mant_b_l;
                D1.debug_printf("mant_comp is b");
            }
        }
    }
    uint64_t debug_refe_c=0x7fffff800000;

    D1.debug_printf("mant_s is %lu",mant_s)                    ;
    D1.debug_printf("mant_s is %lb",mant_s)                    ;
    D1.debug_printf("mant_c is %lu",mant_c)                    ;
    D1.debug_printf("mant_c is %lb",mant_c)                    ;
    // D1.debug_printf("mant_b is %lb",mant_b_l)                  ;
    D1.debug_printf("refe_c is %lb",debug_refe_c)              ;
    D1.debug_printf("shift_more_than_48 is %d",static_cast<int>(shift_more_than_48));


    //-----------mant_a and mant_b and mant_2
    D1.debug_printf("---------------------------");
    D1.debug_printf("------into mant_2---------");
    D1.debug_printf("---------------------------");
    uint64_t mant_a     ;
    uint64_t mant_b     ;
    uint64_t mant_1     ;

    mant_a      =   mant_s                                      ;
    mant_b      =   same_sign ? (a_big ? mant_a_l :mant_b_l)    :
                    mant_c                                      ;

    mant_1      =   mant_a    + mant_b                          ;
    
    bool mant_carry     ;
    bool mant_1_sign    ;
    bool sign_2         ;
    
    uint64_t mant_mid   ;
    uint32_t zero_nums  ;

    uint64_t mant_2     ;
    uint64_t expo_2     ;
    
    mant_1_sign = mant_1 & mantissa_49_bit                      ; 
    sign_2      = sign_neg             ? (!mant_1_sign) : mant_1_sign     ;
    mant_mid    = (sign_2 || sign_neg) ? (1-mant_1)     : mant_1          ;

    //leading zero detects,actually it can be detecting the leading 1 nums in mant_1
    mant_carry  = mant_mid & mantissa_48_bit                    ;
    mant_2      = mant_carry ?  mant_mid >> 1 : mant_mid        ;
    expo_2      = mant_carry ?  expo_1 + 1    : expo_1          ;

    zero_nums=get_the_zero_nums_add(mant_2);
    D1.debug_printf("the zero_nums is %d",zero_nums);

    if((mant_2&mantissa_48_bit)||mant_2&(mantissa_49_bit))
        printf("mant_2'48 and 49 bit is not always 0");
    if(expo_2<=zero_nums){
        D1.debug_printf("into expo<=zero_nums");
        uint32_t right_shift;
        right_shift= expo_2 ? (expo_2-1):expo_2; 
        mant_2 = mant_2<<right_shift;
        expo_2 = 0                 ;
    }
    else
    {
        D1.debug_printf("into expo>zero_nums");
        mant_2 = mant_2 <<zero_nums;
        expo_2 = expo_2 - zero_nums;
    }

    
    D1.debug_printf("mant_1 is %lb"     ,mant_1)                           ;
    D1.debug_printf("refe_c is %lb"    ,debug_refe_c)                      ;
    D1.debug_printf("mant_1_sign is %d",static_cast<int>(mant_1_sign))     ;
    D1.debug_printf("sign_2 is %d"     ,static_cast<int>(sign_2))          ;
    D1.debug_printf("mant_mid is %lb"  ,mant_mid)                          ;
    D1.debug_printf("refe_c   is %lb"  ,debug_refe_c)                      ;

    D1.debug_printf("mant_carry is %d",static_cast<int>(mant_carry))       ;

    D1.debug_printf("mant_2 is %lb"    ,mant_2)                            ;
    D1.debug_printf("refe_c is %lb"    ,debug_refe_c)                      ;

    D1.debug_printf("expo_2 is %u"     ,expo_2)                            ;
    D1.debug_printf("---------------------------");
    D1.debug_printf("------into rounding--------");
    D1.debug_printf("---------------------------");

    bool sign_c;
    sign_c  = same_sign ? a.sign : sign_2 ;
    bool     g          ;
    bool     r          ;
    bool     s          ;
    bool     s_bit_r    ;
    bool     rnd_carry  ;
    
    uint32_t expo_3     ;
    uint32_t mant_3     ;
    uint64_t mant_rnd   ;

    s_bit_r = mant_mid & 0x01                      ;                                                                           
    g       = mant_2 & the_24_binary               ;
    r       = mant_2 & the_23_binary               ;
    s       = (mant_2 & last_22_binary) || s_bit_r ;

    rnd_carry  =   (round_mode==3) ? ((r && (g||s))    ?1:0) : 
                   (round_mode==2) ? ((!sign_c) &&(a_is_n0) && (b_is_n0) &&(r||s||(expo_2==0 && mant_2==0) ?1:0)) :
                   (round_mode==1) ? (sign_c    &&(a_is_n0) && (b_is_n0) &&(r||s||(expo_2==0 && mant_2==0) ?1:0)) :
                    0;
    
    
    mant_rnd    =   (mant_2>>23) + rnd_carry                 ;
    expo_3      =   (mant_rnd&the_25_binary)||((!expo_2)&&(mant_rnd&the_24_binary)) ? (expo_2 + 1)                    : expo_2                     ;
    mant_3      =   (mant_rnd&the_25_binary)                                        ? ((mant_rnd>>1) & last_23_binary): (mant_rnd & last_23_binary);  
    
    D1.debug_printf("rnd_mode is %d",  round_mode);
    D1.debug_printf("g is %d"              ,static_cast<int>(g))           ;
    D1.debug_printf("r is %d"              ,static_cast<int>(r))           ;
    D1.debug_printf("s is %d"              ,static_cast<int>(s))           ;
    D1.debug_printf("a_is_n0 is %d"        ,static_cast<int>(a_is_n0))     ;
    D1.debug_printf("b_is_n0 is %d"        ,static_cast<int>(b_is_n0))     ;  
    D1.debug_printf("b_is_n0 is %d"        ,static_cast<int>(b_is_n0))     ;
    D1.debug_printf("l_con is %d"          ,static_cast<int>(r||s||(expo_2==0)))   ;
    D1.debug_printf("rnd carry is %d"      ,static_cast<int>(rnd_carry))   ;
    D1.debug_printf("mant_rnd is %lb"      ,mant_rnd)                      ;
    D1.debug_printf("mant_rnd_25 is %lb"   ,mant_rnd&the_25_binary)        ;
    D1.debug_printf("expo_3 is %u"         ,expo_3)                        ;
    D1.debug_printf("mant_3 is %lb"        ,mant_3)                        ;

    //------masking 
    D1.debug_printf("---------------------------");
    D1.debug_printf("------into masking---------");
    D1.debug_printf("---------------------------");
    bool    overflow   ;

    bool    a_is_0     ;
    bool    b_is_0     ;
    bool    a_is_inf   ;
    bool    b_is_inf   ;
    bool    a_is_nan   ;
    bool    b_is_nan   ;

    bool    r_is_nan   ;
    bool    is_inf_nan ;
    uint32_t mant_unsel;
    uint32_t mant_nan  ;
    bool     sign_unsel;
    bool     sign_nan  ;


    overflow    = expo_3>=255                                ;
    a_is_0      = (a.exponent==0)         && (mantissa==0)     ;
    b_is_0      = (rhs.exponent==0)     && (rhs.mantissa==0) ;
    a_is_inf    = (a.exponent==255)       && (mantissa==0)     ;
    b_is_inf    = (rhs.exponent==255)   && (rhs.mantissa==0) ;
    a_is_nan    = (a.exponent==255)       && (mantissa!=0)     ;
    b_is_nan    = (rhs.exponent==255)   && (rhs.mantissa!=0) ;

    r_is_nan    = a_is_nan | b_is_nan                        ;
    is_inf_nan  = a_is_nan | b_is_nan | a_is_inf | b_is_inf  ;
    
    mant_unsel  = ((a.exponent<<1)+(mantissa!=0) >= (rhs.exponent<<1)+(rhs.mantissa!=0) ) ? ((1<<22)+(mantissa & last_22_binary)) :((1<<22)+(rhs.mantissa & last_22_binary));
    mant_nan    = r_is_nan                   ? mant_unsel                            : 0                                       ;

    sign_unsel  = ((a.exponent<<1)+(mantissa!=0) >= (rhs.exponent<<1)+(rhs.mantissa!=0) ) ?  a.sign                                  : rhs.sign                                ;
    result.sign = r_is_nan                   ? sign_unsel                            : sign_c                                  ;

    D1.debug_printf("mant_unsel is %b",mant_unsel);

    bool    mode_1  ;
    bool    mode_0 ;

    mode_1      = round_mode & 0x02;
    mode_0      = round_mode & 0x01;

    
    if(is_inf_nan){
        D1.debug_printf("result is inf or nan");
        result.exponent = 255               ;
        result.mantissa = mant_nan          ;
    }
    else{
        D1.debug_printf("into regular")        ;
        if(overflow){
            D1.debug_printf("overflow")        ;
            result.exponent   = ((mode_1 && (!result.sign)) || (mode_0 && result.sign)) ? 255 : 254         ;
            result.mantissa   = ((mode_1 && (!result.sign)) || (mode_0 && result.sign)) ? 0   : 0x7fffff    ;  
        }
        else{
            D1.debug_printf("not overflow")    ;
            result.exponent   = expo_3      ;
            result.mantissa   = mant_3      ;
            D1.debug_printf("mant_3 is %lb",mant_3);
            D1.debug_printf("mant_r is %lb",result.mantissa);
        }
    }
    D1.debug_printf("sign_c is %d",static_cast<int>(sign_c ));
    return result;
    }

Fp32 Fp32::mul(const Fp32 &a,const Fp32 &rhs,const Fp32 &dont_care){

    D1.debug_printf("-------------------------");
    D1.debug_printf("-------into c model------");
    D1.debug_printf("-------------------------");

    Fp32 result(0.0f);
    //--------------to determine 0, NAN and INF
    uint64_t a_expo_is_zero  =   (a.exponent==0)                      ?1:0;                      //|exponent
    uint64_t b_expo_is_zero  =   (rhs.exponent==0)                    ?1:0;                      //|rhs.exponent
    uint64_t a_expo_is_ff    =   (a.exponent==255)                    ?1:0;                      //&exponent
    uint64_t b_expo_is_ff    =   (rhs.exponent==255)                  ?1:0;                      //&rhs.exponent
    uint64_t a_mant_is_00    =   (mantissa==0)                        ?1:0;           
    uint64_t b_mant_is_00    =   (rhs.mantissa==0)                    ?1:0;
    
    uint64_t a_is_0          =   (a_expo_is_zero&&a_mant_is_00)       ?1:0;                         
    uint64_t b_is_0          =   (b_expo_is_zero&&b_mant_is_00)       ?1:0;
    uint64_t a_is_nan        =   (a_expo_is_ff  &&(!a_mant_is_00))    ?1:0;
    uint64_t b_is_nan        =   (b_expo_is_ff  &&(!b_mant_is_00))    ?1:0;
    uint64_t a_is_inf        =   (a_expo_is_ff  &&a_mant_is_00)       ?1:0;
    uint64_t b_is_inf        =   (b_expo_is_ff  &&b_mant_is_00)       ?1:0;

    //--------------debuging 
    D1.debug_printf("a_is_0 is %lu",a_is_0);
    D1.debug_printf("b_is_0 is %lu",b_is_0);
    D1.debug_printf("a_is_inf is %lu,a_is_inf");
    D1.debug_printf("b_is_inf is %lu,b_is_inf");
    D1.debug_printf("r_is_nan is %lu,r_is_nan");

    //--------------generate sign
    result.sign              =   a.sign^rhs.sign;
    D1.debug_printf("result.sign is %d",result.sign);
    //--------------generate expontemp_exponentent,input exponent is 8 bit,temperory exponent is 10 bit, 1 bit for expantion,1 bit for sign
    uint32_t temp_expo;
    temp_expo                =   a.exponent+rhs.exponent-0x7f+a_expo_is_zero+b_expo_is_zero;       //can be optimized into -0x80+1

    //--------------generate mantissa,and generating the final exponent results
    uint64_t temp_mant;
    uint64_t temp_mant_last_47;
    uint64_t a_mant_total;
    uint64_t b_mant_total;
    uint32_t zero_nums;

    uint64_t mant_long;
    uint32_t man_expo;

    uint32_t bit_s_record;
    uint32_t shift_left;

    a_mant_total             =   ((1-a_expo_is_zero)<<23)+a.mantissa;
    b_mant_total             =   ((1-b_expo_is_zero)<<23)+rhs.mantissa;
    temp_mant                =   a_mant_total*b_mant_total;
    temp_mant_last_47        =   temp_mant&mantissa_get_47_bit;
    zero_nums                =   get_the_zero_nums(&temp_mant_last_47);
    bit_s_record             =   0;

    uint64_t temp_mant_l_48=temp_mant&mantissa_48_bit;
    uint64_t temp_mant_ref=0xffffffffffff;
    D1.debug_printf("temp_mant is    %lb",temp_mant);
    D1.debug_printf("temp_mant_48 is %lb",temp_mant_l_48);
    D1.debug_printf("temp_mant_rf is %lb",temp_mant_ref);
    D1.debug_printf("temp_mant_47 is %lb",temp_mant_last_47);
    D1.debug_printf("temp_mant is    %lu",temp_mant);
    D1.debug_printf("temp_mant_47 is %lu",temp_mant_last_47);

    if((temp_mant&mantissa_48_bit)&&(!(temp_expo&exponent_sign))){                                                             //1x.xxxxxxx
        D1.debug_printf("into shift right");
        man_expo             =   temp_expo+1;
        bit_s_record         =   temp_mant&0x01;
        mant_long            =   temp_mant>>1;
    }
    else{
        D1.debug_printf("not into shift right");
        uint32_t expo_sign=temp_expo&exponent_sign;
        D1.debug_printf("exponent sign is %u",expo_sign);
        D1.debug_printf("zero_num is %u",zero_nums);
        uint64_t temp_mant_l_47=temp_mant_last_47&mantissa_47_bit;
        D1.debug_printf("temp_mant's 47 bit is %lb",temp_mant_l_47);
        D1.debug_printf("temp_mant's 47 bit is %lu",temp_mant_l_47);

        if(!(temp_expo&exponent_sign) && ((temp_expo&exponent_num)>zero_nums) && (temp_mant_last_47&mantissa_47_bit)){         //0.xxxxxx and normailized
            D1.debug_printf("into 0.xxxx and normalized");
            man_expo         =   temp_expo-zero_nums;
            mant_long        =   temp_mant_last_47;
        }
        else{                                                                                                                  //0.xxxxxxx and denormailized
            D1.debug_printf("into 0.xxxx and denormalized");
            man_expo         =   0;
            if(!(temp_expo&exponent_sign) && (temp_expo&exponent_num)){                                                     //e-127->-126
                D1.debug_printf("temp_expo>-126,but level shift left");
                D1.debug_printf("temp_expo is %u",temp_expo);
                D1.debug_printf("temp_expo-1 is %u",temp_expo-1);
                shift_left  =   ((temp_expo-1)>63)?63:temp_expo-1;
                mant_long   =   temp_mant<<shift_left;
            }
            else {
                    D1.debug_printf("temp_expo<-126,temp_mant 48 is not 1");
                    D1.debug_printf("temp_expo is %u",temp_expo);
                    D1.debug_printf("1-temp_expo is %u",1-temp_expo);
                    shift_left  =   ((1-temp_expo)>63)?63:1-temp_expo;
                    mant_long   =   temp_mant>>shift_left;
            }
        }                                                                                           
    }
    D1.debug_printf("man_expo is %b",man_expo);
    D1.debug_printf("mant_long is %lb",mant_long);
    D1.debug_printf("---------------------------");
    D1.debug_printf("------into rounding--------");
    D1.debug_printf("---------------------------");
    //--------------rounding
    uint64_t mant_unround          ;
    uint32_t mant_round            ;
    uint32_t round_expo            ;   
    uint32_t mant_short            ;

    uint32_t g                     ;                                                        //24 bit
    uint32_t r                     ;                                                        //23 bit
    uint32_t s                     ;                                                        //22-1 bit
    uint32_t mant_carry            ;
    
    mant_unround    =   mant_long & mantissa_with_g_r;                                      //47-23 bit
    D1.debug_printf("mant_unround is %lb",mant_unround);
    uint64_t mant_unround_s = mant_long & last_22_binary;
    D1.debug_printf("mant_round_s is %lb",mant_unround_s);
    D1.debug_printf("mant_round_r is %lb",last_22_binary);

    s               =   bit_s_record | (mant_long & last_22_binary);
    D1.debug_printf("s is %u",s);
    g               =   mant_unround & g_bit;

    D1.debug_printf("g is %b",g);
    r               =   mant_unround & r_bit;
    D1.debug_printf("r is %lb",r);
    D1.debug_printf("round_mode is %u",round_mode);
    mant_carry      =   (round_mode==3) ? ((r       && (g||s))  ?1:0) : 
                        (round_mode==2) ? ((!result.sign) &&(!a_is_0)&&(!b_is_0)&&(r||s||(!man_expo))  ?1:0) :
                        (round_mode==1) ? (result.sign    &&(!a_is_0)&&(!b_is_0)&&(r||s||(!man_expo))  ?1:0) :
                        0;
    D1.debug_printf("result_sign is %d",result.sign);
    D1.debug_printf("mant_carry is %u",mant_carry);
    mant_round      =   (mant_unround>>23) + mant_carry;
    round_expo      =   (mant_round&the_25_binary)||((!man_expo)&&(mant_round&the_24_binary)) ? (man_expo + 1) : man_expo;

    mant_short      =   (mant_round&the_25_binary) ? ((mant_round>>1) & last_23_binary) : (mant_round & last_23_binary);
    
    //------------generating output,it seems like inf and nan can't be optimized into the current process.
    D1.debug_printf("---------------------------");
    D1.debug_printf("------into masking---------");
    D1.debug_printf("---------------------------");

    uint32_t overflow           ;
    uint32_t result_is_nan      ;
    uint32_t is_inf_nan         ;
    uint32_t mant_unselect      ;                                                          //output mantissa nan is the same as the first input nan
    uint32_t mant_nan           ;
    uint32_t r_is_0nan          ;
    bool     sign_nan           ;
    bool     sign_unselect      ;

    overflow        =   (round_expo>=255)||(man_expo>=255);                                //0xff
    result_is_nan   =   a_is_nan | b_is_nan | (a_is_0&b_is_inf)| (b_is_0&a_is_inf);
    is_inf_nan      =   a_is_nan | b_is_nan | a_is_inf | b_is_inf;
    // mant_unselect   =   ((mantissa & 0x07fffff) > (rhs.mantissa & 0x07fffff)) ? ((1<<22)+(mantissa & last_22_binary)) : ((1<<22)+(rhs.mantissa & last_22_binary));
    mant_unselect   =   ((a.exponent & 0xff) > (rhs.exponent & 0xff)) ? ((1<<22)+(mantissa & last_22_binary)) : ((1<<22)+(rhs.mantissa & last_22_binary));  
    mant_nan        =   result_is_nan ? mant_unselect : 0;

    r_is_0nan       =   (a_is_0&b_is_inf)| (b_is_0&a_is_inf);
    sign_unselect   =   ((a.exponent & 0xff) > (rhs.exponent & 0xff)) ? sign : rhs.sign;
    sign_nan        =   result_is_nan ?  (r_is_0nan | sign_unselect): result.sign;

    D1.debug_printf("result_is_nan is %lu",result_is_nan);
    D1.debug_printf("mant_unselect is %lb",mant_unselect);
    D1.debug_printf("mant_nan is %lb",mant_nan);
    uint32_t mant_nan_len = get_length(mant_nan);
    D1.debug_printf("mant_nan_len is %u",mant_nan_len);
    D1.debug_printf("result is %b",result_is_nan);
    if(is_inf_nan){
        D1.debug_printf("into is_inf_nan");
        result.sign         =   sign_nan ;
        result.exponent     =   255;
        result.mantissa     =   mant_nan;
        D1.debug_printf("result.exponent is %lb",result.exponent);
        D1.debug_printf("result.mantissa is %lb",result.mantissa);

    }
    else{
        D1.debug_printf("into regular");
        if(overflow){
            int mode_1        =    round_mode&0x02;
            int mode_0        =    round_mode&0x01;
            result.exponent   =    ((mode_1 && (!result.sign)) || (mode_0 && result.sign)) ? 255 : 254;
            result.mantissa   =    ((mode_1 && (!result.sign)) || (mode_0 && result.sign)) ? 0   : 0x7fffff;  
        }
        else{
            result.exponent     =  round_expo;
            result.mantissa     =  mant_short & last_23_binary;
        }
    }
    D1.debug_printf("result_sign is %d\n",result.sign);
    return result;
    }
    
uint32_t Fp32::get_the_zero_nums(uint64_t *mantissa_in){
    uint32_t zero_nums;
    uint32_t zero_num_5;
    uint32_t zero_num_4;
    uint32_t zero_num_3;
    uint32_t zero_num_2;
    uint32_t zero_num_1;
    uint32_t zero_num_0;
    zero_num_5      =   !detect_one(&*mantissa_in,32);
    zero_num_4      =   !detect_one(&*mantissa_in,16);
    zero_num_3      =   !detect_one(&*mantissa_in,8);
    zero_num_2      =   !detect_one(&*mantissa_in,4);
    zero_num_1      =   !detect_one(&*mantissa_in,2);
    zero_num_0      =   !detect_one(&*mantissa_in,1);
    zero_nums       =   (zero_num_5<<5)+(zero_num_4<<4)+(zero_num_3<<3)+(zero_num_2<<2)+(zero_num_1<<1)+zero_num_0;
    return zero_nums;
}

uint32_t Fp32::detect_one(uint64_t *mantissa_in,uint32_t width)
{   
    uint32_t one_exist        =   0;
    uint64_t mantissa_record  =   *mantissa_in;
    while(width>0)
    {
        one_exist            |=   ((*mantissa_in)&mantissa_47_bit)   ? 1:0;
        width                -=   1;
        (*mantissa_in)      <<=   1;
    }
    (*mantissa_in)            =   one_exist ?   mantissa_record:(*mantissa_in);
    return one_exist;
}

uint32_t Fp32::detect_one_long(mp::cpp_int *mantissa_in,uint32_t width)
{   
    uint32_t one_exist           =   0;
    mp::cpp_int mantissa_record  =   *mantissa_in;
    mp::cpp_int get_judge_bit_h  =   mantissa_l_75; 
    mp::cpp_int get_judge_bit    =   get_judge_bit_h<<64; 
    mp::cpp_int AND_res;

    while(width>0)
    {
        AND_res               =   get_judge_bit & (*mantissa_in);
        one_exist            |=   AND_res? 1:0;
        // D1.debug_printf("-----------------------------");
        // D1.debug_printf("width_%d",width);
        // D1.debug_printf("get_judge_bit:");
        // D1.print_cpp_int(get_judge_bit,"b");
        // D1.debug_printf("one exist is %u",one_exist);
        // D1.debug_printf("shifted mantissa:");
        // D1.print_cpp_int((*mantissa_in),"b");
        // D1.debug_printf("-----------------------------");
        width                -=   1;
        (*mantissa_in)      <<=   1;
    }
    (*mantissa_in)            =   one_exist ?   mantissa_record:(*mantissa_in);
    return one_exist;
}

uint32_t Fp32::get_the_zero_nums_add(uint64_t mantissa_in){
    uint32_t zero_nums;
    uint32_t zero_num_5;
    uint32_t zero_num_4;
    uint32_t zero_num_3;
    uint32_t zero_num_2;
    uint32_t zero_num_1;
    uint32_t zero_num_0;
    zero_num_5      =   !detect_one(&mantissa_in,32);
    zero_num_4      =   !detect_one(&mantissa_in,16);
    zero_num_3      =   !detect_one(&mantissa_in,8);
    zero_num_2      =   !detect_one(&mantissa_in,4);
    zero_num_1      =   !detect_one(&mantissa_in,2);
    zero_num_0      =   !detect_one(&mantissa_in,1);
    zero_nums   =   (zero_num_5<<5)+(zero_num_4<<4)+(zero_num_3<<3)+(zero_num_2<<2)+(zero_num_1<<1)+zero_num_0;
    return zero_nums;
}

uint32_t Fp32::get_the_zero_nums_add_long(mp::cpp_int mantissa_in){
    uint32_t zero_nums;
    uint32_t zero_num_6;
    uint32_t zero_num_5;
    uint32_t zero_num_4;
    uint32_t zero_num_3;
    uint32_t zero_num_2;
    uint32_t zero_num_1;
    uint32_t zero_num_0;
    D1.debug_printf("===========get_the_zero_nums_add_long=======");
    D1.debug_printf("mantissa_in:");
    D1.print_cpp_int(mantissa_in,"b");
    zero_num_6      =   !detect_one_long(&mantissa_in,64);
    // D1.debug_printf("after 64=======mantissa_in:");
    // D1.print_cpp_int(mantissa_in,"b");
    // D1.debug_printf("zero_num_6:    %u",zero_num_6);
    zero_num_5      =   !detect_one_long(&mantissa_in,32);
    // D1.debug_printf("after 32=======mantissa_in:");
    // D1.print_cpp_int(mantissa_in,"b");
    zero_num_4      =   !detect_one_long(&mantissa_in,16);
    // D1.debug_printf("after 16=======mantissa_in:");
    // D1.print_cpp_int(mantissa_in,"b");
    zero_num_3      =   !detect_one_long(&mantissa_in,8);
    zero_num_2      =   !detect_one_long(&mantissa_in,4);
    
    zero_num_1      =   !detect_one_long(&mantissa_in,2);
    zero_num_0      =   !detect_one_long(&mantissa_in,1);
    zero_nums   =(zero_num_6<<6)+(zero_num_5<<5)+(zero_num_4<<4)+(zero_num_3<<3)+(zero_num_2<<2)+(zero_num_1<<1)+zero_num_0;
    return zero_nums;
}

Fp32 Fp32::fma(const Fp32 &a,const Fp32 &b,const Fp32 &c){
    Fp32 result(0.0f);
    //judge 
    bool a_expo_is_zero = (a.exponent==0)   ; 
    bool b_expo_is_zero = (b.exponent==0)   ;
    bool c_expo_is_zero = (c.exponent==0)   ;
    bool a_expo_is_ff   = (a.exponent==255) ;
    bool b_expo_is_ff   = (b.exponent==255) ;
    bool c_expo_is_ff   = (c.exponent==255) ;
    bool a_mant_is_00   = (a.mantissa==0)   ;
    bool b_mant_is_00   = (b.mantissa==0)   ;
    bool c_mant_is_00   = (c.mantissa==0)   ;

    bool a_is_0         = (a_expo_is_zero && a_mant_is_00)      ;
    bool b_is_0         = (b_expo_is_zero && b_mant_is_00)      ;
    bool c_is_0         = (c_expo_is_zero && c_mant_is_00)      ;
    bool a_is_nan       = (a_expo_is_ff   && (!a_mant_is_00))   ;
    bool b_is_nan       = (b_expo_is_ff   && (!b_mant_is_00))   ;
    bool c_is_nan       = (c_expo_is_ff   && (!c_mant_is_00))   ;
    bool a_is_inf       = (a_expo_is_ff   && a_mant_is_00)      ;
    bool b_is_inf       = (b_expo_is_ff   && b_mant_is_00)      ;
    bool c_is_inf       = (c_expo_is_ff   && c_mant_is_00)      ;

    //-------------------debug
    D1.debug_printf("--------------------");
    D1.debug_printf("------judge---------");
    D1.debug_printf("--------------------");
    D1.debug_printf("a_is_0 is %lu",a_is_0);
    D1.debug_printf("b_is_0 is %lu",b_is_0);
    D1.debug_printf("c_is_0 is %lu",c_is_0);
    D1.debug_printf("a_is_inf is %lu",a_is_inf);
    D1.debug_printf("b_is_inf is %lu",b_is_inf);
    D1.debug_printf("c_is_inf is %lu",c_is_inf);
    D1.debug_printf("a_is_nan is %lu",a_is_nan);
    D1.debug_printf("b_is_nan is %lu",b_is_nan);
    D1.debug_printf("c_is_nan is %lu",c_is_nan);
    
    //------------------------- 
    //--------------mul a and b 
    //------------------------- 
    D1.debug_printf("--------------------");
    D1.debug_printf("------Mul a and b---------");
    D1.debug_printf("--------------------");

    uint64_t mant_mul; //48 bits
    uint64_t a_mant_in;//24 bits
    uint64_t b_mant_in;//24 bits
    a_mant_in = (((1-a_expo_is_zero)<<23)+a.mantissa);
    b_mant_in = (((1-b_expo_is_zero)<<23)+b.mantissa);
    mant_mul  = a_mant_in * b_mant_in;

    uint32_t expo_mul;//10 bits
    expo_mul  = a.exponent + b.exponent - 0x7f + a_expo_is_zero + b_expo_is_zero;

    D1.debug_printf("a_mant_in is %b",a_mant_in);
    D1.debug_printf("b_mant_in is %b",b_mant_in);
    D1.debug_printf("*_mant_re is %b",last_23_binary);
    D1.debug_printf("mant_mul is %lb",mant_mul);
    D1.debug_printf("mant_ref is %lb",mantissa_get_47_bit);
    D1.debug_printf("expo_mul is %d",expo_mul);

    //-------------------------------------------  
    //---------------shift and inversion of a 
    //-------------------------------------------  
    D1.debug_printf("--------------------");
    D1.debug_printf("-------shift and inversion of a---------");
    D1.debug_printf("--------------------");
    uint32_t expo_stor; //store the correct expo_nums,for the sake of shifter,10 bits

    bool sign_mul = a.sign ^ b.sign;
    bool sub      = sign_mul ^ c.sign;
    mp::cpp_int c_mant_in;//24 bits
    
    c_mant_in = (((1-c_expo_is_zero)<<23)+c.mantissa);

    uint32_t d; //the diff between c-(a+b)
    d= (c.exponent + c_expo_is_zero)- (expo_mul);
    bool d_sign = d & exponent_sign;//the 10 bit
    uint32_t shift_num;//7 bit
    uint32_t shift_num_0;//when d>0
    uint32_t shift_num_1;//when d<0
    
    
    shift_num_0 = ((27-d)>27) ? 27 : (27-d);
    shift_num_1 = ((74-d)>74) ? 74 : (74-d);
    shift_num   = d_sign      ? shift_num_1 : shift_num_0 ;
    expo_stor   = d_sign      ? expo_mul    : c.exponent  ;


    mp::cpp_int c_mant_l; //74 bit 
    mp::cpp_int c_mant_s; 
    mp::cpp_int c_inv;    //74 bit

    c_mant_l = c_mant_in << 50;       //{c_mant_in,50'b0}
    c_inv    = sub ? (0-c_mant_l) : c_mant_l;
    c_mant_s = c_inv >> shift_num;    //shift right with sign extension

    D1.debug_printf("sign_mul is %d",static_cast<int>(sign_mul));
    D1.debug_printf("sub is %d",static_cast<int>(sub));
    D1.debug_printf("c_mant_in:");
    D1.print_cpp_int(c_mant_in,"b");
    D1.debug_printf("*_mant_re is %b",last_23_binary);
    D1.debug_printf("d is %u",d);
    D1.debug_printf("d_sign is %d",static_cast<int>(d_sign));
    D1.debug_printf("shift_num_0 is %u",shift_num_0);
    D1.debug_printf("shift_num_1 is %u",shift_num_1);
    D1.debug_printf("shift_num is %u",shift_num);
    D1.debug_printf("expo_stor is %u",expo_stor);
    D1.debug_printf("c_mant_l:");
    D1.print_cpp_int(c_mant_l,"b");
    D1.debug_printf("c_inv:");
    D1.print_cpp_int(c_inv,"b");   
    D1.debug_printf("c_mant_s:");
    D1.print_cpp_int(c_mant_s,"b");   

    //-------------------------------------------  
    //-----------get st1
    //-------------------------------------------  
    D1.debug_printf("--------------------");
    D1.debug_printf("------get st1---------");
    D1.debug_printf("--------------------");
    mp::cpp_int shift_num_bit=0;
    for(int i=0;i<shift_num;i++){
        shift_num_bit=(shift_num_bit<<1)+1;
    }

    bool st1;
    if(sub){
        if(!((~c_inv)&shift_num))
            st1=true;
        else
            st1=false;
    }
    else{
        D1.debug_printf("into sub=0");
        if((!(c_inv&shift_num))){
            st1=true;
        }
        else
            st1=false;
    }
    D1.debug_printf("shift_num_bit:");
    D1.print_cpp_int(shift_num_bit,"b");
    D1.debug_printf("st1 is %d\n",static_cast<int>(st1));
    //-------------------------------------------  
    //-----------sign detection 
    //-------------------------------------------   
    D1.debug_printf("--------------------");
    D1.debug_printf("------sign detection---------");
    D1.debug_printf("--------------------");
    bool complement;
    //a 25 bit comperator 
    bool c_bigger; //whether the add operand is bigger  
    uint32_t mul_top_25;
    mul_top_25 = mant_mul>>23;//the top 25 bits of mant_mul;
    c_bigger   = c_mant_in>mul_top_25;
    complement = sub && ((c_bigger&&(d_sign))||(d>0));
    bool sign_add;
    sign_add   = complement ? (!sign_mul) : sign_mul;
    D1.debug_printf("complement is %d",static_cast<int>(complement));
    D1.debug_printf("c_bigger is %d",static_cast<int>(c_bigger));
    D1.debug_printf("sign_add is %d",static_cast<int>(sign_add));  

    //---------------------------------------------------------------------------------
    //-----------csa add.one 48-bit adder + one 2-bit adder + two 24-bit adders + 
    //---------------------------------------------------------------------------------
    D1.debug_printf("--------------------");
    D1.debug_printf("------csa add---------");
    D1.debug_printf("--------------------");
    uint64_t mant_low_res;//48 bits
    uint32_t mant_high_res;//24 bits
    uint32_t mant_mid_res;//2 bits

    //available result is 48 bits + a carry bit + a sign bit 
    
    //because there are two 1's to complement the first one added at the adder the second one added at the carry and sticky cal

    uint64_t mant_add_in;//the lower 48 bits of c_mant_s
    mant_add_in=cpp_int_last_bit(c_mant_s,12);//
    
    uint64_t mant_add_low;//48-bit of c_mant_s + 49-bit {mul_csa_carry,sub} + 49-bit {mul_csa_sum,st1_bar}
    uint64_t mant_add_low_inv;//adder_inv + 1 
    //two adder one for normal add , one for inverted add
    uint64_t mant_add_in_l;
    uint64_t mant_mul_l;
    uint64_t mant_add_in_l_inv;
    uint64_t mant_mul_l_inv;
    mant_add_in_l      = (mant_add_in<<1) + sub ;
    mant_mul_l         = (mant_mul<<1)+(!st1);

    mant_add_in_l_inv  = (0-mant_add_in_l-1);//!!!!!
    mant_mul_l_inv     = (0-mant_mul_l-1);//!!!!!!

    mant_add_low       = mant_add_in_l + mant_mul_l; //49-bit adder,result 50 bit, available  49 bits.
    D1.debug_printf("mant_add_low is        %lb",mant_add_low);
    mant_add_low       = mant_add_low>>1;

    mant_add_low_inv   = mant_add_in_l_inv + mant_mul_l_inv + 2; //inverted adder 49-bit adder ,result 51 bit ,available 50 bits
    // mant_add_low_inv   = mant_add_in_l_inv + mant_mul_l_inv ; //inverted adder 49-bit adder ,result 51 bit ,available 50 bits

    // D1.debug_printf("mant_add_low_inv is    %lb",mant_add_low_inv);
    // D1.debug_printf("mant_add_low_inv is    %lb",mant_add_in_l_inv + mant_mul_l_inv);
    // D1.debug_printf("mant_add_in_l           is   %lb",mant_add_in_l);
    // D1.debug_printf("mant_add_in_l_inv       is   %lb",mant_add_in_l);
    // D1.debug_printf("mant_mul_l              is   %lb",mant_mul_l);
    // D1.debug_printf("mant_mul_l_inv          is   %lb",mant_mul_l_inv);

    // when shifting,keep the sign of mant_add_low_inv
    uint64_t mant_add_low_inv_sign=mant_add_low_inv&mantissa_64_bit;

    mant_add_low_inv   = (mant_add_low_inv_sign)+(mant_add_low_inv>>1);
    //overlap 2 bits

    mant_low_res = complement ? (mant_add_low_inv & mantissa_get_48_bit) : (mant_add_low & mantissa_get_48_bit);       //the last 48 bits

    uint32_t mant_add_high;        //the top 24 bits of c_mant_s         ,24 bits long 
    uint32_t mant_add_high_a1;     //the top 24 bits of c_mant_s with one carry,24 bits long
    uint32_t mant_add_high_inv;    //the inversion of mant_add_high
    uint32_t mant_add_high_inv_a1; //the inversion ofmant_add_high with one carry

    uint32_t mant_add_mid_a; //the top 25-26 bits of c_mant_s      ,2 bits long
    uint32_t mant_add_mid_b; //the top 2 bits of mant_add_lower_inv,2 bits long


    mant_add_mid_a  = complement ? (0-cpp_int_cut_bit(c_mant_s,2,48)) : cpp_int_cut_bit(c_mant_s,2,48) ; 
    mant_add_mid_b  = complement ? (mant_add_low_inv & 0x3000000000000) : (mant_add_low & 0x3000000000000) ;

    mant_add_high       = cpp_int_cut_bit(c_mant_s,24,50);    
    mant_add_high_a1    = mant_add_high + 1;// input 24 bits , output 24 bits 
    mant_add_high_inv   =(0-cpp_int_cut_bit(c_mant_s,24,50)-1); //!!!!!!
    mant_add_high_inv_a1= mant_add_high_inv + 1;
    
    bool mant_higher_add;
    uint32_t mant_mid_add ;
    mant_mid_add    = mant_add_mid_a + mant_add_mid_b;
    mant_higher_add = mant_mid_add & 0x04;
    mant_mid_res    = mant_mid_add & 0x03;

    if(complement){         //case in rtl
        if(mant_higher_add)
            mant_high_res = mant_add_high_inv_a1;
        else
            mant_high_res = mant_add_high_inv;
    }
    else{
        if(mant_higher_add)
            mant_high_res = mant_add_high_a1;
        else
            mant_high_res = mant_add_high;
    }

    mp::cpp_int mant_add;
    mant_add=(mant_high_res<<50)+(mant_mid_res%(1<<2)<<48)+(mant_low_res);//75 bits
    D1.debug_printf("-----------input c shifted mant:-----------------------------------------------------------------------------");
    D1.debug_printf("mant_add_in(the lower 48 bits of c_mant_s) is %lb",mant_add_in);
    D1.debug_printf("48 bits ref:                                  %lb",mantissa_get_48_bit);
    D1.debug_printf("sub                                        is %d",static_cast<int>(sub));
    D1.debug_printf("!st1                                       is %d",static_cast<int>(!st1));
    D1.debug_printf("------------mant_mul:----------------------------------------------------------------------------------------");
    D1.debug_printf("mant_mul                                   is %lb",mant_mul);
    D1.debug_printf("------------mant_add_res:------------------------------------------------------------------------------------");
    D1.debug_printf("------------mant_add::48bits:");
    D1.debug_printf("mant_add_low(the low 50 bits of mant_add)  is %lb",mant_add_low);
    D1.debug_printf("mant_add_low_inv                           is %lb",mant_add_low_inv);
    D1.debug_printf("mant_low_res(the low 48 bits of mant_add)  is %lb",mant_low_res);
    D1.debug_printf("------------mant_add::49-50bits:");
    D1.debug_printf("mant_add_mid_a(mid bits of c_mant_s)       is %lb",mant_add_mid_a);
    D1.debug_printf("mant_add_mid_b(mid bits of mant_add_low)   is %lb",mant_add_mid_b);
    D1.debug_printf("mant_mid_res                               is %lb",mant_mid_res);
    D1.debug_printf("------------mant_add::74-51bits:");
    D1.debug_printf("mant_high_add(add 1 to higher bit or not)  is %d",static_cast<int>(mant_higher_add));
    D1.debug_printf("mant_add_high(original)                    is %lb",mant_add_high);
    D1.debug_printf("mant_add_high_inv(inverted)                is %lb",mant_add_high_inv);
    D1.debug_printf("mant_add_high_a1(add 1)                    is %lb",mant_add_high_a1);
    D1.debug_printf("mant_add_high_inv_a1(inverted add 1)       is %lb",mant_add_high_inv_a1);
    D1.debug_printf("mant_high_res                              is %lb",mant_high_res);
    D1.debug_printf("mant_add");
    D1.print_cpp_int(mant_add,"b");



    //-------------------------------------------  
    //-----------loa,parallel with the adder
    //-------------------------------------------   
    
    //-----------get the adder leading zeros,only depanding on the p g a and d and sub, so it can be merged with the adder
    //-----------can get the leading zero of the higher bits and the lower bits seperately, sacrifices the area for the sake of speed
    //-----------get the 24 bits of zero and get the 51 bits of zero
    //-----------has two leading zero detector for normal and converted ones    
    D1.debug_printf("--------------------");
    D1.debug_printf("------loa---------");
    D1.debug_printf("--------------------");
    
    mp::cpp_int a_in(mant_add_in_l);
    mp::cpp_int b_in(mant_mul_l);
    mp::cpp_int a_in_v(mant_add_in_l_inv);
    mp::cpp_int b_in_v(mant_mul_l_inv+2);

    D1.debug_printf("a_in:");
    D1.print_cpp_int(a_in,"b");
    D1.debug_printf("b_in:");
    D1.print_cpp_int(b_in,"b");
    // D1.debug_printf("a_in_v:");
    // D1.print_cpp_int(a_in_v,"b");
    // D1.debug_printf("b_in_v:");
    // D1.print_cpp_int(b_in_v,"b");

    uint32_t zero_nums_nor;
    uint32_t zero_nums_inv;
    uint32_t zero_nums;
    zero_nums_nor   = loa(75,a_in,b_in);
    // zero_nums_inv   = loa(75,a_in_v,b_in_v);
    zero_nums       = complement ? zero_nums_inv:zero_nums_nor;

    D1.debug_printf("zero_nums_nor is %b",zero_nums_nor);
    D1.debug_printf("zero_nums_inv is %b",zero_nums_inv);
    D1.debug_printf("complement    is %d",static_cast<int>(complement));
    D1.debug_printf("zero_nums     is %b",zero_nums);

    
    //-------------------------------------------  
    //-----------shifter,get the top 24 bits
    //-------------------------------------------   
    //75-bit shifter can be a 27-bit and a 48-bit shifter

    //if zero_nums <= shift_nums+1, left shift zero_nums;
    //---------------------------------------------if zero_nums=shift_num,expo+1
    //---------------------------------------------else                  ,expo remains the same
    //if zero_nums >  shift_nums+1,
    //-----------------------d>=0,store c exponent, shift_dif=zero_nums-(shift_num+1)<expo_stor,left shift zero_nums ,expo=expo_store-shift_dif;
                                                                                           //else expo=0,shift_left=expo_stor

    //-----------------------d<0,store mul exponent,shift_dif=zero_nums-(shift_num+1)<expo_stor,left shift zero_nums ,expo=expo_store-shift_dif;
    //---------------------------------------------shift_dif=zero_nums-(shift_num+1)>=expo_stor,expo=0
                                                                                          //expo_stor >=0,shift_left <<27 + expo_stor //no more than 46
                                                                                          //expo_stor <0,shift_left <<27 - (-expo_stor)//no less than 0
    D1.debug_printf("--------------------");
    D1.debug_printf("------shifter,get the top 24 bits---------");
    D1.debug_printf("--------------------");
    uint32_t expo_shift;//10 bits outputs
    mp::cpp_int mant_shift;//75 bits outputs 
    

    bool st2;

    uint32_t re_shift_num=shift_num+1;  //7bit

    uint32_t shift_1_ena;//control the long shift    
    shift_1_ena     = d_sign?27:0;
    uint32_t left_shift_1;//control the  zero_nums shift 
    left_shift_1    = zero_nums-shift_1_ena;

    uint32_t shift_dif;  // condition for if else
    shift_dif       = zero_nums-(re_shift_num);
    
    // uint32_t left_shift_2;//control the expo_stor - shift_dif shift
    // left_shift_2    = expo_stor-shift_dif;

    mp::cpp_int mant_lshift_in;

    mp::cpp_int mant_shift_1;//75 bits,the short shift results;

    mant_shift_1 = mant_add << 27;

    mant_lshift_in=d_sign?mant_shift_1:mant_add;

    if(zero_nums<=re_shift_num)
    {   
        st2         = false;
        expo_shift  = (zero_nums==re_shift_num)? (expo_stor + 1) : expo_stor ;
        mant_shift  =  mant_lshift_in << left_shift_1;
    }
    else
    {
        if(shift_dif<expo_stor)
        {
            st2         = false;
            expo_shift  = expo_stor - shift_dif;
            mant_shift  = mant_lshift_in << left_shift_1;
        }
        else
        {
            expo_shift  = 0;
            if(!(expo_stor & exponent_sign))  //d>=0
            {
                st2         = false;
                mant_shift  = mant_lshift_in << expo_stor;
            }
            else
            {
                mp::cpp_int mask;
                mask        = (1ULL<<(-expo_stor)) - 1;
                st2         = ((mask & mant_lshift_in)!=0);
                mant_shift  = mant_lshift_in >> (-expo_stor);
            }
        }
    }
    D1.debug_printf("zero_nums      is %u",zero_nums);
    D1.debug_printf("shift_nums     is %u",re_shift_num);
    D1.debug_printf("mant_add:");
    D1.print_cpp_int(mant_add,"b");
    D1.debug_printf(" ");
    D1.debug_printf("d_sign         is %d",static_cast<int>(d_sign));
    D1.debug_printf("shift_1_ena    is %u",shift_1_ena);
    D1.debug_printf("shift_dif      is %u",shift_dif);
    D1.debug_printf("left_shift_1   is %u",left_shift_1);

    D1.debug_printf("mant_shift_1   is ");
    D1.print_cpp_int(mant_shift_1,"b");
    D1.debug_printf("mant_lshift_in is ");
    D1.print_cpp_int(mant_lshift_in,"b");

    D1.debug_printf("result:----------");
    D1.debug_printf("st2        is %d",static_cast<int>(st2));
    D1.debug_printf("expo_shift is %b",expo_shift);
    D1.debug_printf("mant_shift is");
    D1.print_cpp_int(mant_shift,"b");


    //-------------------------------------------  
    //-----------rnd,another adding 1 is here
    //-------------------------------------------   
    //top 25 bits , 2:g 1:r ,the last 50 bits get s 
    //output mant_rnd
    //output expo_rnd
    D1.debug_printf("--------------------");
    D1.debug_printf("------rnd---------");
    D1.debug_printf("--------------------");
    uint32_t mant_rnd;
    uint32_t expo_rnd;
    uint32_t mant_rnd_unc;
    bool g;
    bool g_1;
    bool r;
    bool r_1;
    bool s;
    bool mant_add_1;            //mant add 1 condition
    bool expo_add_1;            //expo add complement / carry 1 condition
    bool expo_add_1_c;          //expo add complement and carry 1 condition  
    bool mant_carry;            //normal mant_carry
    bool mant_carry_1;          //mant add complement 1 carry
    bool rnd_carry;

    uint32_t mant_rnd_top;      //top 25 bits
    uint32_t mant_rnd_top_1;    //top 25 bits add comlement_1/carry_1
    uint32_t mant_rnd_top_1_c;  //top 25 bits add complement_1 and carry_1
    uint64_t mant_rnd_last;     //last 50 bits 
    uint32_t expo_shift_1;      //expo_shift add 1

    D1.debug_printf("mant_shift:");
    D1.print_cpp_int(mant_shift,"b");
    mant_rnd_top    = cpp_int_cut_bit(mant_shift,25,51);
    D1.debug_printf("mant_rnd_top is %b",mant_rnd_top);

    mant_rnd_last   = cpp_int_last_bit(mant_shift,50);
    D1.debug_printf("mant_rnd_last is %lb",mant_rnd_last);

    mant_rnd_top_1  = mant_rnd_top + 1;
    mant_rnd_top_1_c= mant_rnd_top + 2;
    
    expo_shift_1    = expo_shift + 1;

    mant_add_1      = (mant_rnd_last == mantissa_get_50_bit)&&complement; //mant_rnd_last is all 1 and complement is enable;
    expo_add_1      = (mant_rnd_top_1   & mantissa_26_bit) || ((!expo_shift) && (mant_rnd_top_1   & the_25_binary));
    expo_add_1_c    = (mant_rnd_top_1_c & mantissa_26_bit) || ((!expo_shift) && (mant_rnd_top_1_c & the_25_binary));

    g               = mant_rnd_top   & 0x02;
    r               = mant_rnd_top   & 0x01;
    g_1             = mant_rnd_top_1 & 0x02;
    r_1             = mant_rnd_top_1 & 0x01;

    s               = (mant_rnd_last != 0) || complement || st1 ||st2;

    mant_carry      = (round_mode==3) ? ((r  && (g||s))   ?1:0) : 
                      (round_mode==2) ? ((!sign_add)      ?1:0) :
                      (round_mode==1) ? (sign_add         ?1:0) :
                       0;    
    mant_carry_1    = (round_mode==3) ? (r_1 && (g||s)    ?1:0) : 
                      (round_mode==2) ? ((!sign_add)      ?1:0) :
                      (round_mode==1) ? (sign_add         ?1:0) :
                       0;    
    
    rnd_carry       = mant_add_1 ? mant_carry_1 : mant_carry    ;
    //case
    if(mant_add_1 && rnd_carry){
        mant_rnd_unc    = mant_rnd_top_1_c;
        expo_rnd        = expo_add_1_c  ?   expo_shift_1 : expo_shift;
        mant_rnd        = expo_add_1_c  ?   ((mant_rnd_unc>>2) & last_23_binary): ((mant_rnd_unc>>1) & last_23_binary);
    }
    else
    {
        if(mant_add_1 ^ rnd_carry)
        {
            mant_rnd_unc    = mant_rnd_top_1;
            expo_rnd        = expo_add_1 ?  expo_shift_1 : expo_shift;
            mant_rnd        = expo_add_1 ?   ((mant_rnd_unc>>2) & last_23_binary): ((mant_rnd_unc>>1) & last_23_binary);

        }
        else
            mant_rnd_unc    = mant_rnd_top;
            mant_rnd        = (mant_rnd_top>>1) & last_23_binary;
    }
    D1.debug_printf("mant_add_1 is     :%u",static_cast<int>(mant_add_1));
    D1.debug_printf("mant_add");

    //-------------------------------------------  
    //-----------mask 
    //-------------------------------------------   
    //if there exist a inf results is inf or nan; result is inf and nan 
    //input sign_add
    //input expo_rnd
    //input mant_rnd
    //input 
    D1.debug_printf("--------------------");
    D1.debug_printf("------mask ---------");
    D1.debug_printf("--------------------");
    bool overflow;
    bool r_is_nan;
    bool is_inf_nan;
    uint32_t  mant_unsel;
    uint32_t  mant_nan;
    bool sign_unsel;
    bool sign_nan;

    overflow=expo_rnd>=255;
    r_is_nan    = a_is_nan || b_is_nan || c_is_nan || (a_is_0 & b_is_inf) || (a_is_inf & b_is_0); 
    is_inf_nan  = r_is_nan || a_is_inf || b_is_inf || c_is_inf;
    uint32_t a_expo_mant; //{expo,|mant}
    uint32_t b_expo_mant;
    uint32_t c_expo_mant;
    uint32_t mid_expo_mant;
    uint32_t mid_mantissa;
    bool     mid_sign;

    a_expo_mant     = (a.exponent<<1)+(a.mantissa!=0);
    b_expo_mant     = (b.exponent<<1)+(b.mantissa!=0);
    c_expo_mant     = (c.exponent<<1)+(c.mantissa!=0);

    mid_expo_mant   = (a_expo_mant>=b_expo_mant)  ? a_expo_mant:b_expo_mant;
    mid_mantissa    = (a_expo_mant>=b_expo_mant)  ? ((1<<22)+(a.mantissa & last_22_binary)) : ((1<<22)+(b.mantissa & last_22_binary));
    mant_unsel      = (mid_expo_mant>=c_expo_mant)? mid_mantissa  : ((1<<22)+(c.mantissa & last_22_binary));
    mant_nan        = r_is_nan                    ? mant_unsel    : 0;

    mid_sign        = (a_expo_mant>=b_expo_mant)  ? a.sign        : b.sign;
    sign_unsel      = (mid_expo_mant>=c_expo_mant)? mid_sign      : c.sign;
    result.sign     = r_is_nan                    ? sign_unsel    : sign_add;

    bool mode_1;
    bool mode_0;

    mode_1      = round_mode & 0x02;
    mode_0      = round_mode & 0x01;

    if(is_inf_nan)
    {
        result.exponent=255;
        result.mantissa=mant_nan;
    }   
    else
    {
        if(overflow)
        {
            result.exponent = ((mode_1 && (!result.sign)) || (mode_0 && result.sign)) ? 255 : 254         ;
            result.mantissa = ((mode_1 && (!result.sign)) || (mode_0 && result.sign)) ? 0   : 0x7fffff    ;  
        }
        else
        {
            result.exponent = expo_rnd;
            result.mantissa = mant_rnd;
        }
    } 

    return result;
}

uint64_t Fp32::cpp_int_last_bit(const mp::cpp_int a,int cut_length){
    int cut_bit_r       =cut_length;
    mp::cpp_int cut_num =cut_length;
    mp::cpp_int a_cut;
    uint64_t    result;

    for(int i=0;i<cut_bit_r;i++){
        cut_num=(cut_num<<1)+1;
    }

    a_cut  = a&(cut_num);
    result = static_cast<uint64_t>(a_cut);

    // D1.debug_printf("------------------");
    // D1.debug_printf("----input cpp_int:");
    // D1.print_cpp_int(a,"b");
    // D1.debug_printf("----a_cut:");
    // D1.print_cpp_int(a_cut,"b");
    // D1.debug_printf("----result is : \n %lb",result);
    // D1.debug_printf("------------------");
    return result;
}

uint32_t Fp32::cpp_int_cut_bit(const mp::cpp_int a, int cut_bit,int shift_bit){
    int cut_bit_r=cut_bit;
    mp::cpp_int shift_num=0;
    mp::cpp_int cut_num=0;
    mp::cpp_int a_cut;
    uint32_t result;
    for(int i=0;i<cut_bit;i++){
        cut_num=(cut_num<<1)+1;
    }

    shift_num =a>>(shift_bit);
    a_cut     =shift_num&cut_num;
    result    =static_cast<uint32_t>(a_cut);
    // std::string oss = mp::to_string(a_cut);
    // uint32_t result = std::stoul(oss, nullptr, 10);
    // D1.debug_printf("------------------");
    // D1.debug_printf("----input cpp_int:");
    // D1.print_cpp_int(a,"b");
    // D1.debug_printf("----a_cut:");
    // D1.print_cpp_int(a_cut,"b");
    // D1.debug_printf("----oss is:");
    // std::cout<<oss<<std::endl;
    // D1.debug_printf("----result is : \n %b",result);
    // D1.debug_printf("------------------");
    
    return result;
}

BIT_CAL(get_p,(a_last ^ b_last));
BIT_CAL(get_g,(a_last & b_last));
BIT_CAL(get_a,(!(a_last | b_last)));

void Fp32::freeArray(int* arr){
    delete[] arr;
}

uint32_t Fp32::loa(int array_length,mp::cpp_int input_a,mp::cpp_int input_b){
    int *k      = new int [array_length];
    int *p      = new int [array_length];
    int *g      = new int [array_length];
    int *a      = new int [array_length];

    int *p_exp  = new int [array_length+1];
    int *p_cin  = new int [array_length];
    get_p(array_length,input_a,input_b,p);
    get_g(array_length,input_a,input_b,g);
    get_a(array_length,input_a,input_b,a);
    int *f      = new int [array_length];

    // for (int i = 0; i < array_length+1; i++)
    // {
    //     if(i==(array_length+1))
    //         p_exp[i]   = 0;
    //     else
    //         p_exp[i]   = p[i];
    // }

    for (int i = 0; i < array_length; i++)
    {
        k[i]    = p[i+1]&g[i];
    }

    for (int i = 0; i < array_length; i++)
    {
        if(i==0)
            p_cin[i]    = 0;
        else
            p_cin[i]    = p[i]&k[i-1] +p[i]&p_cin[i-1]; 
    }

    for (int i=0;i<array_length;i++)
    {
        if(i==0)
            f[i]    = !a[i];
        else
            f[i]    = !(a[i]&(!(p_cin[i-1]||g[i-1])));
    }

    mp::cpp_int mask=0;
    for (int i=0;i<array_length;i++)
    {
        mask    = (mask<<1)+f[array_length-1-i];
    }

    uint32_t zero_nums=0;
    
    zero_nums=get_the_zero_nums_add_long(mask);

    D1.debug_printf("---------array------------------");
    D1.debug_printf("p:");
    D1.print_array(p,array_length);
    // D1.debug_printf("p_exp:");
    // D1.print_array(p_exp,array_length+1);
    D1.debug_printf("g:");
    D1.print_array(g,array_length);
    D1.debug_printf("a:");
    D1.print_array(a,array_length);
    D1.debug_printf("k:");
    D1.print_array(k,array_length);
    D1.debug_printf("p_cin:");
    D1.print_array(p_cin,array_length);
    D1.debug_printf("f:");
    D1.print_array(f,array_length);
    D1.debug_printf("mask:");
    D1.print_cpp_int(mask,"b");
    freeArray(k);
    freeArray(p_exp);
    freeArray(p_cin);
    freeArray(p);
    freeArray(g);
    freeArray(a);
    freeArray(f);
    return zero_nums;
}
