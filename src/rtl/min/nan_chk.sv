module nan_chk #(
    parameter  int unsigned SIGN_W = 1, 
    parameter  int unsigned EXPO_W = 8, 
    parameter  int unsigned MANT_W = 23,
    localparam int unsigned FP_W   = SIGN_W+EXPO_W+MANT_W
) (
    input logic                    ind_sign   ,
    input logic [EXPO_W - 1 : 0]   ind_expo   ,
    input logic [MANT_W - 1 : 0]   ind_mant   ,
    output logic                    res_nan    ,
    output logic                    res_snan       
);

    logic q_hint;

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

    // assign q_hint = !((ind_mant >> MANT_W) == {MANT_W{1'b0}});
    assign q_hint = ind_mant[MANT_W-1];


    //check SNaN
    assign res_nan  = max_expo_chk_res && !zero_mant_chk_res; 
    assign res_snan = res_nan && !q_hint; 

    
endmodule