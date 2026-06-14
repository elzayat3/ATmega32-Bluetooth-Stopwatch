#include "StdTypes.h"
#include "MemMap.h"
#include "TIMER_Int.h"
#include "TIMER_Private.h"
#include "TIMER_Cfg.h"

/* ========================================================================== */
/*                              Callback Pointers                             */
/* ========================================================================== */

static TIMER_Callback_t TIMER0_OVF_Callback   = NULLPTR;
static TIMER_Callback_t TIMER0_COMP_Callback  = NULLPTR;

static TIMER_Callback_t TIMER1_OVF_Callback   = NULLPTR;
static TIMER_Callback_t TIMER1_COMPA_Callback = NULLPTR;
static TIMER_Callback_t TIMER1_COMPB_Callback = NULLPTR;
static TIMER_Callback_t TIMER1_ICU_Callback   = NULLPTR;

static TIMER_Callback_t TIMER2_OVF_Callback   = NULLPTR;
static TIMER_Callback_t TIMER2_COMP_Callback  = NULLPTR;

/* ========================================================================== */
/*                         Private Helper Functions                           */
/* ========================================================================== */

static u8 TIMER1_IsPwmMode(TIMER1_Mode_t mode)
{
    u8 Local_u8Result = 0U;

    if ((mode == TIMER1_MODE_FAST_PWM_ICR1) ||
        (mode == TIMER1_MODE_FAST_PWM_OCR1A) ||
        (mode == TIMER1_MODE_PHASE_CORRECT_ICR1))
    {
        Local_u8Result = 1U;
    }

    return Local_u8Result;
}

static u8 TIMER1_IsTopIcr1Mode(TIMER1_Mode_t mode)
{
    u8 Local_u8Result = 0U;

    if ((mode == TIMER1_MODE_CTC_ICR1) ||
        (mode == TIMER1_MODE_FAST_PWM_ICR1) ||
        (mode == TIMER1_MODE_PHASE_CORRECT_ICR1))
    {
        Local_u8Result = 1U;
    }

    return Local_u8Result;
}

static u8 TIMER1_IsTopOcr1aMode(TIMER1_Mode_t mode)
{
    u8 Local_u8Result = 0U;

    if ((mode == TIMER1_MODE_CTC_OCR1A) ||
        (mode == TIMER1_MODE_FAST_PWM_OCR1A))
    {
        Local_u8Result = 1U;
    }

    return Local_u8Result;
}

/* ========================================================================== */
/*                               Initialization                               */
/* ========================================================================== */

error_t TIMER_Init(void)
{
    error_t Local_ErrorState = OK;

    if (TIMER0_Init() != OK)
    {
        Local_ErrorState = NOK;
    }

    if (TIMER1_Init() != OK)
    {
        Local_ErrorState = NOK;
    }

    if (TIMER2_Init() != OK)
    {
        Local_ErrorState = NOK;
    }

    return Local_ErrorState;
}

