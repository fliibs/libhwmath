/////////////////////////////////////////////////////////////////////////////////
// Company: BYD
// Engineer: yumiaoyu
//
// Create Date: 01/22/2024 15:54:34 PM
// Design Name: 
// Module Name: add
// Project Name: add
// Description:
//              fp32 add
// Dependencies:
//
// Revision:
//              Revision 0.01 - File Created
// Additional Comments:
//
//////////////////////////////////////////////////////////////////////////////////

module add #(
    parameter int unsigned SIGN_W=1,
    parameter int unsigned EXPO_W=8,
    parameter int unsigned MANT_W=23
) (
    input  logic [SIGN_W + EXPO_W + MANT_W -1 : 0]     a   ,
    input  logic [SIGN_W + EXPO_W + MANT_W -1 : 0]     b   ,
    input  logic [1 : 0]                               rnd ,
    output logic [SIGN_W + EXPO_W + MANT_W -1 : 0]     res   
);
//-----------------------------------//
//type define
//-----------------------------------//
localparam int unsigned MANT_EXP_L =  (MANT_W+1)*2+1    ;
localparam int unsigned MANT_EXP_D =  $clog2(MANT_EXP_L);
localparam int unsigned EXPO_EXP_L =  EXPO_W + 2        ;


typedef logic [MANT_W     - 1:0]  mant_l                ;
typedef logic [MANT_EXP_L - 1:0]  mant_exp_l            ;
typedef logic [MANT_EXP_D - 1:0]  mant_exp_d            ;

typedef logic [EXPO_W     - 1:0]  expo_l                ;
typedef logic [EXPO_EXP_L - 1:0]  expo_exp_l            ;

//-----------------------------------//
//internal signals
//-----------------------------------//

//-------------unpack signals
logic                 a_sign        ;              
expo_l                a_expo        ;
mant_l                a_mant        ;
logic                 b_sign        ;
expo_l                b_expo        ;
mant_l                b_mant        ;

//-------------nan flag
logic                 a_is_n0       ;
logic                 b_is_n0       ;
logic                 a_is_inf      ;
logic                 b_is_inf      ;
logic                 a_is_nan      ;
logic                 b_is_nan      ;

//-------------expand mant_a and mant_B to 48 bit
mant_exp_l            mant_a_l      ;
mant_exp_l            mant_b_l      ;

//-------------shift and complement judge 
logic                 a_big         ;
logic                 same_sign     ;
// logic                 sign_with_m   ;
logic                 sign_neg      ;
              
mant_exp_l            mant_s_unc    ;
mant_exp_l            mant_s        ;
mant_exp_l            mant_c        ;
              
logic                 minu_expo_or  ;
logic                 subtr_expo_or ;
              
expo_l                minu_e        ;
expo_l                subtr_e       ;

expo_l                l_shift_unc   ;
mant_exp_d            l_shift       ;
//-------------mant_1
mant_exp_l            mant_a        ;
mant_exp_l            mant_b        ;
expo_exp_l            expo_1        ;
mant_exp_l            mant_1        ;
//-------------mant_2
logic                 mant_carry    ;
logic                 mant_1_sign   ;
logic                 sign_2        ;
mant_exp_l            mant_mid      ;
mant_exp_d            zero_nums     ;
mant_exp_l            mant_2_prs_e  ;
mant_exp_l            mant_2_prs_n  ;
expo_exp_l            r_shift_unc   ;   
mant_exp_d            r_shift       ;
mant_exp_l            mant_2_unc    ;
mant_exp_l            mant_2        ;
expo_exp_l            expo_2_unc    ;
expo_exp_l            expo_2        ;
//-------------rounding 
logic                        g             ;
logic                        r             ;
logic                        s             ;
logic                        rnd_carry     ;
logic                        sign_c        ;
logic[MANT_EXP_L-MANT_W-2:0] mant_rnd      ;
expo_exp_l                   expo_3        ;
mant_l                       mant_3        ;

//-------------masking
logic                overflow       ;
logic                r_is_nan       ;
logic                is_inf_nan     ;
mant_l               mant_unsel     ;
logic                sign_unsel     ;
logic                sign_nan       ;


//-------------masking
logic                res_sign       ;
expo_l               res_expo       ;
mant_l               res_mant       ;

//-----------------------------------
//fix cal
//-----------------------------------
assign a_sign   = a[SIGN_W + EXPO_W + MANT_W - 1]                   ;
assign a_expo   = a[EXPO_W + MANT_W - 1 : MANT_W]                   ;
assign a_mant   = a[MANT_W - 1 : 0]                                 ;

