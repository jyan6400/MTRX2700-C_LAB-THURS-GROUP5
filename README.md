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



Module | Test Description | Method | Expected Outcome | Status
Exercise 1 (A,B,C) – LED Control & Button Interface | Verify LED output mode and state updating | Set various bitmasks via set_led_state(), observe physical LEDs | LEDs reflect correct bit pattern as expected | ✅
 | Confirm button interrupt and callback trigger | Register button_callback, press button on PE0 | Callback function executes (e.g., LED shifts) | ✅
 | Validate state encapsulation with get_led_state() | Set LED pattern, retrieve via getter | Retrieved value matches last set pattern | 
Exercise 1 (D) – Timed LED Updates | Ensure LED changes occur on hardware timer rather than blocking delay | Use timer with registered LED pattern update callback | LED toggles at defined intervals with no UI delay | ✅
 | Confirm immediate return from set function | Set LED state while timer running | No polling delay; system remains responsive | ✅
Exercise 2 (A) – UART Transmit on Button Press | Send string from STM32 to PC over UART | Connect serial terminal (PuTTY/Tera Term), press button | String appears in terminal upon button press | ✅
Exercise 2 (B) – UART Receive into Buffer | Store typed string into buffer until CR (0x0D) | Send string via terminal, inspect buffer via debugger | Characters stored correctly until terminating character | ✅
Exercise 2 (C) – Clock Update for Baud Rate | Test UART after system clock/PLL reconfiguration | Adjust PLL, verify transmission using logic analyzer or terminal | Baud rate still valid; messages received correctly | ✅
Exercise 2 (D) – UART Port Forwarding | Verify message pass-through from PC → MCU1 → MCU2 | Chain UART1 and UART2 on 2 boards, observe forwarded message | Full message appears on MCU2 from PC | ✅
Exercise 3 (A) – Regular Interval Callback | Trigger callback function repeatedly on fixed interval | Register callback with interval (e.g., 500ms), blink LED | Callback executes periodically based on set time | ✅
Exercise 3 (B) – Getter/Setter Encapsulation | Get and set timer period externally while keeping internals private | Use set_period() and get_period(), attempt direct access | Period adjustable only via interface; timer module is encapsulated | ✅
Exercise 3 (C) – One-Shot Timer | Trigger a single delayed callback after defined ms | Set one-shot delay with callback, observe LED or debugger | Callback occurs once and doesn’t repeat | ✅
Exercise 4: Final Integration | End-to-end test: Palindrome → Caesar → UART → LED display | Type message in terminal → check output on MCU2 LEDs | Palindrome is encrypted and decoded; vowel/consonant counts shown at 500ms | ✅
 | Test non-palindrome message handling | Send non-palindrome from PC | Message forwarded as-is to MCU2 | ✅
 | Edge case: input symbols and whitespace | Send string with special characters | Non-alphabetic characters are skipped or ignored safely | ✅


| Module        | Test Description                                                                 | Method                                                                                 | Expected Outcome                                      | Status  |
|---------------|----------------------------------------------------------------------------------|----------------------------------------------------------------------------------------|------------------------------------------------------|---------|
| String Functions (`1.3.2/`) | Test conversion to uppercase/lowercase, palindrome check, and Caesar cipher  | Manually inspect output in memory/registers via debugger                              | Correct string manipulation and transformations      | ✅      |
| LED Control (`1.4.2/`)     | Test bitmask patterns, toggle LED logic with button input, and display counts | Observe LED behavior for various input patterns                                       | LEDs match bitmask patterns and respond to input     |       |
| UART Send (`1.5.2a.s`)     | Transmit string on button press                                              | Connect board to PC, use serial monitor (e.g., Tera Term / PuTTY)                    | String is sent when button is pressed                |       |
| UART Receive (`1.5.2b.s`)  | Receive string until termination character                                   | Type into serial monitor and observe memory buffer updates                           | Full string stored until terminator is entered       |       |
| Clock Update (`1.5.2c.s`)  | Test updated baud rate after clock frequency change                          | Measure UART timing with oscilloscope or observe continued correct transmission       | UART works seamlessly after speed change             |       |
| UART Read/Resend (`1.5.2d.s`) | Echo received UART input                                                      | Send a message and verify echo on serial terminal                                    | Same message is echoed back                          |       |
| Port Forwarding (`1.5.2e.s`) | Forward message from PC to MCU2 via MCU1                                      | Chain 2 boards and verify message is passed through                                  | Message from PC is received on MCU2                  |       |
| Timer Delay (`1.6.2a.s`)   | Delay using hardware timer and multiplier constant                            | Scope or LED blink timing measurement                                                 | Delay corresponds to programmed microsecond count    |       |
| Prescaler Config (`1.6.2b.s`) | Validate 0.1ms delay using prescaler                                         | Count LED toggles in 1s interval                                                      | ~10 toggles per second observed                      |       |
| Timer Preload (`1.6.2c.s`) | Validate preload-based accurate timer                                         | Use scope or loop LED toggling with preload delay                                    | Consistent delay per iteration                       |       |
| Final Integration (`1.7.2a.s`) | Palindrome check → Caesar Cipher → UART → LED vowel/consonant display       | Type message on PC, verify LED pattern changes every 500ms on MCU2                   | System fully integrated and functions as expected    |       |


## Exercise 1
### Parts a, b, c
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


### Part d
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

