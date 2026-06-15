#include "StdTypes.h"
#include "DIO_Int.h"
#include "SEVSEG_Int.h"
#include "SEVSEG_Cfg.h"

/**
 * @file SEVSEG_Cfg.c
 * @author Abdelrahman Elzayat
 * @brief Seven Segment driver configuration source file.
 *
 * @details
 * This file contains the user-selected configuration for the multiplexed
 * Seven Segment display.
 *
 * Current connection assumption:
 * - Segment lines are connected to PORTA.
 * - Segment pattern is shifted left by 1 bit.
 *   Example: pattern 0x3F becomes 0x7E.
 * - Digit enable pins are active LOW.
 * - Four digits are used for stopwatch display in MM:SS format.
 *
 * @note If your segment pins are connected starting from bit0, set
 *       segment_shift to 0U.
 */

/* ========================================================================== */
/*                         Seven Segment Configuration                         */
/* ========================================================================== */

const SEVSEG_Config_t SEVSEG_Config =
{
	.segment_port   = PA,
	.segment_shift  = 1U,

	.type           = SEVSEG_COMMON_CATHODE,

	.digit_polarity = SEVSEG_DIGIT_ACTIVE_HIGH,

	.mode           = SEVSEG_TWO_DIGITS,

	.digit_pins =
	{
		DIO_PINC6,  /* Digit 0: units */
		DIO_PINC7,  /* Digit 1: tens  */
		DIO_PINB3,  /* Not used in 2 digits */
		DIO_PINB4   /* Not used in 2 digits */
	}
};