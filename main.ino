#include "gpio.h"
#include <util/delay.h>

#define DEBOUNCE_MS 20
#define HOLD_MS     400

void setup(void)
{
    GPIO_LED_Init();
    GPIO_Button_Init();
}

void loop(void)
{
    static uint8_t last_stable = 0;
    static uint16_t held_ms = 0;
    static uint8_t toggle_state = 0;
    static uint8_t holding_active = 0;

    uint8_t raw = GPIO_Button_Read();

    if (raw == last_stable) {
        if (raw) {
            held_ms += DEBOUNCE_MS;
            if (held_ms >= HOLD_MS && !holding_active) {
                holding_active = 1;
                GPIO_LED_On();
            }
        }
    } else {
        last_stable = raw;
        held_ms = 0;

        if (!raw) {
            if (holding_active) {
                holding_active = 0;
                if (toggle_state) {
                    GPIO_LED_On();
                } else {
                    GPIO_LED_Off();
                }
            } else {
                toggle_state = !toggle_state;
                GPIO_LED_Toggle();
            }
        }
    }

    _delay_ms(DEBOUNCE_MS);
}
