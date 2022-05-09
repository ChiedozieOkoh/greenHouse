#ifndef DHT11_H
#define DHT11_H

#include "stm32f7xx.h"

#include "stm32f7xx_hal_gpio.h"
#include "stm32f7xx_hal_rcc.h"
#include "stm32f7xx_hal_tim.h"

#define DHT11_SUCCESS        1
#define DHT11_ERROR_CHECKSUM 2
#define DHT11_ERROR_TIMEOUT  3

typedef struct DHT11_Dev
{
	uint8_t temp;
	uint8_t hum;
	GPIO_TypeDef* port;
	uint16_t pin;
} DHT11_Dev;

int DHT11_Init(struct DHT11_Dev* develop, TIM_TypeDef *TIMx, GPIO_TypeDef* port, uint16_t pin);
int DHT11_Read(struct DHT11_Dev* develop);

#endif /* DHT11_H*/