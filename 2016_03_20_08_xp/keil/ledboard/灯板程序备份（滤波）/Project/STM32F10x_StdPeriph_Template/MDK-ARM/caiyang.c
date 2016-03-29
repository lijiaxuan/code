#include "stm32f10x.h"
#include "all.h"
#define count 22
#define proposition 0.5

uint16_t DRBuffer0[8][24];//8����ÿ����24������
uint16_t DRBuffer1[8][24];
uint16_t DRBuffer2[8][24];
uint16_t DRBuffer3[8][24];
uint32_t MAXbuffer[8][4];
uint32_t JUNbuffer[8][4];
uint32_t  LBdis;
uint32_t  LBdata=0;
uint32_t  LBclear;
uint16_t yuzhitemp1[32];
uint16_t yuzhitemp2[32];
uint16_t junzhitemp[32];
uint16_t temp0;
uint32_t temp1;
uint16_t yuzhiH[32];
uint16_t yuzhiL[32];
uint16_t test[24];
u8  caiflag=0;



uint16_t Filter_Data[8][4] = {0};
uint16_t Old_Filter_Data[8][4] = {0};
uint8_t K_Old[8][4] = {0}, K_New[8][4] = {0};
uint32_t filter_1st(uint32_t NEW_DATA, uint32_t OLD_DATA, uint8_t k);
uint8_t filter_1st_k_dynamic( uint32_t new_data,uint32_t old_data, uint8_t k_old);
extern int8_t old_direction, new_direction;


