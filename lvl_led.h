#ifndef __LVL_LED_H
#define __LVL_LED_H

#include "water.h"

#define write_red(flag) HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,flag)
#define write_yellow(flag) HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,flag)
#define write_green(flag) HAL_GPIO_WritePin(GPIOG,GPIO_PIN_6,flag)

void led_init(void);
void clear_all_leds(void);
void disp_lvl(enum WATER_LVL);

#endif
