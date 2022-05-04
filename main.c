#include "water.h"
#include "lvl_led.h"
#include "adc.h"

#define lazy_delay() for(;i < 2147483 ; i++);\
   i = 0
	 
	 
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
      HAL_ADC_Stop(adc);
   }
}

int main(void){
   unsigned volatile int i = 0; 
   uint32_t val = 0;  
   enum WATER_LVL water_lvl; 
   ADC_HandleTypeDef adc;
   ADC_ChannelConfTypeDef channel;
   HAL_StatusTypeDef status;
   HAL_StatusTypeDef gpio_status; 


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
   test_water_lvl(&adc);
   return 0;
}
