#include "ov7725.h"
#include "MT9M111.h"
#include "delay.h"
#include "ext_sram.h"

#define write_SCCB OV7725_WriteReg


void Cam_Init_ov7725_1(void)
{
  	GPIO_InitTypeDef GPIO_InitStructure;
  	DCMI_InitTypeDef DCMI_InitStructure;
    DCMI_CROPInitTypeDef DCMI_CROPInitStruct;
  	DMA_InitTypeDef  DMA_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;//NVIC

  	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_DCMI, ENABLE);//DCMI 
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);//DMA2
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | 
                           RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOE |
	                         RCC_AHB1Periph_GPIOH | RCC_AHB1Periph_GPIOG |
	                         RCC_AHB1Periph_GPIOI , ENABLE);//

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_DCMI);//DCMI_HSYNC 
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_DCMI);//DCMI_PIXCLK
    GPIO_PinAFConfig(GPIOI, GPIO_PinSource4, GPIO_AF_DCMI);//DCMI_D5
    GPIO_PinAFConfig(GPIOI, GPIO_PinSource5, GPIO_AF_DCMI);//DCMI_VSYNC 
    GPIO_PinAFConfig(GPIOI, GPIO_PinSource6, GPIO_AF_DCMI);//DCMI_D6 
    GPIO_PinAFConfig(GPIOI, GPIO_PinSource7, GPIO_AF_DCMI);//DCMI_D7 			  
    GPIO_PinAFConfig(GPIOH, GPIO_PinSource9, GPIO_AF_DCMI);//DCMI_D0 
    GPIO_PinAFConfig(GPIOH, GPIO_PinSource10, GPIO_AF_DCMI);//DCMI_D1 			  
    GPIO_PinAFConfig(GPIOH, GPIO_PinSource11, GPIO_AF_DCMI);//DCMI_D2
    GPIO_PinAFConfig(GPIOH, GPIO_PinSource12, GPIO_AF_DCMI);//DCMI_D3  
    GPIO_PinAFConfig(GPIOH, GPIO_PinSource14, GPIO_AF_DCMI);//DCMI_D4 

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ; 
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11| GPIO_Pin_12| GPIO_Pin_14; 
    GPIO_Init(GPIOH, &GPIO_InitStructure);   
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 ; 
    GPIO_Init(GPIOI, &GPIO_InitStructure);  
    
   
//========================================================== =========
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ; 
    GPIO_Init(GPIOB, &GPIO_InitStructure);//GPIOB 0/1
		
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 ;  //PG6->standby
    GPIO_Init(GPIOG, &GPIO_InitStructure);//	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 ;  // PH13->OE
    GPIO_Init(GPIOH, &GPIO_InitStructure);//
//http://blog.csdn.net/tumblerman/article/details/41449039

//Width=639,StartX=0,Height=119,StartY=0;
//320 * 120

//     DCMI_CROPInitStruct.DCMI_CaptureCount=Width;                         //capcnt
//     DCMI_CROPInitStruct.DCMI_HorizontalOffsetCount=StartX;           //hoffcnt
//     DCMI_CROPInitStruct.DCMI_VerticalLineCount=Height;                 //vline
//     DCMI_CROPInitStruct.DCMI_VerticalStartLine=StartY;                   //vst
//     DCMI_CROPConfig(&DCMI_CROPInitStruct);
//     DCMI_CROPCmd(ENABLE);            
    
    
    //160 * 120
    DCMI_CROPInitStruct.DCMI_HorizontalOffsetCount=0;           //hoffcnt
    DCMI_CROPInitStruct.DCMI_VerticalStartLine=119;                   //vst
    
    DCMI_CROPInitStruct.DCMI_VerticalLineCount=119;                 //vline
    DCMI_CROPInitStruct.DCMI_CaptureCount=639;                         //capcnt
    
    DCMI_CROPConfig(&DCMI_CROPInitStruct);
 //   DCMI_CROPCmd(ENABLE); 

  	DCMI_InitStructure.DCMI_CaptureMode = DCMI_CaptureMode_Continuous;
  	DCMI_InitStructure.DCMI_SynchroMode = DCMI_SynchroMode_Hardware;
  	DCMI_InitStructure.DCMI_PCKPolarity = DCMI_PCKPolarity_Rising ;
  	DCMI_InitStructure.DCMI_VSPolarity = DCMI_VSPolarity_High;
  	DCMI_InitStructure.DCMI_HSPolarity = DCMI_HSPolarity_Low;
  	DCMI_InitStructure.DCMI_CaptureRate = DCMI_CaptureRate_All_Frame;
  	DCMI_InitStructure.DCMI_ExtendedDataMode = DCMI_ExtendedDataMode_8b;
  	DCMI_Init(&DCMI_InitStructure); 
    DCMI_Cmd(ENABLE);
		
    /* DCMI Interrupts config ***************************************************/ 
 //    DCMI_ITConfig(DCMI_IT_VSYNC, ENABLE); 
 //    DCMI_ITConfig(DCMI_IT_LINE, ENABLE); 
    DCMI_ITConfig(DCMI_IT_FRAME, ENABLE);//
    //DCMI_ITConfig(DCMI_IT_ERR, ENABLE); 
