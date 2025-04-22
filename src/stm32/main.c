#include "stm32f103xb.h"
#include "i2c_lcd.h"
#include "adc.h"
#include <stdint.h>
#include <stdio.h>

char buffer[16];

void GPIO_Init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;  // Habilitar reloj GPIOC
    GPIOC->CRH &= ~GPIO_CRH_MODE13;      // Limpiar configuración
    GPIOC->CRH |= GPIO_CRH_MODE13_1;     // Configurar PC13 como salida
    GPIOC->CRH &= ~GPIO_CRH_CNF13;       // Configurar como salida push-pull
}

int main(void) {
    uint16_t adc_value;

    ADC1_Init();
    GPIO_Init();

    i2c_init();
    lcd_init();

    lcd_puts("Iniciando...");
    delay_ms(100);
    lcd_clear();
    lcd_puts("ADC value:");

    while (1) {
        adc_value = ADC1_Read();

        lcd_command(0xC0);
        snprintf(buffer, sizeof(buffer), "%4d", adc_value);
        lcd_puts(buffer);

        delay_ms(100);
    }
}
