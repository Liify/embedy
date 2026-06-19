#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

// peripherals from 2.2.2, p. 77
#define RCC_BASE 0x40021000U
#define PWR_BASE 0x40007000U

// power registers, from 6.4, p. 251
#define RCC_AHB2ENR (*((volatile uint32_t *)(RCC_BASE + 0x4CU)))
#define RCC_APB1ENR1 (*((volatile uint32_t *)(RCC_BASE + 0x58U)))
#define PWR_CR2 (*((volatile uint32_t *)(PWR_BASE  + 0x04U)))

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

// timer registers structure, from p. 1180
typedef struct {
    volatile uint16_t CR1;   uint16_t reserved0;
    volatile uint16_t CR2;   uint16_t reserved1;
    uint32_t reserved_gap1;
    volatile uint16_t DIER;  uint16_t reserved2;
    volatile uint16_t SR;    uint16_t reserved3;
    volatile uint16_t EGR;   uint16_t reserved4;
    uint32_t reserved_gap2[3];
    volatile uint32_t CNT;
    volatile uint16_t PSC;   uint16_t reserved6;
    volatile uint16_t ARR;   uint16_t reserved7;
} TIM_TypeDef;

// register map, page 77
#define GPIOB ((GPIO_TypeDef *) 0x48000400U)
#define GPIOE ((GPIO_TypeDef *) 0x48001000U)
#define GPIOG ((GPIO_TypeDef *) 0x48001800U)
#define TIM6 ((TIM_TypeDef *) 0x40001000U)

typedef enum {
    GPIO_MODE_INPUT = 0x0U,
    GPIO_MODE_OUTPUT = 0x1U,
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

void GPIO_Init(GPIO_TypeDef *port, uint8_t pin, GPIO_Mode mode);

void GPIO_InitRange(GPIO_TypeDef *port, uint8_t start_pin, uint8_t count, GPIO_Mode mode);

void GPIO_WritePin(GPIO_TypeDef *port, uint8_t pin, uint8_t value);

void GPIO_WriteMasked(GPIO_TypeDef *port, uint32_t mask, uint32_t value);

uint8_t GPIO_ReadPin(GPIO_TypeDef *port, uint8_t pin);

#endif
