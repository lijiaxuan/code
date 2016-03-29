module clkctl
(
	clkin,
	clkout,
	cs,
);
input clkin;
output clkout;
input cs;
assign clkout = cs & clkin ;
// always @(posedge clkin) begin
// 	if (!cs) 
// 	begin
		
// 	end
// 	else
// 	begin
// 		clkout <= clkin;
// 	end
// end
endmodule