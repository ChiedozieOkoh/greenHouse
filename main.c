#include "water.h"
#include "lvl_led.h"
#include "adc.h"
#include "buzzer.h"
#include "stm32f7xx_hal.h"
#define lazy_delay() for(i =0;i < 2147483 ; i++);



void test_buzzer(void){
   while(1){
	    play_buzzer();
	 }
} 
void test_adc_input(ADC_HandleTypeDef* adc){
   int i = 0;
   HAL_StatusTypeDef status;	
   while(1){
      HAL_ADC_Start(adc);
      clear_all_leds();
      lazy_delay();
      status = HAL_ADC_PollForConversion(adc,1000);
  
      switch (status){
         case HAL_ERROR:
            write_red(GPIO_PIN_SET);
            write_yellow(GPIO_PIN_SET);
            write_green(GPIO_PIN_SET);
            break;
             
         case HAL_TIMEOUT:
            write_yellow(GPIO_PIN_SET);
            write_green(GPIO_PIN_SET);
            break;
             
         case HAL_OK:
            write_green(GPIO_PIN_SET);
            break;
             
         case HAL_BUSY:
            write_yellow(GPIO_PIN_SET);
            break;
      }
      lazy_delay();
      HAL_ADC_GetValue(adc);
      HAL_ADC_Stop(adc);
   }

}

void test_water_lvl(ADC_HandleTypeDef* adc){
	 
   int i = 0;
   enum WATER_LVL water_lvl;

   
   while(1){
      HAL_ADC_Start(adc);
      lazy_delay();
      water_lvl = read_water_lvl(adc);
      disp_lvl(water_lvl);
      lazy_delay();
      clear_all_leds();
		 
      if(water_lvl == WATER_LOW){
		     play_buzzer();
		  }
		  HAL_ADC_Stop(adc);			
   }
}


/*TIM_HandleTypeDef htim2;


#ifdef __RTX
extern uint32_t os_time;
uint32_t HAL_GetTick(void){
	return os_time;
}
#endif

void SysTick_Handler(void){
  HAL_IncTick();
}*/




void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	/* Enable Power Control clock */
	__HAL_RCC_PWR_CLK_ENABLE();
	/* The voltage scaling allows optimizing the power
	consumption when the device is clocked below the
	maximum system frequency. */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
	/* Enable HSE Oscillator and activate PLL
	with HSE as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 25;
	RCC_OscInitStruct.PLL.PLLN = 64;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);
	/* Select PLL as system clock source and configure
	the HCLK, PCLK1 and PCLK2 clocks dividers */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | 
	RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}


int main(void){
	
   unsigned volatile int i = 0; 
   ADC_HandleTypeDef adc;
   ADC_ChannelConfTypeDef channel;
   HAL_StatusTypeDef status;
   TIM_HandleTypeDef timer; 
   

   buzzer_init();
	
   led_init(); 
   clear_all_leds();
   status = adc_init(&adc);
  
   init_water_gpio();	
	  
	
   if(status == HAL_OK) write_green(GPIO_PIN_SET);
   
   status = adc_channel_init(&adc,&channel);
   if(status == HAL_OK) write_red(GPIO_PIN_SET);

   status = HAL_ADC_PollForConversion(&adc,1000);
   if(status == HAL_OK) write_yellow(GPIO_PIN_SET);
   
   //test_adc_input(&adc);
	 //test_buzzer();
   test_water_lvl(&adc);
   //test_buzzer(&timer);
   return 0;
}
