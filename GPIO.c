/*
 * Copyright (c) 2022 
 * Computer Science and Engineering, University of Dhaka
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE UNIVERSITY AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE UNIVERSITY OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
 
 

#include "GPIO.h"
#include "CLOCK.h"


void GPIO_WritePin(GPIO_TypeDef *GPIOx,uint16_t GPIO_pin,GPIO_PinState PinState)
{
	if (PinState == GPIO_PIN_SET)
		GPIOx->BSRR |= (1<<GPIO_pin);
	else if(PinState == GPIO_PIN_RESET)
		GPIOx->BSRR |= (1<<(GPIO_pin+(uint16_t)16));
}




void GPIO_Init(GPIO_TypeDef* GPIOx,GPIO_InitTypeDef *GPIO_Init)
{
	/*
		temp -> a variable to store the previous configuration
		position-> index of the pin from 0-15. i.e PA1, PA2..PA15 for GPIOx		

		References
		[1] Assingment Documentation
		[2] https://www.disca.upv.es/
	*/
	
	 
	uint32_t temp;
	uint32_t position;
	uint32_t cur_position;
	uint32_t check_position;
	
	// Enabling GPIOA and GPIOB in the AHB1 Bus 
	RCC->AHB1ENR |= (1<<RCC_AHB1ENR_GPIOAEN_Pos);
	RCC->AHB1ENR |= (1<<RCC_AHB1ENR_GPIOBEN_Pos);
    
	
	for (position = 0U;position < GPIO_NUMBER;position++){
		
		/*
			Assuming GPIO_Init->Mode will always be
			in output mode for this particular assignment
		*/
		cur_position = (uint32_t)(1U << position);
		check_position = (uint32_t)(GPIO_Init->Pin) & cur_position;
		
		/*
			The i'th bit of GPIO_Init->Pin indicates if the pin is to 
			be configured or not.
			
			i'th bit of GPIO_Init->Pin = 1  ===> Configure PXi
			
		*/
		if(cur_position == check_position){
			
			/* Set the PIN Mode */
			temp = GPIOx->MODER;
			temp &= ~(GPIO_MODER_MODER0 << (position * 2U));
			temp |= ((GPIO_Init->Mode & GPIO_MODE) << (position * 2U));
			GPIOx->MODER = temp;
			

			
			/* Set the PIN Output Type */
			temp = GPIOx->OTYPER;
			temp &= ~(GPIO_OTYPER_OT_0 << position);
			temp |= (((GPIO_Init->Mode & GPIO_OUTPUT_TYPE) >> 4U) << position);
			GPIOx->OTYPER = temp;
			
			/* Set the PIN Speed */
			temp = GPIOx->OSPEEDR;
			temp &= ~(GPIO_OSPEEDER_OSPEEDR0 << (position * 2U));
			temp |= ((GPIO_Init->Speed) << (position * 2U));
			GPIOx->OSPEEDR = temp;
			
			
			/* Activate the Pull-up or Pull down resistor for the current IO */
			temp = GPIOx->PUPDR;
			temp &= ~(GPIO_PUPDR_PUPDR0 << (position * 2U)); 
			temp |= ((GPIO_Init->Pull) << (position * 2U)); 
			GPIOx->PUPDR = temp; 
			
		}
	}
}


