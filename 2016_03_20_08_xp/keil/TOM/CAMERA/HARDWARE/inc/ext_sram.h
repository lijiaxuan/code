#ifndef __EXT_SRAM_H
#define __EXT_SRAM_H

#include "stm32f4xx.h"

#define Bank1_SRAM3_ADDR  ((u32)(0x68000000))

extern void ext_sram_init(void);
extern void FSMC_SRAM_WriteBuffer8b(u8* pBuffer,u32 WriteAddr,u32 n);
extern void FSMC_SRAM_ReadBuffer8b(u8* pBuffer,u32 ReadAddr,u32 n);
extern void FSMC_SRAM_WriteBuffer16b(u16* pBuffer,u32 WriteAddr,u32 n);
extern void FSMC_SRAM_ReadBuffer16b(u16* pBuffer,u32 ReadAddr,u32 n);

#endif
