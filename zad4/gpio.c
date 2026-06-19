#include "gpio.h"

void GPIO_ClockEnable(GPIO_PortClock port) {
    RCC_AHB2ENR |= (uint32_t)port;
}

void GPIO_Init(GPIO_TypeDef *port, uint8_t pin, GPIO_Mode mode) {
    uint32_t shift = (uint32_t)pin * 2U;
    port->MODER &= ~(0x3U << shift);
    port->MODER |= ((uint32_t)mode << shift);
}

void GPIO_InitRange(GPIO_TypeDef *port, uint8_t start_pin, uint8_t count, GPIO_Mode mode) {
    for (uint8_t i = 0; i < count; i++) {
        GPIO_Init(port, start_pin + i, mode);
    }
}

void GPIO_WritePin(GPIO_TypeDef *port, uint8_t pin, uint8_t value) {
    if (value) {
        port->ODR |= (1U << pin);
    } else {
        port->ODR &= ~(1U << pin);
    }
}

void GPIO_WriteMasked(GPIO_TypeDef *port, uint32_t mask, uint32_t value) {
    port->ODR = (port->ODR & ~mask) | (value & mask);
}

uint8_t GPIO_ReadPin(GPIO_TypeDef *port, uint8_t pin) {
    return (port->IDR >> pin) & 0x1U;
}
