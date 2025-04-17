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


## Project Module Descriptions

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



