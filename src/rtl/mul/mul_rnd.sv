module mul_rnd #(
    parameter int unsigned SIGN_W = 1,
    parameter int unsigned EXPO_W = 8,
    parameter int unsigned MANT_W = 23
) (
    input                             sign_1      ,
    input  logic [2*MANT_W + 1: 0]    mant_2      ,
    input  logic [EXPO_W + 1  : 0]    expo_2      ,
    input  logic                      bit_s_record,
    input  logic [1:0]                rnd         ,
    input  logic                      a_is_n0     ,
    input  logic                      b_is_n0     ,
    output logic [EXPO_W + 1 : 0]     expo_3      ,
    output logic [MANT_W - 1 : 0]     mant_3      
);
logic                               g           ;
logic                               r           ;
logic                               s           ;
logic                               carry       ;
logic   [MANT_W + 1 : 0]            mant_inr_1  ;
logic   [EXPO_W + 1 : 0]            expo_inr_1  ;
logic   [MANT_W + 1 : 0]            mant_rnd    ;
logic   [EXPO_W + 1 : 0]            expo_rnd    ;
    
assign g         =  mant_2[MANT_W]                           ;
assign r         =  mant_2[MANT_W-1]                         ;
assign s         =  (|mant_2[MANT_W-2:0]) | bit_s_record     ;
      
assign carry     =  (rnd==2'b11) ? ((r        && (g || s))                                            ? 1'b1 : 1'b0)  :
                    (rnd==2'b10) ? ((!sign_1) && (a_is_n0) && (b_is_n0) && (r || s || (!(|expo_2 )))  ? 1'b1 : 1'b0)  :
                    (rnd==2'b01) ? (sign_1    && (a_is_n0) && (b_is_n0) && (r || s || (!(|expo_2 )))  ? 1'b1 : 1'b0)  :
                    1'b0                                     ;

assign mant_inr_1 = mant_2[2*MANT_W + 1: MANT_W] + 1'b1      ;
assign expo_inr_1 = expo_2                       + 1'b1      ;

assign mant_rnd  = carry                                                           ?  mant_inr_1                : mant_2[2*MANT_W + 1: MANT_W] ;
assign expo_3    = (mant_rnd[MANT_W + 1] || ((!(|expo_2)) && mant_rnd[MANT_W]))    ?  expo_inr_1                : expo_2                       ;
assign mant_3    = mant_rnd[MANT_W + 1]                                            ?  mant_rnd[MANT_W + 1 : 1]  : mant_rnd[MANT_W    : 0]      ;

endmodule