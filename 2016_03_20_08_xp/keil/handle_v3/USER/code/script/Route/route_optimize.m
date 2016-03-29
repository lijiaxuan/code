
clear
close all
format long

step=5;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%  rout1    直线      起点（0，0）  角度 0  长度 432
%  起点
start_x=34.6621;
start_y=0;
%  长度
len=432;
rout1=0:step:len;
rout1_radian=pi/2-0;

rout1_theta=0*ones(1,length(rout1));
rout1_x=start_x+cos(rout1_radian)*rout1;
rout1_y=start_y+sin(rout1_radian)*rout1;

plot(rout1_x, rout1_y);

axis([-7000 7000 0 14000])
axis equal 
hold on

%rout2 圆弧 圆心（，）0->-15 半径 4614.3883 长度 1208

start=0;
stop=-15*pi/180;
cricl_x=-4614.3883;
cricl_y=432.0000;
radius=4649.0505;
len=1217;

rout2_theta=start:(stop-start)/(len/step):stop;
rout2_x=cricl_x+radius*cos(-(rout2_theta));
rout2_y=cricl_y+radius*sin(-(rout2_theta));
plot(rout2_x,rout2_y);

%rout3 圆弧 圆心（-4482.3235，467.3867）->-30 半径 4477.6648 长度 2344
start=stop;
stop=-(119.3330-90)*pi/180;
cricl_x=-4482.3235;
cricl_y=467.3867;
radius=4512.3269;
len=1128;

rout3_theta=start:(stop-start)/(len/step):stop;
rout3_x=cricl_x+radius*cos(-(rout3_theta));
rout3_y=cricl_y+radius*sin(-(rout3_theta));
plot(rout3_x,rout3_y);

% %
% %rout4 直线 ( 8271.3140 11814.3211 ) 长度 18
% %  起点
% start_x=-1316.1364;
% start_y=3633.5738;
% %  长度
% len=18;
% rout4_radian=pi/2-stop;
% 
% rout4=0:step:len;
% rout4_theta=stop*ones(1,length(rout4));
% rout4_x=start_x+cos(rout4_radian)*rout4;
% rout4_y=start_y+sin(rout4_radian)*rout4;
% plot(rout4_x, rout4_y);

%rout4 圆弧 圆心
start=stop;
stop=-(138.2543-90)*pi/180;
cricl_x=-3163.8961;
cricl_y=1208.2522;
radius=3000.0000;
len=990;

rout4_theta=start:(stop-start)/(len/step):stop;
rout4_x=cricl_x+radius*cos(-(rout4_theta));
rout4_y=cricl_y+radius*sin(-(rout4_theta));
plot(rout4_x,rout4_y);


%rout5 圆弧 
start=stop+0.001;
stop=-21.8488*pi/180;
cricl_x=1837.3228;
cricl_y=6812.4888;
radius=4511.3020;
len=2079;

rout5_theta=start:(stop-start)/(len/step):stop;
rout5_x=cricl_x+radius*cos(pi-(rout5_theta));
rout5_y=cricl_y+radius*sin(pi-(rout5_theta));
plot(rout5_x,rout5_y);



%rout6 直线  长度 
%  起点
start_x=-2349.9001;
start_y=5133.4980;
%  长度
len=824;
rout6_radian=pi/2-stop;

rout6=0:step:len;
rout6_theta=stop*ones(1,length(rout6));
rout6_x=start_x+cos(rout6_radian)*rout6;
rout6_y=start_y+sin(rout6_radian)*rout6;
plot(rout6_x, rout6_y);

%

%rout7 圆弧 
start=stop;
stop=(90+11.0480)*pi/180;
cricl_x=-2144.7248;
cricl_y=6126.4840;
radius=560.0000;
len=1221;

rout7_theta=start:(stop-start)/(len/step):stop;
rout7_x=cricl_x+radius*cos(pi-(rout7_theta));
rout7_y=cricl_y+radius*sin(pi-(rout7_theta));
plot(rout7_x,rout7_y);

%

% %rout8 直线  长度 
% %  起点
% start_x=-2176.4121;
% start_y=6611.7876;
% %  长度
% len=273;
% rout8_radian=pi/2-stop;
% 
% rout8=step:step:len;
% rout8_theta=stop*ones(1,length(rout8));
% rout8_x=start_x+cos(rout8_radian)*rout8;
% rout8_y=start_y+sin(rout8_radian)*rout8;
% plot(rout8_x, rout8_y);

%rout9 圆弧 
start=stop;
stop=(90-31.0374)*pi/180;
cricl_x=-1845.7800;
cricl_y=7657.5725;
radius=1000.0000;
len=734;

rout9_theta=start:(stop-start)/(len/step):stop;
rout9_x=cricl_x+radius*cos(-(rout9_theta));
rout9_y=cricl_y+radius*sin(-(rout9_theta));
plot(rout9_x,rout9_y);

%rout10 圆弧 
start=stop;
stop=(90)*pi/180;
cricl_x=-1041.4481;
cricl_y=6320.9161;
radius=560.0000;
len=303;

