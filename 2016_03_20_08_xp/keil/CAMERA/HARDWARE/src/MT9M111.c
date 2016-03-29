#include "MT9M111.h"
#include "misc.h"
#include "ext_sram.h"


u16 dcmi_val;
DMA_InitTypeDef  DMA_InitStructure = {0};
//u16 testsram[76800] __attribute__((at(0X68000000)));//测试用数组

const  int change[CHANGE_REG_NUM][2]=
{	
  
	{ 0xf0,0x0000 }, //Sensor address page 0  
	{ 0x0d,0x0009 },   
	{ 0x0d,0x0029 },   
	{ 0x0d,0x0008 },

	{0,10 }, //
	{0x9d,0x3ca0 }, //

	{ 0xf0,0x0001 }, //Sensor address page 1 
	{ 0x9d,0x3ca0 },   
	{ 0xf0,0x0002 }, //Sensor address page 2  
	{ 0x24,0x5f20 },   
	{ 0x28,0xef02 },   
	{ 0x5f,0x4143 },   
	{ 0x02,0x00ee },   
	{ 0x03,0x291a },   
	{ 0x04,0x02a4 },   //0x2a4
	{ 0x09,0x0068 },   
	{ 0x0a,0x002a },   
	{ 0x0b,0x0004 },   
	{ 0x0c,0x0093 },   
	{ 0x0d,0x0082 },   
	{ 0x0e,0x0040 },   
	{ 0x0f,0x005f },   
	{ 0x10,0x004e },   
	{ 0x11,0x005b },   
	{ 0x15,0x00c9 },   
	{ 0x16,0x005e },   
	{ 0x17,0x009d },   
	{ 0x18,0x0006 },   
	{ 0x19,0x0089 },   
	{ 0x1a,0x0012 },   
	{ 0x1b,0x00a1 },   
	{ 0x1c,0x00e4 },   
	{ 0x1d,0x007a },   
	{ 0x1e,0x0064 },   
	{ 0xf6,0x005f },   
	{ 0xf0,0x0001 },   
	{ 0x81,0x0000 },   
	{ 0x82,0x0000 },   
	{ 0x83,0x0000 },   
	{ 0x84,0x0000 },   
	{ 0x85,0x0000 },   
	{ 0x86,0x0000 },   
	{ 0x87,0x0000 },   
	{ 0x88,0x0000 },   
	{ 0x89,0x0000 },   
	{ 0x8a,0x0000 },   
	{ 0x8b,0x0000 },   
	{ 0x8c,0x0000 },   
	{ 0x8d,0x0000 },   
	{ 0x8e,0x0000 },   
	{ 0x8f,0x0000 },   
	{ 0x90,0x0000 },   
	{ 0x91,0x0000 },   
	{ 0x92,0x0000 },   
	{ 0x93,0x0000 },   
	{ 0x94,0x0000 },   
	{ 0x95,0x0000 },   
	{ 0xb6,0x0000 },   
	{ 0xb7,0x0000 },   
	{ 0xb8,0x0000 },   
	{ 0xb9,0x0000 },   
	{ 0xba,0x0000 },   
	{ 0xbb,0x0000 },   
	{ 0xbc,0x0000 },   
	{ 0xbd,0x0000 },   
	{ 0xbe,0x0000 },   
	{ 0xbf,0x0000 },   
	{ 0xc0,0x0000 },   
	{ 0xc1,0x0000 },   
	{ 0xc2,0x0000 },   
	{ 0xc3,0x0000 },   
	{ 0xc4,0x0000 },   
  //0xffff,0x0006, 
	{ 0xf0,0x0001 },  //Sensor address page 1
  { 0x3a , 0x0102 },//RG565
  { 0x9b , 0x0102 },
        
	{ 0x53,0x1c12 },   
	{ 0x54,0x402a },   
	{ 0x55,0x7c62 },   
	{ 0x56,0xa994 },   
	{ 0x57,0xcfbc },   
	{ 0x58,0xe000 },   
	{ 0xdc,0x1c12 },   
	{ 0xdd,0x402a },   
	{ 0xde,0x7c62 },   
	{ 0xdf,0xa994 },   
	{ 0xe0,0xcfbc },   
	{ 0xe1,0xe000 },   
	{ 0x34,0x0010 },   
	{ 0x35,0xf010 },   
	{ 0xf0,0x0000 },  //Sensor address page 0 
//  { 0x03 , 480+400 },//240+400
//	{ 0x04 , 814+400 },//320+400
//  { 0x01 , 12+400 },
//  { 0x02 , 28+400 },
	{ 0x05,0x00f1 },   
	{ 0x06,0x000d },   
	{ 0x07,0x00d9 },   
	{ 0x08,0x000d },   
	{ 0x20,0x0100 },   
	{ 0x21,0x0400 },   
	{ 0x22,0x0d2b },   
	{ 0x24,0x8000 },   
	{ 0x59,0x0018 },   
	//{ 0xffff,0x0003, 
  { 0x33 , 0x0400 },
  { 0x34 , 0xc019 }, 
  { 0x3f , 0x2020 },
  { 0x40 , 0x2020 },      
  { 0x5a , 0xc00a },
  { 0x70 , 0x7b0a },
  { 0x71 , 0xff00 },
  { 0x72 , 0x190e },
  { 0x73 , 0x180f },
  { 0x74 , 0x5732 },
  { 0x75 , 0x5634 },
  { 0x76 , 0x7335 },
  { 0x77 , 0x3012 },
        { 0x78 , 0x7902 },
        { 0x79 , 0x7506 },
        { 0x7a , 0x770a },
        { 0x7b , 0x7809 },
        { 0x7c , 0x7d06 },
        { 0x7d , 0x3110 },
        { 0x7e , 0x007e },
        { 0x80 , 0x5904 },
        { 0x81 , 0x5904 },
        { 0x82 , 0x570a },
        { 0x83 , 0x580b },
        { 0x84 , 0x470c },
        { 0x85 , 0x480e },
        { 0x86 , 0x5b02 },
        { 0x87 , 0x005c },
	{ 0xf0,0x0002 }, //Sensor address page 2   
	{ 0x39,0x06c2 },   
	{ 0x3a,0x05f9 },   
	{ 0x3b,0x0410 },   
	{ 0x3c,0x049d },   
	{ 0x57,0x0104 },   
	{ 0x58,0x0138 },   
	{ 0x59,0x0126 },   
	{ 0x5a,0x0161 },   
	{ 0x5c,0x120d }, 
  
	{ 0x9b,0x0300 },  // rgb//
	{ 0x3a,0x0300 }, //
	{ 0xf0,0x0001 },   //Sensor address page 1
        
        { 159, 0  },
        { 160, 1280},  // 
        { 161, imgwidth},//   
        { 162, 0  },    
        { 163, 960}, //    
        { 164, imgheight},  
        { 165, 0   },
        
        { 166, 1280},//
        { 167, imgwidth},//
        { 168, 0   },
        { 169, 960}, //
        { 170, imgheight},
	//{ 0xffff,0x0003, 
	{ 0xf0,0x0002 },//Sensor address page 2 
	{ 0xd2,0x007f },   
	{ 0x5b,0x0000 },   
	{ 0xcc,0x0004 },   
	{ 0xcb,0x0001 },   
	{ 0xf0,0x0000 },//Sensor address page 0

};
      
