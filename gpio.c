#include "gpio.h"

#define LED_DDR   DDRB
#define LED_PORT  PORTB
#define LED_BIT   PB5

#define BTN_DDR   DDRD
#define BTN_PORT  PORTD
#define BTN_PIN   PIND
#define BTN_BIT   PD2

void GPIO_LED_Init(void)
{
    LED_DDR |= (1 << LED_BIT);
    LED_PORT &= ~(1 << LED_BIT);
}

void GPIO_LED_On(void)
{
    LED_PORT |= (1 << LED_BIT);
}

void GPIO_LED_Off(void)
{
    LED_PORT &= ~(1 << LED_BIT);
}

void GPIO_LED_Toggle(void)
{
    PINB = (1 << LED_BIT);
}

void GPIO_Button_Init(void)
{
    BTN_DDR &= ~(1 << BTN_BIT);
    BTN_PORT |= (1 << BTN_BIT);
}

uint8_t GPIO_Button_Read(void)
{
    return !(BTN_PIN & (1 << BTN_BIT));
}
