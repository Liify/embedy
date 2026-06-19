#include "gpio.h"

void GPIO_ClockEnable(GPIO_PortClock port) {
    RCC_AHB2ENR |= (uint32_t)port; //251 wlaczam bez wylaczania taktowanie
}

void GPIO_Init(uint32_t base, uint8_t pin, GPIO_Mode mode) {
    uint32_t shift = (uint32_t)pin * 2U; //na kazdy pin przypadaja 2 bity
    GPIO_MODER(base) &= ~(0x3U << shift); //zerowanie bitow przesuwamy 11 na dobre miejsce i negujemy calosc 111100111
    GPIO_MODER(base) |= ((uint32_t)mode << shift); //wklejamy konfiguracje do czystego miejsca np 01 dla wyjscia
}

void GPIO_InitRange(uint32_t base, uint8_t start_pin, uint8_t count, GPIO_Mode mode) { //pomocnicza - petla na ta wyzej
    for (uint8_t i = 0; i < count; i++) {
        GPIO_Init(base, start_pin + i, mode);
    }
}

void GPIO_WritePin(uint32_t base, uint8_t pin, uint8_t value) { //zmiana pinu/wyswietlanie gaszenie
    if (value) {
        GPIO_ODR(base) |= (1U << pin);
    } else {
        GPIO_ODR(base) &= ~(1U << pin);
    }
}

void GPIO_WriteMasked(uint32_t base, uint32_t mask, uint32_t value) { //wiele pinow na raz jak wyzej musi byc za jednym zamachem
    GPIO_ODR(base) = (GPIO_ODR(base) & ~mask) | (value & mask);
}

uint8_t GPIO_ReadPin(uint32_t base, uint8_t pin) { //odczyt joysticka
    return (GPIO_IDR(base) >> pin) & 0x1U;
}

void delay(uint32_t cycles) { //sztuczne opoznienie
    for (volatile uint32_t i = 0; i < cycles; i++);
}
