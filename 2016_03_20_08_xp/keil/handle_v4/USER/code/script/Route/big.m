clear
close all
format long


figure
axis([-7000 7000 0 14000])
axis equal 
hold on
grid on

step=5;

x=0;
y=0;
theta=0;
l=0;
dot_x=0;%保存每一段的起点的数据
dot_y=0;%保存每一段的起点的数据
%启动区
[x_temp,y_temp,theta_temp]=line_plan(0,0,0,800,step);
x=[x x_temp];
y=[y y_temp];
theta=[theta theta_temp];
l=[l length(x)];
dot_x=[dot_x x_temp(1)];%将第一段的起点的数据采集出来
dot_y=[dot_y y_temp(1)];


[x_temp,y_temp,theta_temp]=cicle_plan_anti(-1819.1508,800.0000,1819.1508,0-0.001,-15*pi/180,476,step);
x=[x x_temp];
y=[y y_temp];
theta=[theta theta_temp];
l=[l length(x)];
dot_x=[dot_x x_temp(1)];
dot_y=[dot_y y_temp(1)];

%斜坡1
[x_temp,y_temp,theta_temp]=line_plan(-61.9861,1270.8309,-15*pi/180,718,step);
x=[x x_temp];
y=[y y_temp];
theta=[theta theta_temp];
l=[l length(x)];
dot_x=[dot_x x_temp(1)];
dot_y=[dot_y y_temp(1)];


[x_temp,y_temp,theta_temp]=cicle_plan_anti(-2004.9829,1493.5347,1819.1508,-15*pi/180,-30*pi/180,476,step);
x=[x x_temp];
y=[y y_temp];
theta=[theta theta_temp];
l=[l length(x)];
dot_x=[dot_x x_temp(1)];
dot_y=[dot_y y_temp(1)];

%山岗1
[x_temp,y_temp,theta_temp]=line_plan(-429.5521,2403.1102,-30*pi/180,700,step);
x=[x x_temp];
y=[y y_temp];
theta=[theta theta_temp];
l=[l length(x)];
dot_x=[dot_x x_temp(1)];
dot_y=[dot_y y_temp(1)];


[x_temp,y_temp,theta_temp]=cicle_plan_anti(-2354.9829,2099.7525,1819.1508,-30*pi/180-0.001,-45*pi/180,476,step);
x=[x x_temp];
y=[y y_temp];
theta=[theta theta_temp];
l=[l length(x)];
dot_x=[dot_x x_temp(1)];
dot_y=[dot_y y_temp(1)];



%斜坡2
[x_temp,y_temp,theta_temp]=line_plan(-1068.6490,3386.0864,-45*pi/180,718,step);
x=[x x_temp];
y=[y y_temp];
theta=[theta theta_temp];
l=[l length(x)];
dot_x=[dot_x x_temp(1)];
dot_y=[dot_y y_temp(1)];



[x_temp,y_temp,theta_temp]=cicle_plan_wise(-290.0178,5180.1230,1819.1508,-45*pi/180,-30*pi/180,476,step);
x=[x x_temp];
y=[y y_temp];
theta=[theta theta_temp];
l=[l length(x)];
dot_x=[dot_x x_temp(1)];
dot_y=[dot_y y_temp(1)];


%山岗2
% [x_temp,y_temp,theta_temp]=line_plan(-1865.4486,4270.5475,-30*pi/180,700,step);
% x=[x x_temp];
% y=[y y_temp];
% theta=[theta theta_temp];
% l=[l length(x)];
% [x_temp,y_temp,theta_temp]=line_plan(-1865.4486,4270.5475,-30*pi/180,934,step);
% x=[x x_temp];
% y=[y y_temp];
% theta=[theta theta_temp];
% l=[l length(x)];

%从0开始第8段
[x_temp,y_temp,theta_temp]=line_plan(-1865.4486,4270.5475,-30*pi/180,700,step);
x=[x x_temp];
y=[y y_temp];
theta=[theta theta_temp];
l=[l length(x)];
dot_x=[dot_x x_temp(1)];
dot_y=[dot_y y_temp(1)];


% [x_temp,y_temp,theta_temp]=cicle_plan_wise(-640.0178,5786.3407,1819.1508,-30*pi/180,-16.4039*pi/180,431,step);
% x=[x x_temp];
% y=[y y_temp];
% theta=[theta theta_temp];
% l=[l length(x)];
% [x_temp,y_temp,theta_temp]=cicle_plan_wise(-773.7746,5979.7127,1800.0000,-30*pi/180,-15.0000*pi/180,471,step);
% x=[x x_temp];
% y=[y y_temp];
% theta=[theta theta_temp];
% l=[l length(x)];
%第9段
[x_temp,y_temp,theta_temp]=cicle_plan_wise(-640.0178,5786.3407,1819.1508,-30*pi/180,-20.2832*pi/180,308,step);
x=[x x_temp];
y=[y y_temp];
theta=[theta theta_temp];
l=[l length(x)];
dot_x=[dot_x x_temp(1)];
dot_y=[dot_y y_temp(1)];


