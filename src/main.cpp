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

  Servo servo;
  I2C i2c;
  i2c.init();
  LCD lcd(i2c);
  lcd.init();
  lcd.lcd_print("Hello World!");
  servo.init();

  timer.init(7199, 9);
  timer.enable_interrupt();
  timer.start();
  uint32_t previous_time = 0;
  uint8_t step = 0;
  servo.set_angle(0);

  while (1) {
    uint32_t now = timer.get_millis();
    if (now - previous_time >= 1000) {
      step++;

      switch (step) {
      case 0: {
        servo.set_angle(0);
        break;
      }
      case 1: {
        servo.set_angle(90);
        break;
      }
      case 2: {
        servo.set_angle(180);
        step = 0;
        break;
      }
      }
      previous_time = now;
    }
  }
}
