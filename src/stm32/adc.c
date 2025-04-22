#include "stm32f103xb.h"
#include "adc.h"

#define ADC1_DR_ADDRESS    ((uint32_t)0x4001244C)

void ADC1_Init(void) {
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

	GPIOA->CRL &= ~(GPIO_CRL_MODE0 | GPIO_CRL_CNF0);

	ADC1->CR2 |= ADC_CR2_ADON;
	for (volatile int i = 0; i < 1000; i++);

	ADC1->CR2 |= ADC_CR2_RSTCAL;
	while (ADC1->CR2 & ADC_CR2_RSTCAL);
	ADC1->CR2 |= ADC_CR2_CAL;
	while (ADC1->CR2 & ADC_CR2_CAL);

	ADC1->SQR3 = ADC_CHANNEL;
	ADC1->CR2 |= ADC_CR2_CONT;
	ADC1->CR2 |= ADC_CR2_ADON;
	ADC1->CR2 |= ADC_CR2_SWSTART;
}

uint16_t ADC1_Read(void) {
    ADC1->CR2 |= ADC_CR2_SWSTART;

    while (!(ADC1->SR & ADC_SR_EOC));

    return (uint16_t)(*(volatile uint32_t*) ADC1_DR_ADDRESS);
}
