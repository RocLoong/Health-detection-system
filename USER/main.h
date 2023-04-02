/*
 * @Author: Rocloong 63897185+RocLoong@users.noreply.github.com
 * @Date: 2023-03-26 09:39:56
 * @LastEditors: Rocloong 63897185+RocLoong@users.noreply.github.com
 * @LastEditTime: 2023-03-27 21:15:18
 * @FilePath: \USER\main.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef MAIN_H
#define MAIN_H

#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "max30102.h" 
#include "myiic.h"
#include "algorithm.h"
#include "oled.h"
#include "hc05.h"
#include "key.h"
#include "debug.h"
#include <stdlib.h>



#define MAX_BRIGHTNESS 255
#define PPG_WAVE_COUNT 	4
#define PPG_MIN_COUNT 	0X3FFFF

#define BEEP PBout(12)
#define SPO2 0
void hrv_display();
void pressure_display();
void max30102_display();
void beep_init();
// enum to switch between different 3 DISPLAY                            
typedef enum { 
    DISPLAY_SPO2, 
    DISPLAY_PRESSURE, 
    DISPLAY_HRV,
}DISPLAY_MODE;      



#endif