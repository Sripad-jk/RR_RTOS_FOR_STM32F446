/*
 * rtos_stack_handler.c
 *
 *  Created on: ನವೆಂ 5, 2020
 *      Author: Sripad
 */

#include "rtos_stack_handler.h"
#include "rtos_task_manager.h"

typedef struct
{
	uint32_t Task_TOS;
	uint32_t Block_Count;
	RTOS_TASK_STATE_t Task_State;
	TaskHandlerPtr_type Task_Handler_Ref;

}RTOS_TCB_t;

RTOS_TCB_t Rtos_TCB[MAX_TASK];

static uint32_t TASK_STACK_TOS = STACK_START_TASK;
extern uint32_t EXECUTING_TASK ;

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
		Rtos_TCB[task_id].Task_TOS = TASK_STACK_TOS ;
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
		Rtos_TCB[task_id].Task_Handler_Ref = task_handler;
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
	uint32_t *ptr_TASK_TOS = NULL;
	uint8_t task_id = 0;
	uint8_t reg_index = 0;

	for(task_id = 0; task_id < MAX_TASK;  task_id++)
	{
		Rtos_TCB[task_id].Task_State = TASK_RUNNING;

		/* Load the Tasks Top of stack */
		ptr_TASK_TOS = (uint32_t *)Rtos_TCB[task_id].Task_TOS;

		ptr_TASK_TOS--;
		/* Init xPSR */
		*ptr_TASK_TOS = DUMMY_xPSR; //(0x01000000U);

		ptr_TASK_TOS--;
		/* Init PC */
		*ptr_TASK_TOS = (uint32_t)Rtos_TCB[task_id].Task_Handler_Ref;

		ptr_TASK_TOS--;
		/* Init LR */
		*ptr_TASK_TOS = EXC_RETURN;

		for(reg_index = 0; reg_index < MAX_REG_STORE; reg_index++)
		{
			ptr_TASK_TOS--;
			/* Init R12, R3, R2, R1, R0, R11, R10, R9, R8, R7, R6, R5, R4 */
			*ptr_TASK_TOS = 0x0;
		}

		Rtos_TCB[task_id].Task_TOS = (uint32_t)ptr_TASK_TOS;

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
	__asm volatile ("BL Get_New_Task_TOS");
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
| Function Name: Get_New_Task_TOS
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
uint32_t Get_New_Task_TOS(void)
{
	return Rtos_TCB[EXECUTING_TASK].Task_TOS;
}

/*------------------------------------------------------------
|
| Function Name: Save_Current_Task_TOS
| Function Description: Save the TOS of the task that finished
| execution.
|
|-------------------------------------------------------------
|
| Function Inputs:
| uint32_t current_task_tos : TOS of the task that finished
| execution.
|
|-------------------------------------------------------------
|
| Function Output:
| void : None
|
|-------------------------------------------------------------*/
void Save_Current_Task_TOS(uint32_t current_task_tos)
{
	Rtos_TCB[EXECUTING_TASK].Task_TOS = current_task_tos;
}
