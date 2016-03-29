/****************************************************************************
* Copyright (C), 2011 �ܶ�Ƕ��ʽ������ www.ourstm.net
*
* �������� �ܶ���STM32������V2,2.1,V3�ϵ���ͨ��           
* QQ: 9191274, ������sun68, Email: sun68@163.com 
* �Ա����̣�ourstm.taobao.com  
*
* �ļ���: Fun.c
* ���ݼ���:
*       �������ṩ�˷ܶ�����2.4����ģ�����������
		
*
* �ļ���ʷ:
* �汾��  ����       ����    ˵��
* v0.2    2011-07-21 sun68  �������ļ�
*
*/


#include "fsmc_sram.h"
#include "font.h"  
#include "demo.h" 
#include "stdlib.h"
 
#define Bank1_LCD_D    ((uint32_t)0x60020000)    //disp Data ADDR
#define Bank1_LCD_C    ((uint32_t)0x60000000)	 //disp Reg ADDR



void LCD_Init(void);
void LCD_WR_REG(unsigned int index);
void LCD_WR_CMD(unsigned int index,unsigned int val);

void LCD_WR_Data(unsigned int val);

void LCD_test(void);
void LCD_clear(unsigned int p);

void Delay(__IO uint32_t nCount);

unsigned short LCD_RD_data(void);
extern void lcd_rst(void);
extern void Delay(__IO uint32_t nCount);

