/*
  modified version of StdPeriph function is located here.
  TODO - what license does apply here?
  original file was lincesed under MCD-ST Liberty SW License Agreement V2
  http://www.st.com/software_license_agreement_liberty_v2
*/

#include <stdbool.h>
#include <stdint.h>

#include "platform.h"

#include "common/utils.h"

#include "stm32f10x.h"
#include "rcc.h"
#include "timer.h"

const timerDef_t timerDefinitions[HARDWARE_TIMER_DEFINITION_COUNT] = {
    { .TIMx = TIM1,  .rcc = RCC_APB2(TIM1)  },
    { .TIMx = TIM2,  .rcc = RCC_APB1(TIM2)  },
    { .TIMx = TIM3,  .rcc = RCC_APB1(TIM3)  },
    { .TIMx = TIM4,  .rcc = RCC_APB1(TIM4)  },
#if defined(STM32F10X_HD) || defined(STM32F10X_CL) || defined(STM32F10X_XL) || defined(STM32F10X_HD_VL)
    { .TIMx = TIM5,  .rcc = RCC_APB1(TIM5)  },
    { .TIMx = TIM6,  .rcc = RCC_APB1(TIM6)  },
    { .TIMx = TIM7,  .rcc = RCC_APB1(TIM7)  },
#endif
#if defined(STM32F10X_XL) || defined(STM32F10X_HD_VL)
    { .TIMx = TIM8,  .rcc = RCC_APB1(TIM8)  },
    { .TIMx = TIM9,  .rcc = RCC_APB2(TIM9)  },
    { .TIMx = TIM10, .rcc = RCC_APB2(TIM10) },
    { .TIMx = TIM11, .rcc = RCC_APB2(TIM11) },
    { .TIMx = TIM12, .rcc = RCC_APB1(TIM12) },
    { .TIMx = TIM13, .rcc = RCC_APB1(TIM13) },
    { .TIMx = TIM14, .rcc = RCC_APB1(TIM14) },
#endif
};

uint8_t timerClockDivisor(TIM_TypeDef *tim)
{
    UNUSED(tim);
    return 1;
}

/**
  * @brief  Selects the TIM Output Compare Mode.
  * @note   This function does NOT disable the selected channel before changing the Output
  *         Compare Mode.
  * @param  TIMx: where x can be 1 to 17 except 6 and 7 to select the TIM peripheral.
  * @param  TIM_Channel: specifies the TIM Channel
  *   This parameter can be one of the following values:
  *     @arg TIM_Channel_1: TIM Channel 1
  *     @arg TIM_Channel_2: TIM Channel 2
  *     @arg TIM_Channel_3: TIM Channel 3
  *     @arg TIM_Channel_4: TIM Channel 4
  * @param  TIM_OCMode: specifies the TIM Output Compare Mode.
  *   This parameter can be one of the following values:
  *     @arg TIM_OCMode_Timing
  *     @arg TIM_OCMode_Active
  *     @arg TIM_OCMode_Toggle
  *     @arg TIM_OCMode_PWM1
  *     @arg TIM_OCMode_PWM2
  *     @arg TIM_ForcedAction_Active
  *     @arg TIM_ForcedAction_InActive
  * @retval None
  */

#define CCMR_Offset                 ((uint16_t)0x0018)

void TIM_SelectOCxM_NoDisable(TIM_TypeDef* TIMx, uint16_t TIM_Channel, uint16_t TIM_OCMode)
{
    uint32_t tmp = 0;

    /* Check the parameters */
    assert_param(IS_TIM_LIST8_PERIPH(TIMx));
    assert_param(IS_TIM_CHANNEL(TIM_Channel));
    assert_param(IS_TIM_OCM(TIM_OCMode));

    tmp = (uint32_t) TIMx;
    tmp += CCMR_Offset;

    if((TIM_Channel == TIM_Channel_1) ||(TIM_Channel == TIM_Channel_3)) {
        tmp += (TIM_Channel>>1);

        /* Reset the OCxM bits in the CCMRx register */
        *(__IO uint32_t *) tmp &= (uint32_t)~((uint32_t)TIM_CCMR1_OC1M);

        /* Configure the OCxM bits in the CCMRx register */
        *(__IO uint32_t *) tmp |= TIM_OCMode;
    } else {
        tmp += (uint16_t)(TIM_Channel - (uint16_t)4) >> (uint16_t)1;

        /* Reset the OCxM bits in the CCMRx register */
        *(__IO uint32_t *) tmp &= (uint32_t)~((uint32_t)TIM_CCMR1_OC2M);

        /* Configure the OCxM bits in the CCMRx register */
        *(__IO uint32_t *) tmp |= (uint16_t)(TIM_OCMode << 8);
    }
}
