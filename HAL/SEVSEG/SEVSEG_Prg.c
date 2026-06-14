#include "StdTypes.h"
#include "DIO_Int.h"
#include "SEVSEG_Int.h"
#include "SEVSEG_Cfg.h"
#include "SEVSEG_Private.h"

/* ========================================================================== */
/*                          Private Definitions                               */
/* ========================================================================== */

#define SEVSEG_BLANK_DIGIT_VALUE   0xFFU

/* ========================================================================== */
/*                          Private Global Variables                          */
/* ========================================================================== */

const u8 SEVSEG_u8DigitPatterns[SEVSEG_DIGIT_COUNT_10] =
{
	0x3F, /* 0 */
	0x06, /* 1 */
	0x5B, /* 2 */
	0x4F, /* 3 */
	0x66, /* 4 */
	0x6D, /* 5 */
	0x7D, /* 6 */
	0x07, /* 7 */
	0x7F, /* 8 */
	0x6F  /* 9 */
};

static u8 SEVSEG_u8DisplayBuffer[SEVSEG_MAX_DIGITS] =
{
	SEVSEG_BLANK_DIGIT_VALUE,
	SEVSEG_BLANK_DIGIT_VALUE,
	SEVSEG_BLANK_DIGIT_VALUE,
	SEVSEG_BLANK_DIGIT_VALUE
};

static u8 SEVSEG_u8CurrentDigit = 0U;

static SEVSEG_State_t SEVSEG_State = SEVSEG_ENABLED;

/* ========================================================================== */
/*                          Private Helper Functions                          */
/* ========================================================================== */

static u8 SEVSEG_GetActiveDigitsCount(void)
{
	u8 Local_u8DigitsCount = SEVSEG_TWO_DIGIT_COUNT;

	if (SEVSEG_Config.mode == SEVSEG_FOUR_DIGITS)
	{
		Local_u8DigitsCount = SEVSEG_FOUR_DIGIT_COUNT;
	}
	else
	{
		Local_u8DigitsCount = SEVSEG_TWO_DIGIT_COUNT;
	}

	return Local_u8DigitsCount;
}

static void SEVSEG_DisableAllDigits(void)
{
	u8 Local_u8Counter;
	u8 Local_u8DigitsCount = SEVSEG_GetActiveDigitsCount();

	for (Local_u8Counter = 0U; Local_u8Counter < Local_u8DigitsCount; Local_u8Counter++)
	{
		if (SEVSEG_Config.digit_polarity == SEVSEG_DIGIT_ACTIVE_LOW)
		{
			DIO_WritePin(SEVSEG_Config.digit_pins[Local_u8Counter], HIGH);
		}
		else
		{
			DIO_WritePin(SEVSEG_Config.digit_pins[Local_u8Counter], LOW);
		}
	}
}

static void SEVSEG_EnableDigit(u8 Copy_u8DigitIndex)
{
	if (SEVSEG_Config.digit_polarity == SEVSEG_DIGIT_ACTIVE_LOW)
	{
		DIO_WritePin(SEVSEG_Config.digit_pins[Copy_u8DigitIndex], LOW);
	}
	else
	{
		DIO_WritePin(SEVSEG_Config.digit_pins[Copy_u8DigitIndex], HIGH);
	}
}

static void SEVSEG_WritePattern(u8 Copy_u8Pattern)
{
	u8 Local_u8PortValue;

	if (SEVSEG_Config.type == SEVSEG_COMMON_ANODE)
	{
		Copy_u8Pattern = (u8)(~Copy_u8Pattern);
	}

	Local_u8PortValue = (u8)(Copy_u8Pattern << SEVSEG_Config.segment_shift);

	DIO_WritePort(SEVSEG_Config.segment_port, Local_u8PortValue);
}

static void SEVSEG_WriteBlank(void)
{
	SEVSEG_WritePattern(SEVSEG_BLANK_PATTERN);
}

/* ========================================================================== */
/*                              Public APIs                                   */
/* ========================================================================== */

