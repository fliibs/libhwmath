module test;
`ifdef DEBUG
  initial begin
    // 这是 DEBUG 为 1 时执行的代码块
    $display("rnd=1");
    // #10 $finish;
  end
`else
  initial begin
    // 这是 DEBUG 为 0 时执行的代码块
    while (1) begin
      #1;
      $display("b=1");
      // #10 $finish;
    end
  end
`endif
endmodule