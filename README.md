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

### Project Flow Diagram





 ## Testing Plan

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

