#include "app.hpp"
#include <string.h>

void App::init() {
  state = LockState::WAITING_FOR_PIN;
  state_time = timer->get_millis();
  close_lock();
  lcd->clear();
  lcd->set_cursor(0, 0);
  lcd->lcd_print("Enter pin:");
}

void App::handle_state(LockState new_state) {
  if (state == new_state)
    return;
  state = new_state;
  state_time = timer->get_millis();

  switch (state) {
  case LockState::WAITING_FOR_PIN:
    close_lock();
    break;
  case LockState::ACCESS_GRANTED:
    open_lock();

    break;
  case LockState::ACCESS_DENIED:
    lcd->clear();
    lcd->lcd_print("Access denied");
    break;
  default:
    break;
  }
}

void App::update() {
  switch (state) {
  case LockState::WAITING_FOR_PIN:
    break;

  case LockState::ACCESS_GRANTED:
    if (timer->get_millis() - state_time >= unlock_duration) {
      handle_state(LockState::WAITING_FOR_PIN);
    }
    break;

  case LockState::ACCESS_DENIED:
    if (timer->get_millis() - state_time >= denied_duration) {
      handle_state(LockState::WAITING_FOR_PIN);
    }
    break;
  }
}

void App::open_lock() {
  servo->set_angle(180);
  lcd->clear();
  lcd->lcd_print("ACCESS GRANTED");
}

void App::close_lock() {
  servo->set_angle(0);
  lcd->clear();
  lcd->lcd_print("Enter pin:");
}

bool App::checkPassword(const char *input) {
  if (strcmp(input, password_) == 0) {
    handle_state(LockState::ACCESS_GRANTED);
    return true;
  }

  handle_state(LockState::ACCESS_DENIED);
  return false;
}

void App::handle_key(char key) {
  if (state != LockState::WAITING_FOR_PIN) {
    return;
  }

  if (key == '#' && pin_length == pin_length_max) {
    pin_buffer[pin_length] = '\0';
    checkPassword(pin_buffer);
    pin_buffer[0] = '\0';
    pin_length = 0;
  } else if (key == '*') {
    pin_buffer[0] = '\0';
    pin_length = 0;
    lcd->clear();
    lcd->lcd_print("Enter pin:");
  } else if (pin_length < pin_length_max && (key >= '0' && key <= '9')) {
    pin_buffer[pin_length++] = key;
    pin_buffer[pin_length] = '\0';
    lcd->set_cursor(0, 10 + pin_length);
    lcd->lcd_print(&pin_buffer[pin_length - 1]);
  }
}
