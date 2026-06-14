#ifndef SEVSEG_CFG_H_
#define SEVSEG_CFG_H_

/**
 * @file SEVSEG_Cfg.h
 * @author Abdelrahman Elzayat
 * @brief Seven Segment driver configuration header file.
 *
 * @details
 * This file contains the configuration structure and external configuration
 * object declaration for the multiplexed Seven Segment driver.
 */

#include "StdTypes.h"
#include "DIO_Int.h"
#include "SEVSEG_Int.h"

/* ========================================================================== */
/*                              Configuration                                 */
/* ========================================================================== */

/**
 * @def SEVSEG_MAX_DIGITS
 * @brief Maximum number of multiplexed digits supported by the driver.
 */
#define SEVSEG_MAX_DIGITS    4U

/**
 * @struct SEVSEG_Config_t
 * @brief Seven Segment driver configuration structure.
 *
 * @details
 * This structure defines:
 * - Segment data port
 * - Segment bit shift
 * - Display type
 * - Digit enable polarity
 * - Display mode
 * - Digit enable pins
 */
typedef struct
{
    DIO_Port_t segment_port;                         /**< Port connected to segment lines */
    u8 segment_shift;                                /**< Shift applied to segment pattern before writing to port */
    SEVSEG_Type_t type;                              /**< Common cathode or common anode */
    SEVSEG_DigitPolarity_t digit_polarity;           /**< Digit enable active state */
    SEVSEG_Mode_t mode;                              /**< Two digits or four digits */
    DIO_Pin_t digit_pins[SEVSEG_MAX_DIGITS];         /**< Digit enable pins */
} SEVSEG_Config_t;

/**
 * @brief External Seven Segment configuration object.
 */
extern const SEVSEG_Config_t SEVSEG_Config;

#endif /* SEVSEG_CFG_H_ */