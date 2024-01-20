/////////////////////////////////////////////////////////////////////////////////
// Company: BYD
// Engineer: yumiaoyu
//
// Create Date: 01/09/2024 15:54:34 PM
// Design Name: 
// Module Name: LOD
// Project Name: mul_para
// Description:
//              leading one detector
// Dependencies:
//
// Revision:
//              Revision 0.01 - File Created
// Additional Comments:
//
//////////////////////////////////////////////////////////////////////////////////

module  mul_lod #(
    parameter  int unsigned INPUT_WIDTH  = 24                   ,
    localparam int unsigned AWIDTH       = $clog2(INPUT_WIDTH)-1
) (
    input  logic [INPUT_WIDTH - 1 : 0]    in_detect,
    output logic [AWIDTH : 0]             zero_nums
);

//---------internal signals----------------------//
logic [INPUT_WIDTH - 1 : 0]    z4                ;
logic [INPUT_WIDTH - 1 : 0]    z3                ;
logic [INPUT_WIDTH - 1 : 0]    z2                ;
logic [INPUT_WIDTH - 1 : 0]    z1                ;
logic [INPUT_WIDTH - 1 : 0]    z0                ;

assign z4 = (|in_detect[INPUT_WIDTH - 1 : INPUT_WIDTH - 16]) ? in_detect : {in_detect[INPUT_WIDTH-17:0],16'b0};
assign z3 = (|z4       [INPUT_WIDTH - 1 : INPUT_WIDTH - 8])  ? z4        : {z4       [INPUT_WIDTH-9:0] ,8'b0} ;
assign z2 = (|z3       [INPUT_WIDTH - 1 : INPUT_WIDTH - 4])  ? z3        : {z3       [INPUT_WIDTH-5:0] ,4'b0} ;
assign z1 = (|z2       [INPUT_WIDTH - 1 : INPUT_WIDTH - 2])  ? z2        : {z2       [INPUT_WIDTH-3:0] ,2'b0} ;
// assign z0 = (|z1       [INPUT_WIDTH - 1 : INPUT_WIDTH - 1])  ? z1        : {z0       [INPUT_WIDTH-2:0] ,1'b0} ;

assign zero_nums[4] = !(|in_detect[INPUT_WIDTH - 1 : INPUT_WIDTH - 16]);
assign zero_nums[3] = !(|z4       [INPUT_WIDTH - 1 : INPUT_WIDTH - 8]) ;
assign zero_nums[2] = !(|z3       [INPUT_WIDTH - 1 : INPUT_WIDTH - 4]) ;
assign zero_nums[1] = !(|z2       [INPUT_WIDTH - 1 : INPUT_WIDTH - 2]) ;
assign zero_nums[0] = !(|z1       [INPUT_WIDTH - 1])                   ;

endmodule

