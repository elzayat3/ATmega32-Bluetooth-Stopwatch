#ifndef SEVSEG_INT_H_
#define SEVSEG_INT_H_

/**
 * @file SEVSEG_Int.h
 * @author Abdelrahman Elzayat
 * @brief Public interface for Seven Segment display driver.
 *
 * @details
 * This driver is designed for multiplexed seven-segment displays
 * using time sharing. The display does not use blocking delays.
 *
 * The application should:
 * - Call SEVSEG_Init() once at startup.
 * - Call SEVSEG_SetNumber() or SEVSEG_SetTime() to update the displayed value.
 * - Call SEVSEG_Update() periodically, preferably every 1ms, to refresh one digit
 *   at a time.
 *
 * @note SEVSEG_Update() must be called continuously to keep the display visible.
 * @note This driver is suitable for stopwatch display such as MM:SS.
 */

#include "StdTypes.h"

/* ========================================================================== */
/*                              Public Types                                  */
/* ========================================================================== */

/**
 * @enum SEVSEG_Type_t
 * @brief Seven segment hardware type.
 */
typedef enum
{
    SEVSEG_COMMON_CATHODE = 0, /**< Segment turns ON with logic HIGH */
    SEVSEG_COMMON_ANODE        /**< Segment turns ON with logic LOW */
} SEVSEG_Type_t;

/**
 * @enum SEVSEG_DigitPolarity_t
 * @brief Digit enable pin active state.
 */
typedef enum
{
    SEVSEG_DIGIT_ACTIVE_LOW = 0, /**< Digit enable pin is active LOW */
    SEVSEG_DIGIT_ACTIVE_HIGH     /**< Digit enable pin is active HIGH */
} SEVSEG_DigitPolarity_t;

/**
 * @enum SEVSEG_Mode_t
 * @brief Number of digits used by the display.
 */
typedef enum
{
    SEVSEG_TWO_DIGITS  = 2, /**< Two multiplexed digits */
    SEVSEG_FOUR_DIGITS = 4  /**< Four multiplexed digits */
} SEVSEG_Mode_t;

/**
 * @enum SEVSEG_State_t
 * @brief Seven segment driver display state.
 */
typedef enum
{
    SEVSEG_DISABLED = 0, /**< Display refresh disabled */
    SEVSEG_ENABLED       /**< Display refresh enabled */
} SEVSEG_State_t;

/* ========================================================================== */
/*                              Public APIs                                   */
/* ========================================================================== */

/**
 * @brief Initializes the seven segment display driver.
 *
 * @details
 * This function clears the internal display buffer, resets the active digit
 * index, and prepares the driver for multiplexed display operation.
 *
 * @return void
 *
 * @note GPIO pin directions should be configured by the DIO configuration
 *       before calling this function.
 */
void SEVSEG_Init(void);

/**
 * @brief Displays an unsigned number on the seven segment display.
 *
 * @details
 * This function does not directly refresh the hardware. It only updates the
 * internal display buffer. The actual display refresh happens inside
 * SEVSEG_Update().
 *
 * For two digits, the number is limited to 0 - 99.
 * For four digits, the number is limited to 0 - 9999.
 *
 * @param[in] Copy_u16Number Number to be displayed.
 *
 * @return void
 *
 * @note The display uses modulo behavior if the number exceeds the available
 *       digits.
 */
void SEVSEG_SetNumber(u16 Copy_u16Number);

/**
 * @brief Displays stopwatch time in MM:SS format.
 *
 * @details
 * This function converts minutes and seconds into four display digits:
 *
 * Digit order:
 * - Digit 0: seconds units
 * - Digit 1: seconds tens
 * - Digit 2: minutes units
 * - Digit 3: minutes tens
 *
 * The actual hardware refresh is still handled by SEVSEG_Update().
 *
 * @param[in] Copy_u8Minutes Minutes value.
 * @param[in] Copy_u8Seconds Seconds value.
 *
 * @return void
 *
 * @note This function is intended for four-digit stopwatch display.
 * @note Seconds are limited to 0 - 59 using modulo behavior.
 */
void SEVSEG_SetTime(u8 Copy_u8Minutes, u8 Copy_u8Seconds);

/**
 * @brief Refreshes one digit of the multiplexed seven segment display.
 *
 * @details
 * This function performs one time-sharing refresh step:
 * - Disable all digits.
 * - Output the segment pattern of the current digit.
 * - Enable only the current digit.
 * - Move to the next digit for the next call.
 *
 * @return void
 *
 * @note This function must be called periodically, preferably every 1ms.
 * @note Do not place blocking delay inside this function.
 * @note It can be called from the main loop or from a timer callback.
 */
void SEVSEG_Update(void);

/**
 * @brief Clears the display buffer.
 *
 * @details
 * This function clears all digits from the internal buffer.
 * The hardware output will be updated on the next calls to SEVSEG_Update().
 *
 * @return void
 */
void SEVSEG_Clear(void);

/**
 * @brief Enables display refreshing.
 *
 * @details
 * When enabled, SEVSEG_Update() refreshes digits normally.
 *
 * @return void
 */
void SEVSEG_Enable(void);

/**
 * @brief Disables display refreshing.
 *
 * @details
 * When disabled, SEVSEG_Update() should turn off all digits and stop refreshing
 * the display.
 *
 * @return void
 */
void SEVSEG_Disable(void);

/**
 * @brief Gets the current display refresh state.
 *
 * @return SEVSEG_State_t
 *         - SEVSEG_ENABLED  : Display refresh is enabled.
 *         - SEVSEG_DISABLED : Display refresh is disabled.
 */
SEVSEG_State_t SEVSEG_GetState(void);

#endif /* SEVSEG_INT_H_ */