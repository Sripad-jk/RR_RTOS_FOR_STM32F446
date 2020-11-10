/*
 * rtos_headstart.h
 *
 *  Created on: ನವೆಂ 8, 2020
 *      Author: Sripad
 */

#ifndef RTOS_HEADSTART_H_
#define RTOS_HEADSTART_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/* Max Number of Task Created By the User */
#define MAX_TASK           (4U)

/* Tick Frequency */
#define TICK_FREQ          (2U)
#define TASK_STACK_SIZE_1K (1024U)

#define T1                 (0U)
#define T2                 (1U)
#define T3                 (2U)
#define T4                 (3U)

typedef enum
{

	TASK_DORMANT,
	//TASK_READY,
	TASK_RUNNING,
	//TASK_PEND,
	//TASK_INTERRUPTED,

}RTOS_TASK_STATE_t;

#endif /* RTOS_HEADSTART_H_ */
