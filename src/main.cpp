#include "app/app.hpp"
#include "drivers/door_sensor.hpp"
#include "drivers/gpio.hpp"
#include "drivers/i2c.hpp"
#include "drivers/lcd.hpp"
#include "drivers/servo.hpp"
#include "drivers/timer.hpp"
#include "stm32f1xx.h"
#include "system/system.hpp"

void delay(volatile uint32_t count) {
  while (count--) {
    __asm volatile("nop");
  }
}
Timer timer;

int main() {
  System system;
  system.system_init();

  timer.init(7199, 9);
  timer.enable_interrupt();
  timer.start();

  GPIO gpio;
  DoorSensor doorSensor(timer, gpio, GPIO::GPIO_Port::GPIO_A, 0);

  gpio.init(GPIO::GPIO_Port::GPIO_C, 13, GPIO::GPIO_Mode::Output,
            GPIO::GPIO_Config::PushPull, GPIO::GPIO_Speed::MHz_2);
  gpio.set(GPIO::GPIO_Port::GPIO_C, 13); // світлодіод вимкнений
  doorSensor.init();

  while (true) {
    doorSensor.update();

    if (doorSensor.get_state() == DoorState::CLOSED) {
      gpio.reset(GPIO::GPIO_Port::GPIO_C, 13); // Low -> світиться
    } else {
      gpio.set(GPIO::GPIO_Port::GPIO_C, 13); // High -> погашений
    }
  }
}
