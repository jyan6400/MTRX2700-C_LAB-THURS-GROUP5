# MTRX2700-C_LAB-THURSPM-GROUP5

## Group Information

**Team Number:** THURSPM-G5  
**Unit:** MTRX2700 – Mechatronics 2  
**Semester:** S1 2025  

### Group Members & Roles

| Name           | Role                         | Responsibilities                                                                                         |
|----------------|------------------------------|----------------------------------------------------------------------------------------------------------|
| Saksham        | Systems & Comms Lead         | UART interrupt architecture, string parsing, integration of callback handlers                            |
| Shirui         | I/O & Control Lead           | LED I/O logic, timer abstraction layer, debouncing, button callbacks                                     |
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


