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

initial begin
  if($test$plusargs("RTL_DEBUG")) begin
    // while(1) begin
    $fscanf(STDIN, "%d %d %d", a,b,rnd);
    #0.000001;
    $display("res=%d",c);
    #10 $finish;
  end
  else begin
    // $display("Time rtl started:");
    // $system("date");
    while(1) begin
    $fscanf(STDIN, "%d %d %d", a,b,rnd);
    // $display("Time rtl get from output pipe:");
    // $system("date");
    #0.000001;
    // $display("Time rtl push to input pipe:");
    $display("res=%d",c);

    end
  end
end

// initial begin
//   if($test$plusargs("RTL_DEBUG")) begin
//   $fsdbDumpfile("*.fsdb");
//   $fsdbDumpvars(0);
//   end
// end
initial begin
  if($test$plusargs("RTL_DEBUG")) begin
    $vcdpluson;
  end
end
endmodule

