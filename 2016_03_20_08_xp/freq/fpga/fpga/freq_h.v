module freq_h
(
	reset,
	outside_triger,
	counter,
	cs,
);
input reset;
input outside_triger;
input cs;
output reg[31:0] counter;
	always @(posedge outside_triger or negedge reset)
	begin
		if(!reset)
		begin
			counter <= 32'b0;
		end
		else if(cs)
		begin
			counter <= counter + 32'b1;
		end
	end
endmodule
