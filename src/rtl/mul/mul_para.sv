//////////////////////////////////////////////////////////////////////////////////
// Company: BYD.LTD
// Engineer: yumiaoyu
//
// Create Date: 01/09/2024 15:54:34 PM
// Design Name: 
// Module Name: mul_para
// Project Name: mul_para
// Description:
//              fp32 mul,lod parallel
// Dependencies:
//
// Revision:
//              Revision 0.01 - File Created
// Additional Comments:
//
//////////////////////////////////////////////////////////////////////////////////

module mul_para #(
    parameter int unsigned SIGN_W = 1,
    parameter int unsigned EXPO_W = 8,
    parameter int unsigned MANT_W=23
) (
    input  logic [SIGN_W + EXPO_W + MANT_W -1 : 0]     a   ,
    input  logic [SIGN_W + EXPO_W + MANT_W -1 : 0]     b   ,
    input  logic [1 : 0]                               rnd ,
    output logic [SIGN_W + EXPO_W + MANT_W -1 : 0]     res  

);

//-----------------------------------//
//internal signals
//-----------------------------------//
//-------------nan flag
logic   a_is_sub     ;
logic   b_is_sub     ;
logic   a_is_nan    ;
logic   b_is_nan    ;
logic   a_is_inf    ;
logic   b_is_inf    ;
logic   a_is_n0      ;
logic   b_is_n0      ;

//-------------unpack signals
logic                               a_sign      ;              
logic   [EXPO_W - 1 : 0]            a_expo      ;
logic   [MANT_W - 1 : 0]            a_mant      ;
logic                               b_sign      ;
logic   [EXPO_W - 1 : 0]            b_expo      ;
logic   [MANT_W - 1 : 0]            b_mant      ;

//-------------pack signals
logic                               res_sign    ;
logic   [EXPO_W - 1 : 0]            res_expo    ;
logic   [MANT_W - 1 : 0]            res_mant    ;


//-------------temp signals
logic                               sign_1      ;
logic   [EXPO_W + 1 : 0]            expo_1      ;
logic   [2*MANT_W + 1 : 0]          mant_1      ;

//-------------lod signals
logic   [MANT_W : 0]                mant_input  ;
logic   [MANT_W : 0]                mask_short  ;
logic   [2*MANT_W + 1 : 0]          mask_long   ;
logic                               revise      ;
logic   [$clog2(MANT_W + 1) -1: 0]  zero_nums_uc;
logic   [$clog2(MANT_W + 1) -1: 0]  zero_nums_c ;

//-------------expo adjust and mant shift signals
logic   [EXPO_W + 1  : 0]           expo_2      ;
logic   [2*MANT_W + 1: 0]           mant_2      ;

//-------------round signals
logic                               bit_s_record;
logic                               g           ;
logic                               r           ;
logic                               s           ;
logic                               carry       ;
logic   [MANT_W + 1 : 0]            mant_inr_1  ;
logic   [EXPO_W + 1 : 0]            expo_inr_1  ;
logic   [MANT_W + 1 : 0]            mant_rnd    ;
logic   [EXPO_W + 1 : 0]            expo_rnd    ;
logic   [EXPO_W + 1 : 0]            expo_3      ;
logic   [MANT_W - 1 : 0]            mant_3      ;
//-------------inf and nan signals 
logic                               overflow    ;
logic                               r_isnan     ;
logic                               is_inf_nan  ;
logic                               r_is0nan    ;
logic                               sign_nan    ;
logic   [MANT_W - 1 : 0]            mant_4      ;
//-------------debug signals
logic   [EXPO_W + 1 : 0]            expo_d_1    ;
logic   [EXPO_W + 1 : 0]            expo_d_2    ;

//-----------------------------------
//fix cal
//-----------------------------------
assign a_sign   = a[SIGN_W + EXPO_W + MANT_W - 1]                   ;
assign a_expo   = a[EXPO_W + MANT_W - 1 : MANT_W]                   ;
assign a_mant   = a[MANT_W - 1 : 0]                                 ;
assign a_is_sub  = |a_expo                                           ;

assign b_sign   = b[SIGN_W + EXPO_W + MANT_W - 1]                   ;
assign b_expo   = b[EXPO_W + MANT_W - 1 : MANT_W]                   ;
assign b_mant   = b[MANT_W - 1 : 0]                                 ;
assign b_is_sub  = |b_expo                                           ;

assign sign_1   = a_sign ^ b_sign                                   ;
assign expo_1   = a_expo + b_expo - 7'h7f + !a_is_sub + !b_is_sub     ;
assign mant_1   = {a_is_sub,a_mant} * {b_is_sub,b_mant}               ;

//-----------------------------------
//loz
//-----------------------------------
assign mant_input = (a_is_sub && (!b_is_sub)) ? {1'b0,b_mant}     :
                    ((!a_is_sub) && b_is_sub) ? {1'b0,a_mant}     :
                    24'hffffff                                  ;
mul_lod #(
    .INPUT_WIDTH(24))
u_lod(
    .in_detect(mant_input),
    .zero_nums(zero_nums_uc)
);

mul_mask #(
    .INPUT_WIDTH(24))
u_mask(
    .input_unmask(mant_input),
    .output_mask(mask_short)
);