void LBcai(void)
		{ 
				LBclear=0x00000001;
				for(i=0;i<8;i++)
					 {
						 for(k=0;k<4;k++)
							 {
								 MAXbuffer[i][k]=0;//��ʼ��
							 }
					 }
				for(i=0;i<8;i++)
						{    
							px(i);
							for(k=0;k<count;k++)//k<22����ÿ��ͨ��ȡ22�η��ص�ֵ��ÿ10usȡһ��
							 {	 
									Delay10us(1);  
									DRBuffer0[i][k]= ADC_DualConvertedValueTab[0];//adc1 1
									DRBuffer2[i][k]=(ADC_DualConvertedValueTab[0]>>16);//adc1 2
									DRBuffer1[i][k]= ADC_DualConvertedValueTab[1];//adc2 1
									DRBuffer3[i][k]=(ADC_DualConvertedValueTab[1]>>16);//adc2 2


									if(DRBuffer0[i][k]>=((uint16_t)(*(int32_t*)(0x0800E080+4*i))))
										 {	
												MAXbuffer[i][0]+=((uint32_t)(DRBuffer0[i][k])-(*(int32_t*)(0x0800E080+4*i)));
										 }
									 else
										 {
												MAXbuffer[i][0]+=((*(int32_t*)(0x0800E080+4*i))-(uint32_t)(DRBuffer0[i][k]));
										 }

									 //1
									if(DRBuffer1[i][k]>=((uint16_t)(*(int32_t*)(0x0800E080+4*(i+8)))))
										 {	
												MAXbuffer[i][1]+=((uint32_t)(DRBuffer1[i][k])-(*(int32_t*)(0x0800E080+4*(i+8))));
										 }
									else
										 {
												MAXbuffer[i][1]+=((*(int32_t*)(0x0800E080+4*(i+8)))-(uint32_t)(DRBuffer1[i][k]));
										 }

										//2
										if(DRBuffer2[i][k]>=((uint16_t)(*(int32_t*)(0x0800E080+4*(i+16)))))
										 {	
												MAXbuffer[i][2]+=((uint32_t)(DRBuffer2[i][k])-(*(int32_t*)(0x0800E080+4*(i+16))));
										 }
									 else
										 {
												MAXbuffer[i][2]+=((*(int32_t*)(0x0800E080+4*(i+16)))-(uint32_t)(DRBuffer2[i][k]));
										 }

									 //3
										if(DRBuffer3[i][k]>=((uint16_t)(*(int32_t*)(0x0800E080+4*(i+24)))))
										 {	
												MAXbuffer[i][3]+=((uint32_t)(DRBuffer3[i][k])-(*(int32_t*)(0x0800E080+4*(i+24))));
										 }
									 else
										 {
												MAXbuffer[i][3]+=((*(int32_t*)(0x0800E080+4*(i+24)))-(uint32_t)(DRBuffer3[i][k]));
										 }
						 
							}

						}
				 for(i=0;i<8;i++)
					 {
						 for(k=0;k<4;k++)
							 {
								 MAXbuffer[i][k]=MAXbuffer[i][k]/count;//�൱�ڶ�22�������������ƽ��ƫ��
							 }
					 }

				 for(i=0;i<8;i++)
						 {
							 for(k=0;k<4;k++)
									 {
											 K_New[i][k] = filter_1st_k_dynamic(MAXbuffer[i][k], Old_Filter_Data[i][k], K_Old[i][k]);//�õ��µ�һ���˲�����kֵ
											 Filter_Data[i][k] = filter_1st(MAXbuffer[i][k], Old_Filter_Data[i][k], K_New[i][k]);//�õ��µ��˲�����
											 K_Old[i][k] = K_New[i][k];
											 Old_Filter_Data[i][k] = Filter_Data[i][k];//Ϊ���еݹ���׼��
							// 				 MAXbuffer[i][k] = (uint16_t)Filter_Data[i][k];
											 
									//			yuzhitemp2[i+k*8]=(uint16_t)MAXbuffer[i][k];	
										} 
							}     	
			 

		 
				 for(yy=0;yy<8;yy++)
						 {
						 for(k=0;k<4;k++)
								 {
								switch(caiflag)//3�ֲ�����ʽ��ͨ������uart���������ж��ź���ѡ��
										{
											case 0:
													{
														if(((uint16_t)Filter_Data[yy][k])<((uint16_t)(*(int32_t*)(0x0800E000+4*(yy+k*8)))))
																{
																 LBdis&=~(1<<(yy+k*8));
																}
														else if(((uint16_t)Filter_Data[yy][k])>((uint16_t)((*(int32_t*)(0x0800E000+4*(yy+k*8)))>>16)))
																{
															
																		LBdis|=(1<<(yy+k*8));	
																}
														else
																{
																	LBdis&=~(1<<(yy+k*8));
																}
													}break;

											 case 1:
														{
																if(((uint16_t)Filter_Data[yy][k])<((uint16_t)(*(int32_t*)(0x0800E100+4*(yy+k*8)))))
																	{
																		 LBdis&=~(1<<(yy+k*8));
																	}
																else if(((uint16_t)Filter_Data[yy][k])>((uint16_t)((*(int32_t*)(0x0800E100+4*(yy+k*8)))>>16)))
																	{
																
																			LBdis|=(1<<(yy+k*8));	
																	}
																 else
																	{
																			LBdis&=~(1<<(yy+k*8));
																	}
															}break;

											case 2:
														{
																if(((uint16_t)Filter_Data[yy][k])<((uint16_t)(*(int32_t*)(0x0800E180+4*(yy+k*8)))))
																	{
																	 LBdis&=~(1<<(yy+k*8));
																	}

																else if(((uint16_t)Filter_Data[yy][k])>((uint16_t)((*(int32_t*)(0x0800E180+4*(yy+k*8)))>>16)))
																	{
																
																			LBdis|=(1<<(yy+k*8));
																	}
																else
																	{
																	 LBdis&=~(1<<(yy+k*8));
																	}
															}break;

											}

								 }
						 }

				 LBdis &= 0x00ffffff;
			 
			 for (yy = 1; yy < 23; yy++)
					 {	
						 if(((LBdis>>yy)&0x00000001) == 1)
						 {
							 if((((LBdis>>(yy-1))&0x00000001) | ((LBdis>>(yy+1))&0x00000001 ))== 0)
								 {
									 LBdis &= ~(LBclear<<yy);
									
								 }
						 }
						 else
						 {
							 if((((LBdis>>(yy-1))&0x00000001) & ((LBdis>>(yy+1))&0x00000001 ))== 1)
								 {
									 LBdis |= (LBclear<<yy);
								 }
						 }
					 }
						
				LBdata=LBdis;
			 
				yy = 0;
			 
				Display(LBdata);//��ADC���ص����ݽ��д���󣬷�������LED���ĳ�����ʹ��Ӧ��led����
		 	    	USART_SendData(USARTy,0xaa);
		 	 while(USART_GetFlagStatus(USARTy, USART_FLAG_TXE) == RESET)
		     {
		     }
		        	USART_SendData(USARTy,LBdata);
		 	 while(USART_GetFlagStatus(USARTy, USART_FLAG_TXE) == RESET)
		     {
		     }
		 	USART_SendData(USARTy,(LBdata>>8) );
		 	 while(USART_GetFlagStatus(USARTy, USART_FLAG_TXE) == RESET)
		     {
		     }
		 	USART_SendData(USARTy,(LBdata>>16) );
		 	 while(USART_GetFlagStatus(USARTy, USART_FLAG_TXE) == RESET)
		     {
		     }
		 	 USART_SendData(USARTy,(LBdata>>24) );
		 	 while(USART_GetFlagStatus(USARTy, USART_FLAG_TXE) == RESET)
		     {
		     }
		 	   	USART_SendData(USARTy,0x55);
		 	 while(USART_GetFlagStatus(USARTy, USART_FLAG_TXE) == RESET)
		     {
		     }
		  	    	USART_SendData(USARTy,0xaa);
		  	 while(USART_GetFlagStatus(USARTy, USART_FLAG_TXE) == RESET)
		      {
		      }

		 			while(USART_GetFlagStatus(USARTy, USART_FLAG_TXE) == RESET)
		       {
		       }
		 	   	USART_SendData(USARTy,junzhitemp[0]>>8);
		 			while(USART_GetFlagStatus(USARTy, USART_FLAG_TXE) == RESET)
		       {
		       }
		 	   	USART_SendData(USARTy,junzhitemp[0]);
		 			while(USART_GetFlagStatus(USARTy, USART_FLAG_TXE) == RESET)
		       {
		       }
		 	   	USART_SendData(USARTy,yuzhitemp1[0]>>8);
		 						while(USART_GetFlagStatus(USARTy, USART_FLAG_TXE) == RESET)
		       {
		       }
		 	   	USART_SendData(USARTy,yuzhitemp1[0]);
		 						while(USART_GetFlagStatus(USARTy, USART_FLAG_TXE) == RESET)
		       {
		       }
		 	   	USART_SendData(USARTy,yuzhitemp2[0]>>8);
		 						while(USART_GetFlagStatus(USARTy, USART_FLAG_TXE) == RESET)
		       {
		       }
		 	   	USART_SendData(USARTy,yuzhitemp2[0]);
		 	
		 			while(USART_GetFlagStatus(USARTy, USART_FLAG_TXE) == RESET)
		      {
		      }
		  	   	USART_SendData(USARTy,yuzhiH[0]>>8);

		 			while(USART_GetFlagStatus(USARTy, USART_FLAG_TXE) == RESET)
		      {
		      }
		 	   	USART_SendData(USARTy,yuzhiH[0]);

		 			while(USART_GetFlagStatus(USARTy, USART_FLAG_TXE) == RESET)
		      {
		      }
		 	   	USART_SendData(USARTy,yuzhiL[0]>>8);

		 			while(USART_GetFlagStatus(USARTy, USART_FLAG_TXE) == RESET)
		      {
		      }
		  	   	USART_SendData(USARTy,yuzhiL[0]);		 

		 		 			while(USART_GetFlagStatus(USARTy, USART_FLAG_TXE) == RESET)
		      {
		      }
		  	   	USART_SendData(USARTy,MAXbuffer[0][0]>>8);		 
				 			while(USART_GetFlagStatus(USARTy, USART_FLAG_TXE) == RESET)
		      {
	      }
		 	   	USART_SendData(USARTy,MAXbuffer[0][0]);		 

		 			while(USART_GetFlagStatus(USARTy, USART_FLAG_TXE) == RESET)
		      {
		      }
		 	   	USART_SendData(USARTy,0x55);
		// 	printf("%d\r\n", MAXbuffer[0][0]);


		 //printf("%d   %d    %d    %d    %d\r\n",yuzhitemp1[4],yuzhitemp2[4],yuzhiH[4],Filter_Data[4][0],((uint16_t)(*(int32_t*)(0x0800E000+4*(4+0)))));
		 //printf("%d\r\n",yuzhiL[0]);
		}

