clear
close all
format long

figure
axis([-7000 7000 0 14000])
axis equal 
hold on

step=5;

x=0;
y=0;
theta=0;

[x_temp,y_temp,theta_temp]=line_plan(0,0,0,900,step);
x=[x x_temp];
y=[y y_temp];
theta=[theta theta_temp];

[x_temp,y_temp,theta_temp]=cicle_plan_anti(-1059.5754,900.0000,1059.5754,-0.001,-15*pi/180,277,step);
x=[x x_temp];
y=[y y_temp];
theta=[theta theta_temp];

[x_temp,y_temp,theta_temp]=line_plan(-36.1042,1174.2383,-15*pi/180,918,step);
x=[x x_temp];
y=[y y_temp];
theta=[theta theta_temp];

[x_temp,y_temp,theta_temp]=cicle_plan_anti(-1297.1713,1786.7199,1059.5754,-15*pi/180,-30*pi/180,277,step);
x=[x x_temp];
y=[y y_temp];
theta=[theta theta_temp];

[x_temp,y_temp,theta_temp]=line_plan(-379.5521,2316.5076,-30*pi/180,900,step);
x=[x x_temp];
y=[y y_temp];
theta=[theta theta_temp];

[x_temp,y_temp,theta_temp]=cicle_plan_anti(-1747.1713,2566.1428,1059.5475,-30*pi/180,-45*pi/180,277,step);
x=[x x_temp];
y=[y y_temp];
theta=[theta theta_temp];

[x_temp,y_temp,theta_temp]=line_plan(-997.9383,3315.3757,-45*pi/180,918,step);
x=[x x_temp];
y=[y y_temp];
theta=[theta theta_temp];


[x_temp,y_temp,theta_temp]=cicle_plan_wise(-897.8294,4713.7327,1059.5475,-45*pi/180,-30*pi/180,277,step);
x=[x x_temp];
y=[y y_temp];
theta=[theta theta_temp];

[x_temp,y_temp,theta_temp]=line_plan(-1815.4486,4183.9450,-30*pi/180,900,step);
x=[x x_temp];
y=[y y_temp];
theta=[theta theta_temp];

[x_temp,y_temp,theta_temp]=cicle_plan_wise(-1347.8294,5493.1556,1059.5475,-30*pi/180,-22.1321*pi/180,277,step);
x=[x x_temp];
y=[y y_temp];
theta=[theta theta_temp];

[x_temp,y_temp,theta_temp]=line_plan(-2329.3326,5093.9669,-22.1321*pi/180,886,step);
x=[x x_temp];
y=[y y_temp];
theta=[theta theta_temp];

[x_temp,y_temp,theta_temp]=cicle_plan_wise(-2144.7248,6126.4840,560.0000,-22.1321*pi/180,pi/2,1095,step);
x=[x x_temp];
y=[y y_temp];
theta=[theta theta_temp];


[x_temp,y_temp,theta_temp]=line_plan(-2144.7248,6686.4840,pi/2,1000,step);
x=[x x_temp];
y=[y y_temp];
theta=[theta theta_temp];
figure

plot(x,y)

x_o=-x;
y_o=y-595;
theta_o=-theta;

fid=fopen('E:\right.txt','w');

for i=1:length(theta_o)
    fprintf(fid,'{ {%f,%f},%f},\n ',x_o(i),y_o(i),theta_o(i));
end

fclose(fid);