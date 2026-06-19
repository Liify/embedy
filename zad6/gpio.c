#include "gpio.h"

void GPIO_Init(GPIO_TypeDef *port, uint8_t pin, GPIO_Mode mode) {
    uint32_t shift = (uint32_t)pin * 2U;
    port->MODER &= ~(0x3U << shift);
    port->MODER |= ((uint32_t)mode << shift);
}

void GPIO_SetAlternateFunction(GPIO_TypeDef *port, uint8_t pin, uint8_t af_num) {
    if (pin < 8) {
    	// pins 0 through 7 use the low register AFRL
        port->AFRL &= ~(0xFU << (pin * 4));     // clear 4 bits
        port->AFRL |=  (af_num << (pin * 4));   // and set the new value
    } else {
    	// pins 8 through 15 use the high register AFRH
        port->AFRH &= ~(0xFU << ((pin - 8) * 4));
        port->AFRH |=  (af_num << ((pin - 8) * 4));
    }
}
