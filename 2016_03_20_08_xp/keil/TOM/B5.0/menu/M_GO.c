#include "all.h"
#include "string.h"
#include "stdio.h"

extern char filename[20];

int retry_cnt=0;//每次进入函数这个函数在目录下新建一个文件




//全程
void go(void)
{	
    static	char send_flag=0;
	char back_1=0;
    camerError=0;
    stopCount=0;
    stopStatus=0;
    
	while(1)
	{
        if(send_flag%10==0)
        {
		LCD_Clear_Forward(7);
		
		LCD_WriteStringXY("ROUTE_NUM",0,0);
		LCD_WriteIntXY(Route_Num,10,0);
		
		LCD_WriteStringXY("X:",0,1);
		LCD_WriteDoubleXY(position_forward.x/1000,2,2,1);//m
		
		LCD_WriteStringXY("Y:",7,1);
		LCD_WriteDoubleXY(position_forward.y/1000,2,9,1);//m
		
		LCD_WriteStringXY("A:",14,1);
		LCD_WriteDoubleXY(Gps_List[0].angle,1,16,1);
		
		LCD_WriteStringXY("X:",0,2);
		LCD_WriteDoubleXY(PointRoute[Point_NowNum].position.x/1000,2,2,2);//m
		
		LCD_WriteStringXY("Y:",7,2);
		LCD_WriteDoubleXY(PointRoute[Point_NowNum].position.y/1000,2,9,2);//m
		
		LCD_WriteStringXY("A:",14,2);
		LCD_WriteDoubleXY(PointRoute[Point_NowNum].selfdir*180/PI,1,17,2);
	
	
		LCD_WriteStringXY("T:",0,3);
		LCD_WriteIntXY(real_time/1000,2,3);
		
		LCD_WriteStringXY("V:",7,3);
		LCD_WriteDoubleXY(Gps_List[0].speed,1,11,3);

        LCD_WriteStringXY("d:",0,4);
		LCD_WriteIntXY(error_dis,2,4);
		
		LCD_WriteStringXY("a:",6,4);
		LCD_WriteDoubleXY(error_angle,1,8,4);

        LCD_WriteStringXY("c:",13,4);
		LCD_WriteDoubleXY(angel,1,15,4);

		LCD_WriteStringXY("RN:",0,6);
		
		LCD_WriteIntXY(Route_Num,2,6);
        
		LCD_WriteStringXY("NN:",8,6);
		
		LCD_WriteIntXY(Point_NowNum,10,6);
        
         refresh();
        }
        if(Point_NowNum>5 && Point_NowNum<15)
        LCD_WriteIntXY(real_time,0,7);
        else if(Point_NowNum>3000 && Point_NowNum<3015)
        LCD_WriteIntXY(real_time,7,7);
        
        send_flag++;
        
        if (KeyReady==1 && KeyValue==5 && back_1 != 1)
		{
			KeyReady=0;
			back_1=1;	
		}
		if (KeyReady==1 && KeyValue==5 && back_1==1)
		{
			KeyReady=0;
			back_1=0;
			SET_ANGLE(0,1);
            SET_ANGLE(0,2);
            
			return;				
		}
 		 
        #if GYRO
			if(Gyro_Error_Flag)
		{
				UART1_DMA_EN();
				Gyro_Error_Flag = 0;
		}	
		#endif
		Go_Path(Route_Num);//执行一次所需时间0.00121187	1.2ms
        
        stop();
        

	    //输出控制信息

                f_printf(&fil,"%d",real_time);
                f_printf(&fil,"%s"," ");
             
                f_printf(&fil,"%d",Point_NowNum);
                f_printf(&fil,"%s"," ");
             
                write_double(PointRoute[Point_NowNum].position.x);
                f_printf(&fil,"%s"," ");
				write_double(PointRoute[Point_NowNum].position.y);
                f_printf(&fil,"%s"," ");
				write_double(PointRoute[Point_NowNum].selfdir);
				f_printf(&fil,"%s"," ");
				write_double(position_forward.x);
				f_printf(&fil,"%s"," ");
				write_double(position_forward.y);
				f_printf(&fil,"%s"," ");
				write_double(Gps_List[0].position.x);
                f_printf(&fil,"%s"," ");
				write_double(Gps_List[0].position.y);
				f_printf(&fil,"%s"," ");
				write_double(Gps_List[0].speed);
				f_printf(&fil,"%s"," ");
                write_double(Gps_List[0].angle);
				f_printf(&fil,"%s"," ");	
				write_double(error_angle);
				f_printf(&fil,"%s"," ");
				write_double(error_dis);
				f_printf(&fil,"%s"," ");
				write_double(angel);	
				f_printf(&fil,"%s"," ");
                
                write_double((double)camerError);
				f_printf(&fil,"%s"," ");
				write_double(error_dis_center);
				f_printf(&fil,"%s"," ");
				
				f_printf(&fil,"\r\n");
			
	delay(0);
	}
}
    int Start_Offset=50+CARLENGTH/5;		//当前前轮对应的点阵下标  前轮的下标 + 车长换算的下标
    Pointfp64 Gps_Start={0,250};//当前
    int StartNum=1;
    

		//初始化路径信息
        
