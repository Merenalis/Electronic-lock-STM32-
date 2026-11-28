#pragma once
#include "gpio.hpp"
#include "stm32f1xx.h"
#include "timer.hpp"

enum class DoorState { OPEN, CLOSED };

class DoorSensor {
public:
  DoorSensor(Timer &timer, GPIO &gpio, GPIO::GPIO_Port port, uint8_t pin)
      : timer_(&timer), gpio_(&gpio), port_(port), pin_(pin) {}

  void init();
  void update();
  DoorState get_state() const;
  bool has_changed();

private:
  Timer *timer_;
  GPIO *gpio_;
  GPIO::GPIO_Port port_;
  uint8_t pin_;
  DoorState raw_to_state(GPIO::GPIO_State raw_state);

  DoorState state;
  DoorState last_raw_state;
  uint32_t state_time;
  uint32_t last_change_time;
  static constexpr uint32_t DEBOUNCE_TIME_MS = 30;
  bool changed = false;
};