%斜坡3
% [x_temp,y_temp,theta_temp]=line_plan(-2385.1198,5272.6007,-16.4039*pi/180,849,step);
% x=[x x_temp];
% y=[y y_temp];
% theta=[theta theta_temp];
% l=[l length(x)];
% [x_temp,y_temp,theta_temp]=line_plan(-2512.4411,5513.8384,-15.0000*pi/180,603,step);
% x=[x x_temp];
% y=[y y_temp];
% theta=[theta theta_temp];
% l=[l length(x)];
[x_temp,y_temp,theta_temp]=line_plan(-2346.3640,5155.7123,-20.2832*pi/180,861,step);
x=[x x_temp];
y=[y y_temp];
theta=[theta theta_temp];
l=[l length(x)];
dot_x=[dot_x x_temp(1)];
dot_y=[dot_y y_temp(1)];


%斜坡3->山岗3
% [x_temp,y_temp,theta_temp]=cicle_plan_wise(-2241.2391,6200.2564,400.0000,-16.4039*pi/180,(90)/180*pi,742,step);
% x=[x x_temp];
% y=[y y_temp];
% theta=[theta theta_temp];
% l=[l length(x)];
[x_temp,y_temp,theta_temp]=cicle_plan_wise(-2176.0588,6137.2345,500.0000,-20.2832*pi/180,102.4944/180*pi,1065,step);
x=[x x_temp];
y=[y y_temp];
theta=[theta theta_temp];
l=[l length(x)];
dot_x=[dot_x x_temp(1)];
dot_y=[dot_y y_temp(1)];

% 山岗3

%第一段直线
% [x_temp,y_temp,theta_temp]=line_plan(-2241.2391,6600.2564,90*pi/180,512,step);
% x=[x x_temp];
% y=[y y_temp];
% theta=[theta theta_temp];
% l=[l length(x)];
% 
% [x_temp,y_temp,theta_temp]=line_plan(-2282.2558,6600.2564,90*pi/180,552,step);
% x=[x x_temp];
% y=[y y_temp];
% theta=[theta theta_temp];
% l=[l length(x)];

% [x_temp,y_temp,theta_temp]=line_plan(-2323.6668,6600.2564,90*pi/180,594,step);
% x=[x x_temp];
% y=[y y_temp];
% theta=[theta theta_temp];
% l=[l length(x)];
% dot_x=[dot_x x_temp(1)];
% dot_y=[dot_y y_temp(1)];


%接着是一段圆弧
%       圆心X       圆心Y      半径        起始解角度  终止             弧长  步长                                         
[x_temp,y_temp,theta_temp]=cicle_plan_anti(-1870.4908,7605.7808,1000.0,102.4944/180*pi,(90-31.0374)/180*pi,807,step);
x=[x x_temp];
y=[y y_temp];
theta=[theta theta_temp];
l=[l length(x)];
dot_x=[dot_x x_temp(1)];
dot_y=[dot_y y_temp(1)];

%与上一段圆弧相切的一段圆弧
%        圆心X       圆心Y      半径        起始解角度  终止             弧长  步长                                         
[x_temp,y_temp,theta_temp]=cicle_plan_wise(-984.3007,6316.1727,564.7434,(90-31.0374)/180*pi,(90)/180*pi,340,step);
x=[x x_temp];
y=[y y_temp];
theta=[theta theta_temp];
l=[l length(x)];
dot_x=[dot_x x_temp(1)];
dot_y=[dot_y y_temp(1)];


%河流
% [x_temp,y_temp,theta_temp]=line_plan(-984.3007,6880.9161,90*pi/180,785,step);
% x=[x x_temp];
% y=[y y_temp];
% theta=[theta theta_temp];
% l=[l length(x)];
[x_temp,y_temp,theta_temp]=line_plan(-984.3007,6880.9161,90*pi/180,856,step);
x=[x x_temp];
y=[y y_temp];
theta=[theta theta_temp];
l=[l length(x)];
dot_x=[dot_x x_temp(1)];
dot_y=[dot_y y_temp(1)];


%河流->高地
% [x_temp,y_temp,theta_temp]=cicle_plan_anti(-198.7064,7830.9161,950.0000,(90)/180*pi,(44.9249)/180*pi,747,step);
% x=[x x_temp];
% y=[y y_temp];
% theta=[theta theta_temp];
% l=[l length(x)];
[x_temp,y_temp,theta_temp]=cicle_plan_anti(-128.0883,7830.9161,950.0000,(90)/180*pi,(44.9249)/180*pi,747,step);
x=[x x_temp];
y=[y y_temp];
theta=[theta theta_temp];
l=[l length(x)];
dot_x=[dot_x x_temp(1)];
dot_y=[dot_y y_temp(1)];

%高地
% [x_temp,y_temp,theta_temp]=line_plan(473.9253,7160.0461,44.9249/180*pi,1612,step);
% x=[x x_temp];
% y=[y y_temp];
% theta=[theta theta_temp];
% l=[l length(x)];
[x_temp,y_temp,theta_temp]=line_plan(544.5434,7160.0461,44.9249/180*pi,1540,step);
x=[x x_temp];
y=[y y_temp];
theta=[theta theta_temp];
l=[l length(x)];
dot_x=[dot_x x_temp(1)];
dot_y=[dot_y y_temp(1)];

