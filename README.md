# MTRX2700-C_LAB-THURSPM-GROUP5

## Group Information

**Team Number:** THURSPM-G5  
**Unit:** MTRX2700 – Mechatronics 2  
**Semester:** S1 2025  

### Group Members & Roles

| Name           | Role                         | Responsibilities                                                                                         |
|----------------|------------------------------|----------------------------------------------------------------------------------------------------------|
| Saksham        | Systems & Comms Lead         | Exercise 1, Exercise 3, Integration                                                                      |
| Shirui         | I/O & Control Lead           | Exercise 2, Integration                                                                                  |
| Jason          | DevOps & Architecture Lead   | Project structure, modular API design, documentation, integration testing, Meeting Minutes               |

---

## Project Description
This project is a series of embedded system modules programmed in **C** for the **STM32F3 Discovery Board**. It was completed over Weeks 6–8 as part of the MTRX2700 C Lab Series.

The goal is to demonstrate modular, low-level embedded development using:
- Manual memory management and pointer logic
- LED and button I/O interaction
- Serial communication (UART)
- Hardware timers and delay functions
- Integration of all components into a working system

## Project Structure

/Exercise 1/
├── Exercise 1 (A,B,C)   -> Digital I/O (LED control, button interrupts, state encapsulation)
├── Exercise 1 (D)       -> Timed LED update logic using hardware timers

/Exercise 2/
├── Exercise 2 (A)       -> UART send on button press
├── Exercise 2 (B)       -> UART receive into memory buffer
├── Exercise 2 (C)       -> Clock update to support faster baud rates
├── Exercise 2 (D)       -> UART forwarding between two STM32 boards

/Exercise 3/
├── Exercise 3 (A)       -> Timer with callback at regular interval
├── Exercise 3 (B)       -> Timer period getter/setter (encapsulation)
├── Exercise 3 (C)       -> One-shot timer with callback

/Exercise 4/
├── Final Integration    -> Palindrome check, Caesar cipher, UART to MCU2, LED display with timer




 ## Testing Plan

| **Module**                          | **Test Description**                                                                 | **Method**                                                                         | **Expected Outcome**                                                        | **Status** |
|-------------------------------------|--------------------------------------------------------------------------------------|------------------------------------------------------------------------------------|--------------------------------------------------------------------------------|----------|
| **Exercise 1 (A,B,C)** – LED Control & Button Interface | Verify LED output mode and state updating                                             | Set various bitmasks via `set_led_state()`, observe physical LEDs                 | LEDs reflect correct bit pattern as expected                                  | ✅        |
|                                     | Confirm button interrupt and callback trigger                                        | Register `button_callback`, press button on PE0                                   | Callback function executes (e.g., LED shifts)                                 | ✅        |
|                                     | Validate state encapsulation with `get_led_state()`                                 | Set LED pattern, retrieve via getter                                               | Retrieved value matches last set pattern                                      | ✅        |
| **Exercise 1 (D)** – Timed LED Updates  | Ensure LED changes occur on hardware timer rather than blocking delay                | Use timer with registered LED pattern update callback                             | LED toggles at defined intervals with no UI delay                             | ✅        |
|                                     | Confirm immediate return from set function                                           | Set LED state while timer running                                                  | No polling delay; system remains responsive                                   | ✅        |
| **Exercise 2 (A)** – UART Transmit on Button Press | Send string from STM32 to PC over UART                                                | Connect serial terminal (PuTTY/Tera Term), press button                           | String appears in terminal upon button press                                  | ✅        |
| **Exercise 2 (B)** – UART Receive into Buffer | Store typed string into buffer until CR (0x0D)                                        | Send string via terminal, inspect buffer via debugger                             | Characters stored correctly until terminating character                       | ✅        |
| **Exercise 2 (C)** – Clock Update for Baud Rate | Test UART after system clock/PLL reconfiguration                                     | Adjust PLL, verify transmission using logic analyzer or terminal                  | Baud rate still valid; messages received correctly                            | ✅        |
| **Exercise 2 (D)** – UART Port Forwarding | Verify message pass-through from PC → MCU1 → MCU2                                    | Chain UART1 and UART2 on 2 boards, observe forwarded message                      | Full message appears on MCU2 from PC                                          | ✅        |
| **Exercise 3 (A)** – Regular Interval Callback | Trigger callback function repeatedly on fixed interval                               | Register callback with interval (e.g., 500ms), blink LED                          | Callback executes periodically based on set time                              | ✅        |
| **Exercise 3 (B)** – Getter/Setter Encapsulation | Get and set timer period externally while keeping internals private                  | Use `set_period()` and `get_period()`, attempt direct access                      | Period adjustable only via interface; timer module is encapsulated            | ✅        |
| **Exercise 3 (C)** – One-Shot Timer     | Trigger a single delayed callback after defined ms                                   | Set one-shot delay with callback, observe LED or debugger                         | Callback occurs once and doesn’t repeat                                        | ✅        |
| **Exercise 4** – Final Integration       | End-to-end test: Palindrome → Caesar → UART → LED display                            | Type message in terminal → check output on MCU2 LEDs                              | Palindrome is encrypted and decoded; vowel/consonant counts shown at 500ms   | ✅        |
|                                     | Test non-palindrome message handling                                                 | Send non-palindrome from PC                                                       | Message forwarded as-is to MCU2                                               | ✅        |
|                                     | Edge case: input symbols and whitespace                                              | Send string with special characters                                               | Non-alphabetic characters are skipped or ignored safely                       | ✅        |



