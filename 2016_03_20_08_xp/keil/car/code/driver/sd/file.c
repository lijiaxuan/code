/*
* @Author: obsidian
* @Date:   2016-03-08 20:12:53
* @Last Modified by:   obsidian
* @Last Modified time: 2016-03-08 20:37:59
*/

#include "file.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
SPI_InitTypeDef  SPI_InitStructure;

u8 SPI2_ReadWriteByte(u8 TxData)
{       
    u8 retry=0;                 
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) //Œì²éÖž¶šµÄSPI±êÖŸÎ»ÉèÖÃÓë·ñ:·¢ËÍ»ºŽæ¿Õ±êÖŸÎ»
        {
        retry++;
        if(retry>200)return 0;
        }             
    SPI_I2S_SendData(SPI2, TxData); //Íš¹ýÍâÉèSPIx·¢ËÍÒ»žöÊýŸÝ
    retry=0;

    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)//Œì²éÖž¶šµÄSPI±êÖŸÎ»ÉèÖÃÓë·ñ:œÓÊÜ»ºŽæ·Ç¿Õ±êÖŸÎ»
        {
        retry++;
        if(retry>200)return 0;
        }                               
    return SPI_I2S_ReceiveData(SPI2); //·µ»ØÍš¹ýSPIx×îœüœÓÊÕµÄÊýŸÝ                      
}

void SPI2_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
  
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE ); 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //žŽÓÃÍÆÍìÊä³ö
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //ÉèÖÃSPIµ¥Ïò»òÕßË«ÏòµÄÊýŸÝÄ£Êœ:SPIÉèÖÃÎªË«ÏßË«ÏòÈ«Ë«¹€
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;       //ÉèÖÃSPI¹€×÷Ä£Êœ:ÉèÖÃÎªÖ÷SPI
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;       //ÉèÖÃSPIµÄÊýŸÝŽóÐ¡:SPI·¢ËÍœÓÊÕ8Î»Ö¡œá¹¹
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;     //Ñ¡ÔñÁËŽ®ÐÐÊ±ÖÓµÄÎÈÌ¬:Ê±ÖÓÐü¿Õžß
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;    //ÊýŸÝ²¶»ñÓÚµÚ¶þžöÊ±ÖÓÑØ
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;       //NSSÐÅºÅÓÉÓ²Œþ£šNSS¹ÜœÅ£©»¹ÊÇÈíŒþ£šÊ¹ÓÃSSIÎ»£©¹ÜÀí:ÄÚ²¿NSSÐÅºÅÓÐSSIÎ»¿ØÖÆ
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;        //¶šÒå²šÌØÂÊÔ€·ÖÆµµÄÖµ:²šÌØÂÊÔ€·ÖÆµÖµÎª256
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;  //Öž¶šÊýŸÝŽ«ÊäŽÓMSBÎ»»¹ÊÇLSBÎ»¿ªÊŒ:ÊýŸÝŽ«ÊäŽÓMSBÎ»¿ªÊŒ
    SPI_InitStructure.SPI_CRCPolynomial = 7;    //CRCÖµŒÆËãµÄ¶àÏîÊœ
    SPI_Init(SPI2, &SPI_InitStructure);  //žùŸÝSPI_InitStructÖÐÖž¶šµÄ²ÎÊý³õÊŒ»¯ÍâÉèSPIxŒÄŽæÆ÷
 
    SPI_Cmd(SPI2, ENABLE); //Ê¹ÄÜSPIÍâÉè
    
    SPI2_ReadWriteByte(0xff);//Æô¶¯Ž«Êä      
}   
//SPI ËÙ¶ÈÉèÖÃº¯Êý
//SpeedSet:
//SPI_BaudRatePrescaler_2   2·ÖÆµ   (SPI 36M@sys 72M)
//SPI_BaudRatePrescaler_8   8·ÖÆµ   (SPI 9M@sys 72M)
//SPI_BaudRatePrescaler_16  16·ÖÆµ  (SPI 4.5M@sys 72M)
//SPI_BaudRatePrescaler_256 256·ÖÆµ (SPI 281.25K@sys 72M)
  
void SPI2_SetSpeed(u8 SpeedSet)
{
    SPI_InitStructure.SPI_BaudRatePrescaler = SpeedSet ;
  SPI_Init(SPI2, &SPI_InitStructure);
    SPI_Cmd(SPI2,ENABLE);
} 

//SPIx ¶ÁÐŽÒ»žö×ÖœÚ
//TxData:ÒªÐŽÈëµÄ×ÖœÚ
//·µ»ØÖµ:¶ÁÈ¡µœµÄ×ÖœÚ

u8  SD_Type=0;

u8 SD_SPI_ReadWriteByte(u8 data)
{
    return SPI2_ReadWriteByte(data);
}     
//SD¿¨³õÊ¼»¯µÄÊ±ºò,ÐèÒªµÍËÙ
void SD_SPI_SpeedLow(void)
{
    SPI2_SetSpeed(SPI_BaudRatePrescaler_256);//ÉèÖÃµ½µÍËÙÄ£Ê½   
}

