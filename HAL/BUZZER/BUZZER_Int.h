#ifndef BUZZER_INT_H_
#define BUZZER_INT_H_

/**
 * @file BUZZER_Int.h
 * @author Abdelrahman Elzayat
 * @brief Public interface for the Buzzer HAL driver.
 *
 * @details
 * This driver provides simple APIs to control a buzzer connected to
 * a digital output pin. The buzzer pin and active state are configured
 * in BUZZER_Cfg.h.
 *
 * The driver depends on the DIO driver to write the required logic level
 * to the configured buzzer pin.
 *
 * @note The buzzer pin must be configured as OUTPUT in DIO_Cfg.c.
 */

/**
 * @brief Turns the buzzer ON.
 *
 * @details
 * This function activates the buzzer by writing the configured active state
 * to the buzzer pin.
 *
 * @return void
 *
 * @note The active state is configured using BUZZER_ACTIVE_STATE
 *       in BUZZER_Cfg.h.
 */
void BUZZER_On(void);

/**
 * @brief Turns the buzzer OFF.
 *
 * @details
 * This function deactivates the buzzer by writing the configured inactive
 * state to the buzzer pin.
 *
 * @return void
 *
 * @note The inactive state is configured using BUZZER_INACTIVE_STATE
 *       in BUZZER_Cfg.h.
 */
void BUZZER_Off(void);

#endif /* BUZZER_INT_H_ */