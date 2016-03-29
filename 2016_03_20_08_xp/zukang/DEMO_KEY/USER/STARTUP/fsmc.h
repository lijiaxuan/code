#ifndef _FSMC_H_
#define _FSMC_H_

void FPGA_FSMC_Init(void);

void FPGA_Write(int addr,void * data,int size);

#endif
