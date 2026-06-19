#include "lpuart.h"
#include "gpio.h"

int LPUART_init(void) {
    RCC_AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
    RCC_APB1ENR2 |= RCC_APB1ENR2_LPUART1EN;

    // PC0 (RX), PC1 (TX) dla AF8
    GPIO_SetAlternateFunction(GPIOC, 0, 8);
    GPIO_SetAlternateFunction(GPIOC, 1, 8);

    GPIO_Init(GPIOC, 0, GPIO_MODE_ALTERNATE);
    GPIO_Init(GPIOC, 1, GPIO_MODE_ALTERNATE);

    LPUART1->CR1 &= ~LPUART_CR1_UE;

    // Dla zegara systemowego (zakładamy 4MHz po resecie MSI) i 115200: (256 * 4000000)/115200
    LPUART1->BRR = 8889;

    LPUART1->CR1 |= LPUART_CR1_TE;
    LPUART1->CR1 |= LPUART_CR1_RE;
    LPUART1->CR1 |= LPUART_CR1_UE;

    return 0;
}

void LPUART_SendChar(unsigned char data) {
    while (!(LPUART1->ISR & LPUART_ISR_TXE));
    LPUART1->TDR = data;
}

void LPUART_ReceiveChar(unsigned char *data) {
    while (!(LPUART1->ISR & LPUART_ISR_RXNE));
    *data = (unsigned char)(LPUART1->RDR & 0xFF);
}

void LPUART_SendString(const char *str) {
    while (*str) {
        LPUART_SendChar((unsigned char)*str);
        str++;
    }
}