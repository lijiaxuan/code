#ifndef FILE_H

#include "ff.h"
#include "MyFatFs.h"
#include "stm32f10x.h"
#define SYSTEM_SUPPORT_UCOS     0       //¶¨ÒåÏµÍ³ÎÄ¼þ¼ÐÊÇ·ñÖ§³ÖUCOS
                                                                        
     
//Î»´ø²Ù×÷,ÊµÏÖ51ÀàËÆµÄGPIO¿ØÖÆ¹¦ÄÜ
//¾ßÌåÊµÏÖË¼Ïë,²Î¿¼<<CM3È¨ÍþÖ¸ÄÏ>>µÚÎåÕÂ(87Ò³~92Ò³).
//IO¿Ú²Ù×÷ºê¶¨Òå
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO¿ÚµØÖ·Ó³Éä
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
 
//IO¿Ú²Ù×÷,Ö»¶Ôµ¥Ò»µÄIO¿Ú!
//È·±£nµÄÖµÐ¡ÓÚ16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //Êä³ö 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //ÊäÈë 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //Êä³ö 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //ÊäÈë 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //Êä³ö 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //ÊäÈë 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //Êä³ö 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //ÊäÈë 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //Êä³ö 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //ÊäÈë

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //Êä³ö 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //ÊäÈë

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //Êä³ö 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //ÊäÈë



#define SD_TYPE_ERR     0X00
#define SD_TYPE_MMC     0X01
#define SD_TYPE_V1      0X02
#define SD_TYPE_V2      0X04
#define SD_TYPE_V2HC    0X06       

#define CMD0    0      
#define CMD1    1
#define CMD8    8      
#define CMD9    9      
#define CMD10   10     
#define CMD12   12     
#define CMD16   16   
#define CMD17   17     
#define CMD18   18     
#define CMD23   23    
#define CMD24   24     
#define CMD25   25     
#define CMD41   41     
#define CMD55   55     
#define CMD58   58     
#define CMD59   59

#define MSD_DATA_OK                0x05
#define MSD_DATA_CRC_ERROR         0x0B
#define MSD_DATA_WRITE_ERROR       0x0D
#define MSD_DATA_OTHER_ERROR       0xFF

#define MSD_RESPONSE_NO_ERROR      0x00
#define MSD_IN_IDLE_STATE          0x01
#define MSD_ERASE_RESET            0x02
#define MSD_ILLEGAL_COMMAND        0x04
#define MSD_COM_CRC_ERROR          0x08
#define MSD_ERASE_SEQUENCE_ERROR   0x10
#define MSD_ADDRESS_ERROR          0x20
#define MSD_PARAMETER_ERROR        0x40
#define MSD_RESPONSE_FAILURE       0xFF
                                                         
#define SD_CS  PAout(8)

extern u8  SD_Type;

u8 SD_SPI_ReadWriteByte(u8 data);
void SD_SPI_SpeedLow(void);
void SD_SPI_SpeedHigh(void);
u8 SD_WaitReady(void);                          //µÈ´ýSD¿¨×¼±¸
u8 SD_GetResponse(u8 Response);                 //»ñµÃÏàÓ¦
u8 SD_Initialize(void);                         //³õÊ¼»¯
u8 SD_ReadDisk(u8*buf,u32 sector,u8 cnt);       //¶Á¿é
u8 SD_WriteDisk(u8*buf,u32 sector,u8 cnt);      //Ð´¿é
u32 SD_GetSectorCount(void);                    //¶ÁÉÈÇøÊý
u8 SD_GetCID(u8 *cid_data);                     //¶ÁSD¿¨CID
u8 SD_GetCSD(u8 *csd_data);                     //¶ÁSD¿¨CSD


#define FILE_H
#endif
