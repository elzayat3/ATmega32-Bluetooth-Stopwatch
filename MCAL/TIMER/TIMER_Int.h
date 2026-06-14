#ifndef TIMER_INT_H_
#define TIMER_INT_H_

/**
 * @file TIMER_Int.h
 * @author Abdelrahman Elzayat
 * @brief Public interface for ATmega32 Timer driver.
 *
 * @details
 * This driver provides a configurable interface for Timer0, Timer1, and Timer2
 * in ATmega32. It supports:
 * - Normal mode
 * - CTC mode
 * - Fast PWM mode
 * - Phase Correct PWM mode
 * - Timer interrupts
 * - Callback registration
 * - Timer1 Input Capture Unit ICU
 *
 * @note Timer0 and Timer1 have the same clock select mapping.
 * @note Timer2 has a different prescaler mapping, so it has a separate prescaler enum.
 * @note Timers are initialized stopped. Use TIMER_Start() to start a selected timer.
 */

#include "StdTypes.h"

/* ========================================================================== */
/*                               Common Types                                 */
/* ========================================================================== */

/**
 * @enum TIMER_Channel_t
 * @brief Identifies the selected timer peripheral.
 */
typedef enum
{
    TIMER0 = 0, /**< Timer0, 8-bit timer */
    TIMER1,     /**< Timer1, 16-bit timer */
    TIMER2      /**< Timer2, 8-bit timer */
} TIMER_Channel_t;

/**
 * @enum TIMER01_Prescaler_t
 * @brief Clock source and prescaler options for Timer0 and Timer1.
 *
 * @details
 * These values map directly to the CS00:CS02 bits for Timer0
 * and CS10:CS12 bits for Timer1.
 */
typedef enum
{
    TIMER01_NO_CLOCK = 0,      /**< Timer stopped */
    TIMER01_PRESCALER_1,       /**< Timer clock = F_CPU / 1 */
    TIMER01_PRESCALER_8,       /**< Timer clock = F_CPU / 8 */
    TIMER01_PRESCALER_64,      /**< Timer clock = F_CPU / 64 */
    TIMER01_PRESCALER_256,     /**< Timer clock = F_CPU / 256 */
    TIMER01_PRESCALER_1024,    /**< Timer clock = F_CPU / 1024 */
    TIMER01_EXTERNAL_FALLING,  /**< External clock source on falling edge */
    TIMER01_EXTERNAL_RISING    /**< External clock source on rising edge */
} TIMER01_Prescaler_t;

/**
 * @enum TIMER2_Prescaler_t
 * @brief Clock source and prescaler options for Timer2.
 *
 * @details
 * Timer2 in ATmega32 has a different prescaler mapping from Timer0 and Timer1.
 * For example, value 3 means F_CPU / 32 in Timer2, but F_CPU / 64 in Timer0/1.
 *
 * @warning Do not use TIMER01_Prescaler_t with Timer2.
 */
typedef enum
{
    TIMER2_NO_CLOCK = 0,    /**< Timer stopped */
    TIMER2_PRESCALER_1,     /**< Timer clock = F_CPU / 1 */
    TIMER2_PRESCALER_8,     /**< Timer clock = F_CPU / 8 */
    TIMER2_PRESCALER_32,    /**< Timer clock = F_CPU / 32 */
    TIMER2_PRESCALER_64,    /**< Timer clock = F_CPU / 64 */
    TIMER2_PRESCALER_128,   /**< Timer clock = F_CPU / 128 */
    TIMER2_PRESCALER_256,   /**< Timer clock = F_CPU / 256 */
    TIMER2_PRESCALER_1024   /**< Timer clock = F_CPU / 1024 */
} TIMER2_Prescaler_t;

/**
 * @enum TIMER_PWM_Mode_t
 * @brief PWM output compare behavior.
 */
typedef enum
{
    TIMER_PWM_NON_INVERTING = 0, /**< Non-inverting PWM mode */
    TIMER_PWM_INVERTING          /**< Inverting PWM mode */
} TIMER_PWM_Mode_t;

