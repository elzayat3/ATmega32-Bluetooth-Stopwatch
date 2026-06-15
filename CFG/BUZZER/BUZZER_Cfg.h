#ifndef BUZZER_CFG_H_
#define BUZZER_CFG_H_

/**
 * @file BUZZER_Cfg.h
 * @author Abdelrahman Elzayat
 * @brief Configuration file for the Buzzer HAL driver.
 *
 * @details
 * This file contains the user-configurable parameters for the buzzer driver:
 * - Buzzer connected pin
 * - Buzzer active state
 * - Buzzer inactive state
 *
 * The buzzer driver uses these macros inside BUZZER_On() and BUZZER_Off().
 *
 * @note The selected buzzer pin must be configured as OUTPUT in DIO_Cfg.c.
 */

#include "StdTypes.h"
#include "DIO_Int.h"

/**
 * @def BUZZER_PIN
 * @brief Defines the microcontroller pin connected to the buzzer.
 *
 * @details
 * In this project, the buzzer is connected to pin PC5.
 *
 * @note Change this macro if the buzzer is connected to another pin.
 */
#define BUZZER_PIN              DIO_PINC5

/**
 * @def BUZZER_ACTIVE_STATE
 * @brief Defines the logic level that turns the buzzer ON.
 *
 * @details
 * Use HIGH if the buzzer is active-high.
 * Use LOW if the buzzer is active-low.
 */
#define BUZZER_ACTIVE_STATE     HIGH

/**
 * @def BUZZER_INACTIVE_STATE
 * @brief Defines the logic level that turns the buzzer OFF.
 *
 * @details
 * This value should be the opposite of BUZZER_ACTIVE_STATE.
 */
#define BUZZER_INACTIVE_STATE   LOW

#endif /* BUZZER_CFG_H_ */