#include "test.h"
#include "fp32.h"
#include "calculate.h"

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
Fp32 Fp32::mul(const Fp32 &rhs) {
        //-----------Instantiate  classes -----------//
        Calculate calculator;
        //-----------Instantiate guard round sticky bits -----------//
        uint32_t s=0;
        uint32_t r=0;
        uint32_t g=0;
        int loop;
        Fp32 result(0.0f); // initialize the result to 0
        //-----------bool flag-------------------------------------//
        uint32_t imp_a  = ((uint32_t)sign << 31)|((uint32_t)exponent << 23)|(mantissa & 0x7fffff);
        uint32_t imp_b  = ((uint32_t)rhs.sign << 31)|((uint32_t)rhs.exponent << 23)|(rhs.mantissa & 0x7fffff);

        int a_is_nan   = calculator.is_nan(imp_a);
        int b_is_nan   = calculator.is_nan(imp_b);
        int one_nan    = (a_is_nan&&(!b_is_nan))||((!a_is_nan)&&b_is_nan);
        int both_nan   = a_is_nan&&b_is_nan;
        int is_nan     = one_nan||both_nan;
        int zero_inf   = (calculator.is_zero(imp_a)&& calculator.is_inf(imp_b))||(calculator.is_zero(imp_b)&&calculator.is_inf(imp_a));
        int is_inf     = (calculator.is_inf(imp_a)||calculator.is_inf(imp_b))&&(!zero_inf)&&(!is_nan);
        int is_zero    = (calculator.is_zero(imp_a)||calculator.is_zero(imp_b))&&(!zero_inf)&&(!is_nan);
        int in_range   = !(is_zero||is_inf||is_nan||zero_inf);

        debug_printf("imp_a  is %d",imp_a);
        debug_printf("imp_b  is %d",imp_b);
        debug_printf("a_is_nan  is %d",a_is_nan);
        debug_printf("b_is_nan  is %d",b_is_nan);
        debug_printf("one_nan  is %d",one_nan);
        debug_printf("both_nan  is %d",both_nan);
        debug_printf("is_nan  is %d",is_nan);
        debug_printf("zero_inf  is %d",zero_inf);
        debug_printf("is_inf  is %d",is_inf);
        debug_printf("is_zero  is %d",is_zero);
        debug_printf("in_range  is %d",in_range);

        //----there is more than a zero in input,set all to zero
        if(is_zero){
            result.sign=sign ^ rhs.sign;
            result.exponent=0x00;
            result.mantissa=0x00;
            debug_printf("into is_zero");
            #if DEBUG_ENABLE
            result.print();
            #endif
        }
        //----there is more than a inf in input 
        if(is_inf){
            result.sign=sign ^ rhs.sign;
            result.exponent=0xff;
            result.mantissa=0x00;
            debug_printf("into is_inf");
            #if DEBUG_ENABLE
            result.print();
            #endif
        }
        //----a zero mul a inf is a nan
        if(zero_inf){
            // result.sign=sign ^ rhs.sign;
            result.sign=1;
            result.exponent=0xff;
            result.mantissa=0x400000;
            debug_printf("into zero_inf");
            #if DEBUG_ENABLE
            result.print();
            #endif

        }
        //----there is a nan in input 
        if(one_nan){
            if(one_nan){
                if(a_is_nan){
                    result.sign=sign;
                    result.exponent=exponent;
                    result.mantissa=mantissa_msb(mantissa);

                }
                if(b_is_nan){
                    result.sign=rhs.sign;
                    result.exponent=rhs.exponent;
                    result.mantissa=mantissa_msb(rhs.mantissa);
                }
            }   
            debug_printf("into one_nan");
            #if DEBUG_ENABLE
            result.print();
            #endif
        }
        //----there are two nans in input 
        if(both_nan){
            result.sign=sign;
            result.exponent=exponent;
            result.mantissa=mantissa_msb(mantissa);
            debug_printf("into both_nan");
            #if DEBUG_ENABLE
            result.print();
            #endif
        }
        //-----both input is in range
        if(in_range){
            debug_printf("into in_range");
            int right_shift=0;
            result.sign         = sign ^ rhs.sign; // calculate the sign of the result
            result.exponent     = exponent + rhs.exponent - 127; // calculate the exponent of the result
            int a_special      = calculator.is_special(imp_a);
            int b_special      = calculator.is_special(imp_b);
            int both_regular   = !(a_special|b_special);
            debug_printf("imp_a is special:%d",a_special);
            debug_printf("imp_b is special:%d",b_special);
            debug_printf("both are regular:%d",both_regular);
            if(both_regular){
                result.mantissa = (mantissa | 0x800000) * (rhs.mantissa | 0x800000); // calculate the significand of the result
            }
            else {
                if(a_special){
                    result.mantissa =mantissa * (rhs.mantissa | 0x800000);
                }
                else{
                    result.mantissa =(mantissa | 0x800000)* rhs.mantissa;
                }
            }
            debug_printf("-----------inputs are---------\n");
            debug_printf("a.sign is     %d",static_cast<int>(sign));
            debug_printf("a.exponent is %u",exponent);
            debug_printf("a.mantissa is %lu",mantissa);
            debug_printf("b.sign is     %d",static_cast<int>(rhs.sign));
            debug_printf("b.exponent is %u",rhs.exponent);
            debug_printf("b.mantissa is %lu",rhs.mantissa);
            debug_printf("-----------total result is---------");
            debug_printf("result.sign is     %d",static_cast<int>(result.sign));
            debug_printf("result.exponent is %u",result.exponent);
            debug_printf("result.exponent is %b",result.exponent);
            debug_printf("underflow_downrange is %b",underflow_downrange);
            debug_printf("underflow_uprange is %b",underflow_uprange);
            debug_printf("result.mantissa is %lb",result.mantissa);
            uint64_t mantissa_ref=0x800000000000;
            debug_printf("mantissa ref is    %lb",mantissa_ref);
            debug_printf("length of mantissa is %d",get_length(result.mantissa));

            if (result.mantissa & 0x800000000000) { // if the significand of the result exceeds 24 bits
                debug_printf("into shift left");
                right_shift=1;
                loop=1;
                calculator.generate_s(&s, &result.mantissa,loop);
                debug_printf("s is %u,result.mantissa is %lb",s, result.mantissa);
                result.exponent++; // increment the exponent
            }

            int left_shift=47-get_length(result.mantissa);
            debug_printf("left_shift is %d",left_shift);
            int result_underflow_down_c=((result.exponent-left_shift)>=underflow_downrange);
            int result_upflow_down_c   =((result.exponent-left_shift)<=underflow_uprange);
            debug_printf("result.exponent-left_shift>=underflow_downrange is %d",result_underflow_down_c);
            debug_printf("result.exponent-left_shift<=underflow_uprange is   %d",result_upflow_down_c);
            result_underflow_down_c=((result.exponent)>=underflow_downrange);
            result_upflow_down_c   =((result.exponent)<=underflow_uprange);
            debug_printf("result.exponent>=underflow_downrange is %d",result_underflow_down_c);
            debug_printf("result.exponent<=underflow_uprange is   %d",result_upflow_down_c);
            //------------if overflow
            if ((result.exponent > 254)&&(result.exponent < 383)) { // if the exponent of the result exceeds the maximum value  //254+254+1-127
                debug_printf("exponent overflow");
                #if NEGATIVE_ENABLE
                if(result.sign){
                    result.exponent = 255; // set the exponent to infinity
                    result.mantissa = 0; // set the significand to 0
                }
                else{
                    result.exponent = 254;
                    result.mantissa = 0x7fffff;
                }
                #endif
                #if POSITIVE_ENABLE
                if(!result.sign){
                    result.exponent = 255; // set the exponent to infinity
                    result.mantissa = 0; // set the significand to 0
                }
                else{
                    result.exponent = 254;
                    result.mantissa = 0x7fffff;
                }
                #endif
                #if NEAREST_ENABLE
                result.exponent = 255; // set the exponent to infinity
                result.mantissa = 0; // set the significand to 0
                #endif
                #if ZERO_ENABLE
                result.exponent = 254;
                result.mantissa = 0x7fffff;   
                #endif             

            } 
            else if (((result.exponent-left_shift)==0)||(((result.exponent)>=underflow_downrange)&&((result.exponent)<=underflow_uprange)||(((result.exponent-left_shift)>=underflow_downrange)&&((result.exponent-left_shift)<=underflow_uprange)))) { // if the exponent of the result is less than the minimum value
                // uint32_t shift_bit=(static_cast<bool>(rhs.exponent)^static_cast<bool>(exponent))?(- result.exponent-1)://there is only one zero
                //                                                                 (- result.exponent);//both are zero 
                while(left_shift>0){
                    result.mantissa<<=1;
                    result.exponent--;
                    left_shift--;
                }
                debug_printf("the length of result.mantissa is %d",get_length(result.mantissa));
                debug_printf("the result.mantissa is %lb",result.mantissa);  
                int nozero_exist=exponent&&rhs.exponent;
                uint32_t shift_bit=(right_shift)?(-result.exponent+1):(-result.exponent);
                shift_bit=shift_bit+nozero_exist-right_shift;
                // uint32_t shift_bit=- result.exponent;
                debug_printf("right_shift is %d",right_shift);
                debug_printf("exponent underflow");
                debug_printf("the length of result.mantissa is %d",get_length(result.mantissa));
                debug_printf("the result.mantissa is %lb",result.mantissa);
                debug_printf("shift_bit is %u",shift_bit);
                calculator.generate_s(&s, &result.mantissa,22);
                r =result.mantissa&0x01;
                g =result.mantissa&0x02;
                result.mantissa     >>=1;
                result.exponent       =0; 
                debug_printf("after shift 23");
                debug_printf("the length of result.mantissa is %d",get_length(result.mantissa));
                debug_printf("the result.mantissa is %lb",result.mantissa);
                if(shift_bit){
                    debug_printf("into shift bit");
                    calculator.generate_s(&s, &result.mantissa,shift_bit-1);
                    r =result.mantissa&0x01;
                    g =result.mantissa&0x02;
                    result.mantissa     >>=1;
                }
                #if NEGATIVE_ENABLE
                debug_printf("into negative_enable");
                round_to_the_negative(&result.sign,&result.exponent,&result.mantissa,g,r,s);
                #endif
                #if POSITIVE_ENABLE
                round_to_the_positive(&result.sign,&result.exponent,&result.mantissa,g,r,s);
                #endif
                #if NEAREST_ENABLE
                round_to_the_nearest(&result.sign,&result.exponent,&result.mantissa,g,r,s);
                #endif
                #if ZERO_ENABLE
                round_to_the_zero(&result.sign,&result.exponent,&result.mantissa,g,r,s);
                #endif
                debug_printf("the length of result.mantissa is %d",get_length(result.mantissa));
                debug_printf("the result.mantissa is %lb",result.mantissa);
            }
            else { // if the exponent of the result is within the valid range
            if(both_regular){
                debug_printf("both are valid");
                uint32_t s_total=get_last_22_bit(result.mantissa);
                loop=22;
                calculator.generate_s(&s, &result.mantissa,loop);
                r=result.mantissa&0x01;
                g=result.mantissa&0x02;
                result.mantissa>>=1;
                result.mantissa &= 0x7fffff; // remove the hidden 1 
                #if NEGATIVE_ENABLE
                debug_printf("into negative_enable");
                round_to_the_negative(&result.sign,&result.exponent,&result.mantissa,g,r,s);
                #endif
                #if POSITIVE_ENABLE
                round_to_the_positive(&result.sign,&result.exponent,&result.mantissa,g,r,s);
                #endif
                #if NEAREST_ENABLE
                round_to_the_nearest(&result.sign,&result.exponent,&result.mantissa,g,r,s);
                #endif
                #if ZERO_ENABLE
                round_to_the_zero(&result.sign,&result.exponent,&result.mantissa,g,r,s);
                #endif
            }          
            else{
                result.exponent+=1;
                uint64_t the_added_1     = calculator.get_the_n_bit(result.mantissa,47);
                uint64_t the_last_47bits = calculator.get_last_n_bit(result.mantissa,47);
                while(!the_added_1){
                    result.mantissa<<=1;
                    result.exponent-=1;
                    the_added_1     = calculator.get_the_n_bit(result.mantissa,47);
                }

                uint32_t s_total=get_last_22_bit(result.mantissa);
                loop=22;
                calculator.generate_s(&s, &result.mantissa,loop);
                r=result.mantissa&0x01;
                g=result.mantissa&0x02;
                result.mantissa>>=1;
                result.mantissa &= 0x7fffff; // remove the hidden 1     
                #if NEGATIVE_ENABLE
                debug_printf("into negative_enable");
                round_to_the_negative(&result.sign,&result.exponent,&result.mantissa,g,r,s);
                #endif
                #if POSITIVE_ENABLE
                round_to_the_positive(&result.sign,&result.exponent,&result.mantissa,g,r,s);
                #endif
                #if NEAREST_ENABLE
                round_to_the_nearest(&result.sign,&result.exponent,&result.mantissa,g,r,s);
                #endif
                #if ZERO_ENABLE
                round_to_the_zero(&result.sign,&result.exponent,&result.mantissa,g,r,s);
                #endif
            }
            }
        }
        return result;
        }

