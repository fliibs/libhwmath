module mul_fixcal #(
    parameter int unsigned SIGN_W = 1,
    parameter int unsigned EXPO_W = 8,
    parameter int unsigned MANT_W = 23
) (
    // input  logic                    a_sign    ,
    // input  logic                    b_sign    ,
    input  logic  [EXPO_W - 1 : 0]  a_expo    ,
    input  logic  [EXPO_W - 1 : 0]  b_expo    ,
    input  logic  [MANT_W - 1 : 0]  a_mant    ,
    input  logic  [MANT_W - 1 : 0]  b_mant    ,
    output logic                    a_is_sub  ,
    output logic                    b_is_sub  ,
    output logic                    a_is_n0   ,
    output logic                    b_is_n0   ,
    output logic                    a_is_nan  ,
    output logic                    b_is_nan  ,
    // output logic                    a_is_inf  ,
    // output logic                    b_is_inf  ,
    output logic                    r_isnan   ,
    output logic                    is_inf_nan,
    output logic                    r_is0nan  
);
logic  a_is_inf     ;
logic  b_is_inf     ;

assign a_is_sub     = |a_expo                               ;
assign b_is_sub     = |b_expo                               ;
assign a_is_n0      =  a_is_sub || (|a_mant)                ;
assign b_is_n0      =  b_is_sub || (|b_mant)                ;
assign a_is_nan     =  (&a_expo)&&(|a_mant)                 ;
assign b_is_nan     =  (&b_expo)&&(|b_mant)                 ;
assign a_is_inf     =  (&a_expo)&&(!(|a_mant))              ;
assign b_is_inf     =  (&b_expo)&&(!(|b_mant))              ;

assign r_isnan      =  a_is_nan    || b_is_nan || r_is0nan                       ;
assign is_inf_nan   =  a_is_nan    || b_is_nan || a_is_inf    || b_is_inf        ;
assign r_is0nan     =  ((!a_is_n0) && b_is_inf)|| ((!b_is_n0) && a_is_inf)       ;



endmodule