## Project Module Explanation

## 📦 Module Descriptions

| **Module** | **Purpose** | **Features** | **Basic Setup** | **✅ Expected Output** |
|------------|-------------|--------------|------------------|------------------------|
| **Exercise 1 (A,B,C)** – LED Control & Button Interface | Encapsulates digital I/O functionality and supports interrupt-based LED updates | - Initializes PE8–PE15 (LEDs), PA0 (button) <br> - Function pointer callback on button press <br> - Clean API: `dio_init()`, `dio_setLED()`, `dio_toggleLED()`, `dio_getLEDState()` | 1. Flash to STM32F3 <br> 2. Observe PE8 ON <br> 3. Press PA0 → LED chase | **Initial:** PE8 ON <br> **Each press:** PE8→PE9→...→PE15 <br> **Wraps** to PE8 |
| **Exercise 1 (D)** – Timed LED Updates | Restricts LED update frequency using TIM2 timer | - Non-blocking timer-based update gating <br> - Clean API: `timer_init()`, `timer_check_flag()`, `timer_clear_flag()` | 1. Flash to board <br> 2. Press PA0 repeatedly <br> 3. LEDs update only every 1s | **Initial:** All OFF <br> **Button press:** One LED toggles per second max |
| **Exercise 2 (A)** – UART Transmit on Button Press | Transmit fixed message over UART1 when PA0 is pressed | - UART1 @ 9600 baud <br> - `uart1_init()`, `uart1_send()` | 1. Connect STM32 to PC <br> 2. Open PuTTY/TeraTerm <br> 3. Press PA0 | **On press:** `"Hello UART"` appears <br> **No lag or corruption** |
| **Exercise 2 (B)** – UART Receive into Buffer | Store UART input until carriage return (0x0D) | - Polling UART receive into buffer <br> - API: `uart1_receive()` | 1. Open terminal <br> 2. Type string + CR <br> 3. Inspect buffer | **Input:** `"test<CR>"` → Buffer: `'t','e','s','t','\0'` |
| **Exercise 2 (C)** – Clock Update for Baud Rate | Reconfigures system clock and verifies UART consistency | - PLL clock increase <br> - BRR re-calculation <br> - `clock_init()`, `uart1_reconfig_baud()` | 1. Flash code <br> 2. Update clock <br> 3. Verify UART output | **Terminal sees** correct output post-clock update, no glitches |
| **Exercise 2 (D)** – UART Port Forwarding | Relays UART1 input → UART2 output across 2 boards | - MCU1: UART1 → UART2 <br> - MCU2: UART2 receive <br> - `uart1_receive()`, `uart2_send()` | 1. Wire two STM32s (TX↔RX) <br> 2. Type message from PC <br> 3. MCU2 reacts | **PC→MCU1→MCU2** works seamlessly <br> Message forwarded intact |
| **Exercise 3 (A)** – Regular Interval Callback | Executes callback at fixed interval using TIM2 | - Uses TIM2 interrupts <br> - API: `timer_init(interval, callback)` | 1. Flash with LED toggle as callback <br> 2. Observe blinking | **PE8 blinks** every 500ms <br> No blocking |
| **Exercise 3 (B)** – Getter/Setter Encapsulation | Encapsulates timer period via accessors | - Internal static period <br> - `timer_set_period()`, `timer_get_period()` | 1. Use setter to change interval <br> 2. Observe LED blink rate | **Faster blinking** post change <br> External access blocked |
| **Exercise 3 (C)** – One-Shot Timer | Triggers single delayed event, no repetition | - Uses TIM2, disables after fire <br> - API: `timer_oneshot(delay, callback)` | 1. Call oneshot with LED toggle <br> 2. Wait 1s | **Single LED toggle** after 1s <br> No repeats |
| **Exercise 4** – Final Integration | Combines UART, cipher, and LED modules across 2 MCUs | - MCU1: Palindrome check → Caesar → UART2 <br> - MCU2: Decode → Count vowels/consonants → LED display | 1. Type `"Racecar"` to MCU1 <br> 2. MCU2 LEDs toggle counts at 500ms | **Palindrome:** Encoded → Decoded <br> **Non-palindrome:** Forwarded <br> **Alternating LED** counts |


