#pragma once
#include "stm32f1xx.h"

class Servo {
public:
  void init();
  void set_angle(uint8_t angle);
};