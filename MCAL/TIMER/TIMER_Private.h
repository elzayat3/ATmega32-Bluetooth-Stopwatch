#ifndef TIMER_PRIVATE_H_
#define TIMER_PRIVATE_H_

/**
 * @file TIMER_Private.h
 * @author Abdelrahman Elzayat
 * @brief Private helper macros for ATmega32 Timer driver.
 *
 * @details
 * This file contains internal macros used only by the Timer driver
 * implementation file. It provides register access helpers, bit masks,
 * and interrupt flag clearing macros for Timer0, Timer1, and Timer2.
 *
 * @note This file should not be included by the application layer.
 */

#include "StdTypes.h"
#include "MemMap.h"
#include "TIMER_Int.h"

/* ========================================================================== */
/*                           Timer Counter Registers                          */
/* ========================================================================== */

/**
 * @brief Write value to Timer0 counter register.
 */
#define TIMER0_SET_COUNTER(value)      (TCNT0 = (u8)(value))

/**
 * @brief Write value to Timer1 counter register.
 */
#define TIMER1_SET_COUNTER(value)      (TCNT1 = (u16)(value))

/**
 * @brief Write value to Timer2 counter register.
 */
#define TIMER2_SET_COUNTER(value)      (TCNT2 = (u8)(value))

/**
 * @brief Read Timer0 counter register.
 */
#define TIMER0_GET_COUNTER()           (TCNT0)

/**
 * @brief Read Timer1 counter register.
 */
#define TIMER1_GET_COUNTER()           (TCNT1)

/**
 * @brief Read Timer2 counter register.
 */
#define TIMER2_GET_COUNTER()           (TCNT2)

/* ========================================================================== */
/*                         Output Compare Registers                           */
/* ========================================================================== */

/**
 * @brief Write value to Timer0 output compare register.
 */
#define TIMER0_SET_COMPARE(value)      (OCR0 = (u8)(value))

/**
 * @brief Write value to Timer1 Output Compare A register.
 */
#define TIMER1_SET_COMPARE_A(value)    (OCR1A = (u16)(value))

/**
 * @brief Write value to Timer1 Output Compare B register.
 */
#define TIMER1_SET_COMPARE_B(value)    (OCR1B = (u16)(value))

/**
 * @brief Write value to Timer2 output compare register.
 */
#define TIMER2_SET_COMPARE(value)      (OCR2 = (u8)(value))

/**
 * @brief Read Timer0 output compare register.
 */
#define TIMER0_GET_COMPARE()           (OCR0)

/**
 * @brief Read Timer1 Output Compare A register.
 */
#define TIMER1_GET_COMPARE_A()         (OCR1A)

/**
 * @brief Read Timer1 Output Compare B register.
 */
#define TIMER1_GET_COMPARE_B()         (OCR1B)

/**
 * @brief Read Timer2 output compare register.
 */
#define TIMER2_GET_COMPARE()           (OCR2)

/* ========================================================================== */
/*                         Timer1 Input Capture Register                       */
/* ========================================================================== */

/**
 * @brief Write value to Timer1 Input Capture register.
 */
#define TIMER1_SET_ICR1(value)         (ICR1 = (u16)(value))

/**
 * @brief Read Timer1 Input Capture register.
 */
#define TIMER1_GET_ICR1()              (ICR1)

/* ========================================================================== */
/*                               Bit Masks                                    */
/* ========================================================================== */

/**
 * @brief Mask for timer clock select bits.
 *
 * @details
 * This mask is used with:
 * - TCCR0  bits CS00:CS02
 * - TCCR1B bits CS10:CS12
 * - TCCR2  bits CS20:CS22
 */
#define TIMER_CLOCK_SELECT_MASK        ((u8)0x07U)

/**
 * @brief Mask for Timer0 waveform generation bits.
 */
#define TIMER0_WGM_MASK                ((u8)((1U << WGM00) | (1U << WGM01)))

/**
 * @brief Mask for Timer2 waveform generation bits.
 */
#define TIMER2_WGM_MASK                ((u8)((1U << WGM20) | (1U << WGM21)))

/**
 * @brief Mask for Timer1 waveform generation bits in TCCR1A.
 */
#define TIMER1_WGM_TCCR1A_MASK         ((u8)((1U << WGM10) | (1U << WGM11)))

/**
 * @brief Mask for Timer1 waveform generation bits in TCCR1B.
 */
#define TIMER1_WGM_TCCR1B_MASK         ((u8)((1U << WGM12) | (1U << WGM13)))

/**
 * @brief Mask for Timer0 Compare Output Mode bits.
 */
#define TIMER0_COM_MASK                ((u8)((1U << COM00) | (1U << COM01)))

/**
 * @brief Mask for Timer2 Compare Output Mode bits.
 */
#define TIMER2_COM_MASK                ((u8)((1U << COM20) | (1U << COM21)))

/**
 * @brief Mask for Timer1 Compare Output Mode bits for channel A.
 */
#define TIMER1_COM1A_MASK              ((u8)((1U << COM1A0) | (1U << COM1A1)))

/**
 * @brief Mask for Timer1 Compare Output Mode bits for channel B.
 */
#define TIMER1_COM1B_MASK              ((u8)((1U << COM1B0) | (1U << COM1B1)))

/* ========================================================================== */
/*                           Interrupt Flag Macros                             */
/* ========================================================================== */

/**
 * @brief Clear Timer0 Overflow flag.
 *
 * @note AVR interrupt flags are cleared by writing logical one to them.
 */
#define TIMER0_CLEAR_OVF_FLAG()        (SET_BIT(TIFR, TOV0))

/**
 * @brief Clear Timer0 Compare Match flag.
 *
 * @note AVR interrupt flags are cleared by writing logical one to them.
 */
#define TIMER0_CLEAR_COMP_FLAG()       (SET_BIT(TIFR, OCF0))

/**
 * @brief Clear Timer1 Overflow flag.
 *
 * @note AVR interrupt flags are cleared by writing logical one to them.
 */
#define TIMER1_CLEAR_OVF_FLAG()        (SET_BIT(TIFR, TOV1))

/**
 * @brief Clear Timer1 Compare Match A flag.
 *
 * @note AVR interrupt flags are cleared by writing logical one to them.
 */
#define TIMER1_CLEAR_COMPA_FLAG()      (SET_BIT(TIFR, OCF1A))

/**
 * @brief Clear Timer1 Compare Match B flag.
 *
 * @note AVR interrupt flags are cleared by writing logical one to them.
 */
#define TIMER1_CLEAR_COMPB_FLAG()      (SET_BIT(TIFR, OCF1B))

/**
 * @brief Clear Timer1 Input Capture flag.
 *
 * @note AVR interrupt flags are cleared by writing logical one to them.
 */
#define TIMER1_CLEAR_ICU_FLAG()        (SET_BIT(TIFR, ICF1))

/**
 * @brief Clear Timer2 Overflow flag.
 *
 * @note AVR interrupt flags are cleared by writing logical one to them.
 */
#define TIMER2_CLEAR_OVF_FLAG()        (SET_BIT(TIFR, TOV2))

/**
 * @brief Clear Timer2 Compare Match flag.
 *
 * @note AVR interrupt flags are cleared by writing logical one to them.
 */
#define TIMER2_CLEAR_COMP_FLAG()       (SET_BIT(TIFR, OCF2))

#endif /* TIMER_PRIVATE_H_ */