###  **Exercise 1 (A,B,C)** – LED Control & Button Interface
### Purpose  
- Encapsulates digital I/O functionality  
- Abstracts register access via clean C functions  
- Enables interrupt-driven LED control  

### Features  
- Initializes LEDs (PE8–PE15) and button (PA0)  
- Supports function pointer callback on button press  
- Provides clean API:
  - `dio_init(void (*callback)(void));`
  - `dio_setLED(int index, bool state);`
  - `dio_toggleLED(int index);`
  - `uint8_t dio_getLEDState(void);`


### Basic Setup
1. Flash to STM32F3 Discovery board
2. Observe PE8 LED ON at startup
3. Press USER button (PA0)
4. On each press, LED pattern shifts right (chase style)

### ✅ Expected Output
- **Initial state**: PE8 ON, others OFF
- **Each button press**: next LED lights up (PE8 → PE9 → PE10...) 
- **Wraps around** after PE15


### **Exercise 1 (D)** – Timed LED Updates
### Purpose  
- Restricts LED update frequency using a hardware timer  
- Ensures non-blocking LED state changes via interrupts  

### Features  
- Implements timer (TIM2) to control minimum interval between LED updates  
- Uses interrupts to asynchronously manage LED state updates  
- Maintains encapsulation and modularity from previous exercises  
- Provides clean API:
  - `void timer_init(uint16_t ms);`
  - `uint8_t timer_check_flag(void);`
  - `void timer_clear_flag(void);`

### Basic Setup
1. Flash to STM32F3 Discovery board
2. System initializes with all LEDs OFF
3. Press USER button (PA0)
4. LEDs change state only if 1000 ms (1 second) elapsed since last update

### ✅ Expected Output
- **Initial state**: All LEDs OFF
- **Button press**: LEDs update in chase sequence (PE8 → PE9 → PE10...) but **only once per second**
- **No immediate consecutive LED changes**: Presses within the 1-second interval **do not affect LED states**

Exercise 2 (A) – UART Transmit on Button Press
Purpose
Transmits a predefined string over UART1 when a button is pressed

Demonstrates interrupt-triggered data transmission

Features
Initializes UART1 for 9600 baud transmission

Uses PA0 (USER button) as the trigger source

Transmits fixed message (e.g., "Hello UART\r\n") to PC terminal

Provides clean API:

void uart1_init(void);

void uart1_send(char* message);

Basic Setup
Flash program and connect STM32F3 board to PC via USB

Open PuTTY / Tera Term on correct COM port (9600 baud)

Press button (PA0) to trigger UART send

✅ Expected Output
Button press → "Hello UART" appears in serial monitor

Repeated presses → Message retransmits with no lag or corruption

Exercise 2 (B) – UART Receive into Buffer
Purpose
Stores incoming serial characters into a local buffer until termination character (CR)

