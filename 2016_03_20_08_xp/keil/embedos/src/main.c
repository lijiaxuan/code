#include "stm32f10x.h"

#include "../inc/includes.h"
void Task1(void *p_arg);
void Task2(void *p_arg);
void Task3(void *p_arg);

OS_STK Task1Stk[TASK_STACK_SIZE];
OS_STK Task2Stk[TASK_STACK_SIZE];
OS_STK Task3Stk[TASK_STACK_SIZE];

int main()
{
    OSInit();  // OS initialization
    OSTaskCreate(Task1, (void*)0, (OS_STK*)&Task1Stk[TASK_STACK_SIZE-1]); // create task 1
    OSTaskCreate(Task2, (void*)0, (OS_STK*)&Task2Stk[TASK_STACK_SIZE-1]); // create task 2
    OSTaskCreate(Task3, (void*)0, (OS_STK*)&Task3Stk[TASK_STACK_SIZE-1]); // create task 3
    SysTickInit(5); // configure the SysTick as 5ms
    LedInit(); // leds initialization
    OSStart(); // start os!
    
    return 0; // never come here
}
void Task1(void *p_arg)
{
    while(1) {
        delayMs(100); // delay 100 * 5ms = 0.5s
        LED1TURN(); // flip the switch of led1
    }
}
void Task2(void *p_arg)
{
    while(1) {
        delayMs(200); // delay 200 * 5ms = 1.0s
        LED2TURN(); // flip the switch of led2 
    }
}

void Task3(void *p_arg)
{
    while(1) {
    }
}