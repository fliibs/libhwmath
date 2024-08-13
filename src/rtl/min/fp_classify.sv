module fp_classify #(
    parameter int unsigned SIGN_W=1,
    parameter int unsigned EXPO_W=8,
    parameter int unsigned MANT_W=23,
    localparam int unsigned FP_W   = SIGN_W+EXPO_W+MANT_W
) (
    input  logic  [FP_W-1 : 0]  ind         ,
    output logic                is_nan      ,
    output logic                is_snan     ,
    output logic                is_zero     ,
    output logic                is_inf      ,
    output logic                is_inf_pos  ,
    output logic                is_inf_neg  ,
    output logic                is_nor    
);

    logic is_q;

    inf_chk #(
        .SIGN_W(SIGN_W),
        .EXPO_W(EXPO_W),
        .MANT_W(MANT_W)
    ) u_inf_chk (
        .ind(ind),
        .res(is_inf),
        .res_pos(is_inf_pos),
        .res_neg(is_inf_neg)
    );

    nan_chk #(
        .SIGN_W(SIGN_W),
        .EXPO_W(EXPO_W),
        .MANT_W(MANT_W)
    ) u_nan_chk (
        .ind(ind),
        .res_nan(is_nan),
        .res_snan(is_snan)
    );

    zero_chk #(
        .SIGN_W(SIGN_W),
        .EXPO_W(EXPO_W),
        .MANT_W(MANT_W)
    ) u_zero_chk (
        .ind(ind),
        .res(is_zero)
    );

    assign is_nor = (!is_inf) && (!is_zero) && (!is_nan);
    

endmodule
