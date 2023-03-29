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


void getString(void);
void parseCommand(void);


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
        
        output_buff[out_idx] = (uint8_t) UART4->DR;
        
        UART4->SR &= ~(USART_SR_RXNE);
    }
    
    if (UART4->SR & USART_SR_TXE){

        UART4->DR = input_buff[in_idx];
        
        UART4->SR &= ~(USART_SR_TXE);
        UART4->CR1 &= ~(USART_CR1_TXEIE);
    }
    
}

void UART5_IRQHandler(void){
    
    if (UART5->SR & USART_SR_RXNE){   
        
        output_buff[out_idx] = (uint8_t) UART5->DR; 
        
        UART5->SR &= ~(USART_SR_RXNE);
        
    }
    if (UART5->SR & USART_SR_TXE){

        UART5->DR = input_buff[in_idx];      
        
        UART5->SR &= ~(USART_SR_TXE);
        UART5->CR1 &= ~USART_CR1_TXEIE;
    }
}





int main(void)
{   
		
	uint32_t i = 0;
	
	/*	Configuration */
	initClock();
	sysInit();
	UART2_Config();
	UART4_Config();
	UART5_Config();
  
    NVIC_SetPriority(USART2_IRQn, 1);
    NVIC_EnableIRQ(USART2_IRQn);
    NVIC_SetPriority(UART4_IRQn, 1);
    NVIC_EnableIRQ(UART4_IRQn);
    NVIC_SetPriority(UART5_IRQn, 1);
    NVIC_EnableIRQ(UART5_IRQn);

	
	
	UART_SendString(USART2,"HELLO I'M IN\n");
	
	
	
	strcpy(input_buff,"hello this is from input\n");
	
	//sending data from input_buff array to output_buff array using UART
	//data goes from uart4 to uart5 and gets stored in output_buff
	
	
	in_idx = 0;
	out_idx = 0;
	//transmit data from UART4 to UART5
	
	for (i = 0;i < strlen(input_buff);i++){
		/*Enable Interrupt*/
		//use UART5->CR1 to transmit data from UART5 to UART4
		UART4->CR1 |= USART_CR1_TXEIE; 	
		while((UART4->CR1 & USART_CR1_TXEIE));	
		ms_delay(1);
		in_idx++;
		out_idx++;
	}
	
	
	output_buff[out_idx++] = '\0';
	
	//transmitted data is st
	
	UART_SendString(USART2,output_buff);
		
//	while(1){
//		
//		ms_delay(500);
//		
//		if(strlen(input_buff) != 0){
//			UART_SendString(USART2,input_buff);
//			strcpy(input_buff,"");
//		}
//		
//		UART_SendString(USART2,"\nhello running\n");

//	}


}