error_t TIMER0_Init(void)
{
    error_t Local_ErrorState = OK;

    /* Stop Timer0 before configuration */
    TCCR0 &= (u8)(~TIMER_CLOCK_SELECT_MASK);

    /* Clear mode bits */
    TCCR0 &= (u8)(~TIMER0_WGM_MASK);

    switch (TIMER0_CFG.mode)
    {
        case TIMER0_MODE_NORMAL:
            /* WGM01 = 0, WGM00 = 0 */
            break;

        case TIMER0_MODE_CTC:
            SET_BIT(TCCR0, WGM01);
            break;

        case TIMER0_MODE_FAST_PWM:
            SET_BIT(TCCR0, WGM00);
            SET_BIT(TCCR0, WGM01);
            break;

        case TIMER0_MODE_PHASE_CORRECT_PWM:
            SET_BIT(TCCR0, WGM00);
            break;

        default:
            Local_ErrorState = OUT_OF_RANGE;
            break;
    }

    if (Local_ErrorState == OK)
    {
        /* Clear compare output mode bits */
        TCCR0 &= (u8)(~TIMER0_COM_MASK);

        if ((TIMER0_CFG.mode == TIMER0_MODE_FAST_PWM) ||
            (TIMER0_CFG.mode == TIMER0_MODE_PHASE_CORRECT_PWM))
        {
            switch (TIMER0_CFG.pwm_mode)
            {
                case TIMER_PWM_NON_INVERTING:
                    SET_BIT(TCCR0, COM01);
                    break;

                case TIMER_PWM_INVERTING:
                    SET_BIT(TCCR0, COM01);
                    SET_BIT(TCCR0, COM00);
                    break;

                default:
                    Local_ErrorState = OUT_OF_RANGE;
                    break;
            }
        }
        else
        {
            if ((u8)TIMER0_CFG.oc_mode <= (u8)TIMER_OC_SET)
            {
                TCCR0 |= (u8)((u8)TIMER0_CFG.oc_mode << COM00);
            }
            else
            {
                Local_ErrorState = OUT_OF_RANGE;
            }
        }
    }

    if (Local_ErrorState == OK)
    {
        TIMER0_SET_COUNTER(TIMER0_CFG.initial_value);
        TIMER0_SET_COMPARE(TIMER0_CFG.compare_value);
    }

    return Local_ErrorState;
}

error_t TIMER1_Init(void)
{
    error_t Local_ErrorState = OK;

    /* Stop Timer1 before configuration */
    TCCR1B &= (u8)(~TIMER_CLOCK_SELECT_MASK);

    /* Clear WGM bits */
    TCCR1A &= (u8)(~TIMER1_WGM_TCCR1A_MASK);
    TCCR1B &= (u8)(~TIMER1_WGM_TCCR1B_MASK);

    switch (TIMER1_CFG.mode)
    {
        case TIMER1_MODE_NORMAL:
            break;

        case TIMER1_MODE_CTC_OCR1A:
            SET_BIT(TCCR1B, WGM12);
            break;

        case TIMER1_MODE_CTC_ICR1:
            SET_BIT(TCCR1B, WGM12);
            SET_BIT(TCCR1B, WGM13);
            break;

        case TIMER1_MODE_FAST_PWM_ICR1:
            SET_BIT(TCCR1A, WGM11);
            SET_BIT(TCCR1B, WGM12);
            SET_BIT(TCCR1B, WGM13);
            break;

        case TIMER1_MODE_FAST_PWM_OCR1A:
            SET_BIT(TCCR1A, WGM10);
            SET_BIT(TCCR1A, WGM11);
            SET_BIT(TCCR1B, WGM12);
            SET_BIT(TCCR1B, WGM13);
            break;

        case TIMER1_MODE_PHASE_CORRECT_ICR1:
            SET_BIT(TCCR1A, WGM11);
            SET_BIT(TCCR1B, WGM13);
            break;

        default:
            Local_ErrorState = OUT_OF_RANGE;
            break;
    }

    if (Local_ErrorState == OK)
    {
        /* Configure OC1A */
        TCCR1A &= (u8)(~TIMER1_COM1A_MASK);

        if (TIMER1_IsPwmMode(TIMER1_CFG.mode) == 1U)
        {
            /*
             * In FAST_PWM_OCR1A mode, OCR1A is used as TOP,
             * so OC1A PWM output is not configured here.
             */
            if (TIMER1_CFG.mode != TIMER1_MODE_FAST_PWM_OCR1A)
            {
                switch (TIMER1_CFG.oc1a_pwm_mode)
                {
                    case TIMER_PWM_NON_INVERTING:
                        SET_BIT(TCCR1A, COM1A1);
                        break;

                    case TIMER_PWM_INVERTING:
                        SET_BIT(TCCR1A, COM1A1);
                        SET_BIT(TCCR1A, COM1A0);
                        break;

                    default:
                        Local_ErrorState = OUT_OF_RANGE;
                        break;
                }
            }
        }
        else
        {
            if ((u8)TIMER1_CFG.oc1a_mode <= (u8)TIMER1_OC_SET)
            {
                TCCR1A |= (u8)((u8)TIMER1_CFG.oc1a_mode << COM1A0);
            }
            else
            {
                Local_ErrorState = OUT_OF_RANGE;
            }
        }
    }

    if (Local_ErrorState == OK)
    {
        /* Configure OC1B */
        TCCR1A &= (u8)(~TIMER1_COM1B_MASK);

        if (TIMER1_IsPwmMode(TIMER1_CFG.mode) == 1U)
        {
            switch (TIMER1_CFG.oc1b_pwm_mode)
            {
                case TIMER_PWM_NON_INVERTING:
                    SET_BIT(TCCR1A, COM1B1);
                    break;

                case TIMER_PWM_INVERTING:
                    SET_BIT(TCCR1A, COM1B1);
                    SET_BIT(TCCR1A, COM1B0);
                    break;

                default:
                    Local_ErrorState = OUT_OF_RANGE;
                    break;
            }
        }
        else
        {
            if ((u8)TIMER1_CFG.oc1b_mode <= (u8)TIMER1_OC_SET)
            {
                TCCR1A |= (u8)((u8)TIMER1_CFG.oc1b_mode << COM1B0);
            }
            else
            {
                Local_ErrorState = OUT_OF_RANGE;
            }
        }
    }

    if (Local_ErrorState == OK)
    {
        TIMER1_SET_COUNTER(TIMER1_CFG.initial_value);
        TIMER1_SET_COMPARE_A(TIMER1_CFG.ocr1a_value);
        TIMER1_SET_COMPARE_B(TIMER1_CFG.ocr1b_value);

        if (TIMER1_IsTopIcr1Mode(TIMER1_CFG.mode) == 1U)
        {
            TIMER1_SET_ICR1(TIMER1_CFG.icr1_value);
        }
    }

    return Local_ErrorState;
}