/**
 * @enum TIMER_OC_Mode_t
 * @brief Output compare behavior for Timer0 and Timer2 in non-PWM modes.
 */
typedef enum
{
    TIMER_OC_DISCONNECTED = 0, /**< OC pin disconnected */
    TIMER_OC_TOGGLE,           /**< Toggle OC pin on compare match */
    TIMER_OC_CLEAR,            /**< Clear OC pin on compare match */
    TIMER_OC_SET               /**< Set OC pin on compare match */
} TIMER_OC_Mode_t;

/* ========================================================================== */
/*                                Timer0 Types                                */
/* ========================================================================== */

/**
 * @enum TIMER0_Mode_t
 * @brief Timer0 operating mode selection.
 */
typedef enum
{
    TIMER0_MODE_NORMAL = 0,       /**< Normal overflow mode */
    TIMER0_MODE_CTC,              /**< Clear Timer on Compare Match mode */
    TIMER0_MODE_FAST_PWM,         /**< Fast PWM mode */
    TIMER0_MODE_PHASE_CORRECT_PWM /**< Phase Correct PWM mode */
} TIMER0_Mode_t;

/**
 * @struct TIMER0_Config_t
 * @brief Timer0 pre-compile configuration structure.
 */
typedef struct
{
    TIMER0_Mode_t mode;             /**< Timer0 operating mode */
    TIMER01_Prescaler_t prescaler;  /**< Timer0 clock source / prescaler */
    TIMER_OC_Mode_t oc_mode;        /**< OC0 behavior in Normal/CTC modes */
    TIMER_PWM_Mode_t pwm_mode;      /**< OC0 behavior in PWM modes */
    u8 initial_value;               /**< Initial value loaded into TCNT0 */
    u8 compare_value;               /**< Compare value loaded into OCR0 */
} TIMER0_Config_t;

/* ========================================================================== */
/*                                Timer2 Types                                */
/* ========================================================================== */

/**
 * @enum TIMER2_Mode_t
 * @brief Timer2 operating mode selection.
 */
typedef enum
{
    TIMER2_MODE_NORMAL = 0,       /**< Normal overflow mode */
    TIMER2_MODE_CTC,              /**< Clear Timer on Compare Match mode */
    TIMER2_MODE_FAST_PWM,         /**< Fast PWM mode */
    TIMER2_MODE_PHASE_CORRECT_PWM /**< Phase Correct PWM mode */
} TIMER2_Mode_t;

/**
 * @struct TIMER2_Config_t
 * @brief Timer2 pre-compile configuration structure.
 */
typedef struct
{
    TIMER2_Mode_t mode;             /**< Timer2 operating mode */
    TIMER2_Prescaler_t prescaler;   /**< Timer2 clock source / prescaler */
    TIMER_OC_Mode_t oc_mode;        /**< OC2 behavior in Normal/CTC modes */
    TIMER_PWM_Mode_t pwm_mode;      /**< OC2 behavior in PWM modes */
    u8 initial_value;               /**< Initial value loaded into TCNT2 */
    u8 compare_value;               /**< Compare value loaded into OCR2 */
} TIMER2_Config_t;

/* ========================================================================== */
/*                                Timer1 Types                                */
/* ========================================================================== */

/**
 * @enum TIMER1_Mode_t
 * @brief Timer1 operating mode selection.
 */
typedef enum
{
    TIMER1_MODE_NORMAL = 0,           /**< Normal overflow mode */
    TIMER1_MODE_CTC_OCR1A,            /**< CTC mode with OCR1A as TOP */
    TIMER1_MODE_CTC_ICR1,             /**< CTC mode with ICR1 as TOP */
    TIMER1_MODE_FAST_PWM_ICR1,        /**< Fast PWM mode with ICR1 as TOP */
    TIMER1_MODE_FAST_PWM_OCR1A,       /**< Fast PWM mode with OCR1A as TOP */
    TIMER1_MODE_PHASE_CORRECT_ICR1    /**< Phase Correct PWM mode with ICR1 as TOP */
} TIMER1_Mode_t;

