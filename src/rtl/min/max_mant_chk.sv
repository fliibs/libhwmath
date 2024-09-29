module max_mant_chk #(
    parameter int unsigned SIGN_W = 1,
    parameter int unsigned EXPO_W = 8,
    parameter int unsigned MANT_W = 23,
    localparam int unsigned FP_W   = SIGN_W+EXPO_W+MANT_W
) (
    input logic                    ind_sign   ,
    input logic [EXPO_W - 1 : 0]   ind_expo   ,
    input logic [MANT_W - 1 : 0]   ind_mant   ,
    output logic                    res                       
);

    assign res = (ind_mant=={MANT_W{1'b1}});
    
endmodule