error_t TIMER2_Init(void)
{
    error_t Local_ErrorState = OK;

    /* Stop Timer2 before configuration */
    TCCR2 &= (u8)(~TIMER_CLOCK_SELECT_MASK);

    /* Clear mode bits */
    TCCR2 &= (u8)(~TIMER2_WGM_MASK);

    switch (TIMER2_CFG.mode)
    {
        case TIMER2_MODE_NORMAL:
            break;

        case TIMER2_MODE_CTC:
            SET_BIT(TCCR2, WGM21);
            break;

        case TIMER2_MODE_FAST_PWM:
            SET_BIT(TCCR2, WGM20);
            SET_BIT(TCCR2, WGM21);
            break;

        case TIMER2_MODE_PHASE_CORRECT_PWM:
            SET_BIT(TCCR2, WGM20);
            break;

        default:
            Local_ErrorState = OUT_OF_RANGE;
            break;
    }

    if (Local_ErrorState == OK)
    {
        /* Clear compare output mode bits */
        TCCR2 &= (u8)(~TIMER2_COM_MASK);

        if ((TIMER2_CFG.mode == TIMER2_MODE_FAST_PWM) ||
            (TIMER2_CFG.mode == TIMER2_MODE_PHASE_CORRECT_PWM))
        {
            switch (TIMER2_CFG.pwm_mode)
            {
                case TIMER_PWM_NON_INVERTING:
                    SET_BIT(TCCR2, COM21);
                    break;

                case TIMER_PWM_INVERTING:
                    SET_BIT(TCCR2, COM21);
                    SET_BIT(TCCR2, COM20);
                    break;

                default:
                    Local_ErrorState = OUT_OF_RANGE;
                    break;
            }
        }
        else
        {
            if ((u8)TIMER2_CFG.oc_mode <= (u8)TIMER_OC_SET)
            {
                TCCR2 |= (u8)((u8)TIMER2_CFG.oc_mode << COM20);
            }
            else
            {
                Local_ErrorState = OUT_OF_RANGE;
            }
        }
    }

    if (Local_ErrorState == OK)
    {
        TIMER2_SET_COUNTER(TIMER2_CFG.initial_value);
        TIMER2_SET_COMPARE(TIMER2_CFG.compare_value);
    }

    return Local_ErrorState;
}

/* ========================================================================== */
/*                                Timer0 API                                  */
/* ========================================================================== */

