//Relevant Header Files
#include <string.h>			//String function header file
#include <stdlib.h>			//Standard library function header file
#include "stm32f303xc.h"	//Device header file for STM32F303 series MCUS
#include "serial.h"			//Serial part header file
#include "dio.h"			//digital io part header file
#include "timer.h"			//timer part header file
#include "command.h"		//command part header file

#define RX_BUFFER_SIZE 1000
#define PUTS(str)  SerialOutputString((uint8_t *)(str), &USART1_PORT)
#define OK(msg)    do { PUTS("\r\n[OK] "  msg "\r\n"); } while (0)
#define ERR(msg)   do { PUTS("\r\n[ERR] " msg "\r\n"); } while (0)

/* ——— 私有 LED 工具 ——— */
static inline void leds_off(void)
{
    for (int i = 0; i < 8; ++i) dio_setLED(i, false);
}

/* oneshot 回调：亮 50 ms 再灭 */
static void flash_all_once(void)
{
    for (int i = 0; i < 8; ++i) dio_setLED(i, true);

    for (volatile uint32_t d = 0; d < 400000; ++d) __NOP();  /* ~50 ms */

    leds_off();
}
/* timer 回调：取反实现闪烁 */
static void blink_toggle_all(void)
{
    for (int i = 0; i < 8; ++i) dio_toggleLED(i);
}

/* ——— 串口帧完成回调 ——— */
void on_receive_done(uint8_t *data, uint32_t len)
{
    char buf[RX_BUFFER_SIZE];
    if (len >= sizeof(buf)) len = sizeof(buf) - 1;
    memcpy(buf, data, len);
    buf[len] = '\0';

    char *cmd = strtok(buf, " ");
    char *arg = strtok(NULL, "");

    if (!cmd) { ERR("Empty command"); return; }

    /* ===== led <8‑bit> ===== */
    if (strcmp(cmd, "led") == 0) {
        timer_stop();
        if (!arg || strlen(arg) != 8) { ERR("led expects 8‑bit binary"); return; }
        for (int i = 0; i < 8; ++i) {
            if (arg[i] != '0' && arg[i] != '1') { ERR("led pattern must be 0/1"); return; }
            dio_setLED(7 - i, arg[i] == '1');
        }
        OK("LED pattern set");
    }

    /* ===== serial <text…> ===== */
    else if (strcmp(cmd, "serial") == 0) {
        PUTS("\r\n"); PUTS(arg ? arg : ""); PUTS("\r\n");
    }

    /* ===== oneshot <delay_ms> ===== */
    else if (strcmp(cmd, "oneshot") == 0) {
        if (!arg) { ERR("oneshot expects delay"); return; }
        uint32_t delay = (uint32_t)atoi(arg);

        timer_stop();
        leds_off();
        timer_oneshot(delay, flash_all_once);
        OK("oneshot armed");
    }

    /* ===== timer <period_ms> ===== */
    else if (strcmp(cmd, "timer") == 0) {
        if (!arg) { ERR("timer expects period"); return; }
        uint32_t period = (uint32_t)atoi(arg);

        timer_stop();
        leds_off();
        timer_init(period, blink_toggle_all);
        OK("timer started");
    }

    /* ===== 未知指令 ===== */
    else {
        ERR("Unknown command");
    }
}
