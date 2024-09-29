module fp_min #(
    parameter int unsigned SIGN_W=1,
    parameter int unsigned EXPO_W=8,
    parameter int unsigned MANT_W=23
) (
    input  logic  [SIGN_W + EXPO_W + MANT_W -1 : 0]     ina ,
    input  logic  [SIGN_W + EXPO_W + MANT_W -1 : 0]     inb ,
    input  logic  [1 : 0]                               rnd ,
    output logic  [4 : 0]                               status,
    output logic  [SIGN_W + EXPO_W + MANT_W -1 : 0]     res 
);

logic                   a_sign;
logic [EXPO_W - 1 : 0]  a_expo;
logic [MANT_W - 1 : 0]  a_mant;
logic                   b_sign;
logic [EXPO_W - 1 : 0]  b_expo;
logic [MANT_W - 1 : 0]  b_mant;

logic   a_is_nan    ;
logic   a_is_snan   ;
logic   a_is_zero   ;
logic   a_is_inf    ;
logic   a_is_inf_pos;
logic   a_is_inf_neg;
logic   a_is_nor    ;

logic   b_is_nan    ;
logic   b_is_snan   ;
logic   b_is_zero   ;
logic   b_is_inf    ;
logic   b_is_inf_pos;
logic   b_is_inf_neg;
logic   b_is_nor    ;


fp_unpack #(
    .SIGN_W(SIGN_W),
    .EXPO_W(EXPO_W),
    .MANT_W(MANT_W)
) u_unpack_a (
    .ind     (ina),
    .ind_sign(a_sign),
    .ind_expo(a_expo),
    .ind_mant(a_mant)
);

fp_unpack #(
    .SIGN_W(SIGN_W),
    .EXPO_W(EXPO_W),
    .MANT_W(MANT_W)
) u_unpack_b (
    .ind     (inb),
    .ind_sign(b_sign),
    .ind_expo(b_expo),
    .ind_mant(b_mant)
);

fp_classify #(
    .SIGN_W(SIGN_W),
    .EXPO_W(EXPO_W),
    .MANT_W(MANT_W)
) u_fp_classify_a (
    .ind_sign  (a_sign),
    .ind_expo  (a_expo),
    .ind_mant  (a_mant),
    .is_nan    (a_is_nan    ),
    .is_snan   (a_is_snan   ),
    .is_zero   (a_is_zero   ),
    .is_inf    (a_is_inf    ),
    .is_inf_pos(a_is_inf_pos),
    .is_inf_neg(a_is_inf_neg),
    .is_nor    (a_is_nor    )
);


fp_classify #(
    .SIGN_W(SIGN_W),
    .EXPO_W(EXPO_W),
    .MANT_W(MANT_W)
) u_fp_classify_b (
    .ind_sign  (b_sign),
    .ind_expo  (b_expo),
    .ind_mant  (b_mant),
    .is_nan    (b_is_nan    ),
    .is_snan   (b_is_snan   ),
    .is_zero   (b_is_zero   ),
    .is_inf    (b_is_inf    ),
    .is_inf_pos(b_is_inf_pos),
    .is_inf_neg(b_is_inf_neg),
    .is_nor    (b_is_nor    )
);

logic test_anan_chk;
logic test_asnan_chk;
logic test_amaxexpo_chk;
logic test_azeromant_chk;
logic test_aq_hint;


assign test_amaxexpo_chk = &(ina[MANT_W +: EXPO_W]);
assign test_azeromant_chk = !(ina[0 +: MANT_W]=={MANT_W{1'b0}});
assign test_aq_hint = !((a_mant >> MANT_W) == {MANT_W{1'b0}});


nan_chk #(
    .SIGN_W(SIGN_W),
    .EXPO_W(EXPO_W),
    .MANT_W(MANT_W)
)u_test_anan_chk(
    .ind_sign   (a_sign),
    .ind_expo   (a_expo),
    .ind_mant   (a_mant),
    .res_nan    (test_anan_chk),
    .res_snan   (test_asnan_chk)
);

`ifndef SYN
always @ (ina or inb or rnd) begin
    $display("@%t TIME,in fp_min",$time);
    `echo(a_sign);
    `echo(a_expo);
    `echo(a_mant);
    `echo(a_is_nan);
    `echo(a_is_snan);
    `echo(test_anan_chk);
    `echo(test_asnan_chk);
    `echo(test_amaxexpo_chk);
    `echo(test_azeromant_chk);
    `echo(test_aq_hint);

end
`endif

// initial begin
//     $display("Time: %0t a_sign = %0f, a_expo = %0f, a_mant = %0f,a_is_nan=%0f,a_is_snan=%0f,a_is_zero=%0f,test_anan_chk=%0f,test_asnan_chk=%0f,test_amaxexpo_chk=%0f,test_azeromant_chk=%0f,test_aq_hint=%0f",$time, a_sign, a_expo,a_mant,a_is_nan,a_is_snan,a_is_zero,test_anan_chk,test_asnan_chk,test_amaxexpo_chk,test_azeromant_chk,test_aq_hint);
//     $display("Time: %0t b_sign = %0f, b_expo = %0f, b_mant = %0f,b_is_nan=%f,b_is_snan=%f,b_is_zero=%f", $time, b_sign, b_expo,b_mant,b_is_nan,b_is_snan,b_is_zero);
//     $display("Time: %0t a = %0f, b = %0f, res = %0f", $time, ina, inb,res);

// end

min_res_gen #(
    .SIGN_W(SIGN_W),
    .EXPO_W(EXPO_W),
    .MANT_W(MANT_W)
) u_min_res_gen(
    .ina         (ina),
    .a_sign      (a_sign      ),
    .a_expo      (a_expo      ),
    .a_mant      (a_mant      ),
    .a_is_nan    (a_is_nan    ),
    .a_is_snan   (a_is_snan   ),
    .a_is_zero   (a_is_zero   ),
    .a_is_inf    (a_is_inf    ),
    .a_is_inf_pos(a_is_inf_pos),
    .a_is_inf_neg(a_is_inf_neg),
    .a_is_nor    (a_is_nor    ),
    .inb         (inb),
    .b_sign      (b_sign      ),
    .b_expo      (b_expo      ),
    .b_mant      (b_mant      ),
    .b_is_nan    (b_is_nan    ),
    .b_is_snan   (b_is_snan   ),
    .b_is_zero   (b_is_zero   ),
    .b_is_inf    (b_is_inf    ),
    .b_is_inf_pos(b_is_inf_pos),
    .b_is_inf_neg(b_is_inf_neg),
    .b_is_nor    (b_is_nor    ),
    .res         (res         ),
    .status      (status      )
);

endmodule