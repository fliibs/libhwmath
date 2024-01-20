module mul_tb;
    parameter int unsigned SIGN_W = 1;
    parameter int unsigned EXPO_W = 8;
    parameter int unsigned MANT_W=23;
    parameter STDIN = 32'h8000_0000;
    logic [SIGN_W+EXPO_W+MANT_W-1 : 0] a      ;
    logic [SIGN_W+EXPO_W+MANT_W-1 : 0] b      ;
    logic [SIGN_W+EXPO_W+MANT_W-1 : 0] c      ;
    logic [SIGN_W+EXPO_W+MANT_W-1 : 0] c_std  ;
    logic [1 : 0]                      rnd    ;
    string                             a_str  ;
    string                             b_str  ;
    string                             c_str  ;
    integer                            file_handle;
    integer                            file_handle_in;

    mul_para #(
      .SIGN_W(SIGN_W),
      .EXPO_W(EXPO_W),
      .MANT_W(MANT_W)
    )
    u_mul_para(
      .a(a),
      .b(b),
      .rnd(rnd),
      .res(c)
    );
`ifndef RTL_DEBUG
  initial begin
      while(1) begin
      $fscanf(STDIN, "%d %d %d", a,b,rnd);
      #0.000001;
      $display("res=%d",c);
      $display("a=%d",a);
      $display("b=%d",b);
      $display("rnd=%d",rnd);
      end
  end
`else
  initial begin
      $fscanf(STDIN, "%d %d %d", a,b,rnd);
      #0.000001;
      $display("res=%d",c);
      $display("a=%d",a);
      $display("b=%d",b);
      $display("rnd=%d",rnd);
      #10 $finish;
  end
  initial begin
      $fsdbDumpfile("*.fsdb");
      $fsdbDumpvars(0);
  end
`endif 

endmodule