int id,succ,fail,CMD=0;
void SENSOR_GPIO_Init(void)
{
  	GPIO_InitTypeDef GPIO_InitStructure;//GPIO
  	DCMI_InitTypeDef DCMI_InitStructure;//DCMI
  	//DMA
	  NVIC_InitTypeDef NVIC_InitStructure;//NVIC

  	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_DCMI, ENABLE);//DCMI
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);//DMA2
	
	
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB |
                           RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOE |
	                         RCC_AHB1Periph_GPIOG | RCC_AHB1Periph_GPIOH |
	                         RCC_AHB1Periph_GPIOI , ENABLE);

//===================DCMI IO====================================
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
//==================================================================
//=====================
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
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;  //PB0->standby; PB1->OE
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ; 
    GPIO_Init(GPIOB, &GPIO_InitStructure);//GPIOB 0/1 
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 ;  //PG6->standby
    GPIO_Init(GPIOG, &GPIO_InitStructure);//	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 ;  // PH13->OE
    GPIO_Init(GPIOH, &GPIO_InitStructure);//
//===================================================================
	

//===================================================================
  	DCMI_InitStructure.DCMI_CaptureMode = DCMI_CaptureMode_Continuous;
  	DCMI_InitStructure.DCMI_SynchroMode = DCMI_SynchroMode_Hardware;
  	DCMI_InitStructure.DCMI_PCKPolarity = DCMI_PCKPolarity_Falling;//
  	DCMI_InitStructure.DCMI_VSPolarity = DCMI_VSPolarity_Low;//
  	DCMI_InitStructure.DCMI_HSPolarity = DCMI_HSPolarity_Low;//
  	DCMI_InitStructure.DCMI_CaptureRate = DCMI_CaptureRate_All_Frame;//
  	DCMI_InitStructure.DCMI_ExtendedDataMode = DCMI_ExtendedDataMode_8b;//
  	DCMI_Init(&DCMI_InitStructure);//DCMI 
