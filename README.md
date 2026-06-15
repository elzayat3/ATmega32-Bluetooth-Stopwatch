# ATmega32 Bluetooth Countdown Timer

A Bluetooth-controlled countdown timer project based on **ATmega32**, **HC-05 Bluetooth module**, **2-digit multiplexed seven segment display**, and a **buzzer**.

The countdown value is controlled from a mobile phone using the **Serial Bluetooth Terminal** application.  
The user can set the countdown value, start, pause, resume, reset, increment, and decrement the timer wirelessly.

---

## Project Overview

This project demonstrates a clean layered embedded systems architecture using:

- **LIB** layer for common data types and memory mapping
- **MCAL** drivers for ATmega32 peripherals
- **HAL** drivers for external hardware components
- **APP** layer for countdown logic and Bluetooth command handling

The system receives commands through the HC-05 Bluetooth module using UART communication.  
The countdown is displayed on a two-digit seven segment display, and the buzzer turns ON when the countdown reaches zero.

---

## Main Features

- Bluetooth control using **HC-05**
- UART communication at **9600 baud rate**
- Countdown value from **00 to 99 seconds**
- Two-digit multiplexed seven segment display
- Timer2-based **1ms system tick**
- Buzzer alarm when countdown reaches zero
- Non-blocking UART receive using interrupt
- Seven segment display refresh using periodic timer interrupt
- Bluetooth command parser for commands like `T45`, `S`, `P`, and `R`
- Clean layered architecture: `LIB`, `MCAL`, `HAL`, `APP`
- Modular and reusable driver structure

---

## Hardware Components

| Component | Description |
|---|---|
| ATmega32 | Main microcontroller |
| HC-05 | Bluetooth module |
| 2-Digit Seven Segment | Countdown display |
| BC546 Transistors | Digit switching for multiplexing |
| Buzzer | Alarm when timer reaches zero |
| Resistors | Segment current limiting and transistor base resistors |
| 5V Power Supply | Main power source |

---

## System Block Diagram

```text
Mobile Phone
Serial Bluetooth Terminal
        |
        | Bluetooth
        v
      HC-05
        |
        | UART
        v
     ATmega32
        |
        +------> 2-Digit Seven Segment Display
        |
        +------> Buzzer
```

---

## Hardware Connections

### HC-05 Bluetooth Module

| HC-05 Pin | ATmega32 Pin | Notes |
|---|---|---|
| VCC | 5V | Most HC-05 breakout modules accept 5V on VCC |
| GND | GND | Common ground |
| TXD | PD0 / RXD | HC-05 transmits to ATmega32 |
| RXD | PD1 / TXD | Preferably through voltage divider |

> **Note:** HC-05 RXD is usually a 3.3V logic input. If ATmega32 is running at 5V, it is recommended to use a voltage divider between ATmega32 TXD and HC-05 RXD.

---

### Seven Segment Segment Pins

| Segment | ATmega32 Pin |
|---|---|
| A | PA1 |
| B | PA2 |
| C | PA3 |
| D | PA4 |
| E | PA5 |
| F | PA6 |
| G | PA7 |
| DOT | PB0 |

---

### Seven Segment Digit Enable Pins

| Digit | ATmega32 Pin | Description |
|---|---|---|
| Digit 0 | PC6 | Units digit |
| Digit 1 | PC7 | Tens digit |

The display is configured as:

```c
.type           = SEVSEG_COMMON_CATHODE,
.digit_polarity = SEVSEG_DIGIT_ACTIVE_HIGH,
.mode           = SEVSEG_TWO_DIGITS
```

---

### Buzzer

| Buzzer Pin | ATmega32 Pin |
|---|---|
| Signal | PC5 |
| GND | GND |

---

## Bluetooth Commands

The project is controlled from the **Serial Bluetooth Terminal** mobile application.

