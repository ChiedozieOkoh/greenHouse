#ifndef __WATER_H
#define __WATER_H

#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_adc.h"

// amount of time in milliseconds that the cpu will wait for a result from the adc 
#define WAIT_TIME 1 
#define MAX_LVL 1200

enum WATER_LVL{
   WATER_LOW,
   WATER_MID,
   WATER_HIGH
}; 

void init_water_gpio(void);
enum WATER_LVL read_water_lvl(ADC_HandleTypeDef* adc); 
#endif
