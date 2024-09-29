module inf_chk #(
    parameter  int unsigned SIGN_W = 1, 
    parameter  int unsigned EXPO_W = 8, 
    parameter  int unsigned MANT_W = 23,
    localparam int unsigned FP_W   = SIGN_W+EXPO_W+MANT_W
) (
    input logic                    ind_sign   ,
    input logic [EXPO_W - 1 : 0]   ind_expo   ,
    input logic [MANT_W - 1 : 0]   ind_mant   ,

    output logic             res        ,
    output logic             res_pos    ,
    output logic             res_neg    
);

    max_expo_chk #(
        .SIGN_W(SIGN_W),
        .EXPO_W(EXPO_W),
        .MANT_W(MANT_W)
    ) u_inf_max_expo_chk (
        .ind_sign(ind_sign),
        .ind_expo(ind_expo),
        .ind_mant(ind_mant),
        .res(max_expo_chk_res)
    );

    zero_mant_chk #(
        .SIGN_W(SIGN_W),
        .EXPO_W(EXPO_W),
        .MANT_W(MANT_W)
    ) u_inf_zero_mant_chk (
        .ind_sign(ind_sign),
        .ind_expo(ind_expo),
        .ind_mant(ind_mant),
        .res(zero_mant_chk_res)
    );

    assign res = max_expo_chk_res && zero_mant_chk_res ; 
    assign res_neg = ind_sign && res;
    assign res_pos = !ind_sign && res;
    
endmodule