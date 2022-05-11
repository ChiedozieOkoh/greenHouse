#include "stm32f7xx_hal_gpio.h"
#include "water.h"
#include <stdint.h>

#define BOUND_INCR MAX_LVL / 3
#define LOW_BOUND (BOUND_INCR)
#define MID_BOUND ((BOUND_INCR) * 2)

void init_water_gpio(void){
   GPIO_InitTypeDef gpio;

   __HAL_RCC_GPIOA_CLK_ENABLE(); 

   gpio.Pin = GPIO_PIN_0; 
   gpio.Mode = GPIO_MODE_ANALOG; 
   gpio.Pull = GPIO_NOPULL;
   HAL_GPIO_Init(GPIOA,&gpio); 
}

// attempts to poll gpio pin for water level 
enum WATER_LVL read_water_lvl(ADC_HandleTypeDef* adc){
   uint32_t val = 0;  
   HAL_ADC_PollForConversion(adc,WAIT_TIME);
   val = HAL_ADC_GetValue(adc);
   if(val > MID_BOUND)return WATER_HIGH;
   if(val > LOW_BOUND)return WATER_MID;
   return WATER_LOW; 
}

