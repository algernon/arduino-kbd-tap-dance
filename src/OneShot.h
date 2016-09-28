#ifndef _KBDHACKS_ONESHOT_H
#define _KBDHACKS_ONESHOT_H 1

#include <stdint.h>

class OneShotKey {
 private:
  void _reset (void);

 protected:
  uint16_t timer;
  uint8_t keycode;
  bool active;
  bool sticky;

  virtual void register_code (uint8_t code) = 0;
  virtual void unregister_code (uint8_t code) = 0;

  virtual void onActivate (void) {};
  virtual void onDeactivate (void) {};

 public:
  OneShotKey (uint8_t code);
  void press (uint8_t code);
  void release (uint8_t code);
  void cycle (void);
};

#endif
