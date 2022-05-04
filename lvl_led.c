#include "lvl_led.h"
#include "stm32f7xx_hal_gpio.h"



void led_init(void){
   GPIO_InitTypeDef gpio;    
   __HAL_RCC_GPIOC_CLK_ENABLE();
   __HAL_RCC_GPIOG_CLK_ENABLE();

   gpio.Mode = GPIO_MODE_OUTPUT_PP;
   gpio.Pull = GPIO_NOPULL; 
   gpio.Speed = GPIO_SPEED_HIGH;

   gpio.Pin = GPIO_PIN_6 | GPIO_PIN_7; 
   HAL_GPIO_Init(GPIOC,&gpio);

   gpio.Pin = GPIO_PIN_6;
   HAL_GPIO_Init(GPIOG,&gpio);
}

void clear_all_leds(void){
   write_red(GPIO_PIN_RESET);
   write_yellow(GPIO_PIN_RESET);
   write_green(GPIO_PIN_RESET); 
}
void disp_lvl(enum WATER_LVL level){
   clear_all_leds();
   switch(level){
      case WATER_LOW: 
         write_red(GPIO_PIN_SET);
         break;
      case WATER_MID:
         write_yellow(GPIO_PIN_SET);
         break;
      case WATER_HIGH:
         write_green(GPIO_PIN_SET);
   }
}