//=======================================================		
     NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 
     
    NVIC_InitStructure.NVIC_IRQChannel = DCMI_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;   
    NVIC_Init(&NVIC_InitStructure);
    
    DMA_DeInit(DMA2_Stream1); 
    
  	DMA_InitStructure.DMA_Channel = DMA_Channel_1;  
  	DMA_InitStructure.DMA_PeripheralBaseAddr = DCMI_DR_ADDRESS;	
    DMA_InitStructure.DMA_Memory0BaseAddr = Bank1_SRAM3_ADDR;
  	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  	DMA_InitStructure.DMA_BufferSize = 0xffff;
  	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
  	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;  
  	
    
//     DMA_DoubleBufferModeCmd(DMA2_Stream1,ENABLE);//?????
//     DMA_MemoryTargetConfig(DMA2_Stream1,Bank1_SRAM3_ADDR+153600,DMA_Memory_1);//??????1
//    DMA_ITConfig(DMA2_Stream1,DMA_IT_TC,ENABLE);//????????   
    DMA_Init(DMA2_Stream1, &DMA_InitStructure);

//     
//     NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream1_IRQn;
//     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//?????0
//     NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//????0
//     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ????
//     NVIC_Init(&NVIC_InitStructure);	//??????????VIC????
        
}


unsigned char   OV7725_Init(void)
{
  unsigned char  i;
  Cam_Init_ov7725_1();
	//SENSOR_GPIO_Init();
  I2C_GPIO_Config();//SCCB_GPIO_Config();//sccb io init..      
  //SENSOR_CLK_init_ON();//PA8 MCO
  GPIOH->BSRRH = GPIO_Pin_13; //GPIOB 1                   
  GPIOG->BSRRH = GPIO_Pin_6; //GPIOB 0
  delay_ms(10); 

	//SCCB_Init();
	if(OV7725_WriteReg(0x12, 0x80)) //Reset SCCB
	{
          return 1 ;//
	}
	delay_ms(5);
  
  	for(i=0;i<OV_REG_NUM;i++)
  	{
          if(OV7725_WriteReg(OV7725_reg[i][0],OV7725_reg[i][1])){return 1;}
  	}
		SENSOR_CLK_init_OFF();
		
	return 0; 
}

				  
void OV7725_HW(unsigned int  hstart,unsigned int vstart,unsigned int hstop,unsigned int vstop)
{
	u8 v;		
	OV7725_WriteReg(0x17,(hstart>>3)&0xff);//HSTART
	OV7725_WriteReg(0x18,(hstop>>3)&0xff);//HSTOP
	OV7725_ReadReg(0x32,&v);
	v=(v&0xc0)|((hstop&0x7)<<3)|(hstart&0x7);
	OV7725_WriteReg(0x32,v);//HREF
	
	OV7725_WriteReg(0x19,(vstart>>2)&0xff);//VSTART 开始高8位
	OV7725_WriteReg(0x1a,(vstop>>2)&0xff);//VSTOP	结束高8位
	OV7725_ReadReg(0x03,&v);
	v=(v&0xf0)|((vstop&0x3)<<2)|(vstart&0x3);	
	OV7725_WriteReg(0x03,v);//VREF																 
	OV7725_WriteReg(0x11,0x00);
}								
/////////////////////////////////////
void SCCB_Init(void)
{
  	GPIO_InitTypeDef  GPIO_InitStructure;
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOB, ENABLE);

  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 ;  //SCCB_SIC:PE2
 	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;  //:PB5
 	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);

}

