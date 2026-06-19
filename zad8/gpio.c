#include "gpio.h"

void GPIO_Init(GPIO_TypeDef *port, uint8_t pin, GPIO_Mode mode) {
    uint32_t shift = (uint32_t)pin * 2U;
    port->MODER &= ~(0x3U << shift);
    port->MODER |= ((uint32_t)mode << shift);
}

void GPIO_SetAlternateFunction(GPIO_TypeDef *port, uint8_t pin, uint8_t af_num) {
    if (pin < 8) {
        port->AFRL &= ~(0xFU << (pin * 4));
        port->AFRL |=  (af_num << (pin * 4));
    } else {
        port->AFRH &= ~(0xFU << ((pin - 8) * 4));
        port->AFRH |=  (af_num << ((pin - 8) * 4));
    }
}