assign b_sign   = b[SIGN_W + EXPO_W + MANT_W - 1]                   ;
assign b_expo   = b[EXPO_W + MANT_W - 1 : MANT_W]                   ;
assign b_mant   = b[MANT_W - 1 : 0]                                 ;

//-----------------------------------
//expand mant_a
//-----------------------------------MANT_EXP_L
assign mant_a_l = {2'b0,{|a_expo},a_mant,23'b0}    ;
assign mant_b_l = {2'b0,{|b_expo},b_mant,23'b0}    ;

//-----------------------------------
//shift and complement judge
//-----------------------------------
assign a_big        =  ({a_expo,a_mant} >= {b_expo,b_mant})? 1'b1:1'b0      ;
assign same_sign    =  (a_sign==b_sign)                    ? 1'b1:1'b0      ;
// assign sign_with_m  =  (!same_sign) && ((!a_big) ^ a_sign)                  ;
assign sign_neg     =  (!same_sign) && (a_big    ^ a_sign)                  ;

assign minu_e       =  a_big   ? a_expo     : b_expo    ;
assign minu_expo_or =  a_big   ? |a_expo    : |b_expo   ;
assign subtr_e      =  a_big   ? b_expo     : a_expo    ;
assign subtr_expo_or=  a_big   ? |b_expo    : |a_expo   ;


assign l_shift_unc  =  minu_e - subtr_e + !(minu_expo_or) - (!subtr_expo_or);
assign l_shift      =  ((l_shift_unc[MANT_EXP_D-1]&&l_shift_unc[MANT_EXP_D-2])||(|l_shift_unc[EXPO_W-1:MANT_EXP_D]))?6'b101111:l_shift_unc[MANT_EXP_D-1:0];

