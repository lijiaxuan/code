#include "can.h"

CAN_InitTypeDef CAN_InitStructure;
CAN_FilterInitTypeDef CAN_FilterInitStructure;
CanTxMsg TxMessage;
CanRxMsg RxMessage;
u32 flag=0xff;

void CAN_Config(void)
{
	CAN_GPIO_Config();
	CAN_NVIC_Config();
	CAN_Mode_Config();
	CAN_Filter_Config();
}

void CAN_Mode_Config(void)
{
	CAN_DeInit(CAN1);
	CAN_InitStructure.CAN_TTCM =DISABLE;           //MCR-TTCM 关闭时间触发通信模式使能
  CAN_InitStructure.CAN_ABOM =ENABLE;            //MCR-ABOM 自动离线管理
	CAN_InitStructure.CAN_AWUM =ENABLE;            //MCR-AWUM 自动唤醒模式
	CAN_InitStructure.CAN_NART =DISABLE;           //MCR-NART 禁止报文自动重传 DISABLE=自动重传
	CAN_InitStructure.CAN_RFLM =DISABLE;           //MCR-RFLM 接收 FIFO 锁定模式 DISABLE-溢出时新报文会覆盖原有报文
	CAN_InitStructure.CAN_TXFP =DISABLE;           //MCR-TXFP 发送 FIFO 优先级 DISABLE-优先级取决于报文标示符
	CAN_InitStructure.CAN_Mode =CAN_Mode_Normal;   //正常工作模式
	CAN_InitStructure.CAN_SJW =CAN_SJW_2tq;        //BTR-SJW 重新同步跳跃宽度 2 个时间单元
	CAN_InitStructure.CAN_BS1 =CAN_BS1_6tq;        //BTR-TS1 时间段 1 占用了 6 个时间单元
	CAN_InitStructure.CAN_BS2 =CAN_BS2_3tq;        //BTR-TS1 时间段 2 占用了 3 个时间单元
	CAN_InitStructure.CAN_Prescaler =4;            //BTR-BRP 波特率分频器 定义了时间单元的时间长度 36/(1+6+3)/4=0. 8Mbps
	
	CAN_Init(CAN1,&CAN_InitStructure);
}

void CAN_Filter_Config(void)
{
	CAN_FilterInitStructure.CAN_FilterNumber =0;   //过滤器组0
	CAN_FilterInitStructure.CAN_FilterMode =CAN_FilterMode_IdMask;//工作在标识符屏蔽位模式
	CAN_FilterInitStructure.CAN_FilterScale =CAN_FilterScale_32bit;//过滤器位宽为单个 32 位
	/* 使能报文标示符过滤器按照标示符的内容进行比对过滤，扩展 ID 不是如下的就
      抛弃掉，是的话，会存入 FIFO0。 */
	CAN_FilterInitStructure.CAN_FilterIdHigh =(((u32)0x1314<<3)&0xFFFF0000)>>16;
	/*要过滤的 ID 高位*/
	CAN_FilterInitStructure.CAN_FilterIdLow=((u32)0x1314<<3|CAN_ID_EXT|CAN_RTR_DATA)&0xFFFF;
	 /*要过滤的 ID 低位*/
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0xFFFF;
	 /*过滤器高 16 位每位必须匹配*/
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0xFFFF;
	/*过滤器低 16 位每位必须匹配*/
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//过滤器被关联到 FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation =ENABLE;//使能过滤器
	CAN_FilterInit(&CAN_FilterInitStructure);
	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);
}

void CAN_SetMsg(void)
{
	TxMessage.ExtId=0x1314;              //使用的扩展 ID
	TxMessage.IDE =CAN_ID_EXT;           //扩展模式
	TxMessage.RTR =CAN_RTR_DATA;         //发送的是数据
	TxMessage.DLC =1;                    //数据长度为 1 字节
	TxMessage.Data [0]=0xAB;
}

void USB_LP_CAN1_RX0_IRQHandler(void)
{
	CAN_Receive(CAN1,CAN_FIFO0,&RxMessage);
	if((RxMessage.ExtId ==0x1314)&&(RxMessage.IDE ==CAN_ID_EXT)&&(RxMessage.DLC ==1)&&((RxMessage.Data [0]<<8)==0xAB))
	{
		flag=0;
  }
	else
	{ 
		flag=0xff;
  }
}
