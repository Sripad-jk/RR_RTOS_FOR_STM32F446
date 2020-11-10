/*
 * rtos_task_manager.c
 *
 *  Created on: ನವೆಂ 9, 2020
 *      Author: Sripad
 */


#include "rtos_task_manager.h"
#include "rtos_stack_handler.h"

uint32_t EXECUTING_TASK = 0;

//TaskHandlerPtr_type TASK_HANDLER_REFERENCE[MAX_TASK];

TaskHandlerPtr_type HIGH_PRIORITY_TASK ;
/*------------------------------------------------------------
|
| Function Name: RTOS_Create_Task
| Function Description: This function processes stack space
| requirement & saves the task handler reference.
|
|-------------------------------------------------------------
|
| Function Inputs:
| uint32_t stack_size : size of stack for the particular task
| being created.
|
| TaskHandlerPtr_type task_handler_func : Reference of Task
| Handler Function
|
| uint8_t task_id : Task Identifier
|
|-------------------------------------------------------------
|
| Function Output:
| bool : TRUE: Successfully created the task,
|        FALSE: Task creation failed.
|
|-------------------------------------------------------------*/
bool RTOS_Create_Task(uint32_t stack_size, TaskHandlerPtr_type task_handler_func, uint8_t task_id)
{
	bool stack_reserved = false;
	bool task_handler_initialised = false;

	stack_reserved = Create_Stack_Space(stack_size, task_id);

	if(true == stack_reserved)
	{
		task_handler_initialised = Init_Task_Handlers(task_handler_func, task_id);

		if(T1 == task_id)
		{
			HIGH_PRIORITY_TASK = task_handler_func;
		}
	}

    return task_handler_initialised;

}

/*------------------------------------------------------------
|
| Function Name: RTOS_Operate
| Function Description: Rolls over to the next task in line
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
void RTOS_Operate (void)
{
	EXECUTING_TASK = EXECUTING_TASK  + 1;
	EXECUTING_TASK = EXECUTING_TASK % MAX_TASK;
}

/*------------------------------------------------------------
|
| Function Name: RTOS_Start
| Function Description: Starts the OS by loading the first task
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
void RTOS_Start(void)
{
	(*HIGH_PRIORITY_TASK)();
}
