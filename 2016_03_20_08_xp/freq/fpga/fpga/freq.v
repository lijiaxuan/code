module	freq
(
	reset,
	sig1,
	sig2,
	clk_50,
	clk_s,
	cs,
	mod,
	sig,
	clk,
	//data,
	counter_h_T,
	counter_per_T,
	counter_per_duty,
	spiclk,
	o_data,
);
input 				mod;
input 				clk_50;
input 				reset;
input 				sig1;
input 				sig2;
input 				cs;
input 				spiclk;
output wire 		o_data;
input clk_s;
//output wire[95:0]			data;
output wire[31:0] 	counter_per_T;
output wire[31:0] 	counter_per_duty;
output wire[31:0] 	counter_h_T;
// output wire[31:0] 	counter_h_duty;
wire[31:0] 	counter;
output   wire 		sig;
output   wire 		clk;
wire[127:0]	i_data;

 assign counter_per_T = i_data[63:32];
 assign counter_h_T = i_data[31:0];
 assign counter_duty = i_data[95:64];

delta 	delt(.mod(mod),.sig1(sig1),.sig2(sig2),.clk(clk),.cs(cs),.sig(sig));
clkctl 	clkl(.cs(cs),.clkin(clk_50),.clkout(clk));
freq_h 	frq_h(.reset(reset),.outside_triger(sig),.counter(i_data[31:0]),.cs(cs));	//1
freq_l 	frq_l(.clk(clk),.reset(reset),.sig(sig),.counter_per_T(i_data[63:32]));//2
//duty_h 	duty_H(.clk(clk),.sig(sig),.counter(i_data[95:64]),.reset(reset));//3
duty_l 	duty_L(.clk(clk),.sig(sig),.counter_duty(i_data[127:96]),.counter_per_duty(i_data[95:64]),.reset(reset));//4
spi		spictl(.spiclk(spiclk),.i_data(i_data),.o_data(o_data),.cs(cs));
endmodule
