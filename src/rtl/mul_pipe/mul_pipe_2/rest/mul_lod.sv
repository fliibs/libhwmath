module mul_lod#(
    parameter  int unsigned INPUT_WIDTH  = 24,
    localparam int unsigned AWIDTH       = $clog2(INPUT_WIDTH)-1
) (
    input  logic [INPUT_WIDTH - 1 : 0]    in_detect,
    output logic [AWIDTH : 0]             zero_nums
);
//------internal signals------------------
logic [INPUT_WIDTH-1:0] z[AWIDTH+1];
assign z[AWIDTH]=in_detect;

genvar i;
generate for (i=0;i<AWIDTH;i++) begin
    assign z[i]        = (|z[i+1][INPUT_WIDTH-1:INPUT_WIDTH-(1<<(i+1))]) ? z[i+1] : {z[i+1][INPUT_WIDTH-(1<<(i+1))-1:0],{(1<<(i+1)){1'b0}}}; 
    assign zero_nums[i]=!(|z[i]  [INPUT_WIDTH-1:INPUT_WIDTH-(1<<i)]);
end
endgenerate

endmodule