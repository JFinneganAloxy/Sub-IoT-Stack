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

/*! \file stm32fl152_pins.c
 *
 */


#include "hwgpio.h"
#include "stm32l1xx_hal_gpio.h"

enum
{
	GPIO_PORTA = 0,
	GPIO_PORTB,
	GPIO_PORTC,
	GPIO_PORTD,
	GPIO_PORTE,
	GPIO_PORTF,
	GPIO_PORTG,
	GPIO_PORTH
} ;

//Definition of the pin_id's for the GPIO ports of the stm32l152
pin_id_t const A5 = {.port = GPIO_PORTA, .pin = 5};
pin_id_t const A6 = {.port = GPIO_PORTA, .pin = 6};
pin_id_t const A7 = {.port = GPIO_PORTA, .pin = 7};
pin_id_t const A8 = {.port = GPIO_PORTA, .pin = 8};
pin_id_t const A9 = {.port = GPIO_PORTA, .pin = 9};
pin_id_t const A10 = {.port = GPIO_PORTA, .pin = 10};
// PORT B
pin_id_t const B4 = {.port = GPIO_PORTB, .pin = 4};
pin_id_t const B5 = {.port = GPIO_PORTB, .pin = 5};
pin_id_t const B6 = {.port = GPIO_PORTB, .pin = 6};
pin_id_t const B10 = {.port = GPIO_PORTB, .pin = 10};
pin_id_t const B13 = {.port = GPIO_PORTB, .pin = 13};
pin_id_t const B14 = {.port = GPIO_PORTB, .pin = 14};
pin_id_t const B15 = {.port = GPIO_PORTB, .pin = 15};
//port C
pin_id_t const C6 = {.port = GPIO_PORTC, .pin = 6};
pin_id_t const C7 = {.port = GPIO_PORTC, .pin = 7};
pin_id_t const C8 = {.port = GPIO_PORTC, .pin = 8};
pin_id_t const C13 = {.port = GPIO_PORTC, .pin = 13};




