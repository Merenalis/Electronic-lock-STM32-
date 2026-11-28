#include "../drivers/lcd.hpp"
#include "../drivers/servo.hpp"
#include "../drivers/timer.hpp"

enum class LockState { WAITING_FOR_PIN, ACCESS_GRANTED, ACCESS_DENIED };

class App {
public:
  App(Timer &timer, Servo &servo, LCD &lcd)
      : timer(&timer), servo(&servo), lcd(&lcd) {}

  void init();
  void handle_state(LockState new_state);
  void update();
  void open_lock();
  void close_lock();
  bool checkPassword(const char *input);
  void handle_key(char key);

private:
  Timer *timer;
  Servo *servo;
  LCD *lcd;
  LockState state;
  const char *password_ = "1234";
  uint32_t unlock_time = 0;
  uint32_t state_time = 0;
  uint32_t unlock_duration = 5000;
  uint32_t denied_duration = 5000;
  char pin_buffer[5];
  uint8_t pin_length = 0;
  const uint8_t pin_length_max = 4;
};