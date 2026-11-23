#include "i2c.hpp"

void I2C::init() {
  RCC->APB2ENR |= (1U << 3);  // GPIOB clock
  RCC->APB1ENR |= (1U << 21); // I2C1 clock

  // PB6 and PB7: AF Open-Drain, 50 MHz
  GPIOB->CRL &= ~((15U << 24) | (15U << 28));
  GPIOB->CRL |= ((15U << 24) | (15U << 28));

  // Reset I2C peripheral
  I2C1->CR1 = (1U << 15);
  I2C1->CR1 = 0;

  // PCLK1 = 8 MHz, I2C = 100 kHz
  I2C1->CR2 = 36;
  I2C1->CCR = 180;
  I2C1->TRISE = 37;

  I2C1->CR1 |= (1U << 0); // PE
}

bool I2C::write(uint8_t address, uint8_t data) {
  uint32_t timeout = 1000000;
  while (I2C1->SR2 & (1U << 1)) {
    if (timeout-- == 0) {
      return false;
    }
  }
  I2C1->CR1 |= (1U << 8); // start
  timeout = 1000000;

  while (!(I2C1->SR1 & (1 << 0))) {
    if (timeout-- == 0) {
      I2C1->CR1 |= (1U << 9); // STOP
      return false;
    }
  }
  I2C1->DR = (address << 1); // slave address

  timeout = 1000000;
  while (!(I2C1->SR1 & (1 << 1)) && !(I2C1->SR1 & (1 << 10))) {
    if (timeout-- == 0) {
      I2C1->CR1 |= (1U << 9); // STOP
      return false;
    }
  }

  if (I2C1->SR1 & (1U << 10)) {
    I2C1->SR1 &= ~(1U << 10); // Clear AF
    I2C1->CR1 |= (1U << 9);   // STOP
    return false;
  }

  // clear ADDR flag
  uint32_t temp = I2C1->SR1;
  temp = I2C1->SR2;

  timeout = 1000000;
  // wait for TXE
  while (!(I2C1->SR1 & (1U << 7))) {
    if (timeout-- == 0) {
      I2C1->CR1 |= (1U << 9); // STOP
      return false;
    }
  }

  I2C1->DR = data; // data transmit

  timeout = 1000000;
  // wait for BTF
  while (!(I2C1->SR1 & (1U << 2)) && !(I2C1->SR1 & (1 << 10))) {
    if (timeout-- == 0) {
      I2C1->CR1 |= (1U << 9); // STOP
      return false;
    }
  }

  if (I2C1->SR1 & (1U << 10)) {
    I2C1->SR1 &= ~(1U << 10); // Cclear AF
    I2C1->CR1 |= (1U << 9);   // STOP
    return false;
  }
  I2C1->CR1 |= (1U << 9); // STOP

  return true;
}

void I2C::stop() { I2C1->CR1 |= (1U << 9); }
void I2C::recover() {
  I2C1->CR1 &= ~(1U << 0);
  bool isPB6High = GPIOB->IDR & (1U << 6);
  bool isPB7High = GPIOB->IDR & (1U << 7);

  if (isPB6High && isPB7High) {
    init();
  } else {
    // maybe finish in the future
  }
}