//���ֵ���洢
void junzhi(void)
		{ 
			for(j=0;j<32;j++)
			 {
				 junzhitemp[j]=0;
			 }
			for(j=0;j<15;j++)
			 { 
				 for(i=0;i<8;i++)
					 {
						 for(k=0;k<4;k++)
							 {
								 JUNbuffer[i][k]=0;
							 }
					 }
					 
					for(i=0;i<8;i++)
						{    
							 px(i);
							 for(k=0;k<count;k++)
									 {	 
											Delay10us(1);  
											DRBuffer0[i][k]= ADC_DualConvertedValueTab[0];
											DRBuffer2[i][k]=(ADC_DualConvertedValueTab[0]>>16);
											DRBuffer1[i][k]= ADC_DualConvertedValueTab[1];
											DRBuffer3[i][k]=(ADC_DualConvertedValueTab[1]>>16);
									
											JUNbuffer[i][0]+=(uint32_t)(DRBuffer0[i][k]);
											JUNbuffer[i][1]+=(uint32_t)(DRBuffer1[i][k]);
											JUNbuffer[i][2]+=(uint32_t)(DRBuffer2[i][k]);
											JUNbuffer[i][3]+=(uint32_t)(DRBuffer3[i][k]);
									 
									}

						}
						
				 for(i=0;i<8;i++)
					 {
						 for(k=0;k<4;k++)
								{
									 JUNbuffer[i][k]=JUNbuffer[i][k]/count;
									 junzhitemp[i+k*8]=junzhitemp[i+k*8]+(uint16_t)JUNbuffer[i][k];
								}
					 }
									 
				}
				
			for(j=0;j<32;j++)
				 {
					 junzhitemp[j]=junzhitemp[j]/15;
				 }
			 
			for(yy=0;yy<32;yy++)
				 {
					 temp1=(uint32_t)junzhitemp[yy];
					 proflash(((uint32_t)(0x0800E080+4*yy)),temp1);//����Ƚ�ֵ
				}
		}
