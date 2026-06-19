#include "gpio.h"
#include "lpuart.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum {
    STATE_TOP,
    STATE_LED,
    STATE_7LED,
    STATE_JOY,
    STATE_LPUART
} MenuState;

MenuState current_state = STATE_TOP;
uint16_t current_7seg_val = 0;

void Hardware_Init(void) {
    LPUART_init();

    // Włączenie zegarów dla portów D i E
    RCC_AHB2ENR |= RCC_AHB2ENR_GPIODEN | RCC_AHB2ENR_GPIOEEN;

    // Inicjalizacja wyjść LED D1-D8 (PD0-PD7)
    for (int i = 0; i <= 7; i++) {
        GPIO_Init(GPIOD, i, GPIO_MODE_OUTPUT);
    }
    // Inicjalizacja wyjść RGB (PC7, PC8, PC9)
    GPIO_Init(GPIOC, 7, GPIO_MODE_OUTPUT);
    GPIO_Init(GPIOC, 8, GPIO_MODE_OUTPUT);
    GPIO_Init(GPIOC, 9, GPIO_MODE_OUTPUT);

    // Inicjalizacja wejść Joysticka: PE0, PE1, PE2, PE3, PE15
    GPIO_Init(GPIOE, 0, GPIO_MODE_INPUT);
    GPIO_Init(GPIOE, 1, GPIO_MODE_INPUT);
    GPIO_Init(GPIOE, 2, GPIO_MODE_INPUT);
    GPIO_Init(GPIOE, 3, GPIO_MODE_INPUT);
    GPIO_Init(GPIOE, 15, GPIO_MODE_INPUT);
}

void Print_Prompt(void) {
    switch(current_state) {
        case STATE_TOP:
            LPUART_SendString("\r\nWybierz opcje:\r\n LED\r\n 7-LED Display\r\n Joy\r\n LPUART\r\n Pomoc\r\n> ");
            break;
        case STATE_LED:    LPUART_SendString("\r\nLED> "); break;
        case STATE_7LED:   LPUART_SendString("\r\n7LED> "); break;
        case STATE_JOY:    LPUART_SendString("\r\nJoy> "); break;
        case STATE_LPUART: LPUART_SendString("\r\nLPUART> "); break;
    }
}

// Funkcja symulująca krótkie opóźnienie (np. dla funkcji Blink)
void Delay_ms(volatile uint32_t count) {
    count *= 1000;
    while(count--) {
        __asm("nop");
    }
}

void Process_LED_Command(char* id, int action) {
    // action: 0=Clear, 1=Set, 2=Toggle, 3=Status
    int is_rgb = 0;
    int pin_idx = -1;
    GPIO_TypeDef* port = GPIOD;

    if (id[0] >= '0' && id[0] <= '7') {
        pin_idx = id[0] - '0';
    } else if (strcmp(id, "R") == 0) { port = GPIOC; pin_idx = 7; is_rgb = 1; }
    else if (strcmp(id, "G") == 0) { port = GPIOC; pin_idx = 8; is_rgb = 1; }
    else if (strcmp(id, "B") == 0) { port = GPIOC; pin_idx = 9; is_rgb = 1; }
    else {
        LPUART_SendString("Nieprawidlowe ID (0-7 lub R, G, B)\r\n");
        return;
    }

    if (action == 0) { // Clear
        port->BRR = (1U << pin_idx);
    } else if (action == 1) { // Set
        port->BSRR = (1U << pin_idx);
    } else if (action == 2) { // Toggle
        port->ODR ^= (1U << pin_idx);
    } else if (action == 3) { // Status
        char buf[32];
        int status = (port->ODR & (1U << pin_idx)) ? 1 : 0;
        sprintf(buf, "Status LED %s: %s\r\n", id, status ? "ON" : "OFF");
        LPUART_SendString(buf);
    }
}

