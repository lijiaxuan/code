%mm ���� 

%б�� 1118mm �ߵ�1100mm б��120�� �ߵ�110��
%Բ�� 15�� �뾶300mm ����78.5398mm Բ������Ϊ8��

close all
 format long

radian_step=15/180*pi;
radius=300;
slope=10:10:1118; %111��
hill=10:10:1100; %110��
arc=0:radian_step/8:radian_step;%9����




%rout1 ֱ�� ��㣨0��0�� �յ㣨0��1000�� 0��
%100����
rout1_radian=0;%ֱ�߽Ƕȶ���

rout1_x=zeros(1,100);
rout1_y=0:10:990;
rout1_theta=zeros(1,100);

plot(rout1_x, rout1_y);

axis([-7000 0 0 7000])
hold on

%rout2 Բ��1 Բ�ģ�-300��1000��0->15��
%9����
rout2_start=0;

rout2_theta=-arc;
rout2_x=-300+radius*cos(rout2_start+arc);
rout2_y=1000+radius*sin(rout2_start+arc);
plot(rout2_x, rout2_y);

%rout3 б��1 ���(-10.826, 1077.484) �Ƕ� -radian_step
rout3_radian=radian_step+pi/2; %�ѿ�������

rout3_theta=-radian_step * ones(1,111);%�Զ�������
rout3_x=-10.825+cos(rout3_radian)*slope;
rout3_y=1077.484+sin(rout3_radian)*slope;
plot(rout3_x, rout3_y);

%rout4 Բ��2 Բ��(-589.5034,2079.7433) 
%
rout4_start=radian_step;

rout4_theta=-radian_step+(-arc);
rout4_x=-589.5034+radius*cos(rout4_start+arc);
rout4_y=2079.7433+radius*sin(rout4_start+arc);
plot(rout4_x, rout4_y);

%rout5 ɽ��1 ���(-329.5521,2229.9051) �Ƕ� -2*radian_step
rout5_radian=2*radian_step+pi/2;

rout5_theta=-2*radian_step * ones(1,110);
rout5_x=-329.5521+cos(rout5_radian)*hill;
rout5_y=2229.9051+sin(rout5_radian)*hill;
plot(rout5_x, rout5_y);

%rout6 Բ��3 Բ��(-1139.8016,3032.0911) 
rout6_start=2*radian_step;

rout6_theta=-2*radian_step+(-arc);
rout6_x=-1139.8016+radius*cos(rout6_start+arc);
rout6_y=3032.0911+radius*sin(rout6_start+arc);
plot(rout6_x, rout6_y);


%rout7 б��2 ���(-927.2277,3244.6651) �Ƕ� -3*radian_step
rout7_radian=3*radian_step+pi/2;

rout7_theta=-3*radian_step * ones(1,111);
rout7_x=-927.2277+cos(rout7_radian)*slope;
rout7_y=3244.6651+sin(rout7_radian)*slope;
plot(rout7_x, rout7_y);

%rout8 Բ��4 Բ��(-1505.6410,4247.3425)

rout8_start=3*radian_step+pi;

rout8_theta=-3*radian_step+arc;
rout8_x=-1505.6410+radius*cos(rout8_start-arc);
rout8_y=4247.3425+radius*sin(rout8_start-arc);
plot(rout8_x, rout8_y);

%rout9 ɽ��2 ���(-1765.4486,4097.3425) �Ƕ� -2*radian_step
rout9_radian=2*radian_step+pi/2;

rout9_theta=-2*radian_step * ones(1,110);
rout9_x=-1765.4486+cos(rout9_radian)*hill;
rout9_y=4097.3425+sin(rout9_radian)*hill;
plot(rout9_x, rout9_y);

%rout10 Բ��5 (-2055.6410,5199.9704)
rout10_start=2*radian_step+pi;

rout10_theta=-2*radian_step+arc;
rout10_x=-2055.6410+radius*cos(rout10_start-arc);
rout10_y=5199.9704+radius*sin(rout10_start-arc);
plot(rout10_x, rout10_y);

%rout11 б��3(-2345.0063,5122.1746)�Ƕ� -radian_step
rout11_radian=radian_step+pi/2;

rout11_theta=-radian_step * ones(1,111);
rout11_x=-2345.0063+cos(rout11_radian)*slope;
rout11_y=5122.1746+sin(rout11_radian)*slope;
plot(rout11_x, rout11_y);

%rout112 Բ��6(-2345.0007,6279.8755)
rout12_start=radian_step+pi;

rout12_theta=-radian_step+arc;
rout12_x=-2345.0007+radius*cos(rout12_start-arc);
rout12_y=6279.8755+radius*sin(rout12_start-arc);
plot(rout12_x, rout12_y);
hold off

x=[rout1_x rout2_x rout3_x rout4_x rout5_x rout6_x rout7_x rout8_x rout9_x rout10_x rout11_x rout12_x];
y=[rout1_y rout2_y rout3_y rout4_y rout5_y rout6_y rout7_y rout8_y rout9_y rout10_y rout11_y rout12_y];
theta=[rout1_theta rout2_theta rout3_theta rout4_theta rout5_theta rout6_theta rout7_theta rout8_theta rout9_theta rout10_theta rout11_theta rout12_theta ];

x=-x;
theta=-theta;
y=y-1000+320;


plot(x,y)
axis([-100 7000 -100 7000])


fid1=fopen('D:\Route_test.txt','wt');

%fprintf(fid1,'const uint16_t ansy[%d][%d]={\n ',m,n);

for i=1:707
    fprintf(fid1,'{ {%f,%f},%f},\n ',x(i),y(i),theta(i));
end

%fprintf(fid1,'}');

fclose(fid1);


