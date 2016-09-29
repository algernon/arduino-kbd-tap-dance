#ifndef _KBDHACKS_ONESHOT_H
#define _KBDHACKS_ONESHOT_H 1

#include <stdint.h>

#define ONESHOT_TIMEOUT_DEFAULT 40

class OneShotKey {
 private:
  void _reset (void);

 protected:
  uint16_t timer;
  uint16_t shot_timeout;
  uint8_t keycode;
  bool active;
  bool sticky;
  bool pressed;
  bool cancel;

  virtual void register_code (uint8_t code) = 0;
  virtual void unregister_code (uint8_t code) = 0;

  virtual void onActivate (void) {};
  virtual void onDeactivate (void) {};

  virtual bool shouldIgnore (uint8_t code);

 public:
  OneShotKey (uint8_t code);
  OneShotKey (uint8_t code, uint16_t timeout);

  void press (uint8_t code);
  void release (uint8_t code);
  void cycle (void);

  uint16_t timeout (uint16_t new_timeout);
};

#endif
