#ifndef GPIO_H
#define GPIO_H

#include <avr/io.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void GPIO_LED_Init(void);
void GPIO_LED_On(void);
void GPIO_LED_Off(void);
void GPIO_LED_Toggle(void);

void GPIO_Button_Init(void);
uint8_t GPIO_Button_Read(void);

#ifdef __cplusplus
}
#endif

#endif
