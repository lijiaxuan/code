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
#include "ext_sram.h"

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
u32 dcmi=0;
u8 flag=0;
void DCMI_IRQHandler(void)
{
      	DMA_InitTypeDef  DMA_InitStructure;
    DCMI_ClearITPendingBit(DCMI_IT_FRAME);
    //DCMI_ClearITPendingBit(DCMI_IT_VSYNC);
    //DCMI_ClearITPendingBit(DCMI_IT_LINE);
    
    dcmi++;	
//    Cam_Stop();
    
//         DMA_DeInit(DMA2_Stream1); 
//     
//   	DMA_InitStructure.DMA_Channel = DMA_Channel_1;  
//   	DMA_InitStructure.DMA_PeripheralBaseAddr = DCMI_DR_ADDRESS;	
//     DMA_InitStructure.DMA_Memory0BaseAddr = Bank1_SRAM3_ADDR;
//   	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
//   	DMA_InitStructure.DMA_BufferSize = 0xffff;
//   	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//   	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
//   	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
//   	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
//   	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
//   	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
//   	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
//   	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
//   	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
//   	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;  
//   	
    
//     DMA_DoubleBufferModeCmd(DMA2_Stream1,ENABLE);//?????
//     DMA_MemoryTargetConfig(DMA2_Stream1,Bank1_SRAM3_ADDR+153600,DMA_Memory_1);//??????1
//    DMA_ITConfig(DMA2_Stream1,DMA_IT_TC,ENABLE);//????????   
//    DMA_Init(DMA2_Stream1, &DMA_InitStructure);
//DMA_Cmd(DMA2_Stream1, DISABLE);
    DMA2_Stream1->CR &= ~(uint32_t)DMA_SxCR_EN;
        DMA_DeInit(DMA2_Stream1); 
    
  	DMA_InitStructure.DMA_Channel = DMA_Channel_1;  
  	DMA_InitStructure.DMA_PeripheralBaseAddr = DCMI_DR_ADDRESS;	
    DMA_InitStructure.DMA_Memory0BaseAddr = Bank1_SRAM3_ADDR+(dcmi%10)*(0x9600)*4;
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
    DMA_Init(DMA2_Stream1, &DMA_InitStructure);
    
    DMA2_Stream1->M0AR = Bank1_SRAM3_ADDR+(dcmi%10)*0x9600*4;
// 	DMA_SetCurrDataCounter(DMA2_Stream1,0x9600);//DMA???DMA?????
    DMA2_Stream1->CR |= (uint32_t)DMA_SxCR_EN;
//DMA_Cmd(DMA2_Stream1, ENABLE);
//        DCMI_CROPCmd(ENABLE); 
//    Cam_Start();
    
}


void DMA2_Stream1_IRQHandler(void)
{
    DMA_ClearITPendingBit(DMA2_Stream1, DMA_IT_TCIF1); 
    frame++;	
//     Cam_Stop();
//     Cam_Start();
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