void TFTSetXY(u16 x,u16 y);     
void ili9481_SetCursor(u16 x,u16 y);
void ili9481_SetPoint(u16 x,u16 y,u16 point);
void ili9481_Clear(u16 dat);
void ili9481_DrawLine_x(u16 x_start,u16 y_start,u16 x_end,u16 y_end,u16 point);
void TFT_CLEAR(u8 x,u16 y,u8 len,u16 wid);
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u16 color,u8 mode);
void LCD_ShowString(u16 x,u16 y,const u8 *p,u16 col,u8 mode);
void LCD_ShowChar_BIG(u16 x,u16 y,u8 num,u8 size,u16 color);
void LCD_ShowString_BIG(u16 x,u16 y,const u8 *p,u16 col);
void LCD_TEST_Picture3(unsigned char *picture1);
void ili9481_fill(u16 x_start,u16 y_start,u16 x_end,u16 y_end,u16 color);
unsigned char picture1[];
u16 POINT_COLOR=GREEN;     				//Ĭ�Ϻ�ɫ  
u16 DeviceCode;
/****************************************************************************
* ��    �ƣ�LCD_WR_REG(unsigned int index)
* ��    �ܣ�FSMCд��ʾ���Ĵ�����ַ����
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/ 
void LCD_WR_REG(unsigned int index)
{
	*(__IO uint16_t *) (Bank1_LCD_C)= index;

}			 

/****************************************************************************
* ��    �ƣ�void LCD_WR_CMD(unsigned int index,unsigned int val)
* ��    �ܣ�FSMCд��ʾ���Ĵ������ݺ���
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/ 
void LCD_WR_CMD(unsigned int index,unsigned int val)
{	
	*(__IO uint16_t *) (Bank1_LCD_C)= index;	
	*(__IO uint16_t *) (Bank1_LCD_D)= val;
}


/****************************************************************************
* ��    �ƣ�unsigned short LCD_RD_data(void)
* ��    �ܣ�FSMC����ʾ��16λ���ݺ���
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/ 
unsigned short LCD_RD_data(void){
	unsigned int a=0;
	a=*(__IO uint16_t *) (Bank1_LCD_D);   //�ղ���
	a=*(__IO uint16_t *) (Bank1_LCD_D);   //������ʵ��16λ��������	  
	return(a);	
}



/****************************************************************************
* ��    �ƣ�LCD_WR_Data(unsigned int val)
* ��    �ܣ�FSMCд16λ���ݺ���
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/ 
void LCD_WR_Data(unsigned int val)
{   
	*(__IO uint16_t *) (Bank1_LCD_D)= val; 	
}

void LCD_WR_Data_8(unsigned int val)
{
	*(__IO uint16_t *) (Bank1_LCD_D)= val;
}
/****************************************************************************
* ��    �ƣ�void Delay(__IO uint32_t nCount)
* ��    �ܣ���ʱ����
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/ 
void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}
//

/****************************************************************************
* ��    �ƣ�void LCD_Init(void)
* ��    �ܣ�2.4 ��TFT ��ʼ������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/ 
void LCD_Init(void)
{	
	unsigned int i;
	
	LCD_WR_REG(0x0011); 
	Delay(500000);
	LCD_WR_REG(0x0013); 
	LCD_WR_CMD(0x00D0, 0x0007); 
	LCD_WR_CMD(0x00D0, 0x0040); 
	LCD_WR_CMD(0x00D0, 0x001c); 

	LCD_WR_CMD(0x00D1, 0x0000); 

	LCD_WR_CMD(0x00D1, 0x0018); 
	LCD_WR_CMD(0x00D1, 0x001d); 
	LCD_WR_CMD(0x00D2, 0x0001);
	LCD_WR_CMD(0x00D2, 0x0011); 

	LCD_WR_CMD(0x00C0, 0x0000);
	LCD_WR_CMD(0x00C0, 0x003B); 
	LCD_WR_CMD(0x00C0, 0x0000); 
	LCD_WR_CMD(0x00C0, 0x0002); 

	LCD_WR_CMD(0x00C0, 0x0011); 
	LCD_WR_CMD(0x00C1, 0x0010); 
	LCD_WR_CMD(0x00C1, 0x000B); 
	LCD_WR_CMD(0x00C1, 0x0088); 

	LCD_WR_CMD(0x00C5, 0x0001); 
	LCD_WR_CMD(0x00C8, 0x0000);

	LCD_WR_CMD(0x00C8, 0x0030); 

	LCD_WR_CMD(0x00C8, 0x0036); 
	LCD_WR_CMD(0x00C8, 0x0045); 
	LCD_WR_CMD(0x00C8, 0x0004); 

	LCD_WR_CMD(0x00C8, 0x0016); 
	LCD_WR_CMD(0x00C8, 0x0037); 

	LCD_WR_CMD(0x00C8, 0x0075);
	LCD_WR_CMD(0x00C8, 0x0077);
	LCD_WR_CMD(0x00C8, 0x0054);
	LCD_WR_CMD(0x00C8, 0x000f);
	LCD_WR_CMD(0x00C8, 0x0000);
	LCD_WR_CMD(0x00E4, 0x00A0);
	LCD_WR_CMD(0x00F0, 0x0001);
	LCD_WR_CMD(0x00F3, 0x0040);
	LCD_WR_CMD(0x00F3, 0x000A);
	LCD_WR_CMD(0x00F7, 0x0080);
	LCD_WR_CMD(0x0036, 0x000a);
	LCD_WR_CMD(0x003A, 0x0055);

	LCD_WR_CMD(0x002A, 0x0000); 
	LCD_WR_CMD(0x002A, 0x0000); 
	LCD_WR_CMD(0x002A, 0x0001); 
	LCD_WR_CMD(0x002A, 0x003F); 
	LCD_WR_CMD(0x002B, 0x0000); 
	LCD_WR_CMD(0x002B, 0x0000); 
	LCD_WR_CMD(0x002B, 0x0001); 
	LCD_WR_CMD(0x002B, 0x00df);

	Delay(500000);
	LCD_WR_REG(0x0029);

	LCD_WR_REG(0x002C);

	for(i=0;i<262144;i++)
	{
	  LCD_WR_Data(0x0000);					 
	}  
}
void ili9481_SetCursor(u16 x,u16 y)//���ù��
{
	LCD_WR_REG(0x002A);
  
	LCD_WR_Data(y>>8);
	LCD_WR_Data(0x00FF&y);

	LCD_WR_Data(0x0001);
	LCD_WR_Data(0x003F);
	
	LCD_WR_REG(0x002B);
	
	LCD_WR_Data((480-x)>>8);
	LCD_WR_Data(0x00FF&(480-x));
		
	LCD_WR_Data(0x0001);
	LCD_WR_Data(0x00df);
}

void TFTSetXY(u16 x,u16 y)
{
    ili9481_SetCursor(x,y);
    LCD_WR_REG(0x002C);    
}

void ili9481_SetPoint(u16 x,u16 y,u16 point)
{
	ili9481_SetCursor(x,y);
	
	LCD_WR_REG(0x002C);
	LCD_WR_Data(0XFFFF-point);
    
}

void ili9481_DrawLine_x(u16 x_start,u16 y_start,u16 x_end,u16 y_end,u16 point)
{
	u32  n;
	ili9481_SetCursor(x_start,y_start);

    LCD_WR_REG(0x002C);

	for(n=x_start;n<=x_end;n++)
	{		
    	LCD_WR_Data(0XFFFF-point);
  	}
  
}

void ili9481_fill(u16 x_start,u16 y_start,u16 x_end,u16 y_end,u16 color)//������������ɫ
{
	u32  n,j;
	ili9481_SetCursor(x_start,y_start);

    LCD_WR_REG(0x002C);
	for(n=x_start;n<=x_end;n++)
	{
		for(j=y_start;j<=y_end;j++)
		{
	    	LCD_WR_Data(0XFFFF-color);
		}
		ili9481_SetCursor(n+1,y_start);
		LCD_WR_REG(0x002C);
	}
}

void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u16 color,u8 mode)
{  
    u8 temp;
    u8 pos,t;
    if(x>479||y>319)return;          
    num=num-' ';                                           //�õ�ƫ�ƺ��ֵ
    if(!mode)                                              //�ǵ��ӷ�ʽ
    {
        for(pos=0;pos<size;pos++)
        {
            if(size==12)
						temp=asc2_1206[num][pos];          //����1206����
            else 
						temp=asc2_1608[num][pos];                 //����1608����
            for(t=0;t<size/2;t++)
            {                 
                if(temp&0x01)ili9481_SetPoint(x+t,y+pos,color);
                else ili9481_SetPoint(x+t,y+pos,WHITE);
                temp>>=1; 
            }
        }   
    }else                                                      //���ӷ�ʽ
    {
        for(pos=0;pos<size;pos++)
        {
            if(size==12)
						temp=asc2_1206[num][pos];              //����1206����
            else 
						temp=asc2_1608[num][pos];                     //����1608����
            for(t=0;t<size/2;t++)
            {                 
                if(temp&0x01)ili9481_SetPoint(x+t,y+pos,color);   //��һ����     
                temp>>=1; 
            }
        }
    }                     
}

void LCD_ShowChar_BIG(u16 x,u16 y,u8 num,u8 size,u16 color)
{  
    u8 temp;
    u8 pos,t;
    if(x>479||y>319)return; 
	ili9481_fill(x,y,x+16,y+32,WHITE);         
    num=num-' ';                                           //�õ�ƫ�ƺ��ֵ                                            
    
        for(pos=0;pos<size;pos++)
        {
            if(size==16)
						temp=asc2_1608[num][pos];          //����4824����
                             
            for(t=0;t<size/2;t++)
            {                 
                if(temp&0x01)
				{
					ili9481_SetPoint(x+2*t,y+2*pos,color);
					ili9481_SetPoint(x+2*t+1,y+2*pos,color);
					ili9481_SetPoint(x+2*t,y+2*pos+1,color);
					ili9481_SetPoint(x+2*t+1,y+2*pos+1,color);
				}
                temp>>=1; 
            }
        }   
                   
}

void LCD_ShowString(u16 x,u16 y,const u8 *p,u16 col,u8 mode)
{     
    while(*p!='\0')
    {       
        if(x>479){x=0;y+=16;}
        if(y>319){y=x=0;ili9481_Clear(WHITE);}
        LCD_ShowChar(x,y,*p,16,col,mode);
        x+=8;
        p++;
    }  
}

void LCD_ShowString_BIG(u16 x,u16 y,const u8 *p,u16 col)
{    
	 
    while(*p!='\0')
    {       
        if(x>470){x=0;y+=32;}
        if(y>300){y=x=0;ili9481_Clear(WHITE);} 
		LCD_ShowChar_BIG(x,y,*p,16,col);
        x+=16;
        p++;
		
    }  
}

void ili9481_Clear(u16 dat)
{
	u32  n;
	
	ili9481_SetCursor(0,0);
  
    LCD_WR_REG(0x002C);

	for(n=0;n<262144;n++)
	{
		LCD_WR_Data(0XFFFF-dat);					 	
	}

}

   void LCD_Clear(void)
	 { 
		 u16 color=MAGENTA;
		 ili9481_Clear(WHITE);
		 LCD_ShowString_BIG(320,0,  "|========|",color);
	   LCD_ShowString_BIG(320,32, "|  XJTU  |",color);
	   LCD_ShowString_BIG(320,64, "|  ROBOT |",color);
	   LCD_ShowString_BIG(320,96, "|  TEAM  |",color);
	   LCD_ShowString_BIG(320,128,"|========|",color);
	   LCD_ShowString_BIG(320,160,"|  HELLO |",color);
	   LCD_ShowString_BIG(320,192,"|   TO   |",color);
	   LCD_ShowString_BIG(320,224,"|   BE   |",color);
	   LCD_ShowString_BIG(320,256,"|   NO.1 |",color);
	   LCD_ShowString_BIG(320,288,"|========|",color);
   }
	 
void LCD_TEST_Picture3(unsigned char *picture1)
{
//     unsigned	char i,j;
//
//		ili9481_SetCursor(0,0);
//  
//    	LCD_WR_REG(0x002C);
//       	 
//			for (j=0;j<260000;j++)
//			{
//				
//              LCD_WR_Data(picture1[j]);                  
//			}
		 

}




/****************************************************************************
* ��    �ƣ�void TFT_CLEAR(u8 x,u16 y,u8 len,u16 wid)
* ��    �ܣ�2.4 ��TFT ��������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/ 
void TFT_CLEAR(u8 x,u16 y,u8 len,u16 wid)
{                    
    u32 n;	
	LCD_WR_CMD(0x0050, x); 				//���ڵ���ʼX
	LCD_WR_CMD(0x0051, x+len-1); 		//���ڵĽ���X
	LCD_WR_CMD(0x0052, y);              //���ڵ���ʼY
	LCD_WR_CMD(0x0053, y+wid-1); 		//���ڵĽ���Y
	ili9481_SetCursor(20,40);
	GPIO_ResetBits(GPIOD,GPIO_Pin_7);
    LCD_WR_REG(0x002C);
	
	for(n=0;n<50000;n++)
	{
		LCD_WR_Data(0x0000);					 
	}
	for(n=0;n<50000;n++)
	{
		LCD_WR_Data(0XFFFF-0xF800);					 
	}
	GPIO_SetBits(GPIOD,GPIO_Pin_7);
}   




///****************************************************************************
//* ��    �ƣ�void TFT_DrawPoint(u8 x,u16 y)
//* ��    �ܣ�2.4 ��TFT ���㺯��
//* ��ڲ�������
//* ���ڲ�������
//* ˵    ����
//* ���÷������� 
//****************************************************************************/ 
//void TFT_DrawPoint(u8 x,u16 y)
//{
//    
//	LCD_WR_CMD(32, x);					//��ʼ����X
//    LCD_WR_CMD(33, y);					//��ʼ����Y
//	LCD_WR_REG(34);				   
//	LCD_WR_Data(POINT_COLOR); 	   	
//} 
///****************************************************************************
//* ��    �ƣ�u32 abs(s32 res)
//* ��    �ܣ�ȡ����ֵ����
//* ��ڲ�������
//* ���ڲ�������
//* ˵    ����
//* ���÷������� 
//****************************************************************************/ 
////u32 abs(s32 res)
////{
////	if(res<0)return -res;
////	else return res;
////}  
//
///****************************************************************************
//* ��    �ƣ�void TFT_ShowChar(u8 x,u16 y,u8 num)
//* ��    �ܣ���ָ��λ����ʾһ���ַ�
//* ��ڲ�������
//* ���ڲ�������
//* ˵    ����
//* ���÷������� 
//****************************************************************************/ 
//void TFT_ShowChar(u8 x,u16 y,u8 num)
//{       
//	#define MAX_CHAR_POSX 232
//	#define MAX_CHAR_POSY 304 
//    u8 temp;
//    u8 pos,t;      
//    if(x>MAX_CHAR_POSX||y>MAX_CHAR_POSY)return;
//    //�趨һ���ַ���ռ�Ĵ��ڴ�С 
//	LCD_WR_CMD(0x0050, x); 				//���ڵ���ʼX
//	LCD_WR_CMD(0x0051, x+5); 			//���ڵĽ���X
//	LCD_WR_CMD(0x0052, y);              //���ڵ���ʼY
//	LCD_WR_CMD(0x0053, y+11); 			//���ڵĽ���Y	 
//	LCD_WR_CMD(32, x);
//    LCD_WR_CMD(33, y);
//	LCD_WR_REG(34);					   
//	num=num-' ';						//�õ�ƫ�ƺ��ֵ
//	for(pos=0;pos<12;pos++)	{
//	    temp=asc2_1206[num][pos];		//�����ģ�����ֵ
//	    for(t=0;t<6;t++)
//	    {                 
//	        if(temp&0x01)LCD_WR_Data(POINT_COLOR);	  //λΪ1 ��ָ����ɫд�뵽����
//	        else LCD_WR_Data(0xffff);                 //λΪ0 �ð�ɫд�뵽����    
//	        temp>>=1; 
//	    }
//	}
//}  	 
///****************************************************************************
//* ��    �ƣ�u32 mn(u8 m,u8 n)
//* ��    �ܣ�
//* ��ڲ�������
//* ���ڲ�������
//* ˵    ����
//* ���÷������� 
//****************************************************************************/ 
//u32 mn(u8 m,u8 n)
//{
//	u32 result=1;	 
//	while(n--)result*=m;    
//	return result;
//}
///****************************************************************************
//* ��    �ƣ�void TFT_ShowNum(u8 x,u16 y,u32 num)
//* ��    �ܣ���ָ��λ����ʾһ������0-65536
//* ��ڲ�������
//* ���ڲ�������
//* ˵    ����
//* ���÷������� 
//****************************************************************************/ 
//void TFT_ShowNum(u8 x,u16 y,u32 num)
//{      
//	u32 res;   	   
//	u8 t=0,t1=0;   
//	res=num;
//	if(!num)TFT_ShowChar(x,y,'0');		//��������Ϊ0 ��ָ��λ����ʾ0
//	while(res)  						//�õ����ֳ���
//	{
//		res/=10;
//		t++;
//	}
//	t1=t;
//	while(t)							//��ʾ����
//	{
//		res=mn(10,t-1); 	 
//	    TFT_ShowChar(x+(t1-t)*6,y,(num/res)%10+'0');	 //��ָ��λ����ʾ����
//		t--;
//	}				     
//} 
///****************************************************************************
//* ��    �ƣ�void TFT_ShowString(u8 x,u16 y,const u8 *p)
//* ��    �ܣ���ָ��λ����ʾ�ַ���
//* ��ڲ�������
//* ���ڲ�������
//* ˵    ����
//* ���÷������� 
//****************************************************************************/ 
//void TFT_ShowString(u8 x,u16 y,const u8 *p)
//{         
//    while(*p!='\0')
//    {       
//        if(x>MAX_CHAR_POSX){x=0;y+=12;}
//        if(y>MAX_CHAR_POSY){y=x=0;TFT_CLEAR(0,0,240,320);}
//        TFT_ShowChar(x,y,*p);
//        x+=6;
//        p++;
//    }  
//}
///****************************************************************************
//* ��    �ƣ�void TFT_ShowBmp(u8 x,u16 y,u8 lenth,u16 wide,const u8 *p)
//* ��    �ܣ���ָ��λ����ʾһ��ͼƬ
//* ��ڲ�������
//* ���ڲ�������
//* ˵    ����
//* ���÷������� 
//****************************************************************************/ 
//void TFT_ShowBmp(u8 x,u16 y,u8 lenth,u16 wide,const u8 *p)
//{      
//    u32 size,temp; 
//    //�趨һ��ͼƬ��ռ�Ĵ��ڴ�С 	
//	LCD_WR_CMD(0x0050, x); 				//���ڵ���ʼX
//	LCD_WR_CMD(0x0051, (u16)x+lenth-1); //���ڵĽ���X
//	LCD_WR_CMD(0x0052, y);              //���ڵ���ʼY
//	LCD_WR_CMD(0x0053, y+wide-1); 		//���ڵĽ���Y	 
//	LCD_WR_CMD(32, x);
//    LCD_WR_CMD(33, y);
//	LCD_WR_REG(34);					   
//
//	temp=(u32)lenth*wide*2;
//	for(size=0;size<temp;size++)LCD_WR_Data_8(p[size]); 
//}         
//
///******************* (C) COPYRIGHT 2011 �ܶ�STM32 *****END OF FILE****/
