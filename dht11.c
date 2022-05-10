#include "dht11.h"
TIM_HandleTypeDef ustim;

int DHT11_Init(struct DHT11_Dev* develop, TIM_TypeDef *TIMx, GPIO_TypeDef* port, uint16_t pin)
{
	GPIO_InitTypeDef GPIOInitStructure;
	// TIMER initialization
	TIM_ClockConfigTypeDef sClkSourceConfig;
	TIM_MasterConfigTypeDef sMasterConfig;
	
	develop->port = port;
	develop->pin = pin;
	
	if(TIMx == TIM1)
	__HAL_RCC_TIM1_CLK_ENABLE();
	else if(TIMx == TIM2)
	__HAL_RCC_TIM2_CLK_ENABLE();
	else if(TIMx == TIM3)
	__HAL_RCC_TIM3_CLK_ENABLE();
	else if(TIMx == TIM4)
	__HAL_RCC_TIM4_CLK_ENABLE();
	
	
	
	
	
	ustim.Instance = TIMx;
	ustim.Init.Prescaler = 84;
	ustim.Init.CounterMode = TIM_COUNTERMODE_UP;
	ustim.Init.Period = 84000000-1;
	ustim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	
	sClkSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	
	HAL_TIM_Base_Start(&ustim);
	
	
	//GPIO DHT11 Intialized
	
GPIOInitStructure.Pin = develop->pin;
GPIOInitStructure.Mode = GPIO_MODE_OUTPUT_PP;
GPIOInitStructure.Speed = GPIO_SPEED_LOW;
HAL_GPIO_Init(develop->port, &GPIOInitStructure);

return 0;
}


int DHT11_Read(struct DHT11_Dev* develop)
{
	// Intialize the sensor
	uint8_t i, j, tmp;
	uint8_t data[5] = {0x00, 0x00, 0x00, 0x00, 0x00};
	GPIO_InitTypeDef GPIOInitStructure;
	
	//START condition
	GPIOInitStructure.Pin = develop->pin;
	GPIOInitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIOInitStructure.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(develop->port, &GPIOInitStructure);
	
	// LOW for 18ms
	HAL_GPIO_WritePin(develop->port, develop->pin, GPIO_PIN_RESET);
	
	//wait for 18ms
	
	ustim.Instance->CNT=0;
	while((ustim.Instance->CNT) <= 18000);
	
	//HIGH for 20 - 40us
	HAL_GPIO_WritePin(develop->port, develop->pin, GPIO_PIN_SET);
	
	//wait for 40us
	ustim.Instance->CNT=0;
	while((ustim.Instance->CNT) <= 40);
	//END condition
	
	//Input mode receiving data
	GPIOInitStructure.Mode = GPIO_MODE_INPUT;
	HAL_GPIO_Init(develop->port, &GPIOInitStructure);
	
	//DHT11 ACK
	ustim.Instance->CNT = 0;
	while(!HAL_GPIO_ReadPin(develop->port, develop->pin))
	{
		if(ustim.Instance->CNT > 100)
			return DHT11_ERROR_TIMEOUT;
	}
	
	for(j = 0; j < 5; ++j)
	{
		for(i = 0; i < 8; ++i)
		{
			//LOW for 50us
			while(!HAL_GPIO_ReadPin(develop->port, develop->pin));
			
			//Start the counter
			ustim.Instance->CNT = 0;
			
			//HIGH for 26-28us = 0 / 70us = 1
			while(HAL_GPIO_ReadPin(develop->port, develop->pin));
			
			tmp = ustim.Instance->CNT;
			
			data[j] = data[j] << 1;
			
			if(tmp > 40)
				data[j] = data[j]+1;
		}
	}
	
	//verifying the Checksum
	if(data[4] != (data[0] + data[2]))
		return DHT11_ERROR_CHECKSUM;
	
	//set data
	develop->temp = data[2];
	develop->hum = data[0];
	
	return DHT11_SUCCESS;
}