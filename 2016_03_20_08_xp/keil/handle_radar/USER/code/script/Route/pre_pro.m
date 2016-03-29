
clear

fid=fopen('E:\机器人队\快速_分段PID之前\2.txt','r');
count=1;
data_need=0;
data_wrong=0;
while(count ~= 0)
[data,~]=fscanf(fid,'%f');
data=data';
data_need=[data_need data];
[data,~]=fscanf(fid,'%c',[1,1]);
 data=data';
 data_wrong=[data_wrong data];
[data,count]=fscanf(fid,'%f');
data=data';
data_need=[data_need data];
end
fclose(fid);

data=zeros(1,length(data_need));
j=1;
for i=1:length(data_need)
    if(data_need(i) < 14000 && data_need(i) > -14000)
        data(j)=data_need(i);
        j=j+1;
    end
end

x_b=0;
y_b=0;
x_f=0;
y_f=0;


x_b_end=0;
y_b_end=0;
x_f_end=0;
y_f_end=300;

i=1;
count=0;
while( i<=length(data))
    if(data(i)==111)
 %       if(count==4 && abs(x_b_end-data(i-4) < 300 ) && abs(y_b_end-data(i-3) < 300 ) && abs(x_f_end-data(i-2) < 300 ) && abs(y_f_end-data(i-1) < 300 ) )
        if(count==4)           
            x_b=[x_b data(i-4)];
            y_b=[y_b data(i-3)];
            x_f=[x_f data(i-2)];
            y_f=[y_f data(i-1)];
            
            x_b_end=data(i-4);
            y_b_end=data(i-3);
            x_f_end=data(i-2);
            y_f_end=data(i-1);
            
        end
        count=0;
    else
        count=count+1;
    end
    i=i+1;
    
end

figure();
axis([-7000 7000 0 14000])
axis equal 
hold on
plot(x_b,y_b)
plot(x_f,y_f)

load('x_r.mat');
load('y_r.mat');
plot(x_r,y_r)

grid on
legend('后轮','前轮','路径');


% speed=data(1,:);
% b_x=data(2,:);
% b_y=data(3,:);
% f_x=data(4,:);
% f_y=data(5,:);
% f_theta=data(6,:);
% t_x=data(7,:);
% t_y=data(8,:);
% t_theta=data(9,:);
% error_angle=data(10,:);
% error_dis=data(11,:);