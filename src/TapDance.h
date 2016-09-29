#ifndef _KBDHACKS_TAPDANCE_H
#define _KBDHACKS_TAPDANCE_H 1

#define TAP_DANCE_TIMEOUT_DEFAULT 40

#include <stdint.h>

class TapDanceKey {
 private:
  void _reset (void);

 protected:
  uint16_t timer;
  uint16_t tap_timeout;
  uint8_t keycode;
  uint8_t count;
  bool pressed;
  bool interrupted;
  bool timedout;

  virtual void onFinish (void) {};
  virtual void onReset (void) {};
  virtual void onEachTap (void) {};

 public:
  TapDanceKey (uint8_t code);
  void press (uint8_t code);
  void release (uint8_t code);
  void cycle (void);
  uint16_t timeout (uint16_t new_timeout);
};

class TapDanceDoubleKey : public TapDanceKey {
 private:
  uint8_t kc1, kc2;

 protected:
  virtual void register_code (uint8_t code) = 0;
  virtual void unregister_code (uint8_t code) = 0;

  virtual void onFinish (void);
  virtual void onReset (void);

 public:
  TapDanceDoubleKey (uint8_t code, uint8_t kc1, uint8_t kc2) : TapDanceKey (code) {
    this->kc1 = kc1;
    this->kc2 = kc2;
  };
};

#endif
