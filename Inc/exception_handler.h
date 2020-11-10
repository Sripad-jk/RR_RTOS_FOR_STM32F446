/*
 * exception_handler.h
 *
 *  Created on: Nov 9, 2020
 *      Author: Sripad
 */

#ifndef EXCEPTION_HANDLER_H_
#define EXCEPTION_HANDLER_H_

#include "rtos_headstart.h"

#define HSI_CLOCK          (16000000U) /* HSI Clock is 16MHz */
#define SYSTICK_TIM_CLK    (HSI_CLOCK)
#define SYST_CSR_ADDR      (0xE000E010U) /* SysTick Control and Status Register */
#define SYST_RVR_ADDR      (0xE000E014U) /* SysTick Reload Value Register */
#define SYST_CVR_ADDR      (0xE000E018U) /* SysTick Current Value Register */
#define SHCSR_ADDR         (0xE000ED24U) /* System handler control and state register */

#define PROC_CLKSOURCE     (0x4U)
#define TICKINT            (0x2U)
#define ENABLE_CNTR        (0x1U)

#define MEMFAULTENA        (0x10000U)
#define BUSFAULTENA        (0x20000U)
#define USGFAULTENA        (0x40000U)


void Init_SysTick_Timer(uint32_t tick_hz);
void Enable_Exceptions(void);
__attribute__((naked)) void SysTick_Handler (void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);

#endif /* EXCEPTION_HANDLER_H_ */