| Command | Function |
|---|---|
| `T45` | Set countdown value to 45 seconds |
| `T09` | Set countdown value to 9 seconds |
| `T99` | Set countdown value to 99 seconds |
| `S` | Start or resume countdown |
| `P` | Pause countdown |
| `R` | Reset countdown to the saved start value |
| `+` | Increase countdown value by 1 second |
| `-` | Decrease countdown value by 1 second |

### Example Usage

```text
T45
S
```

This sets the countdown to **45 seconds** and starts counting down.

```text
P
```

This pauses the countdown at the current value.

```text
S
```

This resumes the countdown.

```text
R
```

This resets the countdown back to the last saved value.

---

## Project Architecture

```text
ATmega32-Bluetooth-Stopwatch/
│
├── README.md
├── .gitignore
│
├── APP/
│   ├── main.c
│   └── COUNTDOWN/
│       ├── COUNTDOWN_Int.h
│       ├── COUNTDOWN_Prg.c
│       └── COUNTDOWN_Private.h
│
├── LIB/
│   ├── StdTypes.h
│   └── MemMap.h
│
├── MCAL/
│   ├── DIO/
│   │   ├── DIO_Int.h
│   │   ├── DIO_Prg.c
│   │   ├── DIO_Private.h
│   │   ├── DIO_Cfg.h
│   │   └── DIO_Cfg.c
│   │
│   ├── UART/
│   │   ├── UART_Int.h
│   │   ├── UART_Prg.c
│   │   ├── UART_Private.h
│   │   ├── UART_Cfg.h
│   │   └── UART_Cfg.c
│   │
│   └── TIMER/
│       ├── TIMER_Int.h
│       ├── TIMER_Prg.c
│       ├── TIMER_Private.h
│       ├── TIMER_Cfg.h
│       └── TIMER_Cfg.c
│
└── HAL/
    ├── SEVSEG/
    │   ├── SEVSEG_Int.h
    │   ├── SEVSEG_Prg.c
    │   ├── SEVSEG_Private.h
    │   ├── SEVSEG_Cfg.h
    │   └── SEVSEG_Cfg.c
    │
    └── BUZZER/
        ├── BUZZER_Int.h
        ├── BUZZER_Prg.c
        └── BUZZER_Cfg.h
```

---

## Software Layers

### LIB Layer

Contains common definitions used by all modules.

#### Files

- `StdTypes.h`
- `MemMap.h`

#### Responsibilities

- Standard data types such as `u8`, `u16`, `u32`
- Boolean and error state definitions
- ATmega32 register mapping
- Global interrupt macros such as `sei()` and `cli()`

---

## MCAL Layer

The Microcontroller Abstraction Layer contains low-level drivers for ATmega32 peripherals.

---

## DIO Driver

Responsible for digital input/output pin control.

### Main APIs

```c
void DIO_Init(void);
void DIO_WritePin(DIO_Pin_t pin, DIO_Voltage_t value);
DIO_Voltage_t DIO_ReadPin(DIO_Pin_t pin);
void DIO_ToggelPin(DIO_Pin_t pin);
void DIO_WritePort(DIO_Port_t port, u8 value);
u8 DIO_ReadPort(DIO_Port_t port);
```

### Used For

- Seven segment segment pins
- Digit enable pins
- Buzzer output pin
- UART pins direction configuration

---

## UART Driver

Responsible for serial communication with HC-05.

### Configuration

- Baud Rate: `9600`
- Mode: Asynchronous
- Data Size: 8-bit
- Parity: Disabled
- Stop Bits: 1
- RX: Enabled
- TX: Enabled

### Main APIs

```c
void UART_Init(void);
void UART_Send(u8 data);
u8 UART_Receive(void);
void UART_SendString(const c8 *str);
u8 UART_ReceiveDirect(void);
void UART_RX_InterruptEnable(void);
void UART_RX_SetCallBack(UART_Callback_t callback);
```

### Used For

- Receiving Bluetooth commands
- Sending feedback messages to the mobile phone

