#include "system.hpp"

void System::system_init() {
  RCC->CR |= (1U << 16); // HSE on

  while (!(RCC->CR & (1U << 17))) { // wait for HSERDY
  }
  FLASH->ACR &= ~(7U << 0);
  FLASH->ACR |= (2U << 0); // two waits latency
  FLASH->ACR |= (1U << 4); // prefetch on

  RCC->CFGR &= ~(15U << 4); // ahb 72mhz
  RCC->CFGR &= ~(7U << 8);
  RCC->CFGR |= (4U << 8); // APB1 36mhz set

  RCC->CFGR &= ~(7U << 11); // APB2 72mhz

  RCC->CFGR |= (6U << 16); // HSE select

  RCC->CFGR &= ~(15U << 18);
  RCC->CFGR |= (7U << 18); // PLL multiplicator x9

  RCC->CR |= (1U << 24); // PLL on

  while (!(RCC->CR & (1U << 25))) { // wait for PLLRDY
  }

  RCC->CFGR &= ~(3U << 0);
  RCC->CFGR |= (2U << 0);

  while ((RCC->CFGR & (3U << 2)) != (2U << 2)) {
  }
}