void TIMER0_SetCounter(u8 value)
{
    TIMER0_SET_COUNTER(value);
}

u8 TIMER0_GetCounter(void)
{
    return TIMER0_GET_COUNTER();
}

void TIMER0_SetCompare(u8 value)
{
    TIMER0_SET_COMPARE(value);
}

u8 TIMER0_GetCompare(void)
{
    return TIMER0_GET_COMPARE();
}

/* ========================================================================== */
/*                                Timer1 API                                  */
/* ========================================================================== */

void TIMER1_SetCounter(u16 value)
{
    TIMER1_SET_COUNTER(value);
}

u16 TIMER1_GetCounter(void)
{
    return TIMER1_GET_COUNTER();
}

void TIMER1_SetCompareA(u16 value)
{
    TIMER1_SET_COMPARE_A(value);
}

u16 TIMER1_GetCompareA(void)
{
    return TIMER1_GET_COMPARE_A();
}

void TIMER1_SetCompareB(u16 value)
{
    TIMER1_SET_COMPARE_B(value);
}

u16 TIMER1_GetCompareB(void)
{
    return TIMER1_GET_COMPARE_B();
}

void TIMER1_SetTop(u16 value)
{
    if (TIMER1_IsTopOcr1aMode(TIMER1_CFG.mode) == 1U)
    {
        TIMER1_SET_COMPARE_A(value);
    }
    else if (TIMER1_IsTopIcr1Mode(TIMER1_CFG.mode) == 1U)
    {
        TIMER1_SET_ICR1(value);
    }
    else
    {
        /* Selected mode does not support programmable TOP */
    }
}

u16 TIMER1_GetTop(void)
{
    u16 Local_u16TopValue = 0xFFFFU;

    if (TIMER1_IsTopOcr1aMode(TIMER1_CFG.mode) == 1U)
    {
        Local_u16TopValue = TIMER1_GET_COMPARE_A();
    }
    else if (TIMER1_IsTopIcr1Mode(TIMER1_CFG.mode) == 1U)
    {
        Local_u16TopValue = TIMER1_GET_ICR1();
    }
    else
    {
        /* Keep default TOP value */
    }

    return Local_u16TopValue;
}

/* ========================================================================== */
/*                                Timer2 API                                  */
/* ========================================================================== */

void TIMER2_SetCounter(u8 value)
{
    TIMER2_SET_COUNTER(value);
}

u8 TIMER2_GetCounter(void)
{
    return TIMER2_GET_COUNTER();
}

void TIMER2_SetCompare(u8 value)
{
    TIMER2_SET_COMPARE(value);
}

u8 TIMER2_GetCompare(void)
{
    return TIMER2_GET_COMPARE();
}

/* ========================================================================== */
/*                            Timer Control API                               */
/* ========================================================================== */

error_t TIMER_Start(TIMER_Channel_t timer)
{
    error_t Local_ErrorState = OK;

    switch (timer)
    {
        case TIMER0:
            TCCR0 &= (u8)(~TIMER_CLOCK_SELECT_MASK);
            TCCR0 |= (u8)(TIMER0_CFG.prescaler);
            break;

        case TIMER1:
            TCCR1B &= (u8)(~TIMER_CLOCK_SELECT_MASK);
            TCCR1B |= (u8)(TIMER1_CFG.prescaler);
            break;

        case TIMER2:
            TCCR2 &= (u8)(~TIMER_CLOCK_SELECT_MASK);
            TCCR2 |= (u8)(TIMER2_CFG.prescaler);
            break;

        default:
            Local_ErrorState = OUT_OF_RANGE;
            break;
    }

    return Local_ErrorState;
}

error_t TIMER_Stop(TIMER_Channel_t timer)
{
    error_t Local_ErrorState = OK;

    switch (timer)
    {
        case TIMER0:
            TCCR0 &= (u8)(~TIMER_CLOCK_SELECT_MASK);
            break;

        case TIMER1:
            TCCR1B &= (u8)(~TIMER_CLOCK_SELECT_MASK);
            break;

        case TIMER2:
            TCCR2 &= (u8)(~TIMER_CLOCK_SELECT_MASK);
            break;

        default:
            Local_ErrorState = OUT_OF_RANGE;
            break;
    }

    return Local_ErrorState;
}