//=====================================================================

/* DCMI Interrupts config ***************************************************/ 
    //DCMI_ITConfig(DCMI_IT_VSYNC, ENABLE); 
    //DCMI_ITConfig(DCMI_IT_LINE, ENABLE); 
    DCMI_ITConfig(DCMI_IT_FRAME, ENABLE);//
    //DCMI_ITConfig(DCMI_IT_ERR, ENABLE); 
//=======================================================		
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 
    NVIC_InitStructure.NVIC_IRQChannel = DCMI_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;   
    NVIC_Init(&NVIC_InitStructure);
//=====================================================================	
  	DMA_DeInit(DMA2_Stream1);
  	DMA_InitStructure.DMA_Channel = DMA_Channel_1;  
  	DMA_InitStructure.DMA_PeripheralBaseAddr = DCMI_DR_ADDRESS;	
  	DMA_InitStructure.DMA_Memory0BaseAddr = Bank1_SRAM3_ADDR;
  	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  	DMA_InitStructure.DMA_BufferSize = 320 * 120;
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
  	DMA_Init(DMA2_Stream1, &DMA_InitStructure);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
unsigned char I2C_GPIO_Config(void)
{
  	GPIO_InitTypeDef  GPIO_InitStructure;
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOB, ENABLE);

  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 ;  //SCCB_SCL:PG8
 	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  	GPIO_Init(GPIOG, &GPIO_InitStructure);
	  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 ;  //SCCB_SDA:PG7
 	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  	GPIO_Init(GPIOG, &GPIO_InitStructure);

	return(0x01);
}


void I2C_delay(void)
{
		
   u16 i=200; 	 //30
   while(i) 
   { 
     i--; 
   }  
}

void delay5ms(void)
{
		
   int i=10000;  
   while(i) 
   { 
     i--; 
   }  
}

void I2C_Stop(void)
{
	SCL_L;
	I2C_delay();
	SDA_L;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SDA_H;
	I2C_delay();
} 


int I2C_Start(void)
{
	SDA_H;
	SCL_H;
	I2C_delay();
	if(!SDA_read)return (0);	
	SDA_L;
	I2C_delay();
	if(SDA_read) return (0);	
	SDA_L;
	I2C_delay();
	return (1);
}


void I2C_Ack(void)
{	
	SCL_L;
	I2C_delay();
	SDA_L;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SCL_L;
	I2C_delay();
}   


void I2C_NoAck(void)
{	
	SCL_L;
	I2C_delay();
	SDA_H;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SCL_L;
	I2C_delay();
} 

int I2C_WaitAck(void) 	
{
	SCL_L;
	I2C_delay();
	SDA_H;			
	I2C_delay();
	SCL_H;
	I2C_delay();
	if(SDA_read)
	{
      SCL_L;
	  I2C_delay();
      return (0);
	}
	SCL_L;
	I2C_delay();
	return (1);
}


void I2C_SendByte(unsigned char SendByte) 
{
    u8 i=8;
    while(i--)
    {
        SCL_L;
        I2C_delay();
      if(SendByte&0x80)
        SDA_H;  
      else 
        SDA_L;   
        SendByte<<=1;
        I2C_delay();
		SCL_H;
        I2C_delay();
    }
    SCL_L;
}  


unsigned char I2C_ReadByte(void)  
{
    u8 i=8;
    u8 ReceiveByte=0;

    SDA_H;				
    while(i--)
    {
      ReceiveByte<<=1;      
      SCL_L;
      I2C_delay();
	  SCL_H;
      I2C_delay();	
      if(SDA_read)
      {
        ReceiveByte|=0x01;
      }
    }
    SCL_L;
    return ReceiveByte;
} 
/////////////////////////////////////////////////////////////////////////////////////////

