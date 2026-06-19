#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

// peripherals from 2.2.2, p. 77
#define RCC_BASE 0x40021000U
#define GPIOC ((GPIO_TypeDef *) 0x48000800U)
#define GPIOD ((GPIO_TypeDef *) 0x48000C00U) // Dodane dla LED
#define GPIOE ((GPIO_TypeDef *) 0x48001000U) // Dodane dla Joysticka

// power registers, from 6.4, p. 251
#define RCC_AHB2ENR (*((volatile uint32_t *)(RCC_BASE + 0x4CU)))
#define RCC_AHB2ENR_GPIOCEN (1U << 2)
#define RCC_AHB2ENR_GPIODEN (1U << 3) // Dodane
#define RCC_AHB2ENR_GPIOEEN (1U << 4) // Dodane

// gpio structure, from 8.4.13, p. 311
typedef struct {
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFRL;
    volatile uint32_t AFRH;
    volatile uint32_t BRR;
    volatile uint32_t ASCR;
} GPIO_TypeDef;

typedef enum {
    GPIO_MODE_INPUT = 0x0U,
    GPIO_MODE_OUTPUT = 0x1U,
    GPIO_MODE_ALTERNATE = 0x2U,
    GPIO_MODE_ANALOG = 0x3U
} GPIO_Mode;

void GPIO_Init(GPIO_TypeDef *port, uint8_t pin, GPIO_Mode mode);
void GPIO_SetAlternateFunction(GPIO_TypeDef *port, uint8_t pin, uint8_t af_num);

#endif