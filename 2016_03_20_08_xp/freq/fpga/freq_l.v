module freq_l
(
	reset,
	clk,
	sig,
	counter_per_T
);
input 	reset;
input 	clk;
input 	sig;
output reg[31:0] 	counter_per_T;
reg[31:0]  counter;

reg sig_d1;
reg sig_d2;

always @(posedge clk or negedge reset)
begin
	if(!reset)
	begin
		sig_d1 <= 1'b0;
		sig_d2 <= 1'b0;
	end
	else
	begin
		sig_d1 <= sig;
		sig_d2 <= sig_d1;
	end
end

reg clr;
reg clr_d1;

always @(posedge clk or negedge reset) begin
	if (!reset) 
	begin
		clr <= 1'b0;	
	end
	else if(clr == 1'b1)
	begin
		clr <= 1'b0;
	end
	else if((sig_d1 == 1'b1) && (sig_d2 == 1'b0))
	begin
		clr <= 1'b1;
	end
	else
	begin
		clr_d1 <= clr;	
	end
end

always @(posedge clk or negedge reset)
begin
	if(!reset)
	begin
		counter <= 32'b0;
	end
	else if((clr_d1 == 1'b0) && (clr == 1'b1))
	begin
		counter <= 32'b0;
	end
	else
	begin
		counter <= counter + 1'b1;
	end
end

always @(posedge clk or negedge reset)
begin
	if(!reset)
	begin
		counter_per_T <= 32'b0;
	end
	else if((sig_d1 == 1'b1) && (sig_d2 == 1'b0))
	begin
		counter_per_T <= counter;
	end
end

endmodule
