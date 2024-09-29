module min_top#(
    parameter int unsigned SIGN_W=1,
    parameter int unsigned EXPO_W=8,
    parameter int unsigned MANT_W=23
) (
    input  logic                                        clk ,
    input  logic  [SIGN_W + EXPO_W + MANT_W -1 : 0]     ina ,
    input  logic  [SIGN_W + EXPO_W + MANT_W -1 : 0]     inb ,
    input  logic  [1 : 0]                               rnd ,
    output logic  [4 : 0]                               status,
    output logic  [SIGN_W + EXPO_W + MANT_W -1 : 0]     res       
);

logic [SIGN_W + EXPO_W + MANT_W -1 : 0] res_tmp;

fp_min #(
    .SIGN_W(SIGN_W),
    .EXPO_W(EXPO_W),
    .MANT_W(MANT_W)
)
u_fp_min(
    .ina(ina),
    .inb(inb),
    .rnd(rnd),
    .status(status),
    .res(res_tmp)
);
always_ff @(posedge clk) begin 
    res<=res_tmp;
end



`ifndef SYN
logic [SIGN_W - 1 : 0]                   top_a_sign;
logic [EXPO_W - 1 : 0]                   top_a_expo;
logic [MANT_W - 1 : 0]                   top_a_mant;
logic [SIGN_W - 1 : 0]                   top_b_sign;
logic [EXPO_W - 1 : 0]                   top_b_expo;
logic [MANT_W - 1 : 0]                   top_b_mant;

fp_unpack #(
    .SIGN_W(SIGN_W),
    .EXPO_W(EXPO_W),
    .MANT_W(MANT_W)
) u_adbg_unpack (
    .ind     (ina),
    .ind_sign(top_a_sign),
    .ind_expo(top_a_expo),
    .ind_mant(top_a_mant)
);

fp_unpack #(
    .SIGN_W(SIGN_W),
    .EXPO_W(EXPO_W),
    .MANT_W(MANT_W)
) u_bdbg_unpack (
    .ind     (ina),
    .ind_sign(top_b_sign),
    .ind_expo(top_b_expo),
    .ind_mant(top_b_mant)
);

always @ (ina or inb or rnd) begin
    $display("@%t TIME,in min top",$time);
    `echo(ina);
    `echo(top_a_sign)
    `echo(top_a_expo)
    `echo(top_a_mant)

    `echo(ina[SIGN_W + EXPO_W + MANT_W - 1])
    `echo(ina[EXPO_W + MANT_W - 1 : MANT_W])
    `echo(ina[MANT_W - 1 : 0])

    `echo(inb);
    `echo(top_b_sign)
    `echo(top_b_expo)
    `echo(top_b_mant)

    `echo(inb[SIGN_W + EXPO_W + MANT_W - 1])
    `echo(inb[EXPO_W + MANT_W - 1 : MANT_W])
    `echo(inb[MANT_W - 1 : 0])

    `echo(rnd);
    `echo(res);
    `echo(status);


end
`endif

endmodule