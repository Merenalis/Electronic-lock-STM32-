#pragma once
#include "stm32f1xx.h"

class GPIO {
public:
  enum class GPIO_Port {
    GPIO_A,
    GPIO_B,
    GPIO_C,
  };

  enum class GPIO_Mode {
    Input,
    Output,
    Alternate,
    Analog,
  };

  enum class GPIO_Config {
    PushPull,
    OpenDrain,
    Alternate_PushPull,
    Alternate_OpenDrain,
    Floating,
    Pull_Up,
    Pull_Down,

  };

  enum class GPIO_Speed {
    MHz_2,
    MHz_10,
    MHz_50,
  };

  enum class GPIO_State : uint8_t {
    Low = 0,
    High = 1,
  };

  void init(GPIO_Port port, uint8_t pin, GPIO_Mode mode, GPIO_Config config,
            GPIO_Speed speed);

  void write(GPIO_Port port, uint8_t pin, bool state);
  void set(GPIO_Port port, uint8_t pin);
  void reset(GPIO_Port port, uint8_t pin);
  void toggle(GPIO_Port port, uint8_t pin);

private:
  void enable_clock(GPIO_Port port);
  GPIO_TypeDef *get_port(GPIO_Port port);
  uint8_t speed_to_mode_bits(GPIO_Speed speed);
  void write(GPIO_Port port, uint8_t pin, GPIO_State state);
  GPIO_State read(GPIO_Port port, uint8_t pin);
};