void SCCB_SID_OUT(void)//设置SCCB_SID为输出
{
  	GPIO_InitTypeDef  GPIO_InitStructure;

  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;               
 	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
void SCCB_SID_IN(void)//设置SCCB_SID为输入
{
  	GPIO_InitTypeDef  GPIO_InitStructure;

  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void SCCB_Start(void)
{
    SCCB_SID_H();     //数据线高电平
    delay_us(2);
    SCCB_SIC_H();	   //在时钟线高的时候数据线由高至低
    delay_us(2);
    SCCB_SID_L();
    delay_us(2);
    SCCB_SIC_L();	 //数据线恢复低电平，单操作函数必要
    delay_us(2);
}

void SCCB_Stop(void)
{
    SCCB_SID_L();
    delay_us(2);
    SCCB_SIC_H();	
    delay_us(2);  
    SCCB_SID_H();	
    delay_us(2);  
}

void noAck(void)
{	
	SCCB_SID_H();	
	delay_us(2);	
	SCCB_SIC_H();	
	delay_us(2);	
	SCCB_SIC_L();	
	delay_us(2);	
	SCCB_SID_L();	
	delay_us(2);
}

unsigned char  SCCB_Write(unsigned char  m_data)
{
	unsigned char  j,tem,K;
        K=m_data;
	for(j=0;j<8;j++) //循环8次发送数据
	{
		if((K&0x80)==0x80)
		{
			SCCB_SID_H();	
		}
		else
		{
			SCCB_SID_L();	
		}
		delay_us(4);
		SCCB_SIC_H();	
		delay_us(3);
                K<<=1;
		SCCB_SIC_L();	
		delay_us(1);
	}
	delay_us(2);
	SCCB_DATA_IN;
	delay_us(2);
	SCCB_SIC_H();	
	delay_us(2);
	if(SCCB_SID_STATE)tem=0;//SDA=1发送失败
	else tem=1;//SDA=0发送成功，返回1
	SCCB_SIC_L();	
	delay_us(2);	
  SCCB_DATA_OUT;
	return tem;  
}

unsigned char SCCB_Read(void)
{
	unsigned char read,j;
	read=0x00;
	
	SCCB_DATA_IN;
	delay_us(2);
	for(j=8;j>0;j--) //循环8次接收数据
	{		     
		delay_us(2);
		SCCB_SIC_H();
		delay_us(2);
		read=read<<1;
		if(SCCB_SID_STATE)read=read+1; 
		SCCB_SIC_L();
		delay_us(2);
	}	
        SCCB_DATA_OUT;
	return read;
}

unsigned char  OV7725_WriteReg(unsigned char  regID, unsigned char  regDat)
{
	I2C_Start();//SCCB_Start();//发送SCCB 总线开始传输命令
	I2C_SendByte(0x42);
	if(0==I2C_WaitAck())//写地址
	{
		I2C_Stop();//SCCB_Stop();//发送SCCB 总线停止传输命令
		return 1;//错误返回
	}
	delay_us(100);
	I2C_SendByte(regID);
  	if(0==I2C_WaitAck())//积存器ID
	{
		I2C_Stop();//SCCB_Stop();//发送SCCB 总线停止传输命令
		return 2;//错误返回
	}
	delay_us(100);
	I2C_SendByte(regDat);
  	if(0==I2C_WaitAck())//写数据到积存器
	{
		I2C_Stop();//SCCB_Stop();//发送SCCB 总线停止传输命令
		return 3;//错误返回
	}
  	I2C_Stop();//SCCB_Stop();//发送SCCB 总线停止传输命令	
  	return 0;//成功返回
}


unsigned char  OV7725_ReadReg(unsigned char  regID, unsigned char  *regDat)
{
	//通过写操作设置寄存器地址
	I2C_Start();//SCCB_Start();
	I2C_SendByte(0x42);
	if(0==I2C_WaitAck())//写地址
	{
		I2C_Stop();//SCCB_Stop();//发送SCCB 总线停止传输命令
		return 1;//错误返回
	}
	delay_us(100);
	I2C_SendByte(regID);
  	if(0==I2C_WaitAck())//积存器ID
	{
		I2C_Stop();//SCCB_Stop();//发送SCCB 总线停止传输命令
		return 2;//错误返回
	}
	I2C_Stop();//SCCB_Stop();//发送SCCB 总线停止传输命令	
	delay_us(100);	
	//设置寄存器地址后，才是读
	I2C_Start();//SCCB_Start();
	I2C_SendByte(0x43);
	if(0==I2C_WaitAck())//读地址
	{
		I2C_Stop();//SCCB_Stop();//发送SCCB 总线停止传输命令
		return 3;//错误返回
	}
	delay_us(100);
  	*regDat=I2C_ReadByte();//SCCB_Read();//返回读到的值
  	I2C_NoAck();//noAck();//发送NACK命令
  	I2C_Stop();//SCCB_Stop();//发送SCCB 总线停止传输命令
  	return 0;//成功返回
}



unsigned char  OV7725_ReadID(void)
{
	unsigned char  temp=0;
	OV7725_ReadReg(0x0b,&temp);
  return temp;
}

void set_15fps()  //pclk=12M
{
  OV7725_WriteReg(0x11, 0x03);
  OV7725_WriteReg(0x0d, 0x41);
  OV7725_WriteReg(0x2a, 0x00);
  OV7725_WriteReg(0x2b, 0x00);
  OV7725_WriteReg(0x33, 0x00);
  OV7725_WriteReg(0x34, 0x00);
  OV7725_WriteReg(0x2d, 0x00);
  OV7725_WriteReg(0x2e, 0x00);
  OV7725_WriteReg(0x0e, 0x65);
}

void set_25fps()//24M
{
	write_SCCB(0x11, 0x01);//
  write_SCCB(0x0d, 0x41);
  write_SCCB(0x2a, 0x00);
  write_SCCB(0x2b, 0x00);
  write_SCCB(0x33, 0x66);
  write_SCCB(0x34, 0x00);
  write_SCCB(0x2d, 0x00);
  write_SCCB(0x2e, 0x00);
  write_SCCB(0x0e, 0x65);
}

void set_30fps()//24M
{
	write_SCCB(0x11, 0x01);//
  write_SCCB(0x0d, 0x41);//
  write_SCCB(0x2a, 0x00);//
  write_SCCB(0x2b, 0x00);//
  write_SCCB(0x33, 0x00);//66
  write_SCCB(0x34, 0x00);//
  write_SCCB(0x2d, 0x00);//
  write_SCCB(0x2e, 0x00);//
  write_SCCB(0x0e, 0x65);//
}

void rmov_banding_50hz()  //for 25fps
{
	write_SCCB(0x13, 0xff); //banding filter enable
  write_SCCB(0x22, 0x98); //50Hz banding filter
  write_SCCB(0x23, 0x03); //4 step for 50hz
}

void rmov_banding_60hz()  //for 30fps
{
	write_SCCB(0x13, 0xff); //banding filter enable
  write_SCCB(0x22, 0x7f); //60Hz banding filter
  write_SCCB(0x23, 0x03); //4 step for 60hz
}
/*--------------------------White Balance----------------------*/
void set_wb_sunny()
{
	write_SCCB(0x13, 0xfd); //AWB off
  write_SCCB(0x01, 0x5a);
  write_SCCB(0x02, 0x5c);
  write_SCCB(0x0e, 0x65); 
  write_SCCB(0x2d, 0x00);
  write_SCCB(0x2e, 0x00);
}

void set_wb_cloudy()
{
  write_SCCB(0x13, 0xfd); //AWB off
  write_SCCB(0x01, 0x58);
  write_SCCB(0x02, 0x60);
  write_SCCB(0x0e, 0x65); 
  write_SCCB(0x2d, 0x00);
  write_SCCB(0x2e, 0x00);
}

void set_wb_office()
{
  write_SCCB(0x13, 0xfd); //AWB off
  write_SCCB(0x01, 0x84);
  write_SCCB(0x02, 0x4c);
  write_SCCB(0x0e, 0x65); 
  write_SCCB(0x2d, 0x00);
}

void set_wb_home()
{
	write_SCCB(0x13, 0xfd); //AWB off
  write_SCCB(0x01, 0x96);
  write_SCCB(0x02, 0x40);
  write_SCCB(0x0e, 0x65); 
  write_SCCB(0x2d, 0x00);
  write_SCCB(0x2e, 0x00);
}

void set_wb_night()
{
	write_SCCB(0x13, 0xff); //AWB on
  write_SCCB(0x0e, 0xe5); 
}
/*-----------------------Saturation-----------------------*/
void set_saturation(int n)
{
	write_SCCB(0xa7, 0x40+n*0x10);
  write_SCCB(0xa8, 0x40+n*0x10);
}
/*-----------------------Brightness-----------------------*/
void set_brightness_plus(int n)
{
	write_SCCB(0x9b, 0x08+n*0x10);
  write_SCCB(0xab, 0x06);
}

void set_brightness_minus(int n)
{
	write_SCCB(0x9b, 0x08+(-1*n)*0x10);
  write_SCCB(0xab, 0x0e);
}
/*---------------------Special Effects--------------------*/
void set_eff_normal()
{
	write_SCCB(0xa6, 0x06);
  write_SCCB(0x60, 0x80);
  write_SCCB(0x61, 0x80);
}

void set_eff_bw()
{
	write_SCCB(0xa6, 0x26);
  write_SCCB(0x60, 0x80);
  write_SCCB(0x61, 0x80);
}

void set_eff_bluish()
{
	write_SCCB(0xa6, 0x1e);
  write_SCCB(0x60, 0xa0);
  write_SCCB(0x61, 0x40);
}

void set_eff_sepia()
{
	write_SCCB(0xa6, 0x1e);
  write_SCCB(0x60, 0x40);
  write_SCCB(0x61, 0xa0);
}

void set_eff_redish()
{
	write_SCCB(0xa6, 0x1e);
  write_SCCB(0x60, 0x80);
  write_SCCB(0x61, 0xc0);
}

void set_eff_greenish()
{
	write_SCCB(0xa6, 0x1e);
  write_SCCB(0x60, 0x60);
  write_SCCB(0x61, 0x60);
}

void set_eff_negative()
{
	write_SCCB(0xa6, 0x46);

}


