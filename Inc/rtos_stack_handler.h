/*
 * rtos_stack_handler.h
 *
 *  Created on: Nov 9, 2020
 *      Author: Sripad
 */

#ifndef RTOS_STACK_HANDLER_H_
#define RTOS_STACK_HANDLER_H_

#include "rtos_headstart.h"
#include "rtos_task_manager.h"

#define SIZE_TASK_STACK    (1024U)
#define SIZE_SCHED_STACK   (1024U)
#define SRAM_START         (0x20000000U)
#define SIZE_SRAM          ((128)*(1024))
#define SRAM_END           (SRAM_START + SIZE_SRAM)
#define STACK_START_SCHED  (SRAM_END)
#define STACK_START_TASK   (STACK_START_SCHED - SIZE_SCHED_STACK)
#define EXC_RETURN         (0xFFFFFFFDU) /* EXC_RETURN : Thread Mode	+ Execution uses PSP*/
#define DUMMY_xPSR         (0x01000000U) /* Indicate Thumb State by setting Bit 24 */
#define MAX_REG_STORE      (13U)

bool Create_Stack_Space( uint32_t task_stack_size, uint8_t task_id);
bool Init_Task_Handlers( TaskHandlerPtr_type task_handler, uint8_t task_id);
void Init_Task_Stack(void);
__attribute__((naked)) void Init_Sched_Stack(uint32_t Scheduler_TOS);
__attribute__((naked)) void Switch_SP_to_PSP(void);
uint32_t Get_New_Task_TOS(void);
void Save_Current_Task_TOS(uint32_t current_task_tos);

#endif /* RTOS_STACK_HANDLER_H_ */
