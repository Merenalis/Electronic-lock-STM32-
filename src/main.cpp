#include "app/app.hpp"
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

  Servo servo;
  servo.init();

  I2C i2c;
  i2c.init();

  LCD lcd(i2c, timer);
  lcd.init();
  App app(timer, servo, lcd);
  app.init();
  app.handle_key('1');
  app.handle_key('2');
  app.handle_key('3');
  app.handle_key('4');
  app.handle_key('5');

  while (1) {
    app.update();
  }
}
