#include "gpio.h"
#include "lpuart.h"

int main(void) {
    LPUART_init();

    // lietry alfabetu jedna po drugiej przez sendchar
    unsigned char alphabet[] = "\r\nabcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ\r\n";
    for (int i = 0; alphabet[i] != '\0'; i++) {
        LPUART_SendChar(alphabet[i]);
    }

    // wyslanie stringa funkcja
    unsigned char welcome_msg[] = "Witaj Mateusz\r\n";
    LPUART_SendString(welcome_msg);

    unsigned char current_char;

    // dynamiczna zmiana wilekosci liter nacisnietych liter
    while (1) {
        LPUART_ReceiveChar(&current_char);
        if (current_char >= 'a' && current_char <= 'z') current_char -= 32;
        else if (current_char >= 'A' && current_char <= 'Z') current_char += 32;
        LPUART_SendChar(current_char);
    }

    return 0;
}