void fu_go(void)
{
//         static char buf[56];
//         static int retry_cnt=0;//每次进入函数这个函数在目录下新建一个文件
//         retry_cnt++;

//         real_time=0;
// 		Route_Num=StartNum;					//路径编号  pid控制参数
//     
// 		SetPointPath(Route[Route_Num-1]+Start_Offset,Route[Route_Num]);//路径的起终点
//         Point_NowNum_Back=0;
//         Point_NowNum_Center=Route[Route_Num-1]+50+CARLENGTH/2;
//     
//         delay(2000);
//     
// 		GPS_Init(Gps_Start,0.0);;//gps信息初始化 设置为当前后轮所在点的GPS 数据 可以考虑放在路径某段的起点或终点（重启时）
//         
//         PID_Clear();
// 		sprintf(buf, "%s_%2d","data", retry_cnt);
//         f_nameDeal(buf);
//         sprintf(buf,"%s.txt",buf);	
// 		file_open(buf); 
//       go();
//         file_close();
//         f_timestamp(buf);
}

//分段跑

void choose_go(void)
{
	while(1)
	{
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString("2.Choose_Route");
				
		LCD_WriteStringXY("NowNum:",0,1);
		
		LCD_WriteIntXY(Point_NowNum,8,1);
        
        refresh();
		
		if(KeyReady==1)
		{
			KeyReady=0;
			switch(KeyValue)
			{
				case key2:
					Input_IntValue(&Route_Num,"Route_Num");
					
					GPS_Init(PointRoute[ Route[Route_Num-1] + 1].position,PointRoute[ Route[Route_Num-1] +1 ].selfdir);	//后轮摆重启区那段路径的起点上
					SetPointPath(Route[Route_Num-1]+1+CARLENGTH/5,Route[Route_Num]);//此时前轮的位置为 Route[Route_Num-1] +1 +61  61为车长除以步长
                    Point_NowNum_Back=Route[Route_Num-1];
                    Point_NowNum_Center=Route[Route_Num-1]+1+CARLENGTH/10;
                
                    PID_Clear();
					go();
					break;
				case keyback:
					return;
			}
		}
       delay(LCD_DELAY);
	}
	

}

void go_right(void)
{
        static char buf[56];
        //static int retry_cnt=0;//每次进入函数这个函数在目录下新建一个文件
        retry_cnt++;
        
         Start_Offset=50+CARLENGTH/5;		//当前前轮对应的点阵下标  前轮的下标 + 车长换算的下标
         Gps_Start.x=0;
         Gps_Start.y=250;
         StartNum=1;
        
        real_time=0;
		Route_Num=StartNum;					//路径编号  pid控制参数
    
		SetPointPath(Route[Route_Num-1]+Start_Offset,Route[Route_Num]);//路径的起终点
        Point_NowNum_Back=0;
        Point_NowNum_Center=Route[Route_Num-1]+50+CARLENGTH/2;
    
        delay(2000);
    
		GPS_Init(Gps_Start,0.0);;//gps信息初始化 设置为当前后轮所在点的GPS 数据 可以考虑放在路径某段的起点或终点（重启时）
        
        PID_Clear();
		sprintf(buf, "%s_%2d","data", retry_cnt);
        f_nameDeal(buf);
        sprintf(buf,"%s.txt",buf);	
		file_open(buf); 
        
        
      go();
        file_close();
        f_timestamp(buf);
}

