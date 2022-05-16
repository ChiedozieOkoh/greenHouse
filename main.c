#include "water.h"
#include "lvl_led.h"
#include "adc.h"
#include "buzzer.h"
#include "stm32f7xx_hal.h"
#include "cmsis_os.h"
#define lazy_delay() osDelay(1000)

#define LOW_WATER_SIGNAL 0x000F

static osThreadId WTR_SENSOR_TID = NULL;
static osThreadId ALARM_TID = NULL;
static ADC_HandleTypeDef adc;
void test_buzzer(void){
   while(1){
	    play_buzzer();
	 }
} 
void test_adc_input(ADC_HandleTypeDef* adc){
   
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

void test_water_lvl(void const* arg){
	 
   
   enum WATER_LVL water_lvl;

   
   while(1){
      HAL_ADC_Start(&adc);
      lazy_delay();
      water_lvl = read_water_lvl(&adc);
      disp_lvl(water_lvl);
		 
      if(water_lvl == WATER_LOW){
        osSignalSet(ALARM_TID,LOW_WATER_SIGNAL); 
        play_buzzer();
	}else{
         osSignalClear(ALARM_TID,LOW_WATER_SIGNAL);
      }

      lazy_delay();
      clear_all_leds();
      HAL_ADC_Stop(&adc);			
   }
}

void sound_alarm(void const* arg){
   osSignalWait(LOW_WATER_SIGNAL, osWaitForever);
   play_buzzer();
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
   
   ADC_ChannelConfTypeDef channel;
   HAL_StatusTypeDef status;
   TIM_HandleTypeDef timer; 
   osThreadDef(test_water_lvl, osPriorityNormal,1, 0);
	 osThreadDef(sound_alarm, osPriorityNormal, 1,0);

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
   
	 
   WTR_SENSOR_TID = osThreadCreate(osThread(test_water_lvl),NULL);
   ALARM_TID = osThreadCreate(osThread(sound_alarm),NULL);
  
   while(1);
   return 0;
}
