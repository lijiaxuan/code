function [x,y,theta]=cicle_plan_anti(cricl_x,cricl_y,radius,start,stop,len,step)

theta=start:(stop-start)/(len/step):stop;
x=cricl_x+radius*cos(-(theta));
y=cricl_y+radius*sin(-(theta));
plot(x,y);
x(1)
y(1)