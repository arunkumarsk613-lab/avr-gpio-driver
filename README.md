# avr-gpio-driver
# Register-Level GPIO Driver (ATmega328P / Arduino Uno)

A minimal, HAL-free GPIO driver that talks straight to the AVR's I/O
registers. No `pinMode()`, no `digitalWrite()` — just direct register
manipulation, built and tested in [Wokwi](https://wokwi.com).

## Hardware

| Signal | Pin      | Register set          |
|--------|----------|------------------------|
| LED    | PB5 (D13)| DDRB / PORTB / PINB   |
| Button | PD2 (D2) | DDRD / PORTD / PIND   |

Button uses the ATmega328P's internal pull-up resistor — active LOW,
no external resistor required.

## Registers used and their addresses (ATmega328P)

| Register | I/O Address | Memory Address | Purpose                                   |
|----------|-------------|-----------------|--------------------------------------------|
| DDRB     | 0x04        | 0x24            | Data Direction Register, Port B (LED)      |
| PORTB    | 0x05        | 0x25            | Output/pull-up control, Port B (LED)       |
| PINB     | 0x03        | 0x23            | Input read / toggle-on-write, Port B       |
| DDRD     | 0x0A        | 0x2A            | Data Direction Register, Port D (Button)   |
| PORTD    | 0x0B        | 0x2B            | Output/pull-up control, Port D (Button)    |
| PIND     | 0x09        | 0x29            | Input read, Port D (Button)                |

(Addresses from the ATmega328P datasheet, I/O register summary.)

## Atomic bit access — the AVR equivalent of BSRR

STM32 uses a BSRR register for atomic set/clear. AVR has no such
register, but achieves the same result differently:

- `PORTx |= (1 << bit)` / `PORTx &= ~(1 << bit)` compile to the single
  AVR instructions `sbi` / `cbi` when the target is a low I/O
  register and the bit is a compile-time constant — these are
  **atomic, single-cycle** operations, not a true read-modify-write
  at the C level.
- Toggling uses the **PINx write-1-to-toggle** hardware feature:
  writing a `1` to a bit in `PINx` flips the corresponding `PORTx`
  output bit in hardware, with no read step at all
  (`PINB = (1 << PB5);`).

This is why the driver never does `PORTB = PORTB ^ (1 << PB5)` style
manual read-modify-write for the toggle — it lets the hardware do it
atomically.

## API

```c
void GPIO_LED_Init(void);
void GPIO_LED_On(void);
void GPIO_LED_Off(void);
void GPIO_LED_Toggle(void);

void GPIO_Button_Init(void);
uint8_t GPIO_Button_Read(void); // 1 = pressed, 0 = released
```

## Behavior (main.ino)

- **Short press** (press + quick release) → toggles the LED
- **Long press** (held past ~400 ms) → LED lights while held, and
  restores its toggle-state on release

## Build note: C/C++ linkage

`main.ino` is compiled as C++ by the Arduino toolchain, while
`gpio.c` is compiled as plain C. Without care, this causes
"undefined reference" linker errors, because C++ name-mangles
function symbols and C does not. `gpio.h` wraps its declarations in
`extern "C" { ... }` (guarded by `#ifdef __cplusplus`) so the
functions defined in `gpio.c` link correctly when called from the
C++-compiled `main.ino`.

## Files

- `gpio.h` — driver API
- `gpio.c` — register-level implementation
- `main.ino` — application logic using only the driver API

## Simulator link

[https://wokwi.com/projects/469589756626747393]
## Build/run

Open the Wokwi project → LED wired through a resistor from D13 to
GND, pushbutton wired between D2 and GND → click ▶ Run.
