module mul_top#(
    parameter int unsigned SIGN_W = 1,
    parameter int unsigned EXPO_W = 8,
    parameter int unsigned MANT_W = 23
) (
    input  logic                                       clk ,
    input  logic [SIGN_W + EXPO_W + MANT_W -1 : 0]     a   ,
    input  logic [SIGN_W + EXPO_W + MANT_W -1 : 0]     b   ,
    input  logic [1 : 0]                               rnd ,
    output logic [SIGN_W + EXPO_W + MANT_W -1 : 0]     res  
);

logic [SIGN_W + EXPO_W + MANT_W -1 : 0] a_reg_in;
logic [SIGN_W + EXPO_W + MANT_W -1 : 0] b_reg_in;
logic [1 : 0]                           rnd_reg_in;
logic [SIGN_W + EXPO_W + MANT_W -1 : 0] res_reg_out;

mul_para #(
    .SIGN_W(SIGN_W),
    .EXPO_W(EXPO_W),
    .MANT_W(MANT_W)
)
u_mul_para(
    .a(a_reg_in),
    .b(b_reg_in),
    .rnd(rnd_reg_in),
    .res(res_reg_out)
);
always_ff @( posedge clk ) begin 
    a_reg_in    <= a;
    b_reg_in    <= b;
    rnd_reg_in  <= rnd;
end
always_ff @( posedge clk ) begin 
    res         <= res_reg_out;
end
endmodule