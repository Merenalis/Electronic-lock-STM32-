#include "drivers/gpio.hpp"
#include "stm32f1xx.h"
#include "system/system.hpp"

void delay(volatile uint32_t count) {
  while (count--) {
    __asm volatile("nop");
  }
}

int main() {
  System system;
  GPIO led_pc13;
  system.system_init();
  led_pc13.init(GPIO::GPIO_Port::GPIO_C, 13, GPIO::GPIO_Mode::Output,
                GPIO::GPIO_Config::PushPull, GPIO::GPIO_Speed::MHz_2);

  while (1) {
    //led_pc13.set(GPIO::GPIO_Port::GPIO_C, 13);
    delay(1000000);
    led_pc13.reset(GPIO::GPIO_Port::GPIO_C, 13);
    delay(1000000);
  }
}