/* ========================================================================== */
/*                           Interrupt Management                             */
/* ========================================================================== */

error_t TIMER_InterruptEnable(TIMER_Channel_t timer,
                              TIMER_InterruptSource_t source)
{
    error_t Local_ErrorState = OK;

    switch (timer)
    {
        case TIMER0:
            switch (source)
            {
                case TIMER_INT_OVF:
                    SET_BIT(TIMSK, TOIE0);
                    break;

                case TIMER_INT_COMP_A:
                    SET_BIT(TIMSK, OCIE0);
                    break;

                default:
                    Local_ErrorState = OUT_OF_RANGE;
                    break;
            }
            break;

        case TIMER1:
            switch (source)
            {
                case TIMER_INT_OVF:
                    SET_BIT(TIMSK, TOIE1);
                    break;

                case TIMER_INT_COMP_A:
                    SET_BIT(TIMSK, OCIE1A);
                    break;

                case TIMER_INT_COMP_B:
                    SET_BIT(TIMSK, OCIE1B);
                    break;

                case TIMER_INT_ICU:
                    SET_BIT(TIMSK, TICIE1);
                    break;

                default:
                    Local_ErrorState = OUT_OF_RANGE;
                    break;
            }
            break;

        case TIMER2:
            switch (source)
            {
                case TIMER_INT_OVF:
                    SET_BIT(TIMSK, TOIE2);
                    break;

                case TIMER_INT_COMP_A:
                    SET_BIT(TIMSK, OCIE2);
                    break;

                default:
                    Local_ErrorState = OUT_OF_RANGE;
                    break;
            }
            break;

        default:
            Local_ErrorState = OUT_OF_RANGE;
            break;
    }

    return Local_ErrorState;
}

error_t TIMER_InterruptDisable(TIMER_Channel_t timer,
                               TIMER_InterruptSource_t source)
{
    error_t Local_ErrorState = OK;

    switch (timer)
    {
        case TIMER0:
            switch (source)
            {
                case TIMER_INT_OVF:
                    CLR_BIT(TIMSK, TOIE0);
                    break;

                case TIMER_INT_COMP_A:
                    CLR_BIT(TIMSK, OCIE0);
                    break;

                default:
                    Local_ErrorState = OUT_OF_RANGE;
                    break;
            }
            break;

        case TIMER1:
            switch (source)
            {
                case TIMER_INT_OVF:
                    CLR_BIT(TIMSK, TOIE1);
                    break;

                case TIMER_INT_COMP_A:
                    CLR_BIT(TIMSK, OCIE1A);
                    break;

                case TIMER_INT_COMP_B:
                    CLR_BIT(TIMSK, OCIE1B);
                    break;

                case TIMER_INT_ICU:
                    CLR_BIT(TIMSK, TICIE1);
                    break;

                default:
                    Local_ErrorState = OUT_OF_RANGE;
                    break;
            }
            break;

        case TIMER2:
            switch (source)
            {
                case TIMER_INT_OVF:
                    CLR_BIT(TIMSK, TOIE2);
                    break;

                case TIMER_INT_COMP_A:
                    CLR_BIT(TIMSK, OCIE2);
                    break;

                default:
                    Local_ErrorState = OUT_OF_RANGE;
                    break;
            }
            break;

        default:
            Local_ErrorState = OUT_OF_RANGE;
            break;
    }

    return Local_ErrorState;
}

/* ========================================================================== */
/*                              Callback API                                  */
/* ========================================================================== */