//�����ֵ

void fengzhih(void)
		{ 
			u8 l;
			for(l=0;l<40;l++)
					{         
						for(j=0;j<32;j++)
							 {
							 yuzhitemp1[j]=0;
							 }
						for(j=0;j<15;j++)
								 {  
									 for(i=0;i<8;i++)
											 {
												 for(k=0;k<4;k++)
													 {
														 MAXbuffer[i][k]=0;
													 }
											 }
										for(i=0;i<8;i++)
												{    
													px(i);
													for(k=0;k<count;k++)
														 {	 
																Delay10us(1);  
																DRBuffer0[i][k]= ADC_DualConvertedValueTab[0];
																DRBuffer2[i][k]=(ADC_DualConvertedValueTab[0]>>16);
																DRBuffer1[i][k]= ADC_DualConvertedValueTab[1];
																DRBuffer3[i][k]=(ADC_DualConvertedValueTab[1]>>16);


															if(DRBuffer0[i][k]>=((uint16_t)(*(int32_t*)(0x0800E080+4*i))))
																 {	
																		MAXbuffer[i][0]+=((uint32_t)(DRBuffer0[i][k])-(*(int32_t*)(0x0800E080+4*i)));
																 }
															 else
																 {
																		MAXbuffer[i][0]+=((*(int32_t*)(0x0800E080+4*i))-(uint32_t)(DRBuffer0[i][k]));
																 }

														 //1
																if(DRBuffer1[i][k]>=((uint16_t)(*(int32_t*)(0x0800E080+4*(i+8)))))
																	 {	
																			MAXbuffer[i][1]+=((uint32_t)(DRBuffer1[i][k])-(*(int32_t*)(0x0800E080+4*(i+8))));
																	 }
																else
																	 {
																			MAXbuffer[i][1]+=((*(int32_t*)(0x0800E080+4*(i+8)))-(uint32_t)(DRBuffer1[i][k]));
																	 }

															//2
																	if(DRBuffer2[i][k]>=((uint16_t)(*(int32_t*)(0x0800E080+4*(i+16)))))
																		 {	
																				MAXbuffer[i][2]+=((uint32_t)(DRBuffer2[i][k])-(*(int32_t*)(0x0800E080+4*(i+16))));
																		 }
																 else
																		 {
																			 MAXbuffer[i][2]+=((*(int32_t*)(0x0800E080+4*(i+16)))-(uint32_t)(DRBuffer2[i][k]));
																		 }

														 //3
																	if(DRBuffer3[i][k]>=((uint16_t)(*(int32_t*)(0x0800E080+4*(i+24)))))
																		 {	
																				MAXbuffer[i][3]+=((uint32_t)(DRBuffer3[i][k])-(*(int32_t*)(0x0800E080+4*(i+24))));
																		 }
																	 else
																		 {
																				MAXbuffer[i][3]+=((*(int32_t*)(0x0800E080+4*(i+24)))-(uint32_t)(DRBuffer3[i][k]));
																		 }
													 
																}

																}
											 for(i=0;i<8;i++)
													 {
															 for(k=0;k<4;k++)
																	 {
																				MAXbuffer[i][k]=MAXbuffer[i][k]/count;
																	 }
													 }
													 
												 for(i=0;i<8;i++)
														 {
															 for(k=0;k<4;k++)
																	 {
																			 K_New[i][k] = filter_1st_k_dynamic(MAXbuffer[i][k], Old_Filter_Data[i][k], K_Old[i][k]);
																			 Filter_Data[i][k] = filter_1st(MAXbuffer[i][k], Old_Filter_Data[i][k], K_New[i][k]);
																			 K_Old[i][k] = K_New[i][k];
																			 Old_Filter_Data[i][k] = Filter_Data[i][k];
																			 MAXbuffer[i][k] = (uint16_t)Filter_Data[i][k];
																			 
																			 yuzhitemp1[i+k*8]=(uint16_t)MAXbuffer[i][k];	
																		} 
															}     	
									 
								}
				}
		}


