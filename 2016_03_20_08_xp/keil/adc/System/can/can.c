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
	CAN_InitStructure.CAN_TTCM =DISABLE;           //MCR-TTCM �ر�ʱ�䴥��ͨ��ģʽʹ��
  CAN_InitStructure.CAN_ABOM =ENABLE;            //MCR-ABOM �Զ����߹���
	CAN_InitStructure.CAN_AWUM =ENABLE;            //MCR-AWUM �Զ�����ģʽ
	CAN_InitStructure.CAN_NART =DISABLE;           //MCR-NART ��ֹ�����Զ��ش� DISABLE=�Զ��ش�
	CAN_InitStructure.CAN_RFLM =DISABLE;           //MCR-RFLM ���� FIFO ����ģʽ DISABLE-���ʱ�±��ĻḲ��ԭ�б���
	CAN_InitStructure.CAN_TXFP =DISABLE;           //MCR-TXFP ���� FIFO ���ȼ� DISABLE-���ȼ�ȡ���ڱ��ı�ʾ��
	CAN_InitStructure.CAN_Mode =CAN_Mode_Normal;   //��������ģʽ
	CAN_InitStructure.CAN_SJW =CAN_SJW_2tq;        //BTR-SJW ����ͬ����Ծ��� 2 ��ʱ�䵥Ԫ
	CAN_InitStructure.CAN_BS1 =CAN_BS1_6tq;        //BTR-TS1 ʱ��� 1 ռ���� 6 ��ʱ�䵥Ԫ
	CAN_InitStructure.CAN_BS2 =CAN_BS2_3tq;        //BTR-TS1 ʱ��� 2 ռ���� 3 ��ʱ�䵥Ԫ
	CAN_InitStructure.CAN_Prescaler =4;            //BTR-BRP �����ʷ�Ƶ�� ������ʱ�䵥Ԫ��ʱ�䳤�� 36/(1+6+3)/4=0. 8Mbps
	
	CAN_Init(CAN1,&CAN_InitStructure);
}

void CAN_Filter_Config(void)
{
	CAN_FilterInitStructure.CAN_FilterNumber =0;   //��������0
	CAN_FilterInitStructure.CAN_FilterMode =CAN_FilterMode_IdMask;//�����ڱ�ʶ������λģʽ
	CAN_FilterInitStructure.CAN_FilterScale =CAN_FilterScale_32bit;//������λ��Ϊ���� 32 λ
	/* ʹ�ܱ��ı�ʾ�����������ձ�ʾ�������ݽ��бȶԹ��ˣ���չ ID �������µľ�
      ���������ǵĻ�������� FIFO0�� */
	CAN_FilterInitStructure.CAN_FilterIdHigh =(((u32)0x1314<<3)&0xFFFF0000)>>16;
	/*Ҫ���˵� ID ��λ*/
	CAN_FilterInitStructure.CAN_FilterIdLow=((u32)0x1314<<3|CAN_ID_EXT|CAN_RTR_DATA)&0xFFFF;
	 /*Ҫ���˵� ID ��λ*/
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0xFFFF;
	 /*�������� 16 λÿλ����ƥ��*/
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0xFFFF;
	/*�������� 16 λÿλ����ƥ��*/
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//�������������� FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation =ENABLE;//ʹ�ܹ�����
	CAN_FilterInit(&CAN_FilterInitStructure);
	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);
}

void CAN_SetMsg(void)
{
	TxMessage.ExtId=0x1314;              //ʹ�õ���չ ID
	TxMessage.IDE =CAN_ID_EXT;           //��չģʽ
	TxMessage.RTR =CAN_RTR_DATA;         //���͵�������
	TxMessage.DLC =1;                    //���ݳ���Ϊ 1 �ֽ�
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