void SD_SPI_SpeedHigh(void)
{
    SPI2_SetSpeed(SPI_BaudRatePrescaler_2);//ÉèÖÃµ½¸ßËÙÄ£Ê½ 
}

void SD_SPI_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE );  //PORTAÊ±ÖÓÊ¹ÄÜ 

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;//PA8 ÍÆÍì   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_SetBits(GPIOA,GPIO_Pin_8);//PA8ÉÏÀ­ 

  SPI2_Init();
    SD_CS=1;
}

void SD_DisSelect(void)
{
    SD_CS=1;
    SD_SPI_ReadWriteByte(0xff);//Ìá¹©¶îÍâµÄ8¸öÊ±ÖÓ
}

u8 SD_Select(void)
{
    SD_CS=0;
    if(SD_WaitReady()==0)return 0;//µÈ´ý³É¹¦
    SD_DisSelect();
    return 1;
}

u8 SD_WaitReady(void)
{
    u32 t=0;
    do
    {
        if(SD_SPI_ReadWriteByte(0XFF)==0XFF)return 0;//OK
        t++;            
    }while(t<0XFFFFFF);
    return 1;
}

u8 SD_GetResponse(u8 Response)
{
    u16 Count=0xFFFF;//µÈ´ý´ÎÊý                           
    while ((SD_SPI_ReadWriteByte(0XFF)!=Response)&&Count)Count--;//µÈ´ýµÃµ½×¼È·µÄ»ØÓ¦     
    if (Count==0)return MSD_RESPONSE_FAILURE;//µÃµ½»ØÓ¦Ê§°Ü   
    else return MSD_RESPONSE_NO_ERROR;//ÕýÈ·»ØÓ¦
}

u8 SD_RecvData(u8*buf,u16 len)
{                 
    if(SD_GetResponse(0xFE))return 1;//µÈ´ýSD¿¨·¢»ØÊý¾ÝÆðÊ¼ÁîÅÆ0xFE
    while(len--)
    {
        *buf=SPI2_ReadWriteByte(0xFF);
        buf++;
    }

    SD_SPI_ReadWriteByte(0xFF);
    SD_SPI_ReadWriteByte(0xFF);                                                         
    return 0;
}

u8 SD_SendBlock(u8*buf,u8 cmd)
{   
    u16 t;            
    if(SD_WaitReady())return 1;
    SD_SPI_ReadWriteByte(cmd);
    if(cmd!=0XFD)
    {
        for(t=0;t<512;t++)SPI2_ReadWriteByte(buf[t]);
        SD_SPI_ReadWriteByte(0xFF);
        SD_SPI_ReadWriteByte(0xFF);
        t=SD_SPI_ReadWriteByte(0xFF);
        if((t&0x1F)!=0x05)return 2;                                      
    }                                                                                   
    return 0;
}
                                                      
u8 SD_SendCmd(u8 cmd, u32 arg, u8 crc)
{
    u8 r1;  
    u8 Retry=0; 
    SD_DisSelect();
    if(SD_Select())return 0XFF;
    SD_SPI_ReadWriteByte(cmd | 0x40);
    SD_SPI_ReadWriteByte(arg >> 24);
    SD_SPI_ReadWriteByte(arg >> 16);
    SD_SPI_ReadWriteByte(arg >> 8);
    SD_SPI_ReadWriteByte(arg);    
    SD_SPI_ReadWriteByte(crc); 
    if(cmd==CMD12)SD_SPI_ReadWriteByte(0xff);

    Retry=0X1F;
    do
    {
        r1=SD_SPI_ReadWriteByte(0xFF);
    }while((r1&0X80) && Retry--);    

    return r1;
}                                                                                         
                                                     
u8 SD_GetCID(u8 *cid_data)
{
    u8 r1;     

    r1=SD_SendCmd(CMD10,0,0x01);
    if(r1==0x00)
    {
        r1=SD_RecvData(cid_data,16);//½ÓÊÕ16¸ö×Ö½ÚµÄÊý¾Ý     
    }
    SD_DisSelect();
    if(r1)return 1;
    else return 0;
}                                                                                 
                                                       
u8 SD_GetCSD(u8 *csd_data)
{
    u8 r1;   
    r1=SD_SendCmd(CMD9,0,0x01);
    if(r1==0)
    {
        r1=SD_RecvData(csd_data, 16);
    }
    SD_DisSelect();
    if(r1)return 1;
    else return 0;
}  
u32 SD_GetSectorCount(void)
{
    u8 csd[16];
    u32 Capacity;  
    u8 n;
    u16 csize;                          
    
    if(SD_GetCSD(csd)!=0) return 0;     

    if((csd[0]&0xC0)==0x40)
    {   
        csize = csd[9] + ((u16)csd[8] << 8) + 1;
        Capacity = (u32)csize << 10;
    }else
    {   
        n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
        csize = (csd[8] >> 6) + ((u16)csd[7] << 2) + ((u16)(csd[6] & 3) << 10) + 1;
        Capacity= (u32)csize << (n - 9);
    }
    return Capacity;
}

