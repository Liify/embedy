#include "gpio.h"

void GPIO_ClockEnable(GPIO_PortClock port) {
    RCC_AHB2ENR |= (uint32_t)port; // 6.4.17, p. 251
}

void GPIO_Init(uint32_t base, uint8_t pin, GPIO_Mode mode) {
    uint32_t shift = (uint32_t)pin * 2U;
    GPIO_MODER(base) &= ~(0x3U << shift);
    GPIO_MODER(base) |= ((uint32_t)mode << shift);
}

void GPIO_InitRange(uint32_t base, uint8_t start_pin, uint8_t count, GPIO_Mode mode) {
    for (uint8_t i = 0; i < count; i++) {
        GPIO_Init(base, start_pin + i, mode);
    }
}

void GPIO_WritePin(uint32_t base, uint8_t pin, uint8_t value) {
    if (value) {
        GPIO_ODR(base) |= (1U << pin);
    } else {
        GPIO_ODR(base) &= ~(1U << pin);
    }
}

void GPIO_WriteMasked(uint32_t base, uint32_t mask, uint32_t value) {
    GPIO_ODR(base) = (GPIO_ODR(base) & ~mask) | (value & mask);
}

uint8_t GPIO_ReadPin(uint32_t base, uint8_t pin) {
    return (GPIO_IDR(base) >> pin) & 0x1U;
}

void delay(uint32_t cycles) {
    for (volatile uint32_t i = 0; i < cycles; i++);
}