---

## TIMER Driver

Responsible for generating the periodic system tick.

### Timer Used

- Timer2
- CTC Mode
- 1ms interrupt

### Timer2 Configuration

```text
F_CPU = 8 MHz
Prescaler = 64
OCR2 = 124
```

### Calculation

```text
Timer Frequency = 8,000,000 / 64 = 125,000 Hz
Timer Tick Time = 1 / 125,000 = 8 us
Compare Match = 125 ticks * 8 us = 1 ms
OCR2 = 124
```

### Used For

- Seven segment multiplexing refresh
- Countdown timing

---

## HAL Layer

The Hardware Abstraction Layer contains drivers for external hardware components.

---

## Seven Segment Driver

Responsible for displaying the countdown value.

### Features

- Supports multiplexed display
- Supports 2-digit mode
- Supports common cathode/common anode configuration
- Uses display buffer internally
- Refreshes one digit per update call
- Supports blanking delay to reduce display ghosting

### Main APIs

```c
void SEVSEG_Init(void);
void SEVSEG_SetNumber(u16 number);
void SEVSEG_Update(void);
void SEVSEG_Clear(void);
void SEVSEG_Enable(void);
void SEVSEG_Disable(void);
SEVSEG_State_t SEVSEG_GetState(void);
```

### Important Note

`SEVSEG_Update()` must be called periodically to refresh the display.  
In this project, it is called from the Timer2 callback.

---

## Buzzer Driver

Responsible for controlling the buzzer.

### Main APIs

```c
void BUZZER_On(void);
void BUZZER_Off(void);
```

### Used For

- Turning ON the alarm when countdown reaches zero
- Turning OFF the alarm when reset or new value is set

---

## APP Layer

The Application Layer contains the main project logic.

---

## Countdown Module

The countdown module handles the countdown state machine and timing logic.

### Countdown States

| State | Description |
|---|---|
| `COUNTDOWN_STOPPED` | Timer is stopped and ready to start |
| `COUNTDOWN_RUNNING` | Timer is currently counting down |
| `COUNTDOWN_PAUSED` | Timer is paused and can resume from current value |
| `COUNTDOWN_FINISHED` | Timer reached zero |

### Main APIs

```c
void COUNTDOWN_Init(void);
void COUNTDOWN_SetValue(u8 seconds);
void COUNTDOWN_Start(void);
void COUNTDOWN_Pause(void);
void COUNTDOWN_Resume(void);
void COUNTDOWN_Stop(void);
void COUNTDOWN_Reset(void);
void COUNTDOWN_Tick(void);
void COUNTDOWN_IncrementValue(void);
void COUNTDOWN_DecrementValue(void);
u8 COUNTDOWN_GetValue(void);
u8 COUNTDOWN_GetStartValue(void);
COUNTDOWN_State_t COUNTDOWN_GetState(void);
```

---

## main.c Responsibilities

The `main.c` file connects all modules together.

### It is responsible for:

- Initializing all drivers
- Starting Timer2
- Enabling UART RX interrupt
- Receiving Bluetooth characters
- Parsing Bluetooth commands
- Calling countdown APIs
- Refreshing the seven segment display
- Sending feedback messages through Bluetooth

### Main Flow

```text
1. Initialize DIO
2. Initialize UART
3. Initialize Timer
4. Initialize Seven Segment
5. Turn buzzer OFF
6. Initialize Countdown module
7. Set Timer2 callback
8. Set UART RX callback
9. Enable interrupts
10. Start Timer2
11. Wait for Bluetooth commands forever
```

---

## Interrupt Usage

### UART RX Interrupt

When a character is received from HC-05:

```text
HC-05 sends char
        |
        v
UART RX interrupt
        |
        v
Read UDR using UART_ReceiveDirect()
        |
        v
Store char in RX buffer
```

The command is not processed inside the interrupt.  
It is processed later inside the main loop.

