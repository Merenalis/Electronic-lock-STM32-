#include "lcd.hpp"

void LCD::init() {
  lcd_write_nibble(0x03, 0);
  wait_ms(5);
  lcd_write_nibble(0x03, 0);
  wait_ms(5);
  lcd_write_nibble(0x03, 0);
  wait_ms(5);

  lcd_write_nibble(0x02, 0); // 4-bit mode
  wait_ms(1);
  lcd_write_byte(0x28, 0);
  lcd_write_byte(0x0C, 0);
  lcd_write_byte(0x06, 0);
  lcd_write_byte(0x01, 0);
  wait_ms(15);
}

void LCD::lcd_print(const char *str) {
  while (*str) {
    lcd_write_byte(*str, 1);
    str++;
  }
}

void LCD::lcd_create_char(uint8_t index, uint8_t *character) {
  if (index < 8) {
    lcd_write_byte(0x40 + (index * 8), 0);

    for (uint8_t i = 0; i < 8; i++) {
      lcd_write_byte(character[i], 1);
    }
  }
}

void LCD::lcd_write_nibble(uint8_t data, uint8_t rs) {
  uint8_t value = (data << 4) | (rs << 0) | (1U << 3);

  i2c->write(LCD_ADDR, value);
  wait_ms(5);
  i2c->write(LCD_ADDR, value | (1U << 2));
  wait_ms(5);
  i2c->write(LCD_ADDR, value);
}

void LCD::lcd_write_byte(uint8_t data, uint8_t rs) {
  lcd_write_nibble((data >> 4), rs);
  lcd_write_nibble((data & 0x0F), rs);
}

void LCD::set_cursor(uint8_t row, uint8_t col) {
  uint8_t address = (row == 0) ? 0x00 : 0x40;
  address += col;

  lcd_write_byte(0x80 | address, 0);
}

void LCD::lcd_command(uint8_t command) { lcd_write_byte(command, 0); }

void LCD::clear() { lcd_command(0x01); }

void LCD::wait_ms(uint32_t ms) {
  uint32_t start = timer->get_millis();
  while ((timer->get_millis() - start) < ms) {
  }
}
