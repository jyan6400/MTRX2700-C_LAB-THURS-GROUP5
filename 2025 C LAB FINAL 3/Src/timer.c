#include "stm32f303xc.h"
#include "timer.h"

static void (*timer_callback)(void) = 0;

/* ─── 内部：装载 TIM2，并根据 oneshot 选择是否启用 OPM ───────── */
static void tim2_start(uint32_t period_ms, int oneshot)
{
    if (period_ms == 0)      period_ms = 1;
    if (period_ms > 65535)   period_ms = 65535;

    /* 计数频率 1 kHz：1 tick = 1 ms（假设系统时钟 8 MHz） */
    TIM2->PSC = (8000000u / 1000u) - 1u;   /* 8000‑1 */
    TIM2->ARR = (uint16_t)(period_ms - 1u);

    TIM2->CNT = 0;
    TIM2->EGR = TIM_EGR_UG;                /* 立即装载 */
    TIM2->SR  = 0;                         /* 清 UIF */

    if (oneshot)
        TIM2->CR1 = TIM_CR1_OPM | TIM_CR1_CEN;  /* One‑Pulse Mode */
    else
        TIM2->CR1 = TIM_CR1_CEN;                /* 周期性 */
}

/* ─── 周期性定时器（原实现） ─────────────────────────────── */
void timer_init(uint32_t period_ms, void (*callback)(void))
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    TIM2->CR1 = 0;
    TIM2->DIER = TIM_DIER_UIE;

    timer_callback = callback;
    tim2_start(period_ms, 0);              /* 0 = 周期性 */

    NVIC_SetPriority(TIM2_IRQn, 1);
    NVIC_EnableIRQ(TIM2_IRQn);
}

/* ─── 一次性定时器（新增） ───────────────────────────────── */
void timer_oneshot(uint32_t delay_ms, void (*callback)(void))
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    TIM2->CR1 = 0;
    TIM2->DIER = TIM_DIER_UIE;

    timer_callback = callback;
    tim2_start(delay_ms, 1);               /* 1 = oneshot */

    NVIC_SetPriority(TIM2_IRQn, 1);
    NVIC_EnableIRQ(TIM2_IRQn);
}

/* ─── TIM2 中断 ─────────────────────────────────────────── */
void TIM2_IRQHandler(void)
{
    if (TIM2->SR & TIM_SR_UIF) {
        TIM2->SR &= ~TIM_SR_UIF;           /* 清标志 */
        if (timer_callback) timer_callback();
    }
}
