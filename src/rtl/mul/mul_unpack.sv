module mul_unpack #(
    parameter int unsigned SIGN_W = 1,
    parameter int unsigned EXPO_W = 8,
    parameter int unsigned MANT_W = 23
)(
    input  logic [SIGN_W + EXPO_W + MANT_W -1 : 0 ] a       ,
    input  logic [SIGN_W + EXPO_W + MANT_W -1 : 0 ] b       ,
    output logic                                    a_sign  ,
    output logic                                    b_sign  ,
    output logic [EXPO_W - 1 : 0]                   a_expo  ,
    output logic [EXPO_W - 1 : 0]                   b_expo  ,
    output logic [MANT_W - 1 : 0]                   a_mant  ,
    output logic [MANT_W - 1 : 0]                   b_mant  
);

assign a_sign   = a[SIGN_W + EXPO_W + MANT_W - 1]                   ;
assign a_expo   = a[EXPO_W + MANT_W - 1 : MANT_W]                   ;
assign a_mant   = a[MANT_W - 1 : 0]                                 ;

assign b_sign   = b[SIGN_W + EXPO_W + MANT_W - 1]                   ;
assign b_expo   = b[EXPO_W + MANT_W - 1 : MANT_W]                   ;
assign b_mant   = b[MANT_W - 1 : 0]                                 ;

endmodule 