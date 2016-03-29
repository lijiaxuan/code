/**
  ******************************************************************************
  * @file    FSMC/SRAM/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    30-September-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "MT9M111.h"
#include "can_database.h"
#include "vision.h"
char state = 0;
/** @addtogroup STM32F4xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup FSMC_SRAM
  * @{
  */ 
  
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
                                                                                                             /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
//SysTick handler
//extern u16 ntime;
u32 gtimer=0;
void SysTick_Handler(void)
{
	//ntime--;
	gtimer++;
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

u16 vsync=0;
u32 frame=0;
void DCMI_IRQHandler(void)
{
	//Cam_Stop();
	//GPIOG->BSRRL = GPIO_Pin_6;
	  DMA_Cmd(DMA2_Stream1, DISABLE);
	//DCMI_CaptureCmd(DISABLE);
	DCMI_ClearITPendingBit(DCMI_IT_FRAME);
// 	state = 1 - state;
// // 		;
	DMA_DeInit(DMA2_Stream1);
    DMA_InitStructure.DMA_Memory0BaseAddr = (unsigned int)0x68000000;
	//DMA_ClearFlag(DMA2_FLAG_TC1); //????????
DMA_SetCurrDataCounter(DMA2_Stream1,0);//???????
    DMA_Init(DMA2_Stream1, &DMA_InitStructure);
    DMA_Cmd(DMA2_Stream1, ENABLE);
	//DCMI_CaptureCmd(ENABLE);
	//GPIOG->BSRRH = GPIO_Pin_6;
    vsync=1;
    frame++;		
}

// void USART1_IRQHandler(void);


/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/


/*can总线接收时判断有点多，望大家见谅
由于can总线挂载设备较多，每块板子都有各自的程序，所有板子都不出
问题的概率较小。Can_Database数据库是外设与主控通信的公共信息，外
设与主控的数据库必须完全统一。否则，就会造成通信故障。在接收时，
进行判断可以快速发现两板之间的信息不统一问题。
*/
void CAN1_RX0_IRQHandler(void)
{
	uint8_t i;
	CanRxMsg   RxMessage;
	
	/** @led add by fxh_bool 3-25 20:00 */
// 	LED_Toggle(3);
	
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
	for(i=0;i<RxMessage.DLC;i++)
		(*(Can_Database[HASH_TABLE[RxMessage.StdId]].Data_ptr+i)) = RxMessage.Data[i];
	Can_Database[HASH_TABLE[RxMessage.StdId]].MenuFunc();

}
void CAN1_RX1_IRQHandler(void)
{
	uint8_t i;
	CanRxMsg   RxMessage;
	
	/** @led add by fxh_bool 3-25 20:00 */
// 	LED_Toggle(3);
	
	CAN_Receive(CAN1, CAN_FIFO1, &RxMessage);
	for(i=0;i<RxMessage.DLC;i++)
		(*(Can_Database[HASH_TABLE[RxMessage.StdId]].Data_ptr+i)) = RxMessage.Data[i];
	Can_Database[HASH_TABLE[RxMessage.StdId]].MenuFunc();

}


/**
  * @}
  */

/**
  * @}
  */


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
