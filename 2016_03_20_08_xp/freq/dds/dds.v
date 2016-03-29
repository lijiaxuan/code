module dds
(
    clk,
    out,
    rst,
    cnt1,
    duty,
);
input rst;
input clk;
input [15:0] duty;
input [15:0] cnt1;
reg  [15:0] cnt2;
output reg out;
//reg [15:0] count1;
reg [15:0] count2;

// always @(posedge clk or posedge rst) begin
//     if (rst) begin
//         count1 <= 0;
        
//     end
//     else if (count1 < cnt1) 
//     begin
//         count1 <= count1 + 1;    
//     end
//     else begin
//         count1 <= 0;
//     end
// end
always @(posedge clk or posedge rst) begin
    if (rst) 
    begin
        count2 <= 0;
    end
    else if (count2 < cnt1) 
    begin
        count2 <= count2 + 1;
    end
    else if(count2 >= cnt1)
    begin
        count2 <= 0;    
    end
end

always @(posedge clk or posedge rst) begin
    if (rst) begin
        cnt2 <= cnt1 * duty / 1000;
    end
    else if(count2 < cnt2)
    begin
        out <= 1;
    end
    else begin
        out <= 0;
    end
end
endmodule