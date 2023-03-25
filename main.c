
#include "CLOCK.h"
#include "GPIO.h"
#include "SYS_INIT.h"
#include "USART.h"
#include "stm32f4xx.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static char input_buff[50],output_buff[50];
static uint32_t in_idx,out_idx;


void UART4_IRQHandler(void);
void UART5_IRQHandler(void);
void USART2_IRQHandler(void);
void USART2_GetString(void);
void transmit_data(uint32_t direction);

void getString(void);



void getString(void){
    uint8_t ch,idx = 0;
    ch = UART_GetChar(USART2);
    while(ch != '!'){
        input_buff[idx++] = ch;
        ch = UART_GetChar(USART2);
        if(ch == '!')break;
    }      
    input_buff[idx] = '\0';
}

void USART2_IRQHandler(void){
    USART2->CR1 &= ~(USART_CR1_RXNEIE);
    getString();
    USART2->CR1 |= (USART_CR1_RXNEIE);
}

void UART4_IRQHandler(void)
{   
    if (UART4->SR & USART_SR_RXNE){
        while(!(UART4->SR & USART_SR_RXNE));
        
        output_buff[out_idx] = (uint8_t) UART4->DR;
        
        UART4->SR &= ~(USART_SR_RXNE);
    }
    
    if (UART4->SR & USART_SR_TXE){
        /*handle queue here*/
        UART4->DR = input_buff[in_idx];
        
        while(!(UART4->SR & USART_SR_TXE));
        
        UART4->SR &= ~(USART_SR_TXE);
        UART4->CR1 &= ~(USART_CR1_TXEIE);
    }
    
}
void UART5_IRQHandler(void){
    
    if (UART5->SR & USART_SR_RXNE){   
        while(!(UART5->SR & USART_SR_RXNE));
        
        output_buff[out_idx] = (uint8_t) UART5->DR; 
        
        UART5->SR &= ~(USART_SR_RXNE);
        
    }
    if (UART5->SR & USART_SR_TXE){
        /*handle queue here*/
        UART5->DR = input_buff[in_idx];  
        while(!(UART5->SR & USART_SR_TXE));        
        
        UART5->SR &= ~(USART_SR_TXE);
        UART5->CR1 &= ~USART_CR1_TXEIE;
    }
}

static char pp = 'p'
static char cc = '#';
    
void USART2_IRQHandler(void) {
	
	if (USART2->SR & USART_SR_RXNE) 
	{
		while(!(USART2->SR & USART_SR_RXNE));
		cc = (uint8_t)USART2->DR;
		USART2->SR &= ~USART_SR_RXNE;
	}
    if (USART2->SR & USART_SR_TXE){
        /*handle queue here*/
        USART2->DR = pp;
        
        while(!(USART2->SR & USART_SR_TXE));
        
        USART2->SR &= ~(USART_SR_TXE);
        USART2->CR1 &= ~(USART_CR1_TXEIE);
    }
    

}
void USART6_IRQHandler(void) {
	
	if (USART6->SR & USART_SR_RXNE) 
	{
		while(!(USART6->SR & USART_SR_RXNE));
		cc = (uint8_t)USART6->DR;
		USART6->SR &= ~USART_SR_RXNE;
	}
	if (USART6->SR & USART_SR_TXE){
        /*handle queue here*/
        USART6->DR = pp;
        
        while(!(USART6->SR & USART_SR_TXE));
        USART6->SR &= ~(USART_SR_TXE);
        USART6->CR1 &= ~(USART_CR1_TXEIE);
    }
}


void transmit_data(uint32_t direction)
{
    /*
    direction = 0 => transmit from UART4 -> UART5
    direction = 1 => transmit from UART5 -> UART4
     */
	
    uint32_t i = 0;
    in_idx = 0;
	out_idx = 0;
    
    for (i = 0;i < strlen(input_buff);i++){
        UART4->CR1 |= USART_CR1_TXEIE;
        
        while((UART4->CR1 & USART_CR1_TXEIE));
        
        ms_delay(2);
        in_idx++;
        out_idx++;
    }
    output_buff[out_idx++] = '\0';
}






int main(void)
{   

	initClock();
	sysInit();
	UART2_Config();
	UART4_Config();
	UART5_Config();
	
    //set interrupt priority and enable IRQ
    NVIC_SetPriority(USART2_IRQn, 1);
    NVIC_EnableIRQ(USART2_IRQn);
    NVIC_SetPriority(UART4_IRQn, 1);
    NVIC_EnableIRQ(UART4_IRQn);
    NVIC_SetPriority(UART5_IRQn, 1);
    NVIC_EnableIRQ(UART5_IRQn);

    
    

    UART_SendString(USART2,"HELLO I'M IN\n");
    
    
  
	
    while(1){
		
    }


}
