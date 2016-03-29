function [x,y,theta]=cicle_plan_wise(cricl_x,cricl_y,radius,start,stop,len,step)

theta=start:(stop-start)/(len/step):stop;
x=cricl_x+radius*cos(pi-(theta));
y=cricl_y+radius*sin(pi-(theta));
plot(x,y);
x(1)
y(1)