/**
 * @enum TIMER1_OC_Mode_t
 * @brief Output compare behavior for Timer1 channels in non-PWM modes.
 */
typedef enum
{
    TIMER1_OC_DISCONNECTED = 0, /**< OC1A/OC1B pin disconnected */
    TIMER1_OC_TOGGLE,           /**< Toggle OC1A/OC1B on compare match */
    TIMER1_OC_CLEAR,            /**< Clear OC1A/OC1B on compare match */
    TIMER1_OC_SET               /**< Set OC1A/OC1B on compare match */
} TIMER1_OC_Mode_t;

/**
 * @struct TIMER1_Config_t
 * @brief Timer1 pre-compile configuration structure.
 *
 * @details
 * Timer1 is a 16-bit timer. Some modes use OCR1A as TOP, and other modes use
 * ICR1 as TOP. The meaning of ocr1a_value and icr1_value depends on the
 * selected mode.
 */
typedef struct
{
    TIMER1_Mode_t mode;             /**< Timer1 operating mode */
    TIMER01_Prescaler_t prescaler;  /**< Timer1 clock source / prescaler */

    TIMER1_OC_Mode_t oc1a_mode;     /**< OC1A behavior in non-PWM modes */
    TIMER1_OC_Mode_t oc1b_mode;     /**< OC1B behavior in non-PWM modes */

    TIMER_PWM_Mode_t oc1a_pwm_mode; /**< OC1A behavior in PWM modes */
    TIMER_PWM_Mode_t oc1b_pwm_mode; /**< OC1B behavior in PWM modes */

    u16 initial_value;              /**< Initial value loaded into TCNT1 */
    u16 ocr1a_value;                /**< Value loaded into OCR1A */
    u16 ocr1b_value;                /**< Value loaded into OCR1B */
    u16 icr1_value;                 /**< Value loaded into ICR1 */
} TIMER1_Config_t;

/* ========================================================================== */
/*                             Interrupt Types                                */
/* ========================================================================== */

/**
 * @enum TIMER_InterruptSource_t
 * @brief Timer interrupt source identifiers.
 *
 * @note For Timer0 and Timer2, TIMER_INT_COMP_A represents the single
 *       compare match interrupt.
 */
typedef enum
{
    TIMER_INT_OVF = 0, /**< Timer overflow interrupt */
    TIMER_INT_COMP_A,  /**< Compare Match A interrupt or single compare interrupt */
    TIMER_INT_COMP_B,  /**< Timer1 Compare Match B interrupt */
    TIMER_INT_ICU      /**< Timer1 Input Capture interrupt */
} TIMER_InterruptSource_t;

/**
 * @typedef TIMER_Callback_t
 * @brief Pointer to function used as a timer interrupt callback.
 */
typedef void (*TIMER_Callback_t)(void);

/**
 * @enum TIMER1_ICU_Edge_t
 * @brief Timer1 Input Capture trigger edge selection.
 */
typedef enum
{
    TIMER1_ICU_FALLING_EDGE = 0, /**< Capture on falling edge */
    TIMER1_ICU_RISING_EDGE       /**< Capture on rising edge */
} TIMER1_ICU_Edge_t;

/* ========================================================================== */
/*                               Initialization                               */
/* ========================================================================== */

/**
 * @brief Initializes Timer0, Timer1, and Timer2 according to configuration.
 *
 * @return error_t
 *         - OK  : All timers initialized successfully.
 *         - NOK : One or more timers failed to initialize.
 *
 * @note This function configures the timers but does not start them.
 *       Use TIMER_Start() after initialization.
 */
error_t TIMER_Init(void);

/**
 * @brief Initializes Timer0 according to TIMER0_CFG.
 *
 * @return error_t
 *         - OK           : Timer0 initialized successfully.
 *         - OUT_OF_RANGE : Invalid Timer0 configuration.
 */
