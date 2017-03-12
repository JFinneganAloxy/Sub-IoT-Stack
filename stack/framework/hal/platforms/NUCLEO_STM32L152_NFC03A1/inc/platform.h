/* * OSS-7 - An opensource implementation of the DASH7 Alliance Protocol for ultra
 * lowpower wireless sensor communication
 *
 * Copyright 2015 University of Antwerp
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __PLATFORM_H_
#define __PLATFORM_H_

#include "platform_defs.h"

#ifndef PLATFORM_NUCLEO_STM32L152_NFC03A1
    #error Mismatch between the configured platform and the actual platform. Expected PLATFORM_NUCLEO_STM32L152_NFC03A1 to be defined
#endif

#include <stm32l152_chip.h>


/********************
 * LED DEFINITIONS *
 *******************/

#define HW_NUM_LEDS 4
#define LED0 A8
#define LED1 B10
#define LED2 B4
#define LED3 B5
#define LED_GREEN 0
#define LED_BLUE 1


/********************
 * UART DEFINITIONS *
 *******************/

// console configuration
#define CONSOLE_UART        PLATFORM_NUCLEO_STM32L152_NFC03A1_CONSOLE_UART
#define CONSOLE_BAUDRATE    PLATFORM_NUCLEO_STM32L152_NFC03A1_CONSOLE_BAUDRATE
#define CONSOLE_LOCATION	0

/*************************
 * DEBUG PIN DEFINITIONS *
 ************************/

// #define DEBUG_PIN_NUM 4
// #define DEBUG0	D4
// #define DEBUG1	D5
// #define DEBUG2	D6
// #define DEBUG3	D7

/**************************
 * USERBUTTON DEFINITIONS *
 *************************/

#define NUM_USERBUTTONS 	1
#define BUTTON0				C13


// CC1101 PIN definitions
#ifdef USE_CC1101
 #define CC1101_SPI_USART    1
 #define CC1101_SPI_BAUDRATE 4000000
 #define CC1101_SPI_LOCATION 0
 #define CC1101_SPI_PIN_CS   C6
 #define CC1101_GDO0_PIN     C8
 //#define CC1101_GDO0_PIN     C7
 #endif
 
#define CR95HF_SPI_USART    1
#define CR95HF_SPI_BAUDRATE 2000000//40000000//8000000
#define CR95HF_SPI_LOCATION 0
#define CR95HF_SPI_PIN_CLK   A5
#define CR95HF_SPI_PIN_MISO   A6
#define CR95HF_SPI_PIN_M0SI   A7
#define CR95HF_SPI_PIN_CS   B6
#define CR95HF_IRQ_IN       A9
#define CR95HF_IRQ_OUT      A10
#define CR95HF_INTERFACE    C7

#define PLATFORM_NUM_TIMERS 1

#endif