assign mant_s_unc   =  a_big   ? (mant_b_l>>l_shift) : (mant_a_l>>l_shift)  ; //47-shifter   !!!optimized
always_comb begin 
    if(a_big) begin
        if((mant_s_unc=={(MANT_EXP_L){1'b0}}) && (mant_b_l!={(MANT_EXP_L){1'b0}}))
            mant_s ={{(MANT_EXP_L-1){1'b0}},{1'b1}}        ;
        else
            mant_s = mant_s_unc;
    end
    else begin
        if((mant_s_unc=={(MANT_EXP_L){1'b0}}) && (mant_a_l!={(MANT_EXP_L){1'b0}}))
            mant_s ={{(MANT_EXP_L-1){1'b0}},{1'b1}}        ;
        else
            mant_s = mant_s_unc;
    end
end
always_comb begin
    if(a_big && (a_sign||sign_neg))
        mant_c = {{(MANT_EXP_L-1){1'b0}},{1'b1}} + (~mant_a_l);
    else begin
        if((a_big) && (!a_sign))
            mant_c = mant_a_l;
        else begin
            if((!a_big) && (b_sign ||sign_neg))
                mant_c = {{(MANT_EXP_L-1){1'b0}},{1'b1}} + (~mant_b_l);
            else
                mant_c = mant_b_l;
        end
    end
end

//-----------------------------------
//mant_1
//-----------------------------------
assign mant_a       =   mant_s                                              ;
assign mant_b       =   same_sign ? (a_big ? mant_a_l : mant_b_l): mant_c   ;
assign expo_1       =   {2'b0,minu_e}                                       ;
assign mant_1       =   mant_a + mant_b                                     ;


//-----------------------------------
//mant_2
//-----------------------------------
assign mant_1_sign  =   mant_1[MANT_EXP_L-1]                                                                    ;
assign sign_2       =   sign_neg             ? (!mant_1_sign)                                : mant_1_sign      ;
assign mant_mid     =   (sign_2 || sign_neg) ? ({{(MANT_EXP_L-1){1'b0}},{1'b1}} + (~mant_1)) : mant_1           ;
assign mant_carry   =   mant_mid[MANT_EXP_L-2]                                                                  ;
assign mant_2_unc   =   mant_carry           ?  {1'b0,mant_mid[MANT_EXP_L-1:1]}              : mant_mid         ;
assign expo_2_unc   =   mant_carry           ?  expo_1 + {{(EXPO_EXP_L){1'b0}},1'b1}         : expo_1           ;

add_lod u_add_lod(
.in_detect(mant_2_unc[MANT_EXP_L-3:0]),
.zero_nums(zero_nums) ,
.out_shift(mant_2_prs_n[MANT_EXP_L-3:0])
);

assign mant_2_prs_n[MANT_EXP_L-1:MANT_EXP_L-2]=2'b0         ;

assign r_shift_unc  = (expo_2_unc!=10'b0) ? (expo_2_unc-10'b1) : expo_2_unc ;
assign r_shift      = ((r_shift_unc[MANT_EXP_D-1]&&r_shift_unc[MANT_EXP_D-2])||(|r_shift_unc[EXPO_W-1:MANT_EXP_D]))?6'b101111:r_shift_unc[MANT_EXP_D-1:0];
assign mant_2_prs_e = mant_2_unc << r_shift ; //47 right shift optimzed!!!!!;
always_comb begin
    if(expo_2_unc<={{(EXPO_EXP_L-MANT_EXP_D){1'b0}},zero_nums})begin
        mant_2 = mant_2_prs_e;
        expo_2 = {EXPO_EXP_L{1'b0}};
    end
    else begin
        mant_2 = mant_2_prs_n;
        expo_2 = expo_2_unc - {{(EXPO_EXP_L-MANT_EXP_D){1'b0}},zero_nums};
    end
end

//-----------------------------------
//rounding 
//-----------------------------------
assign g         =  mant_2[MANT_W]                       ;
assign r         =  mant_2[MANT_W-1]                         ;
assign s         =  (|mant_2[MANT_W-2:0]) || mant_mid[0]   ;
assign sign_c    =  same_sign ? a_sign : sign_2            ;

assign rnd_carry = (rnd==2'b11) ? ((r&&(g||s)) ? 1'b1 : 1'b0):
                   (rnd==2'b10) ? (((!sign_c) && a_is_n0 && b_is_n0 && (r||s||(expo_2==0 && mant_2==0))) ? 1'b1 : 1'b0) :
                   (rnd==2'b01) ? ((sign_c    && a_is_n0 && b_is_n0 && (r||s||(expo_2==0 && mant_2==0))) ? 1'b1 : 1'b0) :
                   0; 

assign mant_rnd  = mant_2[MANT_EXP_L-2:MANT_W]+rnd_carry                                                                                                                            ;
assign expo_3    = (mant_rnd[MANT_EXP_L-MANT_W-2] || ((expo_2==0) && (mant_rnd[MANT_EXP_L-MANT_W-3])))   ? (expo_2 + {{(EXPO_EXP_L-1){1'b0}},{1'b1}}) : expo_2              ;
assign mant_3    = (mant_rnd[MANT_EXP_L-MANT_W-2])                                                       ? mant_rnd[MANT_W:1]                         : mant_rnd[MANT_W-1:0]; 

//-----------------------------------
//masking
//-----------------------------------
assign a_is_n0    = |a[EXPO_W + MANT_W -1 : 0]        ;
assign b_is_n0    = |b[EXPO_W + MANT_W -1 : 0]        ;
assign a_is_nan   = (&a_expo) && (|a_mant)            ;
assign b_is_nan   = (&b_expo) && (|b_mant)            ;
  
assign overflow   = expo_3>={2'b0,8'hff} ? 1'b1: 1'b0 ;
assign r_is_nan   = a_is_nan  || b_is_nan             ;
assign is_inf_nan = (&a_expo) || (&b_expo)            ;

assign mant_unsel = ({a_expo,{a_mant!=0}})  >= ({b_expo,{b_mant!=0}}) ? {1'b1,a_mant[MANT_W-2:0]} : {1'b1,b_mant[MANT_W-2:0]};
assign sign_unsel = ({a_expo,{a_mant!=0}})  >= ({b_expo,{b_mant!=0}}) ? a_sign                    : sign_c                   ;
assign res_sign   = r_is_nan    ?   sign_unsel  :   sign_c  ;

always_comb begin
    if(is_inf_nan)begin
        res_expo = 8'hff;
        res_mant = r_is_nan ? mant_unsel :  0 ;
    end
    else begin
        if(overflow) begin
            res_expo = ((rnd[1] &&(!res_sign)) || (rnd[0] && res_sign)) ? 8'hff : 8'hfe     ;
            res_mant = ((rnd[1] &&(!res_sign)) || (rnd[0] && res_sign)) ? 23'h0 :23'h7fffff ;
        end 
        else begin
            res_expo = expo_3[EXPO_W-1:0] ;
            res_mant = mant_3             ;
        end 
    end
end
assign res = {res_sign,res_expo,res_mant} ;
endmodule