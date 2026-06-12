#ifndef DIO_INT_H_
#define DIO_INT_H_
/**
 * @file DIO_Int.h
 * @brief Digital Input Output (DIO) driver interface for ATmega32
 *
 * This module provides APIs to control GPIO pins and ports on ATmega32.
 *
 * @author Abdelrahman Elzayat
 */ 

#include "StdTypes.h"
 
/**
 * @enum DIO_Status_t
 * @brief Defines the configuration mode of a GPIO pin.
 */
typedef enum
{
    OUTPUT,   /**< Pin configured as output */
    INFREE,   /**< Floating input */
    INPULL    /**< Input with internal pull-up */
}DIO_Status_t;
/**
 * @enum DIO_Pin_t
 * @brief Represents all GPIO pins in ATmega32.
 *
 * Pins are ordered sequentially starting from PORTA to PORTD.
 * This ordering allows easy extraction of port and pin number.
 */
typedef enum
{
	DIO_PINA0 = 0,
	DIO_PINA1,
	DIO_PINA2,
	DIO_PINA3,
	DIO_PINA4,
	DIO_PINA5,
	DIO_PINA6,
	DIO_PINA7,

	DIO_PINB0,
	DIO_PINB1,
	DIO_PINB2,
	DIO_PINB3,
	DIO_PINB4,
	DIO_PINB5,
	DIO_PINB6,
	DIO_PINB7,

	DIO_PINC0,
	DIO_PINC1,
	DIO_PINC2,
	DIO_PINC3,
	DIO_PINC4,
	DIO_PINC5,
	DIO_PINC6,
	DIO_PINC7,

	DIO_PIND0,
	DIO_PIND1,
	DIO_PIND2,
	DIO_PIND3,
	DIO_PIND4,
	DIO_PIND5,
	DIO_PIND6,
	DIO_PIND7,

	TOTAL_PINS
} DIO_Pin_t;
/**
 * @enum DIO_Voltage_t
 * @brief Represents the logic level of a GPIO pin.
 */
typedef enum
{
	LOW,   /**< Logic low (0V) */
	HIGH   /**< Logic high (Vcc) */
}DIO_Voltage_t;
/**
 * @enum DIO_Port_t
 * @brief Represents GPIO ports in ATmega32.
 */
typedef enum
{
	PA,  /**< Port A */
	PB,  /**< Port B */
	PC,  /**< Port C */
	PD   /**< Port D */
}DIO_Port_t;
/**
 * @brief Writes a voltage level to a specific pin.
 *
 * @param pin  The target pin to write to.
 * @param volt The voltage level to set (HIGH or LOW).
 *
 * @note The pin must be configured as OUTPUT before using this function.
 */
void DIO_WritePin(DIO_Pin_t pin, DIO_Voltage_t volt);
/**
 * @brief Reads the logic level of a specific pin.
 *
 * @param pin The target pin to read.
 *
 * @return DIO_Voltage_t
 *         - HIGH if the pin is logic high
 *         - LOW if the pin is logic low
 */
DIO_Voltage_t DIO_ReadPin(DIO_Pin_t pin);
/**
 * @brief Toggles the current state of a specific pin.
 *
 * @param pin The target pin to toggle.
 *
 * @note The pin should be configured as OUTPUT.
 */
void DIO_TogglePin(DIO_Pin_t pin);
/**
 * @brief Writes an 8-bit value to an entire port.
 *
 * @param port Target port (PA, PB, PC, PD)
 * @param data 8-bit value to be written to the port.
 */
void DIO_WritePort(DIO_Port_t port ,u8 data);
/**
 * @brief Reads the current value of a GPIO port.
 *
 * @param port Target port (PA, PB, PC, PD)
 *
 * @return u8 Current 8-bit value of the port.
 */
u8 DIO_ReadPort(DIO_Port_t port);
/**
 * @brief Initializes all GPIO pins according to the configuration array.
 *
 * This function initializes every pin defined in the configuration
 * array (PinsStatusArr) located in DIO_Cfg.c. Each pin will be configured
 * as OUTPUT, INPUT FLOATING, or INPUT PULL-UP depending on the user configuration.
 */
void DIO_Init(void);

#endif /* DIO_INT_H_ */