#pragma once
#include "stm32f1xx.h"

class I2C {
private:
  void stop();
  void recover();

public:
  void init();
  bool write(uint8_t address, uint8_t data);
};