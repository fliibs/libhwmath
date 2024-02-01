module mul_1 #(
    parameter int unsigned SIGN_W = 1,
    parameter int unsigned EXPO_W = 8,
    parameter int unsigned MANT_W = 23
)(
    input  logic                     a_sign  ,
    input  logic                     b_sign  ,
    input  logic                     a_is_sub,
    input  logic                     b_is_sub,
    input  logic [EXPO_W - 1 : 0]    a_expo  ,
    input  logic [EXPO_W - 1 : 0]    b_expo  ,
    input  logic [MANT_W - 1 : 0]    a_mant  ,
    input  logic [MANT_W - 1 : 0]    b_mant  ,
    output logic                     sign_1  ,
    output logic [EXPO_W + 1 : 0]    expo_1  ,
    output logic [2*MANT_W + 1 : 0]  mant_1  
);
assign sign_1 = a_sign ^ b_sign                                   ;
assign expo_1 = a_expo + b_expo - 7'h7f + !a_is_sub + !b_is_sub   ;
// mul_dw02 #(
//     .width(MANT_W+1)
// )u_mul_dw02(
//     .a({a_is_sub,a_mant})   ,
//     .b({b_is_sub,b_mant})   ,
//     .c(mant_1)   
// );
assign mant_1   = {a_is_sub,a_mant} * {b_is_sub,b_mant}            ;
endmodule