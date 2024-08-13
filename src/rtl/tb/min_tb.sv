`include "macro_defs.sv"
module min_tbs;
parameter int unsigned EXPO_W = 8;
parameter int unsigned MANT_W = 23;
parameter STDIN = 32'h8000_0000;

logic                      clk       ;
logic [EXPO_W + MANT_W :0] a         ;
logic [EXPO_W + MANT_W :0] b         ;
logic [EXPO_W + MANT_W :0] c         ;

logic [1:0]                rnd       ;
logic [EXPO_W + MANT_W :0] z         ;
logic [4:0]                inh_status;
logic                      inh_NX    ;
logic                      inh_NV    ;
logic                      inh_OF    ;
logic                      inh_UF    ;
logic                      inh_DZ    ;

// assign inh_NV = inh_status[4];
// assign inh_DZ = inh_status[3];
// assign inh_OF = inh_status[2];
// assign inh_UF = inh_status[1];
// assign inh_NX = inh_status[0];

min_top#(
  .SIGN_W(1         ),
  .EXPO_W(EXPO_W    ),
  .MANT_W(MANT_W    )
)u_mul_top(
  .clk   (clk       ),
  .ina   (a         ),
  .inb   (b         ),
  .rnd   (rnd       ),
  .status(inh_status),
  .res   (z         ) 
);

always #5 clk= ~clk;

initial begin
  clk=1'b0;
  if($test$plusargs("RTL_DEBUG")) begin
    $fscanf(STDIN, "%d %d %d %d", a,b,c,rnd)       ;
    $display("signal time is %t,a   is %d",$time,a);
    $display("signal time is %t,b   is %d",$time,b);
    $display("signal time is %t,rnd is %d",$time,rnd);
    repeat(3) begin
      @(posedge clk);
    end
    `ifdef REG_1
      @(posedge clk); 
    `endif 
    `ifdef REG_2
      @(posedge clk);
    `endif 
    $display("res=%d",z);
    $display("status=%d",inh_status);
    @(posedge clk); 
    $finish;
  end
  else begin
    while(1) begin
      $fscanf(STDIN, "%d %d %d %d", a,b,c,rnd)       ;
      repeat(3) begin
        @(posedge clk);
      end
      `ifdef REG_1
        @(posedge clk); 
      `endif 
      `ifdef REG_2
        @(posedge clk);
      `endif 
        $display("res=%d",z);
        $display("status=%d",inh_status);
        @(posedge clk); 
    end
  end
end

initial begin
    $vcdpluson;
end
endmodule