Fp32 Fp32::operator* (const Fp32 &rhs) {
        return mul(rhs);
    }

void Fp32::print() {
    printf("value      : %12f\n", (float)*this);
    printf(" - sign    : %12ld\n",sign);
    printf(" - exponent: %12ld,%64lb, 2^(%d)\n",exponent,exponent,exponent-127);
    printf(" - mantissa: %12ld,%64lb, %f\n",mantissa,mantissa+ (1 << 23),(float)(mantissa + (1 << 23))/(1<<23));
    printf("\n");
    }

uint64_t Fp32::mantissa_msb(uint64_t input_man){
    //aiming for nan calculate, if 
    uint64_t output_man;
    uint32_t last_22_bit=get_last_22_bit(input_man);
    if(!last_22_bit){
        output_man=input_man;
    }
    else{
        output_man=0x400000|input_man;
    }
    return output_man;
}

void Fp32::normalization(uint32_t* exponent,uint64_t* mantissa,uint32_t* s)
{
    Calculate calculator;
    uint64_t the_added_1  = calculator.get_the_n_bit(*mantissa,47);
    while(!the_added_1)
    {
        *mantissa<<=1;
        *exponent-=1;
    }
    uint32_t s_total=get_last_22_bit(*mantissa);
    int loop=22;
    calculator.generate_s(&*s, &*mantissa,loop);///!!!!figure out
    uint64_t r=*mantissa&0x01;
    uint64_t g=*mantissa&0x02;
    *mantissa>>=1;
    debug_printf("---------guard round and sticky---------\n");
    debug_printf("g is %lb",g);
    debug_printf("r is %lb",r);
    debug_printf("s is %lb",s);
    debug_printf("the total bits of s is %lb",s_total);
    debug_printf("mantissa is %lb",*mantissa);
    *mantissa &= 0x7fffff; // remove the hidden 1
    //--------------------------------------------------------------         
    if ((r&&*s)||(g&&r)){ // if rounding is needed 111 110 011 111 110 
        printf("there is a carry\n");
        *mantissa++; // increment the least significant bit
        if (*mantissa & 0x800000) { // if rounding causes overflow
            *mantissa >>= 1; // shift right by one bit
            *exponent++; // increment the exponent
        }
        }//!!!!After the shift, normalization should be performed once again.
}

