module max_mant_chk #(
    parameter int unsigned SIGN_W = 1,
    parameter int unsigned EXPO_W = 8,
    parameter int unsigned MANT_W = 23,
    localparam int unsigned FP_W   = SIGN_W+EXPO_W+MANT_W
) (
    input  logic [FP_W-1 : 0 ]  ind ,
    output logic                res                       
);

    assign res = (ind[0 +: MANT_W]=={MANT_W{1'b1}});
    
endmodule