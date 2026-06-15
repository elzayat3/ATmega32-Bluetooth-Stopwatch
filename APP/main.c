#include "StdTypes.h"
#include "MemMap.h"

#include "DIO_Int.h"
#include "UART_Int.h"
#include "TIMER_Int.h"

#include "SEVSEG_Int.h"
#include "BUZZER_Int.h"

#include "COUNTDOWN_Int.h"

/* ========================================================================== */
/*                              Private Macros                                */
/* ========================================================================== */

#define APP_RX_BUFFER_SIZE      16U
#define APP_COMMAND_NONE        0U
#define APP_COMMAND_SET_TIME    'T'
#define APP_COMMAND_FIRST_DIGIT 'D'

/* ========================================================================== */
/*                          Private Global Variables                          */
/* ========================================================================== */

static volatile u8 APP_u8RxBuffer[APP_RX_BUFFER_SIZE];
static volatile u8 APP_u8RxHead = 0U;
static volatile u8 APP_u8RxTail = 0U;

/* ========================================================================== */
/*                          Private Function Prototypes                       */
/* ========================================================================== */

static void APP_Timer2TickHandler(void);
static void APP_UART_RX_Handler(void);

static u8 APP_RxBufferIsEmpty(void);
static void APP_RxBufferPush(u8 Copy_u8Data);
static u8 APP_RxBufferPop(u8 *Copy_pu8Data);

static void APP_ProcessBluetoothChar(u8 Copy_u8Char);

/* ========================================================================== */
/*                              main Function                                 */
/* ========================================================================== */

int main(void)
{
    DIO_Init();

    UART_Init();

    TIMER_Init();

    SEVSEG_Init();

    BUZZER_Off();

    COUNTDOWN_Init();

    TIMER_SetCallback(TIMER2, TIMER_INT_COMP_A, APP_Timer2TickHandler);
    TIMER_InterruptEnable(TIMER2, TIMER_INT_COMP_A);

    UART_RX_SetCallBack(APP_UART_RX_Handler);
    UART_RX_InterruptEnable();

    sei();

    TIMER_Start(TIMER2);

    UART_SendString("Bluetooth Countdown Ready\r\n");
    UART_SendString("Commands: Txx, S, P, R, +, -\r\n");

    while (1)
    {
        u8 Local_u8ReceivedChar;

        if (APP_RxBufferPop(&Local_u8ReceivedChar) == OK)
        {
            APP_ProcessBluetoothChar(Local_u8ReceivedChar);
        }
    }
}

/* ========================================================================== */
/*                          Timer Callback Function                           */
/* ========================================================================== */

/**
 * @brief Timer2 compare match callback.
 *
 * @details
 * This function is called every 1ms.
 * It refreshes the Seven Segment display and updates the countdown timing.
 */
static void APP_Timer2TickHandler(void)
{
    SEVSEG_Update();
    COUNTDOWN_Tick();
}

/* ========================================================================== */
/*                          UART RX Callback Function                         */
/* ========================================================================== */

/**
 * @brief UART RX interrupt callback.
 *
 * @details
 * This function is called whenever a byte is received from HC-05.
 * It only reads the received byte and stores it in a small buffer.
 */
static void APP_UART_RX_Handler(void)
{
    u8 Local_u8ReceivedData = UART_ReceiveDirect();

    APP_RxBufferPush(Local_u8ReceivedData);
}

/* ========================================================================== */
/*                              RX Buffer Helpers                             */
/* ========================================================================== */

static u8 APP_RxBufferIsEmpty(void)
{
    u8 Local_u8Result = FALSE;

    if (APP_u8RxHead == APP_u8RxTail)
    {
        Local_u8Result = TRUE;
    }

    return Local_u8Result;
}

static void APP_RxBufferPush(u8 Copy_u8Data)
{
    u8 Local_u8NextHead = (u8)((APP_u8RxHead + 1U) % APP_RX_BUFFER_SIZE);

    if (Local_u8NextHead != APP_u8RxTail)
    {
        APP_u8RxBuffer[APP_u8RxHead] = Copy_u8Data;
        APP_u8RxHead = Local_u8NextHead;
    }
    else
    {
        /* Buffer full: received byte is ignored */
    }
}

