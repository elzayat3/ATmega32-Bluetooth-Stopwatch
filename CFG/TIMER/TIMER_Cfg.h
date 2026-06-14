#ifndef TIMER_CFG_H_
#define TIMER_CFG_H_

/**
 * @file TIMER_Cfg.h
 * @author Abdelrahman Elzayat
 * @brief Timer driver configuration header file for ATmega32.
 *
 * @details
 * This file contains the external declarations of Timer0, Timer1,
 * and Timer2 configuration objects.
 *
 * The actual configuration values are defined in TIMER_Cfg.c.
 */

#include "TIMER_Int.h"

/* ========================================================================== */
/*                      External Configuration Declarations                    */
/* ========================================================================== */

/**
 * @brief External Timer0 configuration object.
 *
 * This object contains Timer0 mode, prescaler, output compare mode,
 * initial value, and compare value.
 */
extern const TIMER0_Config_t TIMER0_CFG;

/**
 * @brief External Timer1 configuration object.
 *
 * This object contains Timer1 mode, prescaler, output compare modes,
 * initial value, compare values, and ICR1 value.
 */
extern const TIMER1_Config_t TIMER1_CFG;

/**
 * @brief External Timer2 configuration object.
 *
 * This object contains Timer2 mode, prescaler, output compare mode,
 * initial value, and compare value.
 */
extern const TIMER2_Config_t TIMER2_CFG;

#endif /* TIMER_CFG_H_ */