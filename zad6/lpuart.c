#include "lpuart.h"
#include "gpio.h"

// TODP: add a full confiuration of the lpuart module

int LPUART_init(void) {
    // enabling the clocks
    RCC_AHB2ENR |= RCC_AHB2ENR_GPIOCEN;       // port c clock
    RCC_APB1ENR2 |= RCC_APB1ENR2_LPUART1EN;   // lpuart1 clock

    // setting the alternate function for pins pc0 (rx) and pc1 (tx). function number for lpuart 1 is 8
    GPIO_SetAlternateFunction(GPIOC, 0, GPIO_AF8_LPUART1);
    GPIO_SetAlternateFunction(GPIOC, 1, GPIO_AF8_LPUART1);

    // enable the af function
    GPIO_Init(GPIOC, 0, GPIO_MODE_ALTERNATE);
    GPIO_Init(GPIOC, 1, GPIO_MODE_ALTERNATE);

    // lpuart configuration
    LPUART1->CR1 &= ~LPUART_CR1_UE; // need to disable uart for the time of configuration
    LPUART1->BRR = 8889; // setting the 115200 baud speed for a 4mhz clock
    // (256 * 4 000 000) / 115200 = 8888,(8), p. 1412

    LPUART1->CR1 |= LPUART_CR1_UE; // enabling uart
    LPUART1->CR1 |= LPUART_CR1_TE; // enabling the tx pin
    LPUART1->CR1 |= LPUART_CR1_RE; // enabling the rx pin

    return 0;
}

// checking interrupt and status register
int LPUART_SendChar(unsigned char data) {
	// wait until the sending register is free (txe = 1)
    while ((LPUART1->ISR & LPUART_ISR_TXE) == 0) {
    }
    LPUART1->TDR = data; // transmit data register
    return 0;
}

int LPUART_ReceiveChar(unsigned char* data) {
	// wait until the receiving register has data (rxne = !)
    while ((LPUART1->ISR & LPUART_ISR_RXNE) == 0) {
    }
    *data = (unsigned char)(LPUART1->RDR & 0xFF); // receive data register
    return 0;
}

int LPUART_SendString(unsigned char* data) {
    while (*data != '\0') {
        LPUART_SendChar(*data);
        data++;
    }
    return 0;
}
