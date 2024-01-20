//////////////////////////////////////////////////////////////////////////////////
// Company: BYD
// Engineer: yumiaoyu 
//
// Create Date: 01/09/2024 15:54:34 PM
// Design Name: 
// Module Name: mask
// Project Name: mul_para
// Description:
//              find the leading one and trun it into one-hot format
// Dependencies:
//
// Revision:
//              Revision 0.01 - File Created
// Additional Comments:
//
//////////////////////////////////////////////////////////////////////////////////
module mul_mask #(
    parameter int unsigned INPUT_WIDTH = 24 
) (
    input  logic [INPUT_WIDTH -1 : 0 ] input_unmask,
    output logic [INPUT_WIDTH -1 : 0 ] output_mask 
);
//-------------internal signals------------

assign output_mask [INPUT_WIDTH-1]    = input_unmask[INPUT_WIDTH-1];
genvar i  ;
generate for (i = INPUT_WIDTH-2;i >= 0;i = i-1 ) begin
    assign output_mask[i] = (!(|input_unmask[ INPUT_WIDTH-1: i+1])) & input_unmask[i];   
end
    
endgenerate

endmodule