u8 SD_Initialize(void)
{
    u8 r1;      
    u16 retry;  
    u8 buf[4];  
    u16 i;

    SD_SPI_Init();      
    SD_SPI_SpeedLow();  
    for(i=0;i<10;i++)SD_SPI_ReadWriteByte(0XFF);
    retry=20;
    do
    {
        r1=SD_SendCmd(CMD0,0,0x95);
    }while((r1!=0X01) && retry--);
    SD_Type=0;
    if(r1==0X01)
    {
        if(SD_SendCmd(CMD8,0x1AA,0x87)==1)//SD V2.0
        {
            for(i=0;i<4;i++)buf[i]=SD_SPI_ReadWriteByte(0XFF);  //Get trailing return value of R7 resp
            if(buf[2]==0X01&&buf[3]==0XAA)//¿¨ÊÇ·ñÖ§³Ö2.7~3.6V
            {
                retry=0XFFFE;
                do
                {
                    SD_SendCmd(CMD55,0,0X01);   //·¢ËÍCMD55
                    r1=SD_SendCmd(CMD41,0x40000000,0X01);//·¢ËÍCMD41
                }while(r1&&retry--);
                if(retry&&SD_SendCmd(CMD58,0,0X01)==0)//¼ø±ðSD2.0¿¨°æ±¾¿ªÊ¼
                {
                    for(i=0;i<4;i++)buf[i]=SD_SPI_ReadWriteByte(0XFF);//µÃµ½OCRÖµ
                    if(buf[0]&0x40)SD_Type=SD_TYPE_V2HC;    //¼ì²éCCS
                    else SD_Type=SD_TYPE_V2;   
                }
            }
        }else
        {
            SD_SendCmd(CMD55,0,0X01);      
            r1=SD_SendCmd(CMD41,0,0X01);   
            if(r1<=1)
            {       
                SD_Type=SD_TYPE_V1;
                retry=0XFFFE;
                do 
                {
                    SD_SendCmd(CMD55,0,0X01);   
                    r1=SD_SendCmd(CMD41,0,0X01);
                }while(r1&&retry--);
            }else
            {
                SD_Type=SD_TYPE_MMC;
                retry=0XFFFE;
                do 
                {                                               
                    r1=SD_SendCmd(CMD1,0,0X01);
                }while(r1&&retry--);  
            }
            if(retry==0||SD_SendCmd(CMD16,512,0X01)!=0)SD_Type=SD_TYPE_ERR;//´íÎóµÄ¿¨
        }
    }
    SD_DisSelect();
    SD_SPI_SpeedHigh();
    if(SD_Type)return 0;
    else if(r1)return r1;      
    return 0xaa;
}

u8 SD_ReadDisk(u8*buf,u32 sector,u8 cnt)
{
    u8 r1;
    if(SD_Type!=SD_TYPE_V2HC)sector <<= 9;
    if(cnt==1)
    {
        r1=SD_SendCmd(CMD17,sector,0X01);
        if(r1==0)
        {
            r1=SD_RecvData(buf,512);   
        }
    }else
    {
        r1=SD_SendCmd(CMD18,sector,0X01);
        do
        {
            r1=SD_RecvData(buf,512);
            buf+=512;  
        }while(--cnt && r1==0);     
        SD_SendCmd(CMD12,0,0X01);   
    }   
    SD_DisSelect();
    return r1;//
}

u8 SD_WriteDisk(u8*buf,u32 sector,u8 cnt)
{
    u8 r1;
    if(SD_Type!=SD_TYPE_V2HC)sector *= 512;//×ª»»Îª×Ö½ÚµØÖ·
    if(cnt==1)
    {
        r1=SD_SendCmd(CMD24,sector,0X01);//¶ÁÃüÁî
        if(r1==0)
        {
            r1=SD_SendBlock(buf,0xFE);//Ð´512¸ö×Ö½Ú    
        }
    }else
    {
        if(SD_Type!=SD_TYPE_MMC)
        {
            SD_SendCmd(CMD55,0,0X01);   
            SD_SendCmd(CMD23,cnt,0X01);//·¢ËÍÖ¸Áî   
        }
        r1=SD_SendCmd(CMD25,sector,0X01);//Á¬Ðø¶ÁÃüÁî
        if(r1==0)
        {
            do
            {
                r1=SD_SendBlock(buf,0xFC);
                buf+=512;  
            }while(--cnt && r1==0);
            r1=SD_SendBlock(0,0xFD);
        }
    }   
    SD_DisSelect();
    return r1;
}   
