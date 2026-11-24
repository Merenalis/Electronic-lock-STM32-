#pragma once
#include "i2c.hpp"
#include "stm32f1xx.h"
#include "timer.hpp"

constexpr uint8_t LCD_ADDR = 0x27;

constexpr uint8_t LCD_RS = 0x01;
constexpr uint8_t LCD_EN = 0x04;
constexpr uint8_t LCD_BL = 0x08;

class LCD {
public:
  LCD(I2C &i2c, Timer &timer) : i2c(&i2c), timer(&timer) {}

  void init();
  void lcd_print(const char *str);
  void set_cursor(uint8_t row, uint8_t col);
  void lcd_create_char(uint8_t index, uint8_t *character);
  void lcd_command(uint8_t command);
  void clear();

private:
  I2C *i2c;
  Timer *timer;
  void lcd_write_nibble(uint8_t data, uint8_t rs);
  void lcd_write_byte(uint8_t data, uint8_t rs);
  void lcd_data(uint8_t data);
  void wait_ms(uint32_t ms);
};