static u8 APP_RxBufferPop(u8 *Copy_pu8Data)
{
    u8 Local_u8ErrorState = NOK;

    if (Copy_pu8Data != NULLPTR)
    {
        if (APP_RxBufferIsEmpty() == FALSE)
        {
            *Copy_pu8Data = APP_u8RxBuffer[APP_u8RxTail];
            APP_u8RxTail = (u8)((APP_u8RxTail + 1U) % APP_RX_BUFFER_SIZE);
            Local_u8ErrorState = OK;
        }
        else
        {
            Local_u8ErrorState = EMPTY;
        }
    }
    else
    {
        Local_u8ErrorState = NULL_PTR;
    }

    return Local_u8ErrorState;
}

/* ========================================================================== */
/*                          Bluetooth Command Parser                          */
/* ========================================================================== */

static void APP_ProcessBluetoothChar(u8 Copy_u8Char)
{
    static u8 Local_u8ParserState = APP_COMMAND_NONE;
    static u8 Local_u8FirstDigit = 0U;

    if ((Copy_u8Char == '\r') || (Copy_u8Char == '\n'))
    {
        /* Ignore line endings from Serial Bluetooth Terminal */
    }
    else if ((Copy_u8Char == 'S') || (Copy_u8Char == 's'))
    {
        if (COUNTDOWN_GetState() == COUNTDOWN_PAUSED)
        {
            COUNTDOWN_Resume();
        }
        else
        {
            COUNTDOWN_Start();
        }

        UART_SendString("START\r\n");
        Local_u8ParserState = APP_COMMAND_NONE;
    }
    else if ((Copy_u8Char == 'P') || (Copy_u8Char == 'p'))
    {
        COUNTDOWN_Pause();
        UART_SendString("PAUSE\r\n");
        Local_u8ParserState = APP_COMMAND_NONE;
    }
    else if ((Copy_u8Char == 'R') || (Copy_u8Char == 'r'))
    {
        COUNTDOWN_Reset();
        UART_SendString("RESET\r\n");
        Local_u8ParserState = APP_COMMAND_NONE;
    }
    else if (Copy_u8Char == '+')
    {
        COUNTDOWN_IncrementValue();
        UART_SendString("INC\r\n");
        Local_u8ParserState = APP_COMMAND_NONE;
    }
    else if (Copy_u8Char == '-')
    {
        COUNTDOWN_DecrementValue();
        UART_SendString("DEC\r\n");
        Local_u8ParserState = APP_COMMAND_NONE;
    }
    else if ((Copy_u8Char == 'T') || (Copy_u8Char == 't'))
    {
        Local_u8ParserState = APP_COMMAND_SET_TIME;
    }
    else if ((Local_u8ParserState == APP_COMMAND_SET_TIME) &&
             (Copy_u8Char >= '0') &&
             (Copy_u8Char <= '9'))
    {
        Local_u8FirstDigit = (u8)(Copy_u8Char - '0');
        Local_u8ParserState = APP_COMMAND_FIRST_DIGIT;
    }
    else if ((Local_u8ParserState == APP_COMMAND_FIRST_DIGIT) &&
             (Copy_u8Char >= '0') &&
             (Copy_u8Char <= '9'))
    {
        u8 Local_u8SecondDigit = (u8)(Copy_u8Char - '0');
        u8 Local_u8SetValue = (u8)((Local_u8FirstDigit * 10U) + Local_u8SecondDigit);

        COUNTDOWN_SetValue(Local_u8SetValue);

        UART_SendString("TIME SET\r\n");

        Local_u8ParserState = APP_COMMAND_NONE;
    }
    else
    {
        UART_SendString("INVALID\r\n");
        Local_u8ParserState = APP_COMMAND_NONE;
    }
}