//macro_def
`define REG_1
`define REG_2
`define REG_3
// `define SYN
`ifndef SYN
    `define echo(h) $display("signal time is %t,h=%d",$time,h);
    // `define echo(h) $display("h=%d",h);
`endif
