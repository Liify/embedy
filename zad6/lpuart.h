#ifndef LPUART_H
#define LPUART_H

#include <stdint.h>

// adresy strona 80
#define RCC_BASE 0x40021000U
#define LPUART1_BASE 0x40008000U

// zegar register strona 256
#define RCC_APB1ENR2 (*((volatile uint32_t *)(RCC_BASE + 0x5CU)))
#define RCC_APB1ENR2_LPUART1EN (1U << 0) //wlaczenie zegara

// lpuart struktura strona 1442 co 4 bajty
typedef struct {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t CR3;
    volatile uint32_t BRR;
    uint32_t reserved_gap1[2];
    volatile uint16_t RQR;
    uint16_t reserved_gap2;
    volatile uint32_t ISR;
    volatile uint32_t ICR;
    volatile uint16_t RDR;
    uint16_t reserved_gap3;
    volatile uint16_t TDR;
    uint16_t reserved_gap4;
} LPUART_TypeDef;

#define LPUART1 ((LPUART_TypeDef *) LPUART1_BASE)

// Control register 1 (LPUART_CR1) strona 1430
#define LPUART_CR1_UE (1U << 0)  // wlaczenie uart
#define LPUART_CR1_RE (1U << 2)  // wlaczenie receivera
#define LPUART_CR1_TE (1U << 3)  // wlaczenie transmitera

// rejestr przerwania/stany strona 1438
#define LPUART_ISR_RXNE (1U << 5) // niepuste dane
#define LPUART_ISR_TXE  (1U << 7) // peły transfer

// af8 funkcja alternatywna stm32 strona 109
#define GPIO_AF8_LPUART1 8U

int LPUART_init(void);
int LPUART_SendChar(unsigned char data);
int LPUART_ReceiveChar(unsigned char* data);
int LPUART_SendString(unsigned char* data);

#endif
