module mul_tb;
    parameter int unsigned SIGN_W = 1;
    parameter int unsigned EXPO_W = 8;
    parameter int unsigned MANT_W=23;
    parameter STDIN = 32'h8000_0000;
    
    logic [SIGN_W+EXPO_W+MANT_W-1 : 0] a        ;
    logic [SIGN_W+EXPO_W+MANT_W-1 : 0] b        ;
    logic [SIGN_W+EXPO_W+MANT_W-1 : 0] res_mul  ;
    logic [SIGN_W+EXPO_W+MANT_W-1 : 0] res_add  ;
    logic [SIGN_W+EXPO_W+MANT_W-1 : 0] res      ;
    logic [SIGN_W+EXPO_W+MANT_W-1 : 0] c        ;


    logic [1 : 0]                      rnd      ;
    logic                              clk      ;

    mul_para #(
      .SIGN_W(SIGN_W),
      .EXPO_W(EXPO_W),
      .MANT_W(MANT_W)
    )
    u_mul_para(
      .a(a),
      .b(b),
      .rnd(rnd),
      .res(res_mul)
    );

    add_top #(
      .SIGN_W(SIGN_W),
      .EXPO_W(EXPO_W),
      .MANT_W(MANT_W)
    )
    u_add(
      .clk(clk),
      .a(a)    ,
      .b(b)    ,
      .rnd(rnd),
      .res(res_add)
    );
always #5 clk=~clk;

initial begin
  clk=1'b0;
  if($test$plusargs("RTL_DEBUG")) begin
    $fscanf(STDIN, "%d %d %d %d", a,b,c,rnd);
    $display("a   is %d",a);
    $display("b   is %d",b);
    $display("c   is %d",c);
    $display("rnd is %d",rnd);
    @(posedge clk);
    @(posedge clk); 
    $display("res=%d",res);
    #10 $finish;
  end
  else begin
    $display("into loop");
    // $system("date");
    while(1) begin
      // for (i=0;i<10 ;i=i+1) begin
      $fscanf(STDIN, "%d %d %d %d", a,b,c,rnd);
      // $display("Time rtl get from output pipe:");
      // $system("date");
      // #1;
      @(posedge clk);
      @(posedge clk);
      $display("res=%d",res);
    end
  end
end 

always@* begin
  if($test$plusargs("MUL")) begin
    res=res_mul;    
  end
  if($test$plusargs("ADD")) begin
    res=res_add;
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

