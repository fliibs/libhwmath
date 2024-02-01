module shifter_r_light#(
    parameter  int unsigned WIDTH  = 48,
    localparam int unsigned AWIDTH = $clog2(WIDTH) - 1
)(
    input  logic [WIDTH -1:0] data_in     ,
    input  logic [AWIDTH-1:0] shift_num   ,
    output logic [WIDTH -1:0] data_out
);

logic [WIDTH-1:0] data_in_4;
logic [WIDTH-1:0] data_in_3;
logic [WIDTH-1:0] data_in_2;
logic [WIDTH-1:0] data_in_1;

assign data_in_4 = shift_num[4] ? {data_in  [WIDTH-17:0],16'b0} : data_in   ;
assign data_in_3 = shift_num[3] ? {data_in_4[WIDTH-9:0] ,8'b0}  : data_in_4 ;
assign data_in_2 = shift_num[2] ? {data_in_3[WIDTH-5:0] ,4'b0}  : data_in_3 ;
assign data_in_1 = shift_num[1] ? {data_in_2[WIDTH-3:0] ,2'b0}  : data_in_2 ;
assign data_out  = shift_num[0] ? {data_in_1[WIDTH-2:0] ,1'b0}  : data_in_1 ;

endmodule  