void Fp32::round_to_the_negative(bool* sign,uint32_t* exponent,uint64_t* mantissa,uint32_t g,uint32_t r,uint32_t s){
    if(*sign) { 
        if (r||s){ // if rounding is needed 111 110 011 111 110 
            (*mantissa)++; // increment the least significant bit
            if ((*mantissa) & 0x800000){ // if rounding causes overflow
                if(*exponent!=0){                        // if exponent is not zero 
                    (*mantissa) >>= 1; // shift right by one bit
                    (*exponent)++; // increment the exponent
                }
                else{
                    (*mantissa)=(*mantissa)& 0x7fffff;
                    (*exponent)++;
                }
            }
        } 
    }
}

void Fp32::round_to_the_positive(bool* sign,uint32_t* exponent,uint64_t* mantissa,uint32_t g,uint32_t r,uint32_t s){
    if(!(*sign)) { 
        if (r||s){ // if rounding is needed 111 110 011 111 110 
            (*mantissa)++; // increment the least significant bit
            if ((*mantissa) & 0x800000){ // if rounding causes overflow
                if(*exponent!=0){                        // if exponent is not zero 
                    (*mantissa) >>= 1; // shift right by one bit
                    (*exponent)++; // increment the exponent
                }
                else{
                    (*mantissa)=(*mantissa)& 0x7fffff;
                    (*exponent)++;
                }
            }
        } 
    }
}

void Fp32::round_to_the_nearest(bool* sign,uint32_t* exponent,uint64_t* mantissa,uint32_t g,uint32_t r,uint32_t s){
    if ((r&&s)||(g&&r)){ // if rounding is needed 111 110 011 111 110 
        (*mantissa)++; // increment the least significant bit
        if ((*mantissa) & 0x800000) { // if rounding causes overflow
            if (*exponent!=0){
                (*mantissa) >>= 1; // shift right by one bit
                (*exponent) ++; // increment the exponent
            }
            else{
                (*mantissa)=(*mantissa)& 0x7fffff;
                (*exponent)++;
            }
        }
        }//!!!!After the shift, normalization should be performed once again.
}

void Fp32::round_to_the_zero(bool* sign,uint32_t* exponent,uint64_t* mantissa,uint32_t g,uint32_t r,uint32_t s){
}