error_t TIMER_SetCallback(TIMER_Channel_t timer,
                          TIMER_InterruptSource_t source,
                          TIMER_Callback_t Copy_pfCallbackFunc)
{
    error_t Local_ErrorState = OK;

    switch (timer)
    {
        case TIMER0:
            switch (source)
            {
                case TIMER_INT_OVF:
                    TIMER0_OVF_Callback = Copy_pfCallbackFunc;
                    break;

                case TIMER_INT_COMP_A:
                    TIMER0_COMP_Callback = Copy_pfCallbackFunc;
                    break;

                default:
                    Local_ErrorState = OUT_OF_RANGE;
                    break;
            }
            break;

        case TIMER1:
            switch (source)
            {
                case TIMER_INT_OVF:
                    TIMER1_OVF_Callback = Copy_pfCallbackFunc;
                    break;

                case TIMER_INT_COMP_A:
                    TIMER1_COMPA_Callback = Copy_pfCallbackFunc;
                    break;

                case TIMER_INT_COMP_B:
                    TIMER1_COMPB_Callback = Copy_pfCallbackFunc;
                    break;

                case TIMER_INT_ICU:
                    TIMER1_ICU_Callback = Copy_pfCallbackFunc;
                    break;

                default:
                    Local_ErrorState = OUT_OF_RANGE;
                    break;
            }
            break;

        case TIMER2:
            switch (source)
            {
                case TIMER_INT_OVF:
                    TIMER2_OVF_Callback = Copy_pfCallbackFunc;
                    break;

                case TIMER_INT_COMP_A:
                    TIMER2_COMP_Callback = Copy_pfCallbackFunc;
                    break;

                default:
                    Local_ErrorState = OUT_OF_RANGE;
                    break;
            }
            break;

        default:
            Local_ErrorState = OUT_OF_RANGE;
            break;
    }

    return Local_ErrorState;
}

/* ========================================================================== */
/*                               Timer1 ICU API                               */
/* ========================================================================== */

error_t TIMER1_ICU_SetEdge(TIMER1_ICU_Edge_t edge)
{
    error_t Local_ErrorState = OK;

    switch (edge)
    {
        case TIMER1_ICU_FALLING_EDGE:
            CLR_BIT(TCCR1B, ICES1);
            break;

        case TIMER1_ICU_RISING_EDGE:
            SET_BIT(TCCR1B, ICES1);
            break;

        default:
            Local_ErrorState = OUT_OF_RANGE;
            break;
    }

    return Local_ErrorState;
}

u16 TIMER1_ICU_GetCaptureValue(void)
{
    return TIMER1_GET_ICR1();
}

void TIMER1_ICU_ClearFlag(void)
{
    TIMER1_CLEAR_ICU_FLAG();
}

void TIMER1_ICU_EnableNoiseCanceler(void)
{
    SET_BIT(TCCR1B, ICNC1);
}

void TIMER1_ICU_DisableNoiseCanceler(void)
{
    CLR_BIT(TCCR1B, ICNC1);
}

/* ========================================================================== */
/*                                  ISRs                                      */
/* ========================================================================== */

ISR(TIMER2_COMP_vect)
{
    if (TIMER2_COMP_Callback != NULLPTR)
    {
        TIMER2_COMP_Callback();
    }
}

ISR(TIMER2_OVF_vect)
{
    if (TIMER2_OVF_Callback != NULLPTR)
    {
        TIMER2_OVF_Callback();
    }
}

ISR(TIMER1_ICU_vect)
{
    if (TIMER1_ICU_Callback != NULLPTR)
    {
        TIMER1_ICU_Callback();
    }
}

ISR(TIMER1_OCA_vect)
{
    if (TIMER1_COMPA_Callback != NULLPTR)
    {
        TIMER1_COMPA_Callback();
    }
}

ISR(TIMER1_OCB_vect)
{
    if (TIMER1_COMPB_Callback != NULLPTR)
    {
        TIMER1_COMPB_Callback();
    }
}

ISR(TIMER1_OVF_vect)
{
    if (TIMER1_OVF_Callback != NULLPTR)
    {
        TIMER1_OVF_Callback();
    }
}

ISR(TIMER0_OC_vect)
{
    if (TIMER0_COMP_Callback != NULLPTR)
    {
        TIMER0_COMP_Callback();
    }
}

ISR(TIMER0_OV_vect)
{
    if (TIMER0_OVF_Callback != NULLPTR)
    {
        TIMER0_OVF_Callback();
    }
}