unsigned char WR_SENSOR_Reg(unsigned char regID, unsigned int regDat)
{
	I2C_Start();//startSCCB();
        I2C_SendByte(0xba);  
	if(0==I2C_WaitAck())
	{
		I2C_Stop();//stopSCCB();
		return(0);
	}
	delay_us(100);
        
  	I2C_SendByte(regID);  
	if(0==I2C_WaitAck())//if(0==SCCBwriteByte(regID))
	{
		I2C_Stop();//stopSCCB();
		return(0);
	}
	delay_us(100);
  	I2C_SendByte((unsigned char)(regDat>>8));  
	if(0==I2C_WaitAck())//if(0==SCCBwriteByte((unsigned char )(regDat>>8)))
	{
		I2C_Stop();//stopSCCB();
		return(0);
	}
        delay_us(100);
  	I2C_SendByte((unsigned char)regDat);  
	if(0==I2C_WaitAck())//if(0==SCCBwriteByte((unsigned char )(regDat)))
	{
		I2C_Stop();//stopSCCB();
		return(0);
	}
  	I2C_Stop();//stopSCCB();
	
  	return(1);
}
////////////////////////////
unsigned char RD_SENSOR_Reg(unsigned char regID, unsigned int *regDat)
{
	unsigned int val;
       
	I2C_Start();//startSCCB();
  I2C_SendByte(0xba);
	if(0==I2C_WaitAck())//0xba
	{
		I2C_Stop();//stopSCCB();
		return(0);
	}
	delay_us(100);
        I2C_SendByte(regID);
  	if(0==I2C_WaitAck())
	{
		I2C_Stop();//stopSCCB();
		return(0);
	}
  I2C_Stop();//stopSCCB();
	delay_us(100);
	I2C_Start();//startSCCB();
  I2C_SendByte(0xbb);
	if(0==I2C_WaitAck())
	{
		I2C_Stop();//stopSCCB();
		return(0);
	}
	delay_us(100);
        val=I2C_ReadByte();//SCCBreadByte();
        val=val<<8;
        delay_us(100);
        I2C_Ack();//Ack();
        delay_us(100);
        *regDat=val|(I2C_ReadByte());
        delay_us(10);
  	I2C_NoAck();//noAck();
        delay_us(10);
  	I2C_Stop();//stopSCCB();
  	return(1);
}

void SENSOR_REG_Write(unsigned char Reg_Page,unsigned char Reg_ID,unsigned int data)
{ 
  WR_SENSOR_Reg(0xf0, Reg_Page);
  WR_SENSOR_Reg(Reg_ID,data); 
}

void SENSOR_CLK_init_ON(void)
{
  	GPIO_InitTypeDef GPIO_InitStructure;
	  GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_MCO);//MCO1:PA8
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
  	GPIO_Init(GPIOA, &GPIO_InitStructure);	     
    RCC_MCO1Config(RCC_MCO1Source_PLLCLK , RCC_MCO1Div_5); //168Mhz/5
}
void SENSOR_CLK_init_OFF(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ; 
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
}


unsigned char SENSOR_init(void)
{
	unsigned int i=0,val;
        SENSOR_GPIO_Init();
        I2C_GPIO_Config();//SCCB_GPIO_Config();//sccb io init..      
        SENSOR_CLK_init_ON();//PA8 MCO
        OE_L(); //GPIOB 1                   
        STANDBY_L(); //GPIOB 0
        delay_ms(10); 
        i=RD_SENSOR_Reg(0,&val);//
      if(RD_SENSOR_Reg(0,&val)==0){I2C_Stop();return(0);}//SENSOR ID
        id=val;
	      if(val!=5178){I2C_Stop();fail++;return(0);}//SENSOR ID 5178
        succ++;  
        
        for(i=0;i<CHANGE_REG_NUM;i++)
				{
						if( 0==WR_SENSOR_Reg(change[i][0],change[i][1]))
						{
							return 0;
						}
						CMD++;
				}
        SENSOR_CLK_init_OFF();
				
	return 0x01; //ok
}


void Cam_Start(void)
{
	
  DMA_Cmd(DMA2_Stream1, ENABLE); 
// 	DCMI_Cmd(ENABLE);
	DCMI_CaptureCmd(ENABLE);
	GPIOG->BSRRH = GPIO_Pin_6;
}
void Cam_Stop(void)
{
	GPIOG->BSRRL = GPIO_Pin_6;
	DMA_Cmd(DMA2_Stream1, DISABLE);
//	DCMI_Cmd(DISABLE);
	DCMI_CaptureCmd(DISABLE);
}

