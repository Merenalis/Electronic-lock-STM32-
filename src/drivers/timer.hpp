#pragma once
#include "stm32f1xx.h"

class Timer {
public:
  void init(uint16_t prescaler, uint16_t period);
  void start();
  void stop();
  void reset();
  void enable_interrupt();
  void disable_interrupt();
  uint32_t get_millis();
};