assign mask_long  = {mask_short,24'b0}                         ;
assign revise     = (|(mant_1 & mask_long)) ? 1'b1 : 1'b0      ;
assign zero_nums_c= revise ? zero_nums_uc - 1 : zero_nums_uc   ;


//----------------------------------------------------------
//expo adjust and mant shift
//----------------------------------------------------------

always_comb begin
    bit_s_record        = 1'b0   ;  //
    mant_2              = 47'b0  ;
    expo_2              = 10'b0  ;
    if(mant_1[2*MANT_W + 1]&&(!expo_1[ EXPO_W + 1 ])) begin
        expo_2          = expo_1 + 1'b1                ;
        mant_2          = {1'b0,mant_1[2*MANT_W+1 : 1]};
        bit_s_record    = mant_1[0]                    ;
    end
    else begin
        if((!expo_1[EXPO_W+1]) && (expo_1[EXPO_W : 0] > zero_nums_c) && (|mant_1)) begin
            expo_2      = expo_1 - zero_nums_c         ;
            mant_2      = mant_1 << zero_nums_c        ;
        end
        else begin
            expo_2      = 10'b0                        ;
            if ((!expo_1[EXPO_W+1]) && (|expo_1[EXPO_W : 0])) 
                mant_2  = mant_1 << (expo_1 - 10'b1)   ;
            else 
                mant_2  = mant_1 >> (10'b1 - expo_1)   ; 
        end
    end
end
//---------------------------------------------------
//rounding
//---------------------------------------------------
assign g         =  mant_2[MANT_W]                           ;
assign r         =  mant_2[MANT_W-1]                         ;
assign s         =  (|mant_2[MANT_W-2:0]) | bit_s_record     ;

assign a_is_n0       =   a_is_sub || (|a_mant)                ;
assign b_is_n0       =   b_is_sub || (|b_mant)                ;
// assign carry     =  (rnd==2'b11) ? ((r        && (g || s))    ? 1'b1 : 1'b0) :
//                     (rnd==2'b10) ? (((!sign_1) && (|mant_2))  ? 1'b1 : 1'b0)  :
//                     (rnd==2'b01) ? ((sign_1    && (|mant_2))  ? 1'b1 : 1'b0)  :
//                     1'b0  
      
assign carry     =  (rnd==2'b11) ? ((r        && (g || s))                                            ? 1'b1 : 1'b0)  :
                    (rnd==2'b10) ? ((!sign_1) && (a_is_n0) && (b_is_n0) && (r || s || (!(|expo_2 )))  ? 1'b1 : 1'b0)  :
                    (rnd==2'b01) ? (sign_1    && (a_is_n0) && (b_is_n0) && (r || s || (!(|expo_2 )))  ? 1'b1 : 1'b0)  :
                    1'b0                                     ;

assign mant_inr_1 = mant_2[2*MANT_W + 1: MANT_W] + 1'b1      ;
assign expo_inr_1 = expo_2                       + 1'b1      ;

assign mant_rnd  = carry                                                           ?  mant_inr_1                : mant_2[2*MANT_W + 1: MANT_W] ;
assign expo_3    = (mant_rnd[MANT_W + 1] || ((!(|expo_2)) && mant_rnd[MANT_W]))    ?  expo_inr_1                : expo_2                       ;
assign mant_3    = mant_rnd[MANT_W + 1]                                            ?  mant_rnd[MANT_W + 1 : 1]  : mant_rnd[MANT_W    : 0]      ;

//---------------------------------------------------
//inf and nan mux
//---------------------------------------------------

assign overflow     =  (expo_2 >= 10'd255) || (expo_3 >= 10'd255)  ;
assign a_is_nan     =  (&a_expo)&&(|a_mant)                 ;
assign b_is_nan     =  (&b_expo)&&(|b_mant)                 ;
assign a_is_inf     =  (&a_expo)&(!(|a_mant))               ;
assign b_is_inf     =  (&b_expo)&(!(|b_mant))               ;

assign r_isnan      =  a_is_nan | b_is_nan | r_is0nan                                                ;
assign is_inf_nan   =  a_is_nan | b_is_nan | a_is_inf | b_is_inf                                     ;
assign r_is0nan     =  ((!a_is_n0) & b_is_inf) | ((!b_is_n0) & a_is_inf)                               ;
always_comb begin
    if(r_isnan) begin
        if({a_expo,a_is_nan} >  {b_expo,b_is_nan}) begin
            mant_4      =  {1'b1,a_mant[MANT_W-2:0]};
            sign_nan    =  a_sign | r_is0nan        ;
        end
        else begin
            mant_4      =  {1'b1,b_mant[MANT_W-2:0]};
            sign_nan    =  b_sign | r_is0nan        ;
        end
    end
    else begin
        mant_4      =   23'b0                       ;
        sign_nan    =   r_is0nan | sign_1           ;
    end
end

always_comb begin
    if (is_inf_nan) begin
        res_sign    =   sign_nan                    ;
        res_expo    =   8'hff                       ;
        res_mant    =   mant_4                      ;                     
    end 
    else begin
        res_sign    =   sign_1                      ;
        if (overflow) begin
            res_expo =  (((rnd[1]) && (!res_sign)) || ((rnd[0]) && res_sign)) ? 8'd255 : 8'd254     ;
            res_mant =  (((rnd[1]) && (!res_sign)) || ((rnd[0]) && res_sign)) ? 23'b0  : 23'h7fffff ;
        end 
        else begin
            res_expo =  expo_3 ;
            res_mant =  mant_3 ;
        end
    end
end
assign res={res_sign,res_expo,res_mant};

endmodule