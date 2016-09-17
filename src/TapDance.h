#ifndef _KBDHACKS_TAPDANCE_H
#define _KBDHACKS_TAPDANCE_H 1

#include <stdint.h>

class TapDanceKey {
 private:
  void _reset (void);

 protected:
  uint16_t timer;
  uint8_t keycode;
  uint8_t count;
  bool pressed;
  bool interrupted;
  bool finished;
  bool timedout;

  virtual void onFinish (void) {};
  virtual void onReset (void) {};
  virtual void onEachTap (void) {};

 public:
  TapDanceKey (uint8_t code);
  void press (uint8_t code);
  void release (uint8_t code);
  void cycle (void);
};

class TapDanceDoubleKey {
 private:
  uint8_t kc1, kc2;

 public:
  TapDanceDoubleKey (uint8_t kc1, uint8_t kc2);
};

#endif
