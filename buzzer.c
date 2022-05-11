#include "buzzer.h"
/*
 The formula used to generated this wave table is outlined in this document:
 https://www.st.com/resource/en/application_note/dm00129215-extending-the-dac-performance-of-stm32-microcontrollers-stmicroelectronics.pdf
*/


static void delay(){
   unsigned int i = 0;
   for(i = 0; i < 200000; i++);
}



static TIM_HandleTypeDef timer = {0};

static void tim_init(){
	TIM_ClockConfigTypeDef sClockSourceConfig;
	TIM_OC_InitTypeDef sConfigOC;

	//Timer configuration
	timer.Instance = TIM3;
	timer.Init.Prescaler = 16000;  //cpu frequency / prescaler = desired frequency 
	timer.Init.CounterMode = TIM_COUNTERMODE_UP;
	timer.Init.Period = 10 - 1; //period is a counter that starts from 0
	timer.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	//timer.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  //    __HAL_TIM_SET_AUTORELOAD(&timer,1);
	HAL_TIM_Base_Init(&timer);

	//Set the timer in PWM mode
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	HAL_TIM_ConfigClockSource(&timer, &sClockSourceConfig);
	HAL_TIM_PWM_Init(&timer);

	//Channel configuration
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	HAL_TIM_PWM_ConfigChannel(&timer, &sConfigOC, TIM_CHANNEL_1);
}

static void output_pin_init(){
	GPIO_InitTypeDef gpio;
	__HAL_RCC_GPIOB_CLK_ENABLE();
	gpio.Pin = GPIO_PIN_4;
	gpio.Mode = GPIO_MODE_AF_PP;
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_HIGH;
	gpio.Alternate = GPIO_AF2_TIM3;
	HAL_GPIO_Init(GPIOB, &gpio);
}
//uses timer 3 channel 1
void buzzer_init(void){
	__HAL_RCC_TIM3_CLK_ENABLE();
	tim_init();
	output_pin_init();
}

void play_buzzer(){
   timer.Instance->CCR1 = 3;
	HAL_TIM_PWM_Start(&timer, TIM_CHANNEL_1);
	delay();	
	delay();
  delay();	
  		
	
	HAL_TIM_PWM_Stop(&timer,TIM_CHANNEL_1);
	delay();	
	delay();
  delay();	
}