//�����ֵ
void fengzhil(void)
		{ 
			u8 l;
			for(l=0;l<40;l++)
					{
						for(j=0;j<32;j++)
							 {
									yuzhitemp2[j]=0;
							 }
					 
					for(j=0;j<15;j++)
							{   
									for(i=0;i<8;i++)
										 {
											 for(k=0;k<4;k++)
													 {
															MAXbuffer[i][k]=0;
													 }
										 }
								for(i=0;i<8;i++)
										{    
												px(i);
											for(k=0;k<count;k++)
													 {	 
															Delay10us(1);  
															DRBuffer0[i][k]= ADC_DualConvertedValueTab[0];
															DRBuffer2[i][k]=(ADC_DualConvertedValueTab[0]>>16);
															DRBuffer1[i][k]= ADC_DualConvertedValueTab[1];
															DRBuffer3[i][k]=(ADC_DualConvertedValueTab[1]>>16);
														 
															if(DRBuffer0[i][k]>=((uint16_t)(*(int32_t*)(0x0800E080+4*i))))
																 {	
																		MAXbuffer[i][0]+=((uint32_t)(DRBuffer0[i][k])-(*(int32_t*)(0x0800E080+4*i)));
																 }
														 else
																 {
																		MAXbuffer[i][0]+=((*(int32_t*)(0x0800E080+4*i))-(uint32_t)(DRBuffer0[i][k]));
																 }

													 //1
															if(DRBuffer1[i][k]>=((uint16_t)(*(int32_t*)(0x0800E080+4*(i+8)))))
																 {	
																		MAXbuffer[i][1]+=((uint32_t)(DRBuffer1[i][k])-(*(int32_t*)(0x0800E080+4*(i+8))));
																 }
															else
																 {
																		MAXbuffer[i][1]+=((*(int32_t*)(0x0800E080+4*(i+8)))-(uint32_t)(DRBuffer1[i][k]));
																 }

												//2
															if(DRBuffer2[i][k]>=((uint16_t)(*(int32_t*)(0x0800E080+4*(i+16)))))
																 {	
																		MAXbuffer[i][2]+=((uint32_t)(DRBuffer2[i][k])-(*(int32_t*)(0x0800E080+4*(i+16))));
																 }
														 else
																 {
																		MAXbuffer[i][2]+=((*(int32_t*)(0x0800E080+4*(i+16)))-(uint32_t)(DRBuffer2[i][k]));
																 }

											 //3
															if(DRBuffer3[i][k]>=((uint16_t)(*(int32_t*)(0x0800E080+4*(i+24)))))
																 {	
																		MAXbuffer[i][3]+=((uint32_t)(DRBuffer3[i][k])-(*(int32_t*)(0x0800E080+4*(i+24))));
																 }
														 else
																 {
																		MAXbuffer[i][3]+=((*(int32_t*)(0x0800E080+4*(i+24)))-(uint32_t)(DRBuffer3[i][k]));
																 }
													 
														}

										}		
										
							  for(i=0;i<8;i++)
									 {
											 for(k=0;k<4;k++)
													 {
															MAXbuffer[i][k]=MAXbuffer[i][k]/count;
													 }
									 }
									 
								 for(i=0;i<8;i++)
										 {
											 for(k=0;k<4;k++)
													 {
														 K_New[i][k] = filter_1st_k_dynamic(MAXbuffer[i][k], Old_Filter_Data[i][k], K_Old[i][k]);
														 Filter_Data[i][k] = filter_1st(MAXbuffer[i][k], Old_Filter_Data[i][k], K_New[i][k]);
														 K_Old[i][k] = K_New[i][k];
														 Old_Filter_Data[i][k] = Filter_Data[i][k];
														 MAXbuffer[i][k] = (uint16_t)Filter_Data[i][k];														 
														 yuzhitemp2[i+k*8]=(uint16_t)MAXbuffer[i][k];	
														} 
											}     	
							}   
				}
		}
				                          