This keeps the interrupt short and efficient.

---

### Timer2 Compare Match Interrupt

Timer2 interrupt is triggered every 1ms.

Every 1ms:

```c
SEVSEG_Update();
COUNTDOWN_Tick();
```

This means:

- Seven segment display is continuously refreshed
- Countdown timing is updated accurately

---

## Command Parser Logic

The Bluetooth command parser reads characters one by one.

For example, when the user sends:

```text
T45
```

The parser receives:

```text
'T' -> Start set-time command
'4' -> Store first digit
'5' -> Calculate value = 45 and call COUNTDOWN_SetValue(45)
```

Then the display shows:

```text
45
```

When the user sends:

```text
S
```

The project calls:

```c
COUNTDOWN_Start();
```

---

## How to Use

1. Power the ATmega32 circuit.
2. Power the HC-05 module.
3. Pair the phone with HC-05.
4. Open **Serial Bluetooth Terminal**.
5. Connect to the HC-05 module.
6. Send a command such as:

```text
T45
```

7. Send:

```text
S
```

8. The countdown starts from 45 seconds.
9. When the countdown reaches zero, the buzzer turns ON.

---

## Serial Bluetooth Terminal Settings

Recommended settings:

| Setting | Value |
|---|---|
| Baud Rate | 9600 |
| Line Ending | No line ending, CR, LF, or CRLF supported |
| Send Mode | Text |
| Commands | `Txx`, `S`, `P`, `R`, `+`, `-` |

The software ignores `\r` and `\n`, so using line endings will not break the command parser.

---

## Example Session

```text
Bluetooth Countdown Ready
Commands: Txx, S, P, R, +, -

> T45
TIME SET

> S
START

> P
PAUSE

> S
START

> R
RESET
```

---

## Important Notes

### HC-05 Power

Most HC-05 breakout modules can be powered from 5V through the VCC pin because they usually include an onboard regulator.

### HC-05 Logic Level

The HC-05 RXD pin is commonly 3.3V logic.  
Use a voltage divider from ATmega32 TXD to HC-05 RXD when possible.

### Seven Segment Ghosting

If some segments appear slightly ON when they should be OFF, this is usually called ghosting.  
It can be reduced by:

- Disabling all digits before changing segment data
- Writing a blank pattern before enabling the next digit
- Adding a small blanking delay in `SEVSEG_Update()`
- Using proper transistor base resistors and pull-down resistors in hardware

### Digit Order

For this project:

```text
Digit 0 = Units  = PC6
Digit 1 = Tens   = PC7
```

If the display shows `54` instead of `45`, swap the digit pins in `SEVSEG_Cfg.c`.

---

## Build Notes

This project is written for AVR C using ATmega32.  
It can be built using AVR-GCC or an AVR-compatible IDE.

Typical required settings:

```text
MCU: ATmega32
F_CPU: 8000000UL
Compiler: AVR-GCC
```

---

## Git Workflow

The project was developed using feature branches:

| Branch | Purpose |
|---|---|
| `DIO` | DIO driver |
| `UART` | UART driver |
| `TIMER` | Timer driver |
| `SEVSEG` | Seven Segment HAL driver |
| `BUZZER` | Buzzer HAL driver |
| `APP` | Countdown application and main logic |

Example commit commands:

```bash
git add .
git commit -m "Add Bluetooth controlled countdown application"
git push
```

---

## Future Improvements

Possible future updates:

- Add EEPROM support to save the last countdown value
- Add long command support such as `START`, `PAUSE`, `RESET`
- Add buzzer timeout instead of continuous alarm
- Add 4-digit display support for `MM:SS`
- Add mobile app buttons/macros for easier control
- Add Proteus simulation files
- Add schematic image to README
- Add Doxygen documentation generation

---

## Author

**Abdelrahman Elzayat**

Embedded Systems Project  
ATmega32 Bluetooth Countdown Timer
