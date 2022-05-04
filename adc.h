#ifndef __ADC_H
#define  __ADC_H
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_adc.h"

HAL_StatusTypeDef  adc_init(ADC_HandleTypeDef*);
HAL_StatusTypeDef  adc_channel_init(ADC_HandleTypeDef*,ADC_ChannelConfTypeDef*);
#endif
