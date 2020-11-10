/*
 * rtos_task_manager.h
 *
 *  Created on: Nov 9, 2020
 *      Author: Sripad
 */

#ifndef RTOS_TASK_MANAGER_H_
#define RTOS_TASK_MANAGER_H_

#include "rtos_headstart.h"

typedef int (*TaskHandlerPtr_type) (void) ;

bool RTOS_Create_Task(uint32_t stack_size, TaskHandlerPtr_type task_handler_func, uint8_t task_id);

void RTOS_Operate (void);

void RTOS_Start(void);

#endif /* RTOS_TASK_MANAGER_H_ */
