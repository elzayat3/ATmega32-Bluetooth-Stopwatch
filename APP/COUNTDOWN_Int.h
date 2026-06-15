#ifndef COUNTDOWN_INT_H_
#define COUNTDOWN_INT_H_

/**
 * @file COUNTDOWN_Int.h
 * @author Abdelrahman Elzayat
 * @brief Public interface for Countdown application module.
 *
 * @details
 * This module controls the countdown timer logic of the project.
 * It is responsible for:
 * - Setting countdown value from 0 to 99 seconds
 * - Starting countdown
 * - Pausing countdown
 * - Resuming countdown
 * - Stopping countdown
 * - Resetting countdown to the saved start value
 * - Decrementing the value every second
 * - Updating the Seven Segment display
 * - Turning the buzzer ON when countdown reaches zero
 *
 * Bluetooth commands such as "T45", "S", "P", and "R" should be parsed
 * in the application layer, then translated into calls to this module.
 *
 * Example:
 * - T45 -> COUNTDOWN_SetValue(45)
 * - S   -> COUNTDOWN_Start()
 * - P   -> COUNTDOWN_Pause()
 * - R   -> COUNTDOWN_Reset()
 *
 * @note COUNTDOWN_Tick() should be called every 1ms from the timer callback.
 * @note SEVSEG_Update() should also be called periodically to refresh display.
 */

#include "StdTypes.h"

/* ========================================================================== */
/*                              Public Macros                                 */
/* ========================================================================== */

/**
 * @def COUNTDOWN_MAX_VALUE
 * @brief Maximum countdown value supported by 2-digit Seven Segment display.
 */
#define COUNTDOWN_MAX_VALUE      99U

/**
 * @def COUNTDOWN_MIN_VALUE
 * @brief Minimum countdown value.
 */
#define COUNTDOWN_MIN_VALUE      0U

/**
 * @def COUNTDOWN_ONE_SECOND_MS
 * @brief Number of 1ms ticks required to form one second.
 */
#define COUNTDOWN_ONE_SECOND_MS  1000U

/* ========================================================================== */
/*                              Public Types                                  */
/* ========================================================================== */

/**
 * @enum COUNTDOWN_State_t
 * @brief Countdown operating states.
 */
typedef enum
{
    COUNTDOWN_STOPPED = 0, /**< Countdown is stopped */
    COUNTDOWN_RUNNING,     /**< Countdown is currently running */
    COUNTDOWN_PAUSED,      /**< Countdown is paused */
    COUNTDOWN_FINISHED     /**< Countdown reached zero */
} COUNTDOWN_State_t;

/* ========================================================================== */
/*                              Public APIs                                   */
/* ========================================================================== */

/**
 * @brief Initializes the countdown module.
 *
 * @details
 * This function resets all internal variables, clears the tick counter,
 * turns the buzzer OFF, sets the countdown state to STOPPED, and updates
 * the Seven Segment display with the initial value.
 *
 * @return void
 */
void COUNTDOWN_Init(void);

/**
 * @brief Sets countdown start value in seconds.
 *
 * @details
 * This function sets both the saved start value and the current countdown value.
 * It also updates the Seven Segment display and turns the buzzer OFF.
 *
 * @param[in] Copy_u8Seconds Countdown value from 0 to 99 seconds.
 *
 * @return void
 *
 * @note If the given value is greater than 99, it will be limited by the
 *       implementation.
 */
void COUNTDOWN_SetValue(u8 Copy_u8Seconds);

/**
 * @brief Starts the countdown.
 *
 * @details
 * If the current countdown value is greater than zero, the state becomes
 * COUNTDOWN_RUNNING.
 *
 * @return void
 */
void COUNTDOWN_Start(void);

/**
 * @brief Pauses the countdown.
 *
 * @details
 * This function pauses the countdown only if it is currently running.
 *
 * @return void
 */
void COUNTDOWN_Pause(void);

/**
 * @brief Resumes the countdown.
 *
 * @details
 * This function resumes the countdown only if it is currently paused.
 *
 * @return void
 */
void COUNTDOWN_Resume(void);

/**
 * @brief Stops the countdown.
 *
 * @details
 * This function stops the countdown without clearing the current value.
 *
 * @return void
 */
void COUNTDOWN_Stop(void);

/**
 * @brief Resets countdown to the saved start value.
 *
 * @details
 * This function restores the current countdown value to the last value set
 * by COUNTDOWN_SetValue(), clears the tick counter, turns the buzzer OFF,
 * updates the display, and sets the state to COUNTDOWN_STOPPED.
 *
 * @return void
 */
void COUNTDOWN_Reset(void);

/**
 * @brief Handles countdown timing.
 *
 * @details
 * This function should be called every 1ms. It accumulates milliseconds
 * internally, and every 1000ms it decrements the countdown value by one second
 * if the countdown is running.
 *
 * When the countdown reaches zero, it changes state to COUNTDOWN_FINISHED
 * and turns the buzzer ON.
 *
 * @return void
 */
void COUNTDOWN_Tick(void);

/**
 * @brief Increments countdown value by one second.
 *
 * @details
 * This function is useful for Bluetooth commands such as '+'.
 * The value will not exceed COUNTDOWN_MAX_VALUE.
 *
 * @return void
 */
void COUNTDOWN_IncrementValue(void);

/**
 * @brief Decrements countdown value by one second.
 *
 * @details
 * This function is useful for Bluetooth commands such as '-'.
 * The value will not go below COUNTDOWN_MIN_VALUE.
 *
 * @return void
 */
void COUNTDOWN_DecrementValue(void);

/**
 * @brief Gets current countdown value.
 *
 * @return u8 Current countdown value in seconds.
 */
u8 COUNTDOWN_GetValue(void);

/**
 * @brief Gets saved countdown start value.
 *
 * @return u8 Saved start value in seconds.
 */
u8 COUNTDOWN_GetStartValue(void);

/**
 * @brief Gets current countdown state.
 *
 * @return COUNTDOWN_State_t Current countdown state.
 */
COUNTDOWN_State_t COUNTDOWN_GetState(void);

#endif /* COUNTDOWN_INT_H_ */