void Process_Command(char* cmd) {
    if (current_state == STATE_TOP) {
        if (strcmp(cmd, "LED") == 0) current_state = STATE_LED;
        else if (strcmp(cmd, "7-LED Display") == 0) current_state = STATE_7LED;
        else if (strcmp(cmd, "Joy") == 0) current_state = STATE_JOY;
        else if (strcmp(cmd, "LPUART") == 0) current_state = STATE_LPUART;
        else if (strcmp(cmd, "Pomoc") == 0) {
            LPUART_SendString("Opcje: LED, 7-LED Display, Joy, LPUART\r\n");
        } else {
            LPUART_SendString("Nieznana komenda na tym poziomie.\r\n");
        }
    }
    else if (current_state == STATE_LED) {
        if (strcmp(cmd, "POWROT") == 0) current_state = STATE_TOP;
        else if (strncmp(cmd, "Set ", 4) == 0) Process_LED_Command(cmd + 4, 1);
        else if (strncmp(cmd, "Clear ", 6) == 0) Process_LED_Command(cmd + 6, 0);
        else if (strncmp(cmd, "Toggle ", 7) == 0) Process_LED_Command(cmd + 7, 2);
        else if (strncmp(cmd, "Status ", 7) == 0) Process_LED_Command(cmd + 7, 3);
        else if (strncmp(cmd, "Blink ", 6) == 0) {
            for(int i=0; i<5; i++) {
                Process_LED_Command(cmd + 6, 2);
                Delay_ms(200);
                Process_LED_Command(cmd + 6, 2);
                Delay_ms(200);
            }
        } else {
            LPUART_SendString("Opcje: Set <id>, Clear <id>, Blink <id>, Status <id>, Toggle <id>, POWROT\r\n");
        }
    }
    else if (current_state == STATE_7LED) {
        if (strcmp(cmd, "POWROT") == 0) current_state = STATE_TOP;
        else if (strncmp(cmd, "Display ", 8) == 0) {
            current_7seg_val = atoi(cmd + 8);
            if (current_7seg_val > 9999) current_7seg_val = 9999;
            LPUART_SendString("Wartosc zaktualizowana wewnętrznie.\r\n");
        } else if (strcmp(cmd, "Read") == 0) {
            char buf[32];
            sprintf(buf, "Wartosc na 7-LED: %d\r\n", current_7seg_val);
            LPUART_SendString(buf);
        } else {
            LPUART_SendString("Opcje: Display <val>, Read, POWROT\r\n");
        }
    }
    else if (current_state == STATE_JOY) {
        if (strcmp(cmd, "POWROT") == 0) current_state = STATE_TOP;
        else if (strncmp(cmd, "Read ", 5) == 0) {
            char* id = cmd + 5;
            char buf[64] = {0};
            uint32_t pe = GPIOE->IDR; // Odczytujemy stan portu E

            if (strcmp(id, "U") == 0) sprintf(buf, "UP: %d\r\n", (pe & (1U<<3)) ? 1 : 0);
            else if (strcmp(id, "D") == 0) sprintf(buf, "DOWN: %d\r\n", (pe & (1U<<2)) ? 1 : 0);
            else if (strcmp(id, "L") == 0) sprintf(buf, "LEFT: %d\r\n", (pe & (1U<<1)) ? 1 : 0);
            else if (strcmp(id, "R") == 0) sprintf(buf, "RIGHT: %d\r\n", (pe & (1U<<0)) ? 1 : 0);
            else if (strcmp(id, "OK") == 0) sprintf(buf, "OK: %d\r\n", (pe & (1U<<15)) ? 1 : 0);
            else if (strcmp(id, "ALL") == 0) {
                sprintf(buf, "L:%d R:%d U:%d D:%d OK:%d\r\n",
                        (pe & (1U<<1))?1:0, (pe & (1U<<0))?1:0,
                        (pe & (1U<<3))?1:0, (pe & (1U<<2))?1:0,
                        (pe & (1U<<15))?1:0);
            } else {
                sprintf(buf, "Nieznane ID.\r\n");
            }
            LPUART_SendString(buf);
        } else {
            LPUART_SendString("Opcje: Read <id>, POWROT\r\n");
        }
    }
    else if (current_state == STATE_LPUART) {
        if (strcmp(cmd, "POWROT") == 0) current_state = STATE_TOP;
        else if (strcmp(cmd, "Status") == 0) {
            char buf[128];
            // Dekodowanie z rejestru - prosta reprezentacja
            uint32_t brr = LPUART1->BRR;
            uint32_t calc_baud = (uint32_t)((256ULL * 4000000) / brr); // Przy 4MHz
            sprintf(buf, "LPUART Status:\r\n Baudrate: %lu\r\n Data bits: 8\r\n Parity: None\r\n", calc_baud);
            LPUART_SendString(buf);
        } else {
            LPUART_SendString("Opcje: Status, POWROT\r\n");
        }
    }
}

int main(void) {
    Hardware_Init();

    LPUART_SendString("\r\n========================================\r\n");
    LPUART_SendString("Wiadomosc powitalna\r\n");
    LPUART_SendString("Nazwa programu: LPUART Kaskadowe Menu\r\n");
    LPUART_SendString("Autor: Mateusz Kowalik\r\n");
    LPUART_SendString("========================================\r\n");

    Print_Prompt();

    char rx_buffer[64];
    int rx_index = 0;
    unsigned char c;

    while (1) {
        LPUART_ReceiveChar(&c);

        if (c == '\r' || c == '\n') { // Użytkownik wciska ENTER
            if (rx_index > 0) {
                rx_buffer[rx_index] = '\0';
                LPUART_SendString("\r\n");
                Process_Command(rx_buffer);
                rx_index = 0;
            }
            Print_Prompt();
        }
        else if (c == '\b' || c == 0x7F) { // Obsługa Backspace
            if (rx_index > 0) {
                rx_index--;
                LPUART_SendString("\b \b");
            }
        }
        else {
            if (rx_index < sizeof(rx_buffer) - 1) {
                rx_buffer[rx_index++] = (char)c;
                LPUART_SendChar(c); // Echo z powrotem na terminal
            }
        }
    }

    return 0;
}