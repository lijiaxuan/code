clear
close all
speed=ones(10)*0.01;
b_x=ones(10);
b_y=ones(10);
f_x=ones(10);
f_y=ones(10);
f_theta=ones(10);
t_x=ones(10);
t_y=ones(10);
t_theta=ones(10);
error_angle=ones(10);
error_dis=ones(10);

fid=fopen('E:\»úÆ÷ÈË¶Ó\10.txt','w');
for i=1:10
fprintf(fid,'%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f;\r\n',...
 		speed(i),b_x(i),b_y(i),...
 		f_x(i),f_y(i),f_theta(i),...
 		t_x(i),t_y(i),t_theta(i),...
 		error_angle(i),error_dis(i));
end
fclose(fid);




