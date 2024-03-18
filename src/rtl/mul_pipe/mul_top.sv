module mul_top#(
    parameter int unsigned SIGN_W = 1,
    parameter int unsigned EXPO_W = 8,
    parameter int unsigned MANT_W = 23
) (
    input  logic                                       clk   ,
    input  logic [SIGN_W + EXPO_W + MANT_W -1 : 0]     a     ,
    input  logic [SIGN_W + EXPO_W + MANT_W -1 : 0]     b     ,
    input  logic [1 : 0]                               rnd   ,
    output logic [4 : 0 ]                              status,
    output logic [SIGN_W + EXPO_W + MANT_W -1 : 0]     res    
);

logic [SIGN_W + EXPO_W + MANT_W -1 : 0] a_reg_in;
logic [SIGN_W + EXPO_W + MANT_W -1 : 0] b_reg_in;
logic [1 : 0]                           rnd_reg_in;
logic [4 : 0 ]                          status_reg_out;
logic [SIGN_W + EXPO_W + MANT_W -1 : 0] res_reg_out;

mul_pipe_top #(
    .SIGN_W(SIGN_W),
    .EXPO_W(EXPO_W),
    .MANT_W(MANT_W)
)
u_mul_pipe_top(
    .clk(clk),
    .a(a_reg_in),
    .b(b_reg_in),
    .rnd(rnd_reg_in),
    .status(status_reg_out),
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

always_ff @( posedge clk ) begin 
    status      <= status_reg_out;
end
endmodule