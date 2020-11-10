/*
 * rtos_stack_handler.c
 *
 *  Created on: ನವೆಂ 5, 2020
 *      Author: Sripad
 */

#include "rtos_stack_handler.h"
#include "rtos_task_manager.h"

static uint32_t TASK_STACK_TOS = STACK_START_TASK;
static uint32_t PSP_OF_TASKS[MAX_TASK];
static TaskHandlerPtr_type TASK_HANDLER_PC_INIT[MAX_TASK];
extern uint32_t TASK_HANDLERS[MAX_TASK];
extern uint32_t Current_Task ;

/*------------------------------------------------------------
|
| Function Name: Init_Sched_Stack
| Function Description: This function initializes the scheduler stack to TOS
|
|-------------------------------------------------------------
|
| Function Inputs:
| uint32_t Scheduler_TOS : Scheduler Top of Stack
|
|-------------------------------------------------------------
|
| Function Output:
| void : None
|
-------------------------------------------------------------*/
__attribute__((naked)) void Init_Sched_Stack(uint32_t Scheduler_TOS)
{
	__asm volatile("MSR MSP, %0": : "r"(Scheduler_TOS): );
	__asm volatile("BX LR");

}

/*------------------------------------------------------------
|
| Function Name: Create_Stack_Space
| Function Description: Reserves Stack Space for give Task
|
|-------------------------------------------------------------
|
| Function Inputs:
| uint32_t task_stack_size : Stack Size for the task
|
| uint8_t task_id : Task Identifier
|
|-------------------------------------------------------------
|
| Function Output:
| bool : TRUE  - Successfully reserved task stack space
|        FALSE - Failed to reserve task stack space
|
|-------------------------------------------------------------*/
bool Create_Stack_Space( uint32_t task_stack_size, uint8_t task_id)
{
	bool successful_init = false;

	if(task_id < MAX_TASK)
	{
		PSP_OF_TASKS[task_id] = TASK_STACK_TOS ;
		TASK_STACK_TOS = TASK_STACK_TOS - task_stack_size;
		successful_init = true;
	}

	return (successful_init);
}

/*------------------------------------------------------------
|
| Function Name: Init_Task_Handlers
| Function Description: Initializes PC to point to task handler
|
|-------------------------------------------------------------
|
| Function Inputs:
| TaskHandlerPtr_type task_handler : Pointer to task handler function
|
| uint8_t task_id : Task Identifier
|
|-------------------------------------------------------------
|
| Function Output:
| bool : TRUE: Successfully initialized the PC
|
|-------------------------------------------------------------*/
bool Init_Task_Handlers( TaskHandlerPtr_type task_handler, uint8_t task_id)
{
	bool successful_init = false;

	if(task_id < MAX_TASK)
	{
		TASK_HANDLER_PC_INIT[task_id] = task_handler;
		successful_init = true;
	}

	return (successful_init);
}

/*------------------------------------------------------------
|
| Function Name: Init_Task_Stack
| Function Description: Initializes stack space before first
| run with following data:
| 1. TOS for each task,
| 2. Dummy xPSR,
| 3. Task Handler entry point for PC,
| 4. EXEC_RETURN,
| 5. Setting General Purpose registers to 0,
| 6. Updates the TOS
|
|-------------------------------------------------------------
|
| Function Inputs:
| void : None
|
|-------------------------------------------------------------
|
| Function Output:
| void : None
|
|-------------------------------------------------------------*/
void Init_Task_Stack(void)
{
	uint32_t *pPSP = NULL;
	uint8_t task_id = 0;
	uint8_t reg_index = 0;

	for(task_id = 0; task_id < MAX_TASK;  task_id++)
	{
		pPSP = (uint32_t *)PSP_OF_TASKS[task_id];

		pPSP--;
		/* Init xPSR */
		*pPSP = DUMMY_xPSR; //(0x01000000U);

		pPSP--;
		/* Init PC */
		*pPSP = (int)TASK_HANDLER_PC_INIT[task_id];

		pPSP--;
		/* Init LR */
		*pPSP = EXC_RETURN;

		for(reg_index = 0; reg_index < MAX_REG_STORE; reg_index++)
		{
			pPSP--;
			/* Init R12, R3, R2, R1, R0, R11, R10, R9, R8, R7, R6, R5, R4 */
			*pPSP = 0x0;
		}

		PSP_OF_TASKS[task_id] = (uint32_t)pPSP;

	}

}

/*------------------------------------------------------------
|
| Function Name: Switch_SP_to_PSP
| Function Description: Gets current PSP Task TOS, switches
| from MSP to PSP.
|
|-------------------------------------------------------------
|
| Function Inputs:
| void : None
|
|-------------------------------------------------------------
|
| Function Output:
| void : None
|
|-------------------------------------------------------------*/
__attribute__((naked)) void Switch_SP_to_PSP(void)
{
	/*Initialise the PSP with Task-1 stack start address*/

	/* Preserve LR (Due to epilogue & prologue sequence)*/
	__asm volatile ("PUSH {LR}");
	/* Get the current PSP */
	__asm volatile ("BL Get_PSP_Value");
	/* Initialise PSP */
	__asm volatile (" MSR PSP, R0");
	/* Retrieve LR */
	__asm volatile ("POP {LR}");

	/* Switch SP from MSP to PSP using CONTROL register */

	/* Set Bit-2 and Move into R0 */
	__asm volatile ("MOV R0, 0x02");
	/* Move R0 into CONTROL Register */
	__asm volatile ("MSR CONTROL, R0");
	/*Branch to Main */
	__asm volatile ("BX LR");

}

/*------------------------------------------------------------
|
| Function Name: Get_PSP_Value
| Function Description: Returns TOS of task to be executed.
|
|-------------------------------------------------------------
|
| Function Inputs:
| void : None
|
|-------------------------------------------------------------
|
| Function Output:
| uint32_t TOS : Top of stack of task to be executed
|
|-------------------------------------------------------------*/
uint32_t Get_PSP_Value(void)
{
	return PSP_OF_TASKS[Current_Task];
}

/*------------------------------------------------------------
|
| Function Name: Save_PSP_Value
| Function Description: Save the TOS of the task that finished
| execution.
|
|-------------------------------------------------------------
|
| Function Inputs:
| uint32_t current_task_PSP : TOS of the task that finished
| execution.
|
|-------------------------------------------------------------
|
| Function Output:
| void : None
|
|-------------------------------------------------------------*/
void Save_PSP_Value(uint32_t current_task_PSP)
{
	PSP_OF_TASKS[Current_Task] = current_task_PSP;
}
