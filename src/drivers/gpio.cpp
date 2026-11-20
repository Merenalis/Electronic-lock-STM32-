#include "gpio.hpp"

void GPIO::init(GPIO_Port port, uint8_t pin, GPIO_Mode mode, GPIO_Config config,
                GPIO_Speed speed) {
  if (pin > 15)
    return;

  enable_clock(port);

  GPIO_TypeDef *gpio = get_port(port);
  uint8_t shift = (pin % 8) * 4;

  uint8_t mode_bits = 0U;
  uint8_t cnf_bits = 0U;

  switch (mode) {
  case GPIO_Mode::Analog:
    mode_bits = 0b00U;
    cnf_bits = 0b00U;
    break;
  case GPIO_Mode::Input:
    mode_bits = 0b00U;
    if (config == GPIO_Config::Pull_Up || config == GPIO_Config::Pull_Down) {
      cnf_bits = 0b10U;
    } else {
      cnf_bits = 0b01U;
    }
    break;
  case GPIO_Mode::Output:
    mode_bits = speed_to_mode_bits(speed);
    if (config == GPIO_Config::OpenDrain ||
        config == GPIO_Config::Alternate_OpenDrain) {
      cnf_bits = 0b01U;
    } else {
      cnf_bits = 0b00U; // PushPull
    }
    break;
  case GPIO_Mode::Alternate:
    mode_bits = speed_to_mode_bits(speed);
    if (config == GPIO_Config::Alternate_OpenDrain) {
      cnf_bits = 0b11U; // AF Open-Drain
    } else {
      cnf_bits = 0b10U; // AF Push-Pull
    }
    break;
  }

  uint32_t pin_cfg = (static_cast<uint32_t>(cnf_bits & 0x03U) << 2U) |
                     static_cast<uint32_t>(mode_bits & 0x03U);
  volatile uint32_t *cr = (pin < 8U) ? &gpio->CRL : &gpio->CRH;

  *cr &= ~(0xFU << shift);
  *cr |= (pin_cfg << shift);
}

void GPIO::enable_clock(GPIO_Port port) {
  switch (port) {
  case GPIO_Port::GPIO_A:
    RCC->APB2ENR |= (1U << 2);
    break;
  case GPIO_Port::GPIO_B:
    RCC->APB2ENR |= (1U << 3);
    break;
  case GPIO_Port::GPIO_C:
    RCC->APB2ENR |= (1U << 4);
    break;
  }
}

GPIO_TypeDef *GPIO::get_port(GPIO_Port port) {
  switch (port) {
  case GPIO_Port::GPIO_A:
    return GPIOA;

  case GPIO_Port::GPIO_B:
    return GPIOB;

  case GPIO_Port::GPIO_C:
    return GPIOC;

  default:
    return nullptr;
  }
}

uint8_t GPIO::speed_to_mode_bits(GPIO_Speed speed) {
  switch (speed) {
  case GPIO_Speed::MHz_10:
    return 0b01U;
  case GPIO_Speed::MHz_2:
    return 0b10U;
  case GPIO_Speed::MHz_50:
    return 0b11U;
  default:
    return 0b10U;
  }
}

void GPIO::write(GPIO_Port port, uint8_t pin, GPIO_State state) {
  if (pin > 15) {
    return;
  }
  GPIO_TypeDef *gpio = get_port(port);
  if (gpio == nullptr) {
    return;
  }

  if (state == GPIO_State::High) {
    gpio->BSRR = (1U << pin);
  } else {
    gpio->BRR = (1U << pin);
  }
}

void GPIO::set(GPIO_Port port, uint8_t pin) {
  write(port, pin, GPIO_State::High);
}

void GPIO::reset(GPIO_Port port, uint8_t pin) {
  write(port, pin, GPIO_State::Low);
}

GPIO::GPIO_State GPIO::read(GPIO_Port port, uint8_t pin) {
  if (pin > 15) {
    return GPIO_State::Low;
  }
  GPIO_TypeDef *gpio = get_port(port);
  if (gpio == nullptr) {
    return GPIO_State::Low;
  }

  return ((gpio->IDR & (1U << pin)) != 0U) ? GPIO_State::High : GPIO_State::Low;
}

void GPIO::toggle(GPIO_Port port, uint8_t pin) {
  if (pin > 15) {
    return;
  }
  GPIO_TypeDef *gpio = get_port(port);
  if (gpio == nullptr) {
    return;
  }

  gpio->ODR ^= (1U << pin);
}