#include "stm32f10x.h"
#include "platform_config.h"
#include "all.h"

int main(void)
{  
		adcint();//C4a1 5a0 B01a2																													 
		Usartint();//A9 10
		dispint();//A45 B89
		SWint();//c10sw1 11sw2 12sw3
		pxint();//c 6 7 8 ����ѡ��cdоƬ��ͨ·������һ��ͨ·��ģ��ֵ����оƬ��ADC����
		S_flag=SWx();
   switch(S_flag)
			{
			case 0x00:
				{	
					for(nn=0;nn<3;nn++)
						{
								 Display(0x55555555);//display�����ã������ϲ��LED��
								 Delay10us(20000);
								 Display(0xaaaaaaaa);
								 Delay10us(20000);
						 }
					 
					while(1)
					 {					 
					   LBcai();
						 if(S_flag==0xee)
							 {
								for(mm=0;mm<3;mm++)
									{
											for(nn=0;nn<32;nn++)
												{
													Display(0x01<<nn);
													Delay10us(2000);
												}
									 }  
									
								Display(0x19900423);
								intflash();
									
								while(S_flag!=0xa1)
										 {
										 }
										 
								junzhi();
								fengzhih();
								Display(0x00ffff00);
										 
								while(S_flag!=0xa2)
										 {
										 }
										 
								fengzhil();
								chizhi();
								Display(0xff0000ff);
									for(nn=0;nn<4;nn++)
										 {
											 Display(0x00000000);
											 Delay10us(20000);
											 Display(0xffffffff);
											 Delay10us(20000);
											}  
									while(1)
										 {
											 LBcai();
										 }
							}
         }
       }break;

			 
			case 0x01:
			 {
			  for(nn=0;nn<4;nn++)
					{
						 Display(0x55555555);
						 Delay10us(20000);
						 Display(0xaaaaaaaa);
						 Delay10us(20000);
					 }
					 px(4);
					 LBcai();
				for(nn=0;nn<24;nn++)
					 {
					   Usend(DRBuffer0[2][nn]);
					 }
			  }break;

			case 0x02:{}break;
				
			case 0x03:
			  {	
					 for(mm=0;mm<3;mm++)
							{
								for(nn=0;nn<32;nn++)
									{
										Display(0x01<<nn);
										Delay10us(2000);
									 }
						  }  
							
							
						while(1)
						 {
							if(S_flag==0xee)
								{
									break;
								}
						 } 
						
						 Display(0x19900423);
						 intflash();
						
							while(S_flag!=0xa1)//��uart���յ����ݣ�����uart�ж�ͨ����Ȼ�����Ӧ��s_flag���и�ֵ������ִ����Ӧ�ĳ���
						   {
						   }
							 
							junzhi();
							fengzhih();
							Display(0x00ffff00);
							 
						while(S_flag!=0xa2)
							 {
							 }
							 
							fengzhil();
							chizhi();
							Display(0xff0000ff);
							 
						while(S_flag!=0xb1)
							 {
							 }
							 
							 fengzhih();
							 Display(0x00ffff00);
							 
						while(S_flag!=0xb2)
							 {
							 }
							 
								fengzhil();
								chizhired();
								Display(0xff0000ff);
							 
						 while(S_flag!=0xc1)
							 {
							 }
								 fengzhih();
								 Display(0x00ffff00);
							 
							while(S_flag!=0xc2)
							 {
							 }
							 
								fengzhil();
								chizhiblue();
							 
							for(nn=0;nn<4;nn++)
								{
									Display(0x00000000);
									Delay10us(20000);
									Display(0xffffffff);
									Delay10us(20000);
								 } 								 
								while(1)
								 {
								  LBcai();
								 }

			 }break;
			 
			case 0x04:{}break;
				
			case 0x05:{}break;
				
			case 0x06:{}break;
				
			case 0x07:
				{
					intflash();
					for(nn=0;nn<6;nn++)
						{
							 Display(0x55555555);
							 Delay10us(20000);
							 Display(0xaaaaaaaa);
							 Delay10us(20000);
						 } 
						 
					  junzhi();
						Display(0x00ffff00);
						Delay10us(500000);
						fengzhih();
						Display(0xff0000ff);
						Delay10us(500000);
						fengzhil();
						chizhi();
						 
					for(nn=0;nn<4;nn++)
						{
							 Display(0x00000000);
							 Delay10us(20000);
							 Display(0xffffffff);
							 Delay10us(20000);
						 } 
					 
					 while(SWx()!=0x03)
					   {

					   }
						 
					 for(nn=0;nn<6;nn++)
							{
								 Display(0x55555555);
								 Delay10us(20000);
								 Display(0xaaaaaaaa);
								 Delay10us(20000);
							 }  
							 
						 Display(0x00ffff00);
						 Delay10us(500000);
						 fengzhih();
						 Display(0xff0000ff);
						 Delay10us(500000);
						 fengzhil();
						 chizhired();
							 
					for(nn=0;nn<5;nn++)
						{
							Display(0x00000000);
							Delay10us(20000);
							Display(0xffffffff);
							Delay10us(20000);
						 }  

					while(SWx()!=0x01)
					  {

					  }
						
					for(nn=0;nn<6;nn++)
						{
							 Display(0x55555555);
							 Delay10us(20000);
							 Display(0xaaaaaaaa);
							 Delay10us(20000);
						 } 
						 
						Display(0x00ffff00);
						Delay10us(500000);
						fengzhih();
						Display(0xff0000ff);
						Delay10us(500000);
						fengzhil();
						chizhiblue();
						 
					for(nn=0;nn<4;nn++)
						{
							 Display(0x00000000);
							 Delay10us(20000);
							 Display(0xffffffff);
							 Delay10us(20000);
						 }  
						 
			}break;

      default: break;
			
   } 			 
 


				while(1)
				 { 
			 
			   }
		}


#ifdef  USE_FULL_ASSERT

void assert_failed(uint8_t* file, uint32_t line)
{  
  while (1)
  {
  }
}
#endif

