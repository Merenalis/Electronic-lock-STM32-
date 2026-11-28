#include "door_sensor.hpp"
#include "gpio.hpp"

void DoorSensor::init() {
  if (gpio_ == nullptr || timer_ == nullptr) {
    return;
  }
  gpio_->init(port_, pin_, GPIO::GPIO_Mode::Input, GPIO::GPIO_Config::Pull_Up,
              GPIO::GPIO_Speed::MHz_2);
  GPIO::GPIO_State raw_state = gpio_->read(port_, pin_);
  state = raw_to_state(raw_state);
  last_raw_state = raw_to_state(raw_state);
  uint32_t current_time = timer_->get_millis();
  state_time = current_time;
  last_change_time = current_time;
}

void DoorSensor::update() {
  GPIO::GPIO_State raw_state = gpio_->read(port_, pin_);
  DoorState current_door_state = raw_to_state(raw_state);
  uint32_t now = timer_->get_millis();

  if (current_door_state != last_raw_state) {
    last_raw_state = current_door_state;
    last_change_time = now;
    return;
  }

  if (now - last_change_time < DEBOUNCE_TIME_MS) {
    return;
  }

  if (current_door_state == state) {
    return;
  }

  state = current_door_state;
  state_time = now;
  changed = true;
}

DoorState DoorSensor::raw_to_state(GPIO::GPIO_State raw_state) {
  if (raw_state == GPIO::GPIO_State::High) {
    return DoorState::OPEN;
  } else {
    return DoorState::CLOSED;
  }
}

DoorState DoorSensor::get_state() const {
  return state;
  ;
};

bool DoorSensor::has_changed() {
  bool current = changed;
  changed = false;
  return current;
}