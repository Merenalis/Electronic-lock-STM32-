#include "drivers/gpio.hpp"
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
  GPIO led_pc13;
  led_pc13.init(GPIO::GPIO_Port::GPIO_C, 13, GPIO::GPIO_Mode::Output,
                GPIO::GPIO_Config::PushPull, GPIO::GPIO_Speed::MHz_2);

  timer.init(7199, 9);
  timer.enable_interrupt();
  timer.start();
  uint32_t previous_time = 0;

  while (1) {
    uint32_t now = timer.get_millis();
    if (now - previous_time >= 500) {
      led_pc13.toggle(GPIO::GPIO_Port::GPIO_C, 13);
      previous_time = now;
    }
  }
}