%山道弯1
% [x_temp,y_temp,theta_temp]=cicle_plan_anti(1130.9994,8781.7743,680.0000,(44.9249)/180*pi,-43.7549/180*pi,1052,step);
% x=[x x_temp];
% y=[y y_temp];
% theta=[theta theta_temp];
% l=[l length(x)];
[x_temp,y_temp,theta_temp]=cicle_plan_anti(1172.2127,8709.8101,650.0000,(44.9249)/180*pi,-43.7549/180*pi,1006,step);
x=[x x_temp];
y=[y y_temp];
theta=[theta theta_temp];
l=[l length(x)];
dot_x=[dot_x x_temp(1)];
dot_y=[dot_y y_temp(1)];

% [x_temp,y_temp,theta_temp]=line_plan(1622.1666,9252.0452,-43.7549/180*pi,919,step);
% x=[x x_temp];
% y=[y y_temp];
% theta=[theta theta_temp];
% l=[l length(x)];
[x_temp,y_temp,theta_temp]=line_plan(1641.7108,9159.3338,-43.7549/180*pi,899,step);
x=[x x_temp];
y=[y y_temp];
theta=[theta theta_temp];
l=[l length(x)];
dot_x=[dot_x x_temp(1)];
dot_y=[dot_y y_temp(1)];

%山道弯2
% [x_temp,y_temp,theta_temp]=cicle_plan_wise(1419.9610,10330.8208,600.0000,(-43.7549)/180*pi,51.8718/180*pi,1001,step);
% x=[x x_temp];
% y=[y y_temp];
% theta=[theta theta_temp];
% l=[l length(x)];
[x_temp,y_temp,theta_temp]=cicle_plan_wise(1489.4533,10258.2406,650.0000,(-43.7549)/180*pi,51.8718/180*pi,1084,step);
x=[x x_temp];
y=[y y_temp];
theta=[theta theta_temp];
l=[l length(x)];
dot_x=[dot_x x_temp(1)];
dot_y=[dot_y y_temp(1)];

% [x_temp,y_temp,theta_temp]=line_plan(1049.5072,10802.7997,51.8718/180*pi,553,step);
% x=[x x_temp];
% y=[y y_temp];
% theta=[theta theta_temp];
% l=[l length(x)];
[x_temp,y_temp,theta_temp]=line_plan(1088.1284,10769.5511,51.8718/180*pi,528,step);
x=[x x_temp];
y=[y y_temp];
theta=[theta theta_temp];
l=[l length(x)];
dot_x=[dot_x x_temp(1)];
dot_y=[dot_y y_temp(1)];

%山道弯3
% [x_temp,y_temp,theta_temp]=cicle_plan_anti(960.0299,11813.1263,850.0000,(51.8718)/180*pi,0.0000/180*pi,769,step);
% x=[x x_temp];
% y=[y y_temp];
% theta=[theta theta_temp];
% l=[l length(x)];

[x_temp,y_temp,theta_temp]=cicle_plan_anti(1010.0299,11725.2467,800.0000,(51.8718)/180*pi,0.0000/180*pi,724,step);
x=[x x_temp];
y=[y y_temp];
theta=[theta theta_temp];
l=[l length(x)];
dot_x=[dot_x x_temp(1)];
dot_y=[dot_y y_temp(1)];

% 
% [x_temp,y_temp,theta_temp]=line_plan(1810.0299,11813.1263,0.0/180*pi,2305,step);
% x=[x x_temp];
% y=[y y_temp];
% theta=[theta theta_temp];
% l=[l length(x)];

[x_temp,y_temp,theta_temp]=line_plan(1810.0299,11725.2467,0.0/180*pi,2392,step);
x=[x x_temp];
y=[y y_temp];
theta=[theta theta_temp];
l=[l length(x)];
dot_x=[dot_x x_temp(1)];
dot_y=[dot_y y_temp(1)];


x_r=-x;
y_r=y;%495
theta_r=-theta;
plot(x_r,y_r,'*');


% fid=fopen('C:\Users\Administrator\Desktop\赛道\Route导出\Route\route_start_point.txt','wt');
% for i=2:25
%     fprintf(fid,'{%f,%f},\n',dot_x(i),dot_y(i));
% end
% fclose(fid);
fid=fopen('E:\right.txt','wt');
 for i=1:length(theta_r)
     fprintf(fid,'{ {%f,%f},%f},\n ',x_r(i),y_r(i),theta_r(i));
 end
 fclose(fid);

% fid=fopen('E:\left.txt','wt');
% for i=1:length(theta)
%     fprintf(fid,'{ {%f,%f},%f},\n ',x(i),y(i),theta(i));
% end
% fclose(fid);
% 
% %l=l-l(2);
% fid=fopen('E:\point.txt','wt');
% for i=1:length(l)
%     fprintf(fid,'%d, ',l(i));
% end
% fclose(fid);




