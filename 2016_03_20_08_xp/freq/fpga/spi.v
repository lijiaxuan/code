module spi
(
	spiclk,
	cs,
	i_data,
	o_data
);
input wire [127:0] i_data;
input spiclk;
input cs;
output reg o_data;
reg [7:0] index;

always @(posedge cs or posedge spiclk) begin
	if (cs) 
	begin
		// reset
		index <= 7'b0;		
	end
	else if(index == 127)
	begin
		index <= 127;
	end
	else
	begin
		o_data <= i_data[index];
		index  <= index + 7'b1;
	end
end

endmodule