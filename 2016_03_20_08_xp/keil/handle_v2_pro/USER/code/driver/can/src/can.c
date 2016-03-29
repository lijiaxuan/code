/* 
* @Author: obsidian
* @Date:   2015-12-19 08:55:16
* @Last Modified by:   obsidian
* @Last Modified time: 2015-12-19 09:14:32
*/
#include "../inc/can.h"
#include "module/camera/inc/camera_global.h"

#define CAN1_BAUDRATE 1000


#define USE_CAN1_R0	1
#define USE_CAN1_R1	1

void Can_NVIC_Config(void)
{
	NVIC_InitTypeDef  NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
#if  USE_CAN1_R0
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
#endif
#if  USE_CAN1_R1
	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
#endif
}

void Can_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	/* Configure CAN1 IOs **********************************************/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
#if USE_CAN1_R0 || USE_CAN1_R1
	/* Configure CAN RX and TX pins */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	         //PA11:CAN1-RX 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			   //ÊäÈëÉÏÀ­
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;					 //PA12:CAN1-TX 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			 //žŽÓÃÄ£Êœ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
#endif
}

void CAN_CAN_Config(void)
{
	CAN_InitTypeDef        CAN_InitStructure;
	
	/* Enable CAN clock */
#if USE_CAN1_R0 || USE_CAN1_R1
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);
	CAN_DeInit(CAN1);
#endif

	/* CAN cell init */
	CAN_StructInit(&CAN_InitStructure);

	CAN_InitStructure.CAN_TTCM=DISABLE;//Ê§ÄÜÊ±ŒäŽ¥·¢ÍšÑ¶Ä£Êœ
	CAN_InitStructure.CAN_ABOM=DISABLE;//Ê§ÄÜ×Ô¶¯ÀëÏß¹ÜÀí
	CAN_InitStructure.CAN_AWUM=DISABLE;//Ê§ÄÜ×Ô¶¯»œÐÑÄ£Êœ
	CAN_InitStructure.CAN_NART=DISABLE;//Ê§ÄÜ·Ç×Ô¶¯ÖØŽ«ÊäÄ£Êœ,ŒŽ×Ô¶¯ÖØŽ«
	CAN_InitStructure.CAN_RFLM=DISABLE;//Ê§ÄÜœÓÊÕFIFOËø¶šÄ£Êœ,ŒŽFIFOÒç³öÊ±ÐÂÊýŸÝž²žÇ
	CAN_InitStructure.CAN_TXFP=ENABLE;//Ê§ÄÜ·¢ËÍFIFOÓÅÏÈŒ¶,±šÎÄË³ÐòÓÉ·¢ËÍË³ÐòŸö¶š DISABLE:IDŸö¶š
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;//Õý³£Ä£Êœ 

	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;		   //BTR-SJW ÖØÐÂÍ¬²œÌøÔŸ¿í¶È 1žöÊ±Œäµ¥Ôª
	CAN_InitStructure.CAN_BS1=CAN_BS1_2tq;		   //BTR-TS1 Ê±Œä¶Î1 ÕŒÓÃÁË2žöÊ±Œäµ¥Ôª
	CAN_InitStructure.CAN_BS2=CAN_BS2_3tq;		   //BTR-TS1 Ê±Œä¶Î2 ÕŒÓÃÁË3žöÊ±Œäµ¥Ôª
	
	//×ÜÏß²šÌØÂÊ,²šÌØÂÊŒÆËãÇë²ÎŒû F1ÊÖ²á¡°22.7.7 Bit timing¡±
	//ŽËŽŠ£¬APB1Ê±ÖÓÎª72M×ŽÌ¬£¬ËäÈ»ÊÖ²áÉÏËµ£¬APB1Ö»ÄÜ×îŽóÎª36M
#if CAN1_BAUDRATE == 1000 /* 1MBps */
  CAN_InitStructure.CAN_Prescaler =12;			   //BTR-BRP ²šÌØÂÊ·ÖÆµÆ÷  ¶šÒåÁËÊ±Œäµ¥ÔªµÄÊ±Œä³€¶È 72/(1+2+3)/12=1Mbps
#elif CAN1_BAUDRATE == 500 /* 500KBps */
  CAN_InitStructure.CAN_Prescaler =24;
#elif CAN1_BAUDRATE == 250 /* 250KBps */
  CAN_InitStructure.CAN_Prescaler =48;
#elif CAN1_BAUDRATE == 125 /* 125KBps */
  CAN_InitStructure.CAN_Prescaler =96;
#endif 

#if USE_CAN1_R0 || USE_CAN1_R1
	CAN_Init(CAN1, &CAN_InitStructure);	
#endif
}

// void Can_FILTER_Config(void)
// {
// 	uint8_t i=0;
// 	uint8_t can1_filter = 0;
// 	uint16_t id_temp;
	
// 	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	
// 	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdList;
// 	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_16bit;
// 	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
// 	for(i=0;i<Can_Data_Num-4;i++)
// 	{
// 		if(Can_Database[i].Data_type==WRITE_ONLY)
// 		{
// 			CAN_FilterInitStructure.CAN_FilterNumber = can1_filter;
// 			can1_filter++;
// 			id_temp = (uint16_t)(Can_Database[i].Data_ID)<<5;
// 			CAN_FilterInitStructure.CAN_FilterIdHigh = id_temp;
// 			CAN_FilterInitStructure.CAN_FilterIdLow = id_temp;
// 			CAN_FilterInitStructure.CAN_FilterMaskIdHigh = id_temp;
// 			CAN_FilterInitStructure.CAN_FilterMaskIdLow = id_temp;
// 			CAN_FilterInitStructure.CAN_FilterFIFOAssignment = Can_Database[i].Fifo_num;
			
// 			CAN_FilterInit(&CAN_FilterInitStructure);
// 		}
// 	}
//     i = Can_Data_Num-4;
//     CAN_FilterInitStructure.CAN_FilterNumber = can1_filter;
//     can1_filter=13;
//     id_temp = (uint16_t)(Can_Database[i].Data_ID)<<5;
//     CAN_FilterInitStructure.CAN_FilterIdHigh = id_temp;
    
//     i++;
//     id_temp = (uint16_t)(Can_Database[i].Data_ID)<<5;
//     CAN_FilterInitStructure.CAN_FilterIdLow = id_temp;
//     i++;
//     id_temp = (uint16_t)(Can_Database[i].Data_ID)<<5;
//     CAN_FilterInitStructure.CAN_FilterMaskIdHigh = id_temp;
//     i++;
//     id_temp = (uint16_t)(Can_Database[i].Data_ID)<<5;
//     CAN_FilterInitStructure.CAN_FilterMaskIdLow = id_temp;
//     CAN_FilterInitStructure.CAN_FilterFIFOAssignment = Can_Database[i].Fifo_num;
    
//     CAN_FilterInit(&CAN_FilterInitStructure);
// }

void Can_IT_Config(void)
{
#if  USE_CAN1_R0
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
#endif
#if  USE_CAN1_R1
	CAN_ITConfig(CAN1, CAN_IT_FMP1, ENABLE);
#endif
}

void Can1_Init(void)
{
	Can_NVIC_Config();
	Can_GPIO_Config();
	CAN_CAN_Config();
	//Can_FILTER_Config();
	Can_IT_Config();
}
void USB_LP_CAN1_RX0_IRQHandler(void)
{
	uint8_t i;
	CanRxMsg   RxMessage;
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
	camera.message.angle = 0;
}