rout10_theta=start:(stop-start)/(len/step):stop;
rout10_x=cricl_x+radius*cos(pi-(rout10_theta));
rout10_y=cricl_y+radius*sin(pi-(rout10_theta));
plot(rout10_x,rout10_y);


%rout11 直线  长度 
%  起点
start_x=-1041.4481;
start_y=6880.9161;
%  长度
len=821;
rout11_radian=pi/2-stop;

rout11=step:step:len;
rout11_theta=stop*ones(1,length(rout11));
rout11_x=start_x+cos(rout11_radian)*rout11;
rout11_y=start_y+sin(rout11_radian)*rout11;
plot(rout11_x, rout11_y);

%rout12 圆弧 
start=stop;
stop=(45.1608)*pi/180;
cricl_x=-219.4557;
cricl_y=7880.9159;
radius=1000.0000;
len=786;

rout12_theta=start:(stop-start)/(len/step):stop;
rout12_x=cricl_x+radius*cos(-(rout12_theta));
rout12_y=cricl_y+radius*sin(-(rout12_theta));
plot(rout12_x,rout12_y);


%rout13 直线  长度 
%  起点
start_x=485.6644;
start_y=7171.8279;
%  长度
len=1497;
rout13_radian=pi/2-stop;

rout13=step:step:len;
rout13_theta=stop*ones(1,length(rout13));
rout13_x=start_x+cos(rout13_radian)*rout13;
rout13_y=start_y+sin(rout13_radian)*rout13;
plot(rout13_x, rout13_y);

%rout14 圆弧 
start=stop;
stop=(-44.0295)*pi/180;
cricl_x=985.2327;
cricl_y=8796.8107;
radius=800.0000;
len=1245;

rout14_theta=start:(stop-start)/(len/step):stop;
rout14_x=cricl_x+radius*cos(-(rout14_theta));
rout14_y=cricl_y+radius*sin(-(rout14_theta));
plot(rout14_x,rout14_y);

%rout15 直线  长度 
%  起点
start_x=1560.4179;
start_y=9352.8341;
%  长度
len=884;
rout15_radian=pi/2-stop;

rout15=step:step:len;
rout15_theta=stop*ones(1,length(rout15));
rout15_x=start_x+cos(rout15_radian)*rout15;
rout15_y=start_y+sin(rout15_radian)*rout15;
plot(rout15_x, rout15_y);


%rout16 圆弧 
start=stop;
stop=(50.5233)*pi/180;
cricl_x=1439.8395;
cricl_y=10340.8410;
radius=600.0000;
len=990;

rout16_theta=start:(stop-start)/(len/step):stop;
rout16_x=cricl_x+radius*cos(pi-(rout16_theta));
rout16_y=cricl_y+radius*sin(pi-(rout16_theta));
plot(rout16_x,rout16_y);

%rout17 直线  长度 
%  起点
start_x=1058.3807;
start_y=10803.9707;
%  长度
len=501;
rout17_radian=pi/2-stop;

rout17=step:step:len;
rout17_theta=stop*ones(1,length(rout17));
rout17_x=start_x+cos(rout17_radian)*rout17;
rout17_y=start_y+sin(rout17_radian)*rout17;
plot(rout17_x, rout17_y);

%rout18 圆弧 
start=stop;
stop=(0)*pi/180;
cricl_x=810.0299;
cricl_y=11894.9488;
radius=1000.0000;
len=881;

rout18_theta=start:(stop-start)/(len/step):stop;
rout18_x=cricl_x+radius*cos(-(rout18_theta));
rout18_y=cricl_y+radius*sin(-(rout18_theta));
plot(rout18_x,rout18_y);

%rout19 直线  长度 
%  起点
start_x=1810.0299;
start_y=11894.9488;
%  长度
len=1234;%正常234mm
rout19_radian=pi/2-stop;

rout19=step:step:len;
rout19_theta=stop*ones(1,length(rout19));
rout19_x=start_x+cos(rout19_radian)*rout19;
rout19_y=start_y+sin(rout19_radian)*rout19;
plot(rout19_x, rout19_y);

hold off

x=[ ...
    rout1_x rout2_x rout3_x rout4_x rout5_x...
    rout6_x rout7_x     rout9_x rout10_x ...
    rout11_x rout12_x rout13_x rout14_x rout15_x ...
    rout16_x rout17_x rout18_x rout19_x ...
    ];

y=[ ...
    rout1_y rout2_y rout3_y rout4_y rout5_y ...
    rout6_y rout7_y      rout9_y rout10_y ...
    rout11_y rout12_y rout13_y rout14_y rout15_y ...
    rout16_y rout17_y rout18_y  rout19_y ...
    ];

theta=[ ...
    rout1_theta rout2_theta rout3_theta rout4_theta rout5_theta... 
    rout6_theta rout7_theta      rout9_theta rout10_theta ...
    rout11_theta rout12_theta rout13_theta rout14_theta rout15_theta... 
    rout16_theta rout17_theta rout18_theta rout19_theta ...
    ];

figure
plot(x,y)
axis([-7000 7000 0 14000])