void chizhi(void)
		{
				//���ͱȽϻ���
				for(i=0;i<32;i++)
						{
								yuzhiH[i]=(uint16_t)yuzhitemp1[i]*proposition+(uint16_t)yuzhitemp2[i]*(1-proposition);
								yuzhiL[i]=(uint16_t)yuzhitemp1[i]*proposition+(uint16_t)yuzhitemp2[i]*(1-proposition);
						}
				//����ֵ����Flash
				 for(yy=0;yy<32;yy++)
						 {
								 temp1=(uint32_t)yuzhiL[yy]+(((uint32_t)yuzhiH[yy])<<16);
								 proflash(((uint32_t)(0x0800E000+4*yy)),temp1);
						 }
		 
		}
 
void chizhired(void)
		{
		//���ͱȽϻ���
				for(i=0;i<32;i++)
						{
								temp0=(yuzhitemp1[i]-yuzhitemp2[i])*3/4;
								yuzhiH[i]=temp0+(uint16_t)yuzhitemp2[i];
								temp0=(yuzhitemp1[i]-yuzhitemp2[i])*6/9;
								yuzhiL[i]=temp0+(uint16_t)yuzhitemp2[i];
						}
				//����ֵ����Flash
				 for(yy=0;yy<32;yy++)
						{
								temp1=(uint32_t)yuzhiL[yy]+(((uint32_t)yuzhiH[yy])<<16);
								proflash(((uint32_t)(0x0800E100+4*yy)),temp1);
						} 
		}