void SEVSEG_Init(void)
{
	SEVSEG_Clear();
	SEVSEG_u8CurrentDigit = 0U;
	SEVSEG_State = SEVSEG_ENABLED;

	SEVSEG_DisableAllDigits();
	SEVSEG_WriteBlank();
}

void SEVSEG_SetNumber(u16 Copy_u16Number)
{
	u8 Local_u8DigitsCount = SEVSEG_GetActiveDigitsCount();
	u8 Local_u8Counter;

	if (Local_u8DigitsCount == SEVSEG_TWO_DIGIT_COUNT)
	{
		Copy_u16Number %= SEVSEG_MAX_TWO_DIGIT_VALUE;
	}
	else
	{
		Copy_u16Number %= SEVSEG_MAX_FOUR_DIGIT_VALUE;
	}

	for (Local_u8Counter = 0U; Local_u8Counter < Local_u8DigitsCount; Local_u8Counter++)
	{
		SEVSEG_u8DisplayBuffer[Local_u8Counter] = (u8)(Copy_u16Number % 10U);
		Copy_u16Number /= 10U;
	}
}

void SEVSEG_SetTime(u8 Copy_u8Minutes, u8 Copy_u8Seconds)
{
	Copy_u8Seconds %= SEVSEG_SECONDS_LIMIT;
	Copy_u8Minutes %= SEVSEG_MAX_TWO_DIGIT_VALUE;

	SEVSEG_u8DisplayBuffer[0] = Copy_u8Seconds % 10U;  /* Seconds units */
	SEVSEG_u8DisplayBuffer[1] = Copy_u8Seconds / 10U;  /* Seconds tens  */

	if (SEVSEG_Config.mode == SEVSEG_FOUR_DIGITS)
	{
		SEVSEG_u8DisplayBuffer[2] = Copy_u8Minutes % 10U; /* Minutes units */
		SEVSEG_u8DisplayBuffer[3] = Copy_u8Minutes / 10U; /* Minutes tens  */
	}
}

void SEVSEG_Update(void)
{
	u8 Local_u8DigitsCount = SEVSEG_GetActiveDigitsCount();
	u8 Local_u8DigitValue;

	SEVSEG_DisableAllDigits();

	if (SEVSEG_State == SEVSEG_ENABLED)
	{
		if (SEVSEG_u8CurrentDigit >= Local_u8DigitsCount)
		{
			SEVSEG_u8CurrentDigit = 0U;
		}

		Local_u8DigitValue = SEVSEG_u8DisplayBuffer[SEVSEG_u8CurrentDigit];

		if (Local_u8DigitValue < SEVSEG_DIGIT_COUNT_10)
		{
			SEVSEG_WritePattern(SEVSEG_u8DigitPatterns[Local_u8DigitValue]);
		}
		else
		{
			SEVSEG_WriteBlank();
		}

		SEVSEG_EnableDigit(SEVSEG_u8CurrentDigit);

		SEVSEG_u8CurrentDigit++;

		if (SEVSEG_u8CurrentDigit >= Local_u8DigitsCount)
		{
			SEVSEG_u8CurrentDigit = 0U;
		}
	}
	else
	{
		SEVSEG_WriteBlank();
	}
}

void SEVSEG_Clear(void)
{
	u8 Local_u8Counter;

	for (Local_u8Counter = 0U; Local_u8Counter < SEVSEG_MAX_DIGITS; Local_u8Counter++)
	{
		SEVSEG_u8DisplayBuffer[Local_u8Counter] = SEVSEG_BLANK_DIGIT_VALUE;
	}
}

void SEVSEG_Enable(void)
{
	SEVSEG_State = SEVSEG_ENABLED;
}

void SEVSEG_Disable(void)
{
	SEVSEG_State = SEVSEG_DISABLED;
	SEVSEG_DisableAllDigits();
	SEVSEG_WriteBlank();
}

SEVSEG_State_t SEVSEG_GetState(void)
{
	return SEVSEG_State;
}