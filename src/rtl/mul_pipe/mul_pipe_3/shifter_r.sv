module shifter_r#(
    parameter  int unsigned WIDTH  = 48,
    localparam int unsigned AWIDTH = $clog2(WIDTH)
)(
    input  logic [WIDTH -1:0] data_in     ,
    input  logic [AWIDTH-1:0] shift_num   ,
    output logic [WIDTH -1:0] data_out    ,
    output logic              r_rcd
);

// logic [WIDTH-1:0] data_in_5;
// logic [WIDTH-1:0] data_in_4;
// logic [WIDTH-1:0] data_in_3;
// logic [WIDTH-1:0] data_in_2;
// logic [WIDTH-1:0] data_in_1;

// assign data_in_5 = shift_num[5] ? {32'b0,data_in  [WIDTH-1 : 32]} : data_in   ;
// assign data_in_4 = shift_num[4] ? {16'b0,data_in_5[WIDTH-1 : 16]} : data_in_5 ;
// assign data_in_3 = shift_num[3] ? {8'b0, data_in_4[WIDTH-1 : 8]}  : data_in_4 ;
// assign data_in_2 = shift_num[2] ? {4'b0, data_in_3[WIDTH-1 : 4]}  : data_in_3 ;
// assign data_in_1 = shift_num[1] ? {2'b0, data_in_2[WIDTH-1 : 2]}  : data_in_2 ;
// assign data_out  = shift_num[0] ? {1'b0, data_in_1[WIDTH-1 : 1]}  : data_in_1 ;

logic [WIDTH-1:0]   data_in_ary[AWIDTH-1];
logic [AWIDTH-1:0]  r_rcd_ary            ;

assign data_in_ary[AWIDTH-2]= shift_num[AWIDTH-1] ? {{(1<<(AWIDTH-1)){1'b0}},data_in [WIDTH - 1 : 1<< (AWIDTH-1)]} : data_in;
assign r_rcd_ary  [AWIDTH-1]= (|data_in[(1<<AWIDTH-1)-1:0]) && shift_num[AWIDTH-1];

genvar i;
generate for (i= 0; i<AWIDTH-2 ;i++ ) begin
    assign data_in_ary[i]   = shift_num[i+1] ? {{(1<<(i+1)){1'b0}},data_in_ary[i+1][WIDTH - 1   : 1<<(i+1)]}: data_in_ary[i+1];
end
endgenerate

generate for (i= 0; i<AWIDTH-1 ;i++) begin
   assign r_rcd_ary[i]      = (|data_in_ary[i][(1<<i+1)-1:0]) && shift_num[i];
end
endgenerate


assign data_out             = shift_num[0]   ? {1'b0,data_in_ary[0][WIDTH-1:1]} : data_in_ary[0];
assign r_rcd                = |r_rcd_ary;
endmodule  