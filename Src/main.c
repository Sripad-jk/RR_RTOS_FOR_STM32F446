/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

#include "rtos_headstart.h"
#include "exception_handler.h"
#include "rtos_task_manager.h"
#include "rtos_stack_handler.h"


static bool print_t1 = true;
static bool print_t2 = true;
static bool print_t3 = true;
static bool print_t4 = true;

void Task1_Handler(void);
void Task2_Handler(void);
void Task3_Handler(void);
void Task4_Handler(void);

int main(void)
{
    Enable_Exceptions();
    Init_Sched_Stack(STACK_START_SCHED);

    RTOS_Create_Task(TASK_STACK_SIZE_1K, Task1_Handler, T1);
    RTOS_Create_Task(TASK_STACK_SIZE_1K, Task2_Handler, T2);
    RTOS_Create_Task(TASK_STACK_SIZE_1K, Task3_Handler, T3);
    RTOS_Create_Task(TASK_STACK_SIZE_1K, Task4_Handler, T4);

	Init_Task_Stack();

	Init_SysTick_Timer(TICK_FREQ);

	Switch_SP_to_PSP();

	RTOS_Start();

    /* Loop forever */
	for(;;);
}

void Task1_Handler(void)
{

 while(1)
 {
   if(true == print_t1)
	{
	   printf(" This is Task - 1\n");
	   print_t1 = false;
	   print_t2 = true;
	   print_t3 = true;
	   print_t4 = true;
	}
 }

}

void Task2_Handler(void)
{

 while(1)
 {
   if(true == print_t2)
	{
	   printf(" This is Task - 2\n");
	   print_t2 = false;
	   print_t1 = true;
	   print_t3 = true;
	   print_t4 = true;
	}
 }

}

void Task3_Handler(void)
{

 while(1)
 {
	if(true == print_t3)
	{
	   printf(" This is Task - 3\n");
	   print_t3 = false;
	   print_t1 = true;
	   print_t2 = true;
	   print_t4 = true;
	}
 }


}

void Task4_Handler(void)
{

 while(1)
 {
	if(true == print_t4)
	{
	   printf(" This is Task - 4\n");
	   print_t4 = false;
	   print_t1 = true;
	   print_t2 = true;
	   print_t3 = true;
	}
 }

}
