module fp_unpack #(
    parameter int unsigned SIGN_W = 1,
    parameter int unsigned EXPO_W = 8,
    parameter int unsigned MANT_W = 23
)(
    input  logic [SIGN_W + EXPO_W + MANT_W -1 : 0 ] ind       ,
    output logic [SIGN_W - 1 : 0]                   ind_sign  ,
    output logic [EXPO_W - 1 : 0]                   ind_expo  ,
    output logic [MANT_W - 1 : 0]                   ind_mant  
);

assign ind_sign   = ind[SIGN_W + EXPO_W + MANT_W - 1]   ;
assign ind_expo   = ind[EXPO_W + MANT_W - 1 : MANT_W]   ;
assign ind_mant   = ind[MANT_W - 1 : 0]                 ;

endmodule 