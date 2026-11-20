#include "timer.hpp"

volatile uint32_t milliseconds = 0;

void Timer::init(uint16_t prescaler, uint16_t period) {
  RCC->APB1ENR |= (1U << 0); // Tim2 on
  TIM2->CR1 = 0;             // total reset
  TIM2->CR1 |= (1U << 7);    // ARR buffer

  TIM2->PSC = prescaler; // prescaler
  TIM2->ARR = period;
  TIM2->EGR |= (1U << 0); // update event
  TIM2->SR &= ~(1U << 0); // reset UIF flag
}

void Timer::start() { TIM2->CR1 |= (1U << 0); }

void Timer::stop() { TIM2->CR1 &= ~(1U << 0); }
void Timer::reset() { TIM2->CNT = 0; }

void Timer::enable_interrupt() {
  TIM2->DIER |= (1U << 0);
  NVIC_EnableIRQ(TIM2_IRQn);
}

void Timer::disable_interrupt() { TIM2->DIER &= ~(1U << 0); }

extern "C" void TIM2_IRQHandler() {
  if (TIM2->SR & (1U << 0)) {
    TIM2->SR &= ~(1 << 0);
    milliseconds++;
  }
}

uint32_t Timer::get_millis() { return milliseconds; }