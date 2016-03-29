
	;THUMB
	;RSEG CODE:CODE:NOROOT(2)
NVIC_INT_CTRL	EQU		0xe000ed04
NVIC_PENDSVSET  EQU		0x10000000
NVIC_SYSPRI14   EQU		0xe000ed22
NVIC_PENDSV_PRI EQU     0xff
                PRESERVE8
                THUMB

                AREA |.text| ,CODE ,READONLY
 
	;AREA |.text| ,CODE ,READONLY
	;THUMB
	
 IMPORT OSTCBCur
 IMPORT OSTCBNext

 EXPORT OS_ENTER_CRITICAL
 EXPORT OS_EXIT_CRITICAL
 EXPORT OSStart
 EXPORT PendSV_Handler
 EXPORT OSCtxSw

OS_ENTER_CRITICAL
		CPSID	I
		BX		LR
OS_EXIT_CRITICAL
		CPSIE	I
		BX		LR
OSStart
		CPSID	I
		LDR		R0,=NVIC_SYSPRI14
		LDR		R1,=NVIC_PENDSV_PRI
		STRB	R1,[R0]
	
	
		LDR		R4, =0x0
		MSR		PSP,R4
	
		LDR		R4,=NVIC_INT_CTRL
		LDR		R5,=NVIC_PENDSVSET
		STR 	R5,[R4]
	
		CPSIE	I
OSStartHang
		B 		OSStartHang
	
PendSV_Handler
		CPSID	I
		MRS 	R0,PSP
		CBZ		R0,PendSV_Handler_NoSave
		SUB		R0,R0,#0x20
		STR 	R4,[R0]
		ADD		R0,R0,#0x4
		STR 	R5,[R0]
		ADD		R0,R0,#0x4
		STR 	R6,[R0]
		ADD		R0,R0,#0x4
		STR 	R7,[R0]
		ADD		R0,R0,#0x4
		STR 	R8,[R0]
		ADD		R0,R0,#0x4
		STR 	R9,[R0]
		ADD		R0,R0,#0x4
		STR 	R10,[R0]
		ADD		R0,R0,#0x4
		STR 	R11,[R0]
		ADD		R0,R0,#0x4
		SUB		R0,R0,#0x20
	
		;STM	R0,{R4-R11}
	
		LDR		R1,=OSTCBCur
		LDR		R1,[R1]
		STR 	R0,[R1]
PendSV_Handler_NoSave
		    LDR     R0, =OSTCBCur           ; R0 = OSTCBCur;
	    LDR     R1, =OSTCBNext          ; R1 = OSTCBNext;
	    LDR     R2, [R1]                ; R2 = OSTCBNext->OSTCBStkPtr;
	    STR     R2, [R0]                ; *R0 = R2;(OSTCBCur->OSTCBStkPtr = OSTCBNext->OSTCBStkPtr)
	    
	    LDR     R0, [R2]                 ; R0 = *R2;(R0 = OSTCBNext->OSTCBStkPtr)
	    ; LDM     R0, {R4-R11}
	    ; load R4 
	    LDR     R4, [R0]                 ; R4 = *R0;(R4 = *(OSTCBNext->OSTCBStkPtr))
	    ADD     R0, R0, #0x4             ; R0 = R0 + 0x4;(OSTCBNext->OSTCBStkPtr++)
	    ; load R5 
	    LDR     R5, [R0]                 ; R5 = *R0;(R5 = *(OSTCBNext->OSTCBStkPtr))
	    ADD     R0, R0, #0x4             ; R0 = R0 + 0x4;(OSTCBNext->OSTCBStkPtr++)
	    ; load R6
	    LDR     R6, [R0]                 ; R6 = *R0;(R6 = *(OSTCBNext->OSTCBStkPtr))
	    ADD     R0, R0, #0x4             ; R0 = R0 + 0x4;(OSTCBNext->OSTCBStkPtr++)
	    ; load R7 
	    LDR     R7 , [R0]                ; R7 = *R0;(R7 = *(OSTCBNext->OSTCBStkPtr))
	    ADD     R0, R0, #0x4             ; R0 = R0 + 0x4;(OSTCBNext->OSTCBStkPtr++)
	    ; load R8 
	    LDR     R8 , [R0]                ; R8 = *R0;(R8 = *(OSTCBNext->OSTCBStkPtr))
	    ADD     R0, R0, #0x4             ; R0 = R0 + 0x4;(OSTCBNext->OSTCBStkPtr++)
	    ; load R9 
	    LDR     R9 , [R0]                ; R9 = *R0;(R9 = *(OSTCBNext->OSTCBStkPtr))
	    ADD     R0, R0, #0x4             ; R0 = R0 + 0x4;(OSTCBNext->OSTCBStkPtr++)
	    ; load R10 
	    LDR     R10 , [R0]               ; R10 = *R0;(R10 = *(OSTCBNext->OSTCBStkPtr))
	    ADD     R0, R0, #0x4             ; R0 = R0 + 0x4;(OSTCBNext->OSTCBStkPtr++)
	    ; load R11 
	    LDR     R11 , [R0]               ; R11 = *R0;(R11 = *(OSTCBNext->OSTCBStkPtr))
	    ADD     R0, R0, #0x4             ; R0 = R0 + 0x4;(OSTCBNext->OSTCBStkPtr++)
	    
	    MSR     PSP, R0                 ; PSP = R0;(PSP = OSTCBNext->OSTCBStkPtr)
	    ; P42
	    ; P139 (key word: EXC_RETURN)
	    ; use PSP
	    ORR     LR, LR, #0x04           ; LR = LR | 0x04;
	    CPSIE     I                     ; OS_EXIT_CRITICAL();
	    BX    LR                        ; return;
	
OSCtxSw ;OS context switch
	    PUSH    {R4, R5}                
	    LDR     R4, =NVIC_INT_CTRL       ; R4 = NVIC_INT_CTRL
	    LDR     R5, =NVIC_PENDSVSET      ; R5 = NVIC_PENDSVSET
	    STR     R5, [R4]                 ; *R4 = R5
	    POP     {R4, R5}
	    BX     LR                        ; return;
	    
	    align 4
	end