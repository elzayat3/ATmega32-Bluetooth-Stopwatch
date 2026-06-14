#include "TIMER_Int.h"
#include "TIMER_Cfg.h"

/**
 * @file TIMER_Cfg.c
 * @author Abdelrahman Elzayat
 * @brief Timer driver configuration source file for ATmega32.
 *
 * @details
 * This file contains the pre-compile configuration objects for Timer0,
 * Timer1, and Timer2.
 *
 * For the stopwatch project, Timer2 is configured in CTC mode to generate
 * a 1ms interrupt using:
 * - F_CPU = 8 MHz
 * - Prescaler = 64
 * - OCR2 = 124
 *
 * Calculation:
 * Timer frequency = 8,000,000 / 64 = 125,000 Hz
 * Timer tick time = 1 / 125,000 = 8 us
 * Compare match time = 125 ticks * 8 us = 1 ms
 *
 * Since the timer counts from 0 to OCR2, OCR2 = 124 gives 125 ticks.
 */

/* ========================================================================== */
/*                          Timer0 Configuration                              */
/* ========================================================================== */

/**
 * @brief Timer0 configuration.
 *
 * @note Timer0 is currently not used in the stopwatch project.
 */
const TIMER0_Config_t TIMER0_CFG =
{
    .mode          = TIMER0_MODE_NORMAL,
    .prescaler     = TIMER01_NO_CLOCK,
    .oc_mode       = TIMER_OC_DISCONNECTED,
    .pwm_mode      = TIMER_PWM_NON_INVERTING,
    .initial_value = 0U,
    .compare_value = 0U
};

/* ========================================================================== */
/*                          Timer1 Configuration                              */
/* ========================================================================== */

/**
 * @brief Timer1 configuration.
 *
 * @note Timer1 is currently not used in the stopwatch project.
 */
const TIMER1_Config_t TIMER1_CFG =
{
    .mode           = TIMER1_MODE_NORMAL,
    .prescaler      = TIMER01_NO_CLOCK,

    .oc1a_mode      = TIMER1_OC_DISCONNECTED,
    .oc1b_mode      = TIMER1_OC_DISCONNECTED,

    .oc1a_pwm_mode  = TIMER_PWM_NON_INVERTING,
    .oc1b_pwm_mode  = TIMER_PWM_NON_INVERTING,

    .initial_value  = 0U,

    .ocr1a_value    = 0U,
    .ocr1b_value    = 0U,
    .icr1_value     = 0U
};

/* ========================================================================== */
/*                          Timer2 Configuration                              */
/* ========================================================================== */

/**
 * @brief Timer2 configuration.
 *
 * @details
 * Timer2 is used as the main time base for the stopwatch.
 * It is configured in CTC mode to generate an interrupt every 1ms.
 */
const TIMER2_Config_t TIMER2_CFG =
{
    .mode          = TIMER2_MODE_CTC,
    .prescaler     = TIMER2_PRESCALER_64,
    .oc_mode       = TIMER_OC_DISCONNECTED,
    .pwm_mode      = TIMER_PWM_NON_INVERTING,
    .initial_value = 0U,
    .compare_value = 124U
};