#include "lpuart.h"
#include "gpio.h"

// TODP: add a full confiuration of the lpuart module

int LPUART_init(void) {
    // 251 port c i zegar lpuart
    RCC_AHB2ENR |= RCC_AHB2ENR_GPIOCEN;       // port c clock
    RCC_APB1ENR2 |= RCC_APB1ENR2_LPUART1EN;   // lpuart1 clock

    // ustawienie funkcji alternatywnej na piny pc0 (rx) i pc1 (tx)
    GPIO_SetAlternateFunction(GPIOC, 0, GPIO_AF8_LPUART1);
    GPIO_SetAlternateFunction(GPIOC, 1, GPIO_AF8_LPUART1);

    // wlaczanie funkcji af stm32 strona 109
    GPIO_Init(GPIOC, 0, GPIO_MODE_ALTERNATE);
    GPIO_Init(GPIOC, 1, GPIO_MODE_ALTERNATE);

    // lpuart konfiguaracja
    LPUART1->CR1 &= ~LPUART_CR1_UE; // wylaczenie uart na czas konfiguracji
    LPUART1->BRR = 8889; // ustawienie 115200 baud predkosci dla 4mhz zegara
    // (256 * 4 000 000) / 115200 = 8888,(8), strona 1412

    LPUART1->CR1 |= LPUART_CR1_UE; // wlaczanie uart
    LPUART1->CR1 |= LPUART_CR1_TE; // wlaczanie tx pin
    LPUART1->CR1 |= LPUART_CR1_RE; // wlaczanie the rx pin

    return 0;
}

// sprawdzanie rejestru przerwania/stany
int LPUART_SendChar(unsigned char data) {
	// czekamy az rejestr jest wolny (txe = 1)
    while ((LPUART1->ISR & LPUART_ISR_TXE) == 0) {
    }
    LPUART1->TDR = data; // rejestr transmisji danych 1440
    return 0;
}

int LPUART_ReceiveChar(unsigned char* data) {
	// czekamy az rejestr ma dane (rxne = !)
    while ((LPUART1->ISR & LPUART_ISR_RXNE) == 0) {
    }
    *data = (unsigned char)(LPUART1->RDR & 0xFF); // rejestr odbioru danych 1440
    return 0;
}

int LPUART_SendString(unsigned char* data) {
    while (*data != '\0') {
        LPUART_SendChar(*data);
        data++;
    }
    return 0;
}