error_t TIMER0_Init(void);

/**
 * @brief Initializes Timer1 according to TIMER1_CFG.
 *
 * @return error_t
 *         - OK           : Timer1 initialized successfully.
 *         - OUT_OF_RANGE : Invalid Timer1 configuration.
 */
error_t TIMER1_Init(void);

/**
 * @brief Initializes Timer2 according to TIMER2_CFG.
 *
 * @return error_t
 *         - OK           : Timer2 initialized successfully.
 *         - OUT_OF_RANGE : Invalid Timer2 configuration.
 */
error_t TIMER2_Init(void);

/* ========================================================================== */
/*                                Timer0 API                                  */
/* ========================================================================== */

/**
 * @brief Writes a value to Timer0 counter register.
 *
 * @param[in] value Value to be written to TCNT0.
 */
void TIMER0_SetCounter(u8 value);

/**
 * @brief Reads Timer0 counter register.
 *
 * @return u8 Current TCNT0 value.
 */
u8 TIMER0_GetCounter(void);

/**
 * @brief Writes a value to Timer0 compare register.
 *
 * @param[in] value Value to be written to OCR0.
 */
void TIMER0_SetCompare(u8 value);

/**
 * @brief Reads Timer0 compare register.
 *
 * @return u8 Current OCR0 value.
 */
u8 TIMER0_GetCompare(void);

/* ========================================================================== */
/*                                Timer1 API                                  */
/* ========================================================================== */

/**
 * @brief Writes a value to Timer1 counter register.
 *
 * @param[in] value Value to be written to TCNT1.
 */
void TIMER1_SetCounter(u16 value);

/**
 * @brief Reads Timer1 counter register.
 *
 * @return u16 Current TCNT1 value.
 */
u16 TIMER1_GetCounter(void);

/**
 * @brief Writes a value to Timer1 Compare A register.
 *
 * @param[in] value Value to be written to OCR1A.
 *
 * @note In modes where OCR1A is used as TOP, this function changes
 *       the Timer1 top value.
 */
void TIMER1_SetCompareA(u16 value);

/**
 * @brief Reads Timer1 Compare A register.
 *
 * @return u16 Current OCR1A value.
 */
u16 TIMER1_GetCompareA(void);

/**
 * @brief Writes a value to Timer1 Compare B register.
 *
 * @param[in] value Value to be written to OCR1B.
 */
void TIMER1_SetCompareB(u16 value);

/**
 * @brief Reads Timer1 Compare B register.
 *
 * @return u16 Current OCR1B value.
 */
u16 TIMER1_GetCompareB(void);

/**
 * @brief Sets Timer1 TOP value according to the selected Timer1 mode.
 *
 * @param[in] value TOP value to be configured.
 *
 * @note The value is written to OCR1A or ICR1 depending on the configured mode.
 * @note If the selected mode does not support programmable TOP, no action is taken.
 */
void TIMER1_SetTop(u16 value);

/**
 * @brief Gets Timer1 TOP value according to the selected Timer1 mode.
 *
 * @return u16 Current Timer1 TOP value.
 *
 * @note The returned value comes from OCR1A, ICR1, or default max value
 *       depending on the selected mode.
 */
u16 TIMER1_GetTop(void);

/* ========================================================================== */
/*                                Timer2 API                                  */
/* ========================================================================== */

/**
 * @brief Writes a value to Timer2 counter register.
 *
 * @param[in] value Value to be written to TCNT2.
 */
void TIMER2_SetCounter(u8 value);

/**
 * @brief Reads Timer2 counter register.
 *
 * @return u8 Current TCNT2 value.
 */
u8 TIMER2_GetCounter(void);

/**
 * @brief Writes a value to Timer2 compare register.
 *
 * @param[in] value Value to be written to OCR2.
 */
void TIMER2_SetCompare(u8 value);

/**
 * @brief Reads Timer2 compare register.
 *
 * @return u8 Current OCR2 value.
 */
u8 TIMER2_GetCompare(void);

