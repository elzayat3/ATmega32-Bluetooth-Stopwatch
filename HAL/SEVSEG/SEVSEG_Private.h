#ifndef SEVSEG_PRIVATE_H_
#define SEVSEG_PRIVATE_H_

/**
 * @file SEVSEG_Private.h
 * @author Abdelrahman Elzayat
 * @brief Private definitions for Seven Segment driver.
 *
 * @details
 * This file contains private macros and lookup tables used internally
 * by the Seven Segment driver implementation.
 *
 * @note This file should not be included by the application layer.
 */

#include "StdTypes.h"
#include "SEVSEG_Cfg.h"

/* ========================================================================== */
/*                              Private Macros                                */
/* ========================================================================== */

/**
 * @def SEVSEG_DIGIT_COUNT_10
 * @brief Number of decimal digits from 0 to 9.
 */
#define SEVSEG_DIGIT_COUNT_10     10U

/**
 * @def SEVSEG_BLANK_PATTERN
 * @brief Pattern used to turn off all segments.
 */
#define SEVSEG_BLANK_PATTERN      0x00U

/**
 * @def SEVSEG_MAX_TWO_DIGIT_VALUE
 * @brief Maximum displayable value on two digits.
 */
#define SEVSEG_MAX_TWO_DIGIT_VALUE    100U

/**
 * @def SEVSEG_MAX_FOUR_DIGIT_VALUE
 * @brief Maximum displayable value on four digits.
 */
#define SEVSEG_MAX_FOUR_DIGIT_VALUE   10000U

/**
 * @def SEVSEG_SECONDS_LIMIT
 * @brief Maximum seconds range for stopwatch display.
 */
#define SEVSEG_SECONDS_LIMIT      60U

/**
 * @def SEVSEG_TWO_DIGIT_COUNT
 * @brief Number of digits in two-digit mode.
 */
#define SEVSEG_TWO_DIGIT_COUNT    2U

/**
 * @def SEVSEG_FOUR_DIGIT_COUNT
 * @brief Number of digits in four-digit mode.
 */
#define SEVSEG_FOUR_DIGIT_COUNT   4U

/* ========================================================================== */
/*                          Private Lookup Tables                             */
/* ========================================================================== */

/**
 * @brief Seven segment patterns for decimal digits 0 to 9.
 *
 * @details
 * These patterns are written for Common Cathode display.
 * For Common Anode display, the pattern should be inverted in the source file.
 *
 * Bit mapping:
 * - bit0 = segment A
 * - bit1 = segment B
 * - bit2 = segment C
 * - bit3 = segment D
 * - bit4 = segment E
 * - bit5 = segment F
 * - bit6 = segment G
 * - bit7 = DP
 */
extern const u8 SEVSEG_u8DigitPatterns[SEVSEG_DIGIT_COUNT_10];

#endif /* SEVSEG_PRIVATE_H_ */