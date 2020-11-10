/*
 * exception_handler.c
 *
 *  Created on: ನವೆಂ 5, 2020
 *      Author: Sripad
 */

#include "exception_handler.h"
#include "rtos_stack_handler.h"
#include "rtos_task_manager.h"

void Init_SysTick_Timer(uint32_t tick_hz)
{
	/* Count Value to be loaded into the SYSTICK Reload register */
	uint32_t count_value = (SYSTICK_TIM_CLK/tick_hz) - 1;

	uint32_t *pSYST_RVR = (uint32_t *)SYST_RVR_ADDR;
	uint32_t *pSYST_CVR = (uint32_t *)SYST_CVR_ADDR;
	uint32_t *pSYST_CSR = (uint32_t *)SYST_CSR_ADDR;

	/* Clear the reload value register */
	*pSYST_RVR = 0x00000000U ;

	/* load the count value into the reload register */
	*pSYST_RVR = count_value;

	/* Clear the current value register */
	*pSYST_CVR = 0x00000000U ;

	/* Use Processor Clock source, enable the SysTick Interrupt, and enable the counter*/
	*pSYST_CSR = (*pSYST_CSR | PROC_CLKSOURCE | TICKINT  | ENABLE_CNTR) ;

}

void Enable_Exceptions(void)
{

	uint32_t *pSHSCR = (uint32_t *)SHCSR_ADDR;

	/* Enable MemManage Fault, Bus fault, and Usage fault */
	*pSHSCR = (*pSHSCR | MEMFAULTENA | BUSFAULTENA | USGFAULTENA) ;

}

__attribute__((naked)) void SysTick_Handler (void)
{

	/*-- Save the context of current Task --*/

	/* Get current running task's PSP value */
	__asm volatile (" MRS R0, PSP");

	/* Store R4-R11 registers ion current task stack*/
	__asm volatile (" STMDB R0!, {R4-R11}");

	/* Preserve LR (Due to epilogue & prologue sequence)*/
	__asm volatile ("PUSH {LR}");

	/* Save the current value of PSP which is in R0 above*/
	__asm volatile("BL Save_Current_Task_TOS");


	/*-- Retrieve the context of next task --*/

	/* Roll over to the next task in line */
	__asm volatile("BL RTOS_Operate");

	/* Get last stored PSP value */
	__asm volatile("BL Get_New_Task_TOS");

	/* Retrieve R4-R11 from the PSP */
	__asm volatile("LDMIA R0!, {R4-R11}");

	/* Update PSP and Exit */
	__asm volatile (" MSR PSP, R0");

	/* Retrieve LR */
	__asm volatile ("POP {LR}");

	/* Load EXEC_Return to PC to exit exception */
	__asm volatile ("BX LR");


}

void BusFault_Handler (void)
{
	printf("Exception Handler - Bus Fault\n");
	while(1){;}
}

void MemManage_Handler (void)
{
	printf("Exception Handler - Mem Manage Fault\n");
	while(1){;}
}

void UsageFault_Handler (void)
{
	printf("Exception Handler - Usage Fault\n");
	while(1){;}
}
