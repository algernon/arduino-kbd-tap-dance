#include "OneShot.h"
#include "assert.h"

class OneShotTestKey : public OneShotKey {
protected:
  virtual void register_code (uint8_t kc) { last_reg_kc = kc; };
  virtual void unregister_code (uint8_t kc) { last_unreg_kc = kc; };

public:
  OneShotTestKey (uint8_t keycode) : OneShotKey (keycode) {
    cnt_onActivate = 0;
    cnt_onDeactivate = 0;
    last_reg_kc = 0;
    last_unreg_kc = 0;
  };

  uint8_t cnt_onActivate, cnt_onDeactivate, last_reg_kc, last_unreg_kc;

  bool get_active (void) { return this->active; };
  bool get_sticky (void) { return this->sticky; };

  void onActivate () { cnt_onActivate++; };
  void onDeactivate () { cnt_onDeactivate++; };

  bool shouldIgnore (uint8_t code) {
    if (code == 132 || code == 133)
      return true;
    return false;
  }
};

void
test_oneshot_activate_deactivate_with_timeout (void) {
  OneShotTestKey t = OneShotTestKey(42);

  t.press (1);
  t.release (1);
  assert (t.cnt_onActivate == 0);

  t.press (42);
  // the key should activate immediately
  assert (t.cnt_onActivate == 1);
  assert (t.last_reg_kc == 42);

  // releasing the key shall not immediately unregister
  t.release (42);
  assert (t.last_unreg_kc == 0);

  // timing it out shall deactivate the key
  for (int c = 0; c < 39; c++) {
    t.cycle ();
    assert (t.cnt_onDeactivate == 0);
  }
  t.cycle ();
  assert (t.cnt_onDeactivate == 1);
  assert (t.last_unreg_kc == 42);
}

void
test_oneshot_cancel_by_keypress (void) {
  OneShotTestKey t = OneShotTestKey (42);

  t.press (42);
  t.release (42);
  t.cycle ();

  assert (t.cnt_onActivate == 1);
  assert (t.last_reg_kc == 42);

  t.press (1);

  // we do not deactivate when another key is pressed...
  assert (t.cnt_onDeactivate == 0);
  assert (t.last_reg_kc == 42);

  // we deactivate on the next scan
  t.cycle ();

  assert (t.cnt_onDeactivate == 1);
  assert (t.last_unreg_kc == 42);
  assert (t.get_active () == false);

  // the next key pressed will not touch the one-shot key
  t.press (2);
  t.release (2);
  assert (t.get_active () == false);
  assert (t.cnt_onActivate == 1);
  assert (t.cnt_onDeactivate == 1);
}

void
test_oneshot_sticky (void) {
  OneShotTestKey t = OneShotTestKey (42);

  t.press (42);
  t.release (42);
  t.cycle ();

  assert (t.get_sticky () == false);

  t.press (42);
  t.release (42);
  t.cycle ();

  // double-tapping activates once, but the stickyness is enabled
  assert (t.cnt_onActivate == 1);
  assert (t.get_active () == true);
  assert (t.get_sticky () == true);
  assert (t.last_reg_kc == 42);

  // tapping any number of keys, when sticky, do not deactivate
  for (uint8_t i = 1; i < 10; i++) {
    t.press (i);
    t.release (i);
    t.cycle ();
  }
  assert (t.cnt_onDeactivate == 0);
  assert (t.get_active () == true);
  assert (t.get_sticky () == true);
  assert (t.last_unreg_kc == 0);

  // tapping a third time disables stickyness
  t.press (42);
  t.release (42);
  t.cycle ();

  assert (t.cnt_onDeactivate == 1);
  assert (t.get_active () == false);
  assert (t.get_sticky () == false);
  assert (t.last_unreg_kc == 42);
}

void
test_oneshot_held (void) {
  OneShotTestKey t = OneShotTestKey (42);

  t.press (42);

  assert (t.cnt_onActivate == 1);
  assert (t.get_active () == true);
  assert (t.get_sticky () == false);
  assert (t.last_reg_kc == 42);

  // holding the key prevents timing out
  for (uint8_t c = 0; c < 80; c++) {
    t.cycle ();
    assert (t.get_active () == true);
  }

  // releasing the key immediately times it out
  t.release (42);
  assert (t.cnt_onDeactivate == 1);
  assert (t.get_active () == false);
  assert (t.last_unreg_kc == 42);
}

void
test_oneshot_hold_and_tap (void) {
  OneShotTestKey t = OneShotTestKey (42);

  t.press (42);
  t.cycle ();

  // holding prevents cancellation
  for (uint8_t c = 0; c < 32; c++) {
    t.press (c);
    t.release (c);
    t.cycle ();

    assert (t.get_active () == true);
  }

  // releasing cancels immediately
  t.release (42);
  assert (t.cnt_onDeactivate == 1);
  assert (t.get_active () == false);
  assert (t.last_unreg_kc == 42);
}

void
test_oneshot_hold_and_tap_through_timeout (void) {
  OneShotTestKey t = OneShotTestKey (42);

  t.press (42);
  t.cycle ();

  // holding prevents cancellation
  for (uint8_t c = 0; c < 32; c++) {
    t.press (c);
    t.release (c);
    t.cycle ();

    assert (t.get_active () == true);
  }

  // holding through a timeout still prevents cancellation
  for (uint8_t c = 0; c < 32; c++) {
    t.cycle ();

    assert (t.get_active () == true);
  }

  // tapping further keys, we are still active
  t.press (1);
  t.release (1);
  assert (t.get_active () == true);

  // releasing cancels immediately
  t.release (42);
  assert (t.cnt_onDeactivate == 1);
  assert (t.get_active () == false);
  assert (t.last_unreg_kc == 42);
}

void
test_oneshot_ignore_codes (void) {
  OneShotTestKey t = OneShotTestKey (42);

  t.press (42);
  t.release (42);

  // Pressing ignored keys does not cancel the one-shotness
  t.press (132);
  t.release (132);
  t.cycle ();
  t.press (133);
  t.release (133);
  t.cycle ();

  assert (t.cnt_onActivate == 1);
  assert (t.get_active () == true);
  assert (t.get_sticky () == false);
  assert (t.last_reg_kc == 42);

  // pressing a non-ignored one does cancel us
  t.press (1);
  t.release (1);
  t.cycle ();

  assert (t.get_active () == false);
}

void
test_oneshot_custom_timeout (void) {
  OneShotTestKey t = OneShotTestKey (42);

  t.timeout (42);
  assert (t.timeout () == 42);

  t.press (42);
  t.release (42);
  for (uint8_t i = 0; i < 41; i++) {
    t.cycle ();
  }
  assert (t.cnt_onActivate == 1);
  assert (t.cnt_onDeactivate == 0);
  assert (t.get_active () == true);

  t.cycle ();
  assert (t.cnt_onActivate == 1);
  assert (t.cnt_onDeactivate == 1);
  assert (t.get_active () == false);
}

int
main (void) {

  test_oneshot_activate_deactivate_with_timeout ();
  test_oneshot_cancel_by_keypress ();
  test_oneshot_sticky ();
  test_oneshot_held ();
  test_oneshot_hold_and_tap ();
  test_oneshot_hold_and_tap_through_timeout ();

  test_oneshot_ignore_codes ();

  test_oneshot_custom_timeout ();

  return 0;
}