void go_left(void)
{
    


        static char buf[56];
        retry_cnt++;

         Start_Offset=50+CARLENGTH/5;		//当前前轮对应的点阵下标  前轮的下标 + 车长换算的下标
         Gps_Start.x=0;
         Gps_Start.y=250;
         StartNum=26;
        
        real_time=0;
		Route_Num=StartNum;					//路径编号  pid控制参数
    
		SetPointPath(Route[Route_Num-1]+Start_Offset,Route[Route_Num]);//路径的起终点
        Point_NowNum_Back=0;
        Point_NowNum_Center=Route[Route_Num-1]+50+CARLENGTH/2;
    
        delay(2000);
    
		GPS_Init(Gps_Start,0.0);;//gps信息初始化 设置为当前后轮所在点的GPS 数据 可以考虑放在路径某段的起点或终点（重启时）
        
        PID_Clear();
		sprintf(buf, "%s_%2d","data", retry_cnt);
        f_nameDeal(buf);
        sprintf(buf,"%s.txt",buf);	
		file_open(buf); 
        
      go();
        file_close();
        f_timestamp(buf);
}



//     #ifdef	UART_DEBUG
// //     //帧头
// //     USART_SendData(USART3,0xff);
// //     while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);    
// //     USART_SendData(USART3,0xff);
// //     while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);    
// //     //目标点
// //     WriteDouble(PointRoute[Point_NowNum].position.x,1);
// //     WriteDouble(PointRoute[Point_NowNum].position.y,1);
// //     WriteDouble(PointRoute[Point_NowNum].selfdir,1);
// //     //前轮GPS
// //     WriteDouble(position_forward.x,1);
// //     WriteDouble(position_forward.y,1);
// //     //后轮GPS
// //     WriteDouble(Gps_List[0].position.x,1);
// //     WriteDouble(Gps_List[0].position.y,1);
// //     //speed angle
// //     
// //     WriteDouble(Gps_List[0].speed,1);
// //     WriteDouble(Gps_List[0].angle,1);
// //     
// //     //控制信息 error_angle error_dis angel 输出角度
// //     WriteDouble(error_angle,1);
// //     WriteDouble(error_dis,1);
// //     WriteDouble(angel,1);

// //         //帧头
// //     
// //     if(send_flag%10==0)
// //     {
// //     USART3_TxBuffer[0]=PointRoute[Point_NowNum].position.x;
// //     USART3_TxBuffer[1]=PointRoute[Point_NowNum].position.y;
// //     USART3_TxBuffer[2]=PointRoute[Point_NowNum].selfdir;
// //     
// //     USART3_TxBuffer[3]=position_forward.x;
// //     USART3_TxBuffer[4]=position_forward.y;
// //     
// //     USART3_TxBuffer[5]=Gps_List[0].position.x;
// //     USART3_TxBuffer[6]=Gps_List[0].position.y;
// //     
// //     USART3_TxBuffer[7]=Gps_List[0].speed;
// //     USART3_TxBuffer[8]=Gps_List[0].angle;
// //     
// //     USART3_TxBuffer[9]=error_angle;
// //     USART3_TxBuffer[10]=error_dis;
// //     USART3_TxBuffer[11]=angel;
// //     
// //     USART_SendData(USART3,0xff);
// //     while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);    
// //     USART_SendData(USART3,0xff);
// //     while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);  
// //     
// //     DMA_Cmd(DMA1_Channel2, DISABLE );  //??USART1 TX DMA1 ??????   
// //     DMA1_Channel2->CMAR = (uint32_t)USART3_TxBuffer;
// //  	DMA_SetCurrDataCounter(DMA1_Channel2,48);//DMA???DMA?????
// //  	DMA_Cmd(DMA1_Channel2, ENABLE);  //??USART1 TX DMA1 ?????? 
// //     }
//     #endif