void chizhiblue(void)
		{
		//���ͱȽϻ���
				for(i=0;i<32;i++)
						{
							 temp0=(yuzhitemp1[i]-yuzhitemp2[i])*3/4;
							 yuzhiH[i]=temp0+(uint16_t)yuzhitemp2[i];	 
							 temp0=(yuzhitemp1[i]-yuzhitemp2[i])*6/9;
							 yuzhiL[i]=temp0+(uint16_t)yuzhitemp2[i];
						}
						//����ֵ����Flash
				 for(yy=0;yy<32;yy++)
						{
							 temp1=(uint32_t)yuzhiL[yy]+(((uint32_t)yuzhiH[yy])<<16);
							 proflash(((uint32_t)(0x0800E180+4*yy)),temp1);
						}
		 
		}

//һ�׵�ͨ�˲�
//NEW_DATA:�²���ֵ
//OLD_DATA:�ϴ��˲����
//k:�˲�ϵ����0~255��
uint32_t filter_1st(uint32_t NEW_DATA, uint32_t OLD_DATA, uint8_t k)
		{
			uint32_t result;
			
			if(NEW_DATA < OLD_DATA)
					{
						result = OLD_DATA - NEW_DATA;
						result *= k;
						result += 128;
						result /= 256;
						result = OLD_DATA - result;
					}
			else if(NEW_DATA > OLD_DATA)
					{
						result = NEW_DATA - OLD_DATA;
						result *= k;
						result += 128;
						result /= 256;
						result = OLD_DATA +result;
					} 
			else
					result = OLD_DATA;
			
			return (uint32_t)result;
		}


//һ���˲�ϵ��K��̬��������д��һ���˲�����֮ǰ����.
//old_data:�ϴ��˲����
//new_data:�²���ֵ
//k_old:�ϴ��˲�ϵ��

#define THRESHOLD_INC_ADC_VALUE    120          //200--10000
//#define THRESHOLD_INC_ADC_VALUE    	    20000          //400--5000
#define MAX_COUNTER  45			
#define K_INC   100
#define K_MAX   250

int8_t old_direction = 0, new_direction = 0;


uint8_t filter_1st_k_dynamic( uint32_t new_data,uint32_t old_data, uint8_t k_old)
		{
			uint16_t k_new;
			uint32_t differ;
			static uint8_t counter;
			//uint16_t k_inc_inc = 0;
				//�ж����ݱ仯���򣬲��Ҽ������ݱ仯��С
				if(new_data >= old_data)
						{
							new_direction = 1;
							if(old_direction == 0)
							old_direction = new_direction;
							differ = new_data - old_data;
						}
				else if(new_data < old_data)
						{
							new_direction = -1;			
							if(old_direction == 0)
							old_direction = new_direction;
							differ = old_data - new_data;
						}
				
				//���ݶ���ʱ�����ȶ�ֵ�����仯ʱ����ȡKֵ�㶨
				if(old_direction != new_direction)
						{
							 old_direction = new_direction;
							 counter = 0;
							 k_new = 5;
							 return k_new;
						}
				//���ݱ仯ʱ
				else
						{
							counter  += 2;
							old_direction = new_direction;
							//���ݿ��ٱ仯ʱ�����ڱ仯��ֵ��
							if(differ > THRESHOLD_INC_ADC_VALUE)
									{
							//		LED3 = ~LED3;
							//		counter = counter + 30;	
										k_new = 250;
										counter = 0;
										return k_new;
									}
							
							if(counter > MAX_COUNTER)
									{
										k_new = k_old + K_INC;
										counter = 0;
										if(k_new > K_MAX)
										k_new = K_MAX;
										return k_new;
									}
							else
									{
										k_new = k_old;
						// 			if(k_new == 5)
						// 			k_new = 10;
										return k_new;
									}
						}	
		}





