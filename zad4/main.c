#include "gpio.h"

#define TIME 1000

// 4 displays (PB5-PB2)sch9
#define DISPLAY_PORT GPIOB_BASE
#define DISPLAY_PIN_BASE 2U
#define DISPLAY_PIN_COUNT 4U

// one 7-segment display (PG0-PG6)
#define SEGMENT_PORT GPIOG_BASE
#define SEGMENT_PIN_BASE 0U
#define SEGMENT_PIN_COUNT 7U
#define SEGMENT_MASK 0x7FU

// joystick (PE15)
#define JOYSTICK_PORT GPIOE_BASE
#define JOYSTICK_PIN 15U

static const uint8_t digits[10] = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66,
    0x6D, 0x7D, 0x07, 0x7F, 0x6F
};

static void board_init(void) { //inicjalizacja plytki
	// vddio2 izolowana domena zasilania
    RCC_APB1ENR1 |= (1U << 28); // 6.4.19, p. 254
    PWR_CR2 |= (1U << 9); // 5.4.2, p. 185

    GPIO_ClockEnable(GPIO_PORT_B);
    GPIO_ClockEnable(GPIO_PORT_E);
    GPIO_ClockEnable(GPIO_PORT_G);

    GPIO_InitRange(DISPLAY_PORT, DISPLAY_PIN_BASE, DISPLAY_PIN_COUNT, GPIO_MODE_OUTPUT);
    GPIO_InitRange(SEGMENT_PORT, SEGMENT_PIN_BASE, SEGMENT_PIN_COUNT, GPIO_MODE_OUTPUT);
    GPIO_Init(JOYSTICK_PORT, JOYSTICK_PIN, GPIO_MODE_INPUT);
}

volatile uint32_t system_ms = 0;

void SysTick_Handler(void) { //przerwanie co 1ms
    system_ms++;
}

static void SysTick_Init(void) {

	// 4000U zegar działa na 4MHz
    SYSTICK_RVR = 4000U - 1U; //248
    // licznik
    SYSTICK_CVR = 0U;

    // byte 0: enable the countdown
    // byte 1: run systick_handler at 0
    // byte 2: use the main cpu clock
    SYSTICK_CSR = 0x07U;
}

static void display_show(uint8_t pos, uint8_t val) {
    GPIO_WriteMasked(DISPLAY_PORT, 0xFU << DISPLAY_PIN_BASE, 0U); //gaszenie
    GPIO_WriteMasked(SEGMENT_PORT, SEGMENT_MASK, 0U);

    GPIO_WriteMasked(SEGMENT_PORT, SEGMENT_MASK, digits[val]); //zapalanie
    GPIO_WritePin(DISPLAY_PORT, 5U - pos, 1U);
}

int main(void) {
    board_init();
    SysTick_Init();

    int counter = 0;
    uint32_t last_tick = 0;

    while (1) {
        display_show(3, (uint8_t)((counter / 1000) % 10)); delay(1000U);
        display_show(2, (uint8_t)((counter / 100) % 10)); delay(1000U);
        display_show(1, (uint8_t)((counter / 10) % 10)); delay(1000U);
        display_show(0, (uint8_t)( counter % 10)); delay(1000U);

        if ((system_ms - last_tick) >= 1000) {
            last_tick = system_ms;

            if (GPIO_ReadPin(JOYSTICK_PORT, JOYSTICK_PIN) == 0) {
                if (--counter < 0) counter = 9999;
            } else {
                if (++counter > 9999) counter = 0;
            }
        }
    }
}
