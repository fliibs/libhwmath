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