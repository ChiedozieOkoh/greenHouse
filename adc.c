#include "adc.h"
#include "lvl_led.h"
HAL_StatusTypeDef adc_init(ADC_HandleTypeDef* adc){
	
   __HAL_RCC_ADC1_CLK_ENABLE();	
   adc->Instance = ADC1;  
   adc->Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV8;
   adc->Init.Resolution = ADC_RESOLUTION_12B; 
   adc->Init.DataAlign = ADC_DATAALIGN_LEFT;
   adc->Init.ScanConvMode = DISABLE;
   adc->Init.ContinuousConvMode = ENABLE; 
   adc->Init.DiscontinuousConvMode = DISABLE;
   adc->Init.DMAContinuousRequests = DISABLE;
   adc->Init.EOCSelection = ADC_EOC_SEQ_CONV;
   adc->Init.NbrOfConversion = 1;
   //adc->Init.ExternalTrigConv = ADC_EXTERNALTRIG;
   //adc->Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
   return HAL_ADC_Start(adc);
}

HAL_StatusTypeDef adc_channel_init(ADC_HandleTypeDef* adc,ADC_ChannelConfTypeDef* channel){
   channel->Channel  = ADC_CHANNEL_0;
   channel->Rank = 1;
   channel->SamplingTime = ADC_SAMPLETIME_56CYCLES;
   channel->Offset = 0;
   return HAL_ADC_ConfigChannel(adc,channel);
}
