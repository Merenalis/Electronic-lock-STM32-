#include "servo.hpp"

void Servo::init() {
  RCC->APB2ENR |= (1U << 2);  // GPIOA
  RCC->APB2ENR |= (1U << 11); // TIM1
  GPIOA->CRH &= ~(15U << 0);  // PA8
  GPIOA->CRH |= (11U << 0);
  TIM1->CR1 |= (1U << 7); // ARPE

  TIM1->PSC = 71;
  TIM1->ARR = 19999;
  TIM1->CCR1 = 0;

  TIM1->CCMR1 &= ~(7U << 4);
  TIM1->CCMR1 &= ~(3U << 0);

  TIM1->CCMR1 |= (6U << 4); // PWM mode
  TIM1->CCMR1 |= (1U << 3); // Preload Enable
  TIM1->CCER |= (1U << 0);  // Capture/Compare 1
  TIM1->BDTR |= (1U << 15);
  TIM1->EGR |= (1U << 0);
  TIM1->CR1 |= (1U << 0);
}

void Servo::set_angle(uint8_t angle) {
  if (angle > 180) {
    angle = 180;
  }

  TIM1->CCR1 = 500 + (angle * 1900U) / 180U;
}
