#ifndef LPUART_H
#define LPUART_H

#include <stdint.h>

#define RCC_BASE 0x40021000U
#define LPUART1_BASE 0x40008000U

#define RCC_APB1ENR2 (*((volatile uint32_t *)(RCC_BASE + 0x5CU)))
#define RCC_APB1ENR2_LPUART1EN (1U << 0)

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

#define LPUART_CR1_UE (1U << 0)
#define LPUART_CR1_RE (1U << 2)
#define LPUART_CR1_TE (1U << 3)

// Flagi statusu zdefiniowane dla ISR
#define LPUART_ISR_RXNE (1U << 5)
#define LPUART_ISR_TXE  (1U << 7)

int LPUART_init(void);
void LPUART_SendChar(unsigned char data);
void LPUART_ReceiveChar(unsigned char *data);
void LPUART_SendString(const char *str);

#endif