Enables asynchronous string input from PC

Features
Receives data over UART1 using polling

Stores characters into ring buffer

Detects termination on 0x0D (carriage return)

Provides clean API:

void uart1_receive(char* buffer);

Basic Setup
Connect STM32F3 to PC with serial terminal open

Type "example string<CR>"

Inspect memory buffer in debugger

✅ Expected Output
Input: "test<CR>"

Buffer result: ['t','e','s','t','\0']

Stops receiving after 0x0D

Exercise 2 (C) – Clock Update for Baud Rate
Purpose
Verifies UART stability after increasing system clock

Ensures serial communication still functions at higher speeds

Features
Switches from default internal clock to PLL-based system clock (e.g., 24MHz)

Recalculates UART BRR (baud rate register) accordingly

Provides system clock setup:

void clock_init(void);

void uart1_reconfig_baud(uint32_t baud);

Basic Setup
Flash to STM32F3 board

Update clock before UART communication starts

Connect serial terminal

Send/receive message

✅ Expected Output
Terminal sees correct message despite increased clock

No data corruption, confirming baud matches new timing

Exercise 2 (D) – UART Port Forwarding
Purpose
Transfers messages from PC → MCU1 → MCU2 over chained UARTs

Demonstrates full-duplex embedded system communication

Features
MCU1 receives data on UART1 and forwards via UART2

MCU2 receives on UART2 and displays result (e.g., via LEDs or debug)

Provides API on both boards:

MCU1: uart1_receive(); uart2_send();

MCU2: uart2_receive();

Basic Setup
Chain two STM32 boards with jumper wires (TX↔RX)

Open terminal and type message

Observe MCU2 output

✅ Expected Output
Message typed on PC → MCU2 reacts (LED blink / buffer log)

End-to-end integrity preserved across both UART hops

Exercise 3 (A) – Regular Interval Callback
Purpose
Implements a recurring software timer using TIM2

Invokes callback at defined ms intervals

Features
Uses TIM2 in interrupt mode

Accepts function pointer as callback handler

Provides API:

void timer_init(uint16_t interval_ms, void (*callback)(void));

Basic Setup
Flash to board with LED toggle callback

Observe PE8 blinking at 500ms intervals

✅ Expected Output
LED toggles consistently every 0.5s

No blocking delays; system remains responsive

Exercise 3 (B) – Getter/Setter Encapsulation
Purpose
Protects timer interval variable from direct access

Provides controlled access via interface functions

Features
Internal timer state is static

Provides getters and setters only:

void timer_set_period(uint16_t ms);

uint16_t timer_get_period(void);

Basic Setup
Use setter in main() to update delay (e.g., from 1000ms → 250ms)

Observe LED blinks faster after update

✅ Expected Output
Initially slow blink, then accelerates after period is changed

Direct variable access fails if attempted externally

Exercise 3 (C) – One-Shot Timer
Purpose
Triggers a one-time event after delay, then stops

Similar to setTimeout() in JavaScript

Features
Reuses TIM2 with different logic

Disarms timer after callback fires once

API:

void timer_oneshot(uint16_t delay_ms, void (*callback)(void));

Basic Setup
Call timer_oneshot(1000, led_toggle);

Observe LED toggle once after 1s

No further toggles occur

✅ Expected Output
Single LED change occurs after delay

No periodic behavior unless rearmed manually

Exercise 4 – Final Integration
Purpose
Combines all modules: UART input, Caesar/palindrome processing, LED display

Demonstrates real-world, multi-MCU system integration

Features
MCU1:

Receives UART message from PC

Checks palindrome → applies Caesar cipher if true

Forwards message to MCU2

MCU2:

Decodes Caesar cipher

Counts vowels/consonants

Displays count via LEDs, alternating every 500ms

Basic Setup
Connect MCU1 to PC, MCU1 UART2 to MCU2 UART2

Type string: "Racecar" or "Hello"

MCU2 LED shows vowel/consonant count alternating

✅ Expected Output
Palindrome: Encoded → Decoded → LED count

Non-palindrome: Forwarded → LED count

500ms intervals: LED state alternates correctly
