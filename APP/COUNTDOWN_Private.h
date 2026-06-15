#ifndef COUNTDOWN_PRIVATE_H_
#define COUNTDOWN_PRIVATE_H_

/**
 * @file COUNTDOWN_Private.h
 * @author Abdelrahman Elzayat
 * @brief Private definitions for Countdown application module.
 *
 * @details
 * This file contains private macros used internally by the countdown module.
 *
 * @note This file should not be included by the application layer directly.
 */

#include "StdTypes.h"

/* ========================================================================== */
/*                              Private Macros                                */
/* ========================================================================== */

/**
 * @def COUNTDOWN_DEFAULT_VALUE
 * @brief Default countdown value used after initialization.
 *
 * @details
 * The countdown starts with this value unless another value is received
 * from Bluetooth command such as T45.
 */
#define COUNTDOWN_DEFAULT_VALUE       0U

/**
 * @def COUNTDOWN_FINISHED_VALUE
 * @brief Countdown value when the timer reaches zero.
 */
#define COUNTDOWN_FINISHED_VALUE      0U

/**
 * @def COUNTDOWN_TRUE
 * @brief Internal true value.
 */
#define COUNTDOWN_TRUE                1U

/**
 * @def COUNTDOWN_FALSE
 * @brief Internal false value.
 */
#define COUNTDOWN_FALSE               0U

#endif /* COUNTDOWN_PRIVATE_H_ */