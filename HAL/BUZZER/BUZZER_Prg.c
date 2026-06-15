#include "StdTypes.h"

#include "DIO_Int.h"

#include "BUZZER_Int.h"
#include "BUZZER_Cfg.h"

void BUZZER_On(void)
{
	DIO_WritePin(BUZZER_PIN, BUZZER_ACTIVE_STATE);
}

void BUZZER_Off(void)
{
	DIO_WritePin(BUZZER_PIN, BUZZER_INACTIVE_STATE);
}