module mul_b;
parameter int unsigned EXPO_W = 8;
parameter int unsigned MANT_W = 23;
parameter STDIN = 32'h8000_0000;

logic                      clk       ;
logic [EXPO_W + MANT_W :0] a         ;
logic [EXPO_W + MANT_W :0] b         ;
logic [1:0]                rnd       ;
logic [EXPO_W + MANT_W :0] z         ;
logic [4:0]                inh_status;
logic                      inh_NX    ;
logic                      inh_NV    ;
logic                      inh_OF    ;
logic                      inh_UF    ;
logic                      inh_DZ    ;


assign inh_NV = inh_status[4];
assign inh_DZ = inh_status[3];
assign inh_OF = inh_status[2];
assign inh_UF = inh_status[1];
assign inh_NX = inh_status[0];

mul_top#(
  .SIGN_W(1         ),
  .EXPO_W(EXPO_W    ),
  .MANT_W(MANT_W    )
)u_mul_top(
  .clk   (clk       ),
  .a     (a         ),
  .b     (b         ),
  .rnd   (rnd       ),
  .status(inh_status),
  .res   (z         ) 
);

always #5 clk= ~clk;

initial begin
  clk=1'b0;
  a[EXPO_W + MANT_W ]=1;
  a[EXPO_W + MANT_W -1 : MANT_W]=8'd154;
  a[MANT_W -1 : 0]=23'd2536199;
  b[EXPO_W + MANT_W ]=1;
  b[EXPO_W + MANT_W -1 : MANT_W]=8'd28;
  b[MANT_W -1 : 0]=23'd8139365;
  rnd = 2'b0;

  $display("a   is %d",a);
  $display("b   is %d",b);
  $display("c   is %d",z);
  $display("rnd is %d",rnd);

  @(posedge clk);
  @(posedge clk); 
  @(posedge clk); 
  $display("res=%d",z);

  @(posedge clk); 
  $display("res=%d",z);

  @(posedge clk); 
  $display("res=%d",z);
  @(posedge clk); 
  $display("res=%d",z);
  #10 $finish;
end
endmodule