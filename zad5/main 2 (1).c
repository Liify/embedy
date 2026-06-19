#include "gpio.h"

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

static void TIM6_Init(void) {
	// tim zasilanie 253
    RCC_APB1ENR1 |= (1U << 4);

    //strona 1184
    // prescaler configuration, we want 1000ms and the clock is 4 mhz
    // 1000 = 4 000 000 / (PSC + 1)
    // PSC + 1 = 4000
    // PSC = 3999 = 4000 - 1
    TIM6->PSC = 4000U - 1U;

    TIM6->ARR = 2U - 1U; //granica liczenia2ms 34.4.7 strona 1185

    // wymusza ustawienia z powyzej 1175
    TIM6->EGR |= (1U << 0);

    // start zegara 1178
    TIM6->CR1 |= (1U << 0);
}

static void TIM6_Delay_ms() {
    // czeka bo po ARR dostaje flage UIF z bitu rejestru SR - koniec petli
    while ((TIM6->SR & (1U << 0)) == 0) {
    }

    TIM6->SR &= ~(1U << 0); // reset flagi
}

static void display_show(uint8_t pos, uint8_t val) {
    GPIO_WriteMasked(DISPLAY_PORT, 0xFU << DISPLAY_PIN_BASE, 0U);
    GPIO_WriteMasked(SEGMENT_PORT, SEGMENT_MASK, 0U);

    GPIO_WriteMasked(SEGMENT_PORT, SEGMENT_MASK, digits[val]);
    GPIO_WritePin(DISPLAY_PORT, 5U - pos, 1U);
}

int main(void) {
    board_init();
    TIM6_Init();

    int counter = 0;

    uint16_t loop_cycles = 0;

    while (1) {
        display_show(3, (uint8_t)((counter / 1000) % 10)); TIM6_Delay_ms();
        display_show(2, (uint8_t)((counter / 100) % 10)); TIM6_Delay_ms();
        display_show(1, (uint8_t)((counter / 10) % 10)); TIM6_Delay_ms();
        display_show(0, (uint8_t)(counter % 10)); TIM6_Delay_ms();

        loop_cycles++;

        // 125 * 8ms = 1000ms -> sekunda
        if (loop_cycles >= 125) {
            loop_cycles = 0;

            if (GPIO_ReadPin(JOYSTICK_PORT, JOYSTICK_PIN) == 0) {
                if (--counter < 0) counter = 9999;
            } else {
                if (++counter > 9999) counter = 0;
            }
        }
    }
}
