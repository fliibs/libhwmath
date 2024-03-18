module mul_dw02 #(
    parameter int unsigned width = 24
)(
    input [width -1 : 0]    a   ,
    input [width -1 : 0]    b   ,
    output [2*width - 1: 0] c 
);
DW02_mult #(
    .A_width(width),
    .B_width(width)
)u_dw02_mult(
    .A(a)       ,
    .B(b)       ,
    .TC(1'b0)   ,
    .PRODUCT(c)
);
endmodule