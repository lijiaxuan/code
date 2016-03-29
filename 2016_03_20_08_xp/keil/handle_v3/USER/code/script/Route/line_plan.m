function [x,y,theta]= line_plan(start_x,start_y,radian,len,step)

rout=0:step:len;

theta=(radian)*ones(1,length(rout));
x=start_x+cos(pi/2-radian)*rout;
y=start_y+sin(pi/2-radian)*rout;

plot(x,y)