/* ========================================================================== */
/*                            Timer Control API                               */
/* ========================================================================== */

/**
 * @brief Starts the selected timer.
 *
 * @details
 * This function restores the configured prescaler bits for the selected timer.
 *
 * @param[in] timer Timer channel to start.
 *
 * @return error_t
 *         - OK           : Timer started successfully.
 *         - OUT_OF_RANGE : Invalid timer channel.
 */
error_t TIMER_Start(TIMER_Channel_t timer);

/**
 * @brief Stops the selected timer.
 *
 * @details
 * This function clears the clock select bits of the selected timer.
 *
 * @param[in] timer Timer channel to stop.
 *
 * @return error_t
 *         - OK           : Timer stopped successfully.
 *         - OUT_OF_RANGE : Invalid timer channel.
 */
error_t TIMER_Stop(TIMER_Channel_t timer);

/* ========================================================================== */
/*                           Interrupt Management                             */
/* ========================================================================== */

/**
 * @brief Enables a selected interrupt source for a selected timer.
 *
 * @param[in] timer  Timer channel.
 * @param[in] source Interrupt source.
 *
 * @return error_t
 *         - OK           : Interrupt enabled successfully.
 *         - OUT_OF_RANGE : Invalid timer channel or unsupported source.
 *
 * @note Global interrupts must be enabled separately by the application.
 */
error_t TIMER_InterruptEnable(TIMER_Channel_t timer,
                              TIMER_InterruptSource_t source);

/**
 * @brief Disables a selected interrupt source for a selected timer.
 *
 * @param[in] timer  Timer channel.
 * @param[in] source Interrupt source.
 *
 * @return error_t
 *         - OK           : Interrupt disabled successfully.
 *         - OUT_OF_RANGE : Invalid timer channel or unsupported source.
 */
error_t TIMER_InterruptDisable(TIMER_Channel_t timer,
                               TIMER_InterruptSource_t source);

/* ========================================================================== */
/*                              Callback API                                  */
/* ========================================================================== */

/**
 * @brief Registers a callback function for a selected timer interrupt source.
 *
 * @param[in] timer Timer channel.
 * @param[in] source Interrupt source.
 * @param[in] Copy_pfCallbackFunc Pointer to callback function.
 *
 * @return error_t
 *         - OK           : Callback registered successfully.
 *         - OUT_OF_RANGE : Invalid timer channel or unsupported source.
 *
 * @note Passing NULLPTR clears the registered callback.
 * @note The interrupt source must be enabled using TIMER_InterruptEnable().
 * @note Global interrupts must be enabled by the application.
 */
error_t TIMER_SetCallback(TIMER_Channel_t timer,
                          TIMER_InterruptSource_t source,
                          TIMER_Callback_t Copy_pfCallbackFunc);

/* ========================================================================== */
/*                               Timer1 ICU API                               */
/* ========================================================================== */

/**
 * @brief Selects Timer1 Input Capture trigger edge.
 *
 * @param[in] edge Input Capture edge selection.
 *
 * @return error_t
 *         - OK           : Edge configured successfully.
 *         - OUT_OF_RANGE : Invalid edge selection.
 */
error_t TIMER1_ICU_SetEdge(TIMER1_ICU_Edge_t edge);

/**
 * @brief Reads the last captured Timer1 value.
 *
 * @return u16 Captured value stored in ICR1.
 */
u16 TIMER1_ICU_GetCaptureValue(void);

/**
 * @brief Clears Timer1 Input Capture flag.
 *
 * @note In AVR, interrupt flags are cleared by writing logical one to them.
 */
void TIMER1_ICU_ClearFlag(void);

/**
 * @brief Enables Timer1 Input Capture noise canceler.
 */
void TIMER1_ICU_EnableNoiseCanceler(void);

/**
 * @brief Disables Timer1 Input Capture noise canceler.
 */
void TIMER1_ICU_DisableNoiseCanceler(void);

#endif /* TIMER_INT_H_ */