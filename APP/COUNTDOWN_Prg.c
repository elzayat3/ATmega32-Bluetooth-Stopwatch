#include "StdTypes.h"

#include "COUNTDOWN_Int.h"
#include "COUNTDOWN_Private.h"

#include "SEVSEG_Int.h"
#include "BUZZER_Int.h"

/* ========================================================================== */
/*                          Private Global Variables                          */
/* ========================================================================== */

static volatile u8 COUNTDOWN_u8CurrentValue = COUNTDOWN_DEFAULT_VALUE;
static volatile u8 COUNTDOWN_u8StartValue   = COUNTDOWN_DEFAULT_VALUE;
static volatile u16 COUNTDOWN_u16TickCounter = 0U;

static volatile COUNTDOWN_State_t COUNTDOWN_State = COUNTDOWN_STOPPED;

/* ========================================================================== */
/*                          Private Helper Functions                          */
/* ========================================================================== */

static void COUNTDOWN_UpdateDisplay(void)
{
    SEVSEG_SetNumber(COUNTDOWN_u8CurrentValue);
}

static u8 COUNTDOWN_LimitValue(u8 Copy_u8Value)
{
    u8 Local_u8Value = Copy_u8Value;

    if (Local_u8Value > COUNTDOWN_MAX_VALUE)
    {
        Local_u8Value = COUNTDOWN_MAX_VALUE;
    }

    return Local_u8Value;
}

static void COUNTDOWN_DecrementOneSecond(void)
{
    if (COUNTDOWN_u8CurrentValue > COUNTDOWN_MIN_VALUE)
    {
        COUNTDOWN_u8CurrentValue--;
        COUNTDOWN_UpdateDisplay();

        if (COUNTDOWN_u8CurrentValue == COUNTDOWN_MIN_VALUE)
        {
            COUNTDOWN_State = COUNTDOWN_FINISHED;
            BUZZER_On();
        }
    }
    else
    {
        COUNTDOWN_State = COUNTDOWN_FINISHED;
        BUZZER_On();
    }
}

/* ========================================================================== */
/*                              Public APIs                                   */
/* ========================================================================== */

void COUNTDOWN_Init(void)
{
    COUNTDOWN_u8CurrentValue = COUNTDOWN_DEFAULT_VALUE;
    COUNTDOWN_u8StartValue = COUNTDOWN_DEFAULT_VALUE;
    COUNTDOWN_u16TickCounter = 0U;
    COUNTDOWN_State = COUNTDOWN_STOPPED;

    BUZZER_Off();
    COUNTDOWN_UpdateDisplay();
}

void COUNTDOWN_SetValue(u8 Copy_u8Seconds)
{
    u8 Local_u8LimitedValue = COUNTDOWN_LimitValue(Copy_u8Seconds);

    COUNTDOWN_u8StartValue = Local_u8LimitedValue;
    COUNTDOWN_u8CurrentValue = Local_u8LimitedValue;
    COUNTDOWN_u16TickCounter = 0U;
    COUNTDOWN_State = COUNTDOWN_STOPPED;

    BUZZER_Off();
    COUNTDOWN_UpdateDisplay();
}

void COUNTDOWN_Start(void)
{
    if (COUNTDOWN_u8CurrentValue > COUNTDOWN_MIN_VALUE)
    {
        COUNTDOWN_State = COUNTDOWN_RUNNING;
        BUZZER_Off();
    }
}

void COUNTDOWN_Pause(void)
{
    if (COUNTDOWN_State == COUNTDOWN_RUNNING)
    {
        COUNTDOWN_State = COUNTDOWN_PAUSED;
    }
}

void COUNTDOWN_Resume(void)
{
    if ((COUNTDOWN_State == COUNTDOWN_PAUSED) &&
        (COUNTDOWN_u8CurrentValue > COUNTDOWN_MIN_VALUE))
    {
        COUNTDOWN_State = COUNTDOWN_RUNNING;
    }
}

void COUNTDOWN_Stop(void)
{
    COUNTDOWN_State = COUNTDOWN_STOPPED;
    COUNTDOWN_u16TickCounter = 0U;
}

void COUNTDOWN_Reset(void)
{
    COUNTDOWN_u8CurrentValue = COUNTDOWN_u8StartValue;
    COUNTDOWN_u16TickCounter = 0U;
    COUNTDOWN_State = COUNTDOWN_STOPPED;

    BUZZER_Off();
    COUNTDOWN_UpdateDisplay();
}

void COUNTDOWN_Tick(void)
{
    if (COUNTDOWN_State == COUNTDOWN_RUNNING)
    {
        COUNTDOWN_u16TickCounter++;

        if (COUNTDOWN_u16TickCounter >= COUNTDOWN_ONE_SECOND_MS)
        {
            COUNTDOWN_u16TickCounter = 0U;
            COUNTDOWN_DecrementOneSecond();
        }
    }
}

void COUNTDOWN_IncrementValue(void)
{
    if (COUNTDOWN_u8CurrentValue < COUNTDOWN_MAX_VALUE)
    {
        COUNTDOWN_u8CurrentValue++;
        COUNTDOWN_u8StartValue = COUNTDOWN_u8CurrentValue;
        COUNTDOWN_u16TickCounter = 0U;

        BUZZER_Off();
        COUNTDOWN_UpdateDisplay();

        if (COUNTDOWN_State == COUNTDOWN_FINISHED)
        {
            COUNTDOWN_State = COUNTDOWN_STOPPED;
        }
    }
}

void COUNTDOWN_DecrementValue(void)
{
    if (COUNTDOWN_u8CurrentValue > COUNTDOWN_MIN_VALUE)
    {
        COUNTDOWN_u8CurrentValue--;
        COUNTDOWN_u8StartValue = COUNTDOWN_u8CurrentValue;
        COUNTDOWN_u16TickCounter = 0U;

        BUZZER_Off();
        COUNTDOWN_UpdateDisplay();

        if (COUNTDOWN_State == COUNTDOWN_FINISHED)
        {
            COUNTDOWN_State = COUNTDOWN_STOPPED;
        }
    }
}

u8 COUNTDOWN_GetValue(void)
{
    return COUNTDOWN_u8CurrentValue;
}

u8 COUNTDOWN_GetStartValue(void)
{
    return COUNTDOWN_u8StartValue;
}

COUNTDOWN_State_t COUNTDOWN_GetState(void)
{
    return COUNTDOWN_State;
}