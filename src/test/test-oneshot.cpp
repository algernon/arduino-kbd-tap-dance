/*
 * arduino-kbd-tap-dance -- multi-use keys for arduino keyboards
 * Copyright (C) 2016  Gergely Nagy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "OneShot.h"
#include "assert.h"

class OneShotTestKey : public OneShotKey {
public:
  OneShotTestKey (uint8_t keycode) : OneShotKey (keycode) {
    cnt_onActivate = 0;
    cnt_onDeactivate = 0;
  };

  uint8_t cnt_onActivate, cnt_onDeactivate;

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

class OneShotModifierTestKey : public OneShotModifierKey {
protected:
  virtual void register_code (uint8_t code) { last_kc_reg = code; };
  virtual void unregister_code (uint8_t code) { last_kc_unreg = code; };

public:
  OneShotModifierTestKey (uint8_t code, uint8_t kc) : OneShotModifierKey (code, kc) {
    last_kc_reg = last_kc_unreg = 0;
  };
  OneShotModifierTestKey (uint8_t code, uint8_t kc, uint16_t timeout) : OneShotModifierKey (code, kc, timeout) {
    last_kc_reg = last_kc_unreg = 0;
  };

  uint8_t last_kc_reg, last_kc_unreg;
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

  // releasing the key shall not immediately unregister
  t.release (42);

  // timing it out shall deactivate the key
  for (int c = 0; c < 39; c++) {
    t.cycle ();
    assert (t.cnt_onDeactivate == 0);
  }
  t.cycle ();
  assert (t.cnt_onDeactivate == 1);
}

void
test_oneshot_cancel_by_keypress (void) {
  OneShotTestKey t = OneShotTestKey (42);

  t.press (42);
  t.release (42);
  t.cycle ();

  assert (t.cnt_onActivate == 1);

  t.press (1);

  // we do not deactivate when another key is pressed...
  assert (t.cnt_onDeactivate == 0);

  // we deactivate on the next scan
  t.cycle ();

  assert (t.cnt_onDeactivate == 1);
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

  // tapping any number of keys, when sticky, do not deactivate
  for (uint8_t i = 1; i < 10; i++) {
    t.press (i);
    t.release (i);
    t.cycle ();
  }
  assert (t.cnt_onDeactivate == 0);
  assert (t.get_active () == true);
  assert (t.get_sticky () == true);

  // tapping a third time disables stickyness
  t.press (42);
  t.release (42);
  t.cycle ();

  assert (t.cnt_onDeactivate == 1);
  assert (t.get_active () == false);
  assert (t.get_sticky () == false);
}

void
test_oneshot_held (void) {
  OneShotTestKey t = OneShotTestKey (42);

  t.press (42);

  assert (t.cnt_onActivate == 1);
  assert (t.get_active () == true);
  assert (t.get_sticky () == false);

  // holding the key prevents timing out
  for (uint8_t c = 0; c < 80; c++) {
    t.cycle ();
    assert (t.get_active () == true);
  }

  // releasing the key immediately times it out
  t.release (42);
  assert (t.cnt_onDeactivate == 1);
  assert (t.get_active () == false);
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

  // pressing a non-ignored one does cancel us
  t.press (1);
  t.release (1);
  t.cycle ();

  assert (t.get_active () == false);
}

void
test_oneshot_modifiers (void) {
  OneShotModifierTestKey t = OneShotModifierTestKey (42, 132);

  t.press (42);
  assert (t.last_kc_reg == 132);
  t.release (42);
  t.cycle ();

  t.press (1);
  t.release (1);
  t.cycle ();

  assert (t.last_kc_unreg == 132);
}

void
test_oneshot_caller_signal (void) {
  OneShotTestKey t = OneShotTestKey (42);

  assert (t.press (1) == true);
  assert (t.press (42) == false);
  assert (t.release (42) == false);
  assert (t.release (1) == true);
}

int
main (void) {

  test_oneshot_activate_deactivate_with_timeout ();
  test_oneshot_cancel_by_keypress ();
  test_oneshot_sticky ();
  test_oneshot_held ();
  test_oneshot_hold_and_tap ();
  test_oneshot_hold_and_tap_through_timeout ();

  test_oneshot_caller_signal ();
  test_oneshot_ignore_codes ();

  test_oneshot_modifiers ();

  return 0;
}
