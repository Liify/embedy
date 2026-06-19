#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

//peryferia
#define RCC_BASE 0x40021000U //strona 76 ?
#define PWR_BASE 0x40007000U
#define GPIOB_BASE 0x48000400U //strona 77
#define GPIOE_BASE 0x48001000U
#define GPIOG_BASE 0x48001800U

// offsety 303-305
#define MODER_OFFSET 0x00U //wejscie/wyjscie
#define ODR_OFFSET 0x14U //oczyt
#define IDR_OFFSET 0x10U //diody

// power registers 251/3
#define RCC_AHB2ENR (*((volatile uint32_t *)(RCC_BASE + 0x4CU))) //adres glowny + przesuniecie, jako adres(volatile bo sie moze zmienic w kazdym momencie)*wyluskanie wskaznika
#define RCC_APB1ENR1 (*((volatile uint32_t *)(RCC_BASE + 0x58U)))
#define PWR_CR2 (*((volatile uint32_t *)(PWR_BASE  + 0x04U)))//185

// GPIO register makra dostepu
#define GPIO_MODER(base) (*((volatile uint32_t *)((base) + MODER_OFFSET)))
#define GPIO_ODR(base) (*((volatile uint32_t *)((base) + ODR_OFFSET)))
#define GPIO_IDR(base) (*((volatile uint32_t *)((base) + IDR_OFFSET)))

// arm systick(programming 246)
#define SYSTICK_BASE 0xE000E010U
#define SYSTICK_CSR (*((volatile uint32_t *)(SYSTICK_BASE + 0x00U)))
#define SYSTICK_RVR (*((volatile uint32_t *)(SYSTICK_BASE + 0x04U)))
#define SYSTICK_CVR (*((volatile uint32_t *)(SYSTICK_BASE + 0x08U)))

typedef enum { //we/wy/alt/analog
    GPIO_MODE_INPUT = 0x0U,
    GPIO_MODE_OUTPUT = 0x1U,
    GPIO_MODE_AF = 0x2U,
    GPIO_MODE_ANALOG = 0x3U,
} GPIO_Mode;

typedef enum {
    GPIO_PORT_A = (1U << 0),
    GPIO_PORT_B = (1U << 1),
    GPIO_PORT_C = (1U << 2),
    GPIO_PORT_D = (1U << 3),
    GPIO_PORT_E = (1U << 4),
    GPIO_PORT_F = (1U << 5),
    GPIO_PORT_G = (1U << 6),
} GPIO_PortClock;

void GPIO_ClockEnable(GPIO_PortClock port);

void GPIO_Init(uint32_t base, uint8_t pin, GPIO_Mode mode);

void GPIO_InitRange(uint32_t base, uint8_t start_pin, uint8_t count, GPIO_Mode mode);

void GPIO_WritePin(uint32_t base, uint8_t pin, uint8_t value);

void GPIO_WriteMasked(uint32_t base, uint32_t mask, uint32_t value);

uint8_t GPIO_ReadPin(uint32_t base, uint8_t pin);

void delay(uint32_t cycles);

#endif
