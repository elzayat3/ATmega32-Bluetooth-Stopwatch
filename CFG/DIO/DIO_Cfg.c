#include "StdTypes.h"
#include "MemMap.h"
#include "DIO_Int.h"
#include "DIO_Private.h"
#include "DIO_Cfg.h"

/**
 * @file DIO_Cfg.c
 * @brief DIO pin configuration for ATmega32 Bluetooth Countdown project.
 *
 * Pin usage:
 * - PA1:PA7  -> Seven Segment segments A:G
 * - PB0      -> Seven Segment DOT
 * - PC5      -> Buzzer
 * - PC6      -> Seven Segment Digit 0 enable
 * - PC7      -> Seven Segment Digit 1 enable
 * - PD0      -> UART RX
 * - PD1      -> UART TX
 */
const DIO_Status_t PinsStatusArr[TOTAL_PINS] =
{
    /* PORTA */
    INFREE,     /* PA0: Unused */
    OUTPUT,     /* PA1: Seven Segment A */
    OUTPUT,     /* PA2: Seven Segment B */
    OUTPUT,     /* PA3: Seven Segment C */
    OUTPUT,     /* PA4: Seven Segment D */
    OUTPUT,     /* PA5: Seven Segment E */
    OUTPUT,     /* PA6: Seven Segment F */
    OUTPUT,     /* PA7: Seven Segment G */

    /* PORTB */
    OUTPUT,     /* PB0: Seven Segment DOT */
    INFREE,     /* PB1: Unused */
    INFREE,     /* PB2: Unused / INT2 */
    INFREE,     /* PB3: Unused / OC0 */
    INPULL,     /* PB4: Unused / SS */
    INPULL,     /* PB5: Unused / MOSI */
    INPULL,     /* PB6: Unused / MISO */
    INPULL,     /* PB7: Unused / SCK */

    /* PORTC */
    INFREE,     /* PC0: Unused */
    INFREE,     /* PC1: Unused */
    INFREE,     /* PC2: Unused */
    INFREE,     /* PC3: Unused */
    INFREE,     /* PC4: Unused */
    OUTPUT,     /* PC5: Buzzer */
    OUTPUT,     /* PC6: Seven Segment Digit 0 enable */
    OUTPUT,     /* PC7: Seven Segment Digit 1 enable */

    /* PORTD */
    INFREE,     /* PD0: UART RX */
    OUTPUT,     /* PD1: UART TX */
    INFREE,     /* PD2: Unused / INT0 */
    INFREE,     /* PD3: Unused / INT1 */
    INFREE,     /* PD4: Unused / OC1B */
    INFREE,     /* PD5: Unused / OC1A */
    INFREE,     /* PD6: Unused / ICP */
    INFREE      /* PD7: Unused */
};