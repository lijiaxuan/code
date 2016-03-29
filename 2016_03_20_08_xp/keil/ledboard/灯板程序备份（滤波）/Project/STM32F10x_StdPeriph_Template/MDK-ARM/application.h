#ifndef _APPLICATION_H
#define _APPLICATION_H

#include "stm32f10x.h"
#include "platform_config.h"
#include"all.h"

//caiyang
extern uint16_t DRBuffer0[8][24];
extern uint16_t DRBuffer1[8][24];
extern uint16_t DRBuffer2[8][24];
extern uint16_t DRBuffer3[8][24];
extern uint32_t MAXbuffer[8][4];
extern uint32_t JUNbuffer[8][4];
extern uint32_t  LBdis;
extern uint32_t  LBdata;
extern uint16_t yuzhitemp1[32];
extern uint16_t yuzhitemp2[32];
extern uint16_t junzhitemp[32];
extern uint16_t temp0;
extern uint32_t temp1;
extern uint16_t yuzhiH[32];
extern uint16_t yuzhiL[32];
extern u8 caiflag;

extern void LBcai(void);
extern void fengzhih(void);
extern void fengzhil(void);
extern void chizhi(void);
extern void junzhi(void);
extern void chizhired(void);
extern void chizhiblue(void);

#endif
