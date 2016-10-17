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

#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>

#include "OneShot.h"

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

class TestKey : public BasicKey {
public:
  uint8_t last_press, last_release;

  TestKey (uint8_t index) : BasicKey (index) {
    last_press = last_release = 0;
  };

  bool press (uint8_t index) { last_press = index; return false; };
  bool release (uint8_t index) { last_release = index; return false; };
  void cycle () {};
};


class OneShotModifierTestKey : public OneShotModifierKey {
public:
  OneShotModifierTestKey (uint8_t code, BasicKey *kc) : OneShotModifierKey (code, kc) {
    last_kc_reg = last_kc_unreg = 0;
  };
  OneShotModifierTestKey (uint8_t code, BasicKey *kc, uint16_t timeout) : OneShotModifierKey (code, kc, timeout) {
    last_kc_reg = last_kc_unreg = 0;
  };

  uint8_t get_last_reg (void) { return ((TestKey *)mod)->last_press; };
  uint8_t get_last_unreg (void) { return ((TestKey *)mod)->last_release; };

  uint8_t last_kc_reg, last_kc_unreg;
};

TEST_GROUP (OneShot) {
};

TEST (OneShot, activate_deactivate_with_timeout) {
  OneShotTestKey t = OneShotTestKey(42);

  t.press (1);
  t.release (1);
  CHECK (t.cnt_onActivate == 0);

  t.press (42);
  // the key should activate immediately
  CHECK (t.cnt_onActivate == 1);

  // releasing the key shall not immediately unregister
  t.release (42);

  // timing it out shall deactivate the key
  for (int c = 0; c < 39; c++) {
    t.cycle ();
    CHECK (t.cnt_onDeactivate == 0);
  }
  t.cycle ();
  CHECK (t.cnt_onDeactivate == 1);
}

TEST (OneShot, cancel_by_keypress) {
  OneShotTestKey t = OneShotTestKey (42);

  t.press (42);
  t.release (42);
  t.cycle ();

  CHECK (t.cnt_onActivate == 1);

  t.press (1);

  // we do not deactivate when another key is pressed...
  CHECK (t.cnt_onDeactivate == 0);

  // we deactivate on the next scan
  t.cycle ();

  CHECK (t.cnt_onDeactivate == 1);
  CHECK (t.get_active () == false);

  // the next key pressed will not touch the one-shot key
  t.press (2);
  t.release (2);
  CHECK (t.get_active () == false);
  CHECK (t.cnt_onActivate == 1);
  CHECK (t.cnt_onDeactivate == 1);
}

TEST (OneShot, sticky) {
  OneShotTestKey t = OneShotTestKey (42);

  t.press (42);
  t.release (42);
  t.cycle ();

  CHECK (t.get_sticky () == false);

  t.press (42);
  t.release (42);
  t.cycle ();

  // double-tapping activates once, but the stickyness is enabled
  CHECK (t.cnt_onActivate == 1);
  CHECK (t.get_active () == true);
  CHECK (t.get_sticky () == true);

  // tapping any number of keys, when sticky, do not deactivate
  for (uint8_t i = 1; i < 10; i++) {
    t.press (i);
    t.release (i);
    t.cycle ();
  }
  CHECK (t.cnt_onDeactivate == 0);
  CHECK (t.get_active () == true);
  CHECK (t.get_sticky () == true);

  // tapping a third time disables stickyness
  t.press (42);
  t.release (42);
  t.cycle ();

  CHECK (t.cnt_onDeactivate == 1);
  CHECK (t.get_active () == false);
  CHECK (t.get_sticky () == false);
}

TEST (OneShot, held) {
  OneShotTestKey t = OneShotTestKey (42);

  t.press (42);

  CHECK (t.cnt_onActivate == 1);
  CHECK (t.get_active () == true);
  CHECK (t.get_sticky () == false);

  // holding the key prevents timing out
  for (uint8_t c = 0; c < 80; c++) {
    t.cycle ();
    CHECK (t.get_active () == true);
  }

  // releasing the key immediately times it out
  t.release (42);
  CHECK (t.cnt_onDeactivate == 1);
  CHECK (t.get_active () == false);
}

TEST (OneShot, hold_and_tap) {
  OneShotTestKey t = OneShotTestKey (42);

  t.press (42);
  t.cycle ();

  // holding prevents cancellation
  for (uint8_t c = 0; c < 32; c++) {
    t.press (c);
    t.release (c);
    t.cycle ();

    CHECK (t.get_active () == true);
  }

  // releasing cancels immediately
  t.release (42);
  CHECK (t.cnt_onDeactivate == 1);
  CHECK (t.get_active () == false);
}

TEST (OneShot, hold_and_tap_through_timeout) {
  OneShotTestKey t = OneShotTestKey (42);

  t.press (42);
  t.cycle ();

  // holding prevents cancellation
  for (uint8_t c = 0; c < 32; c++) {
    t.press (c);
    t.release (c);
    t.cycle ();

    CHECK (t.get_active () == true);
  }

  // holding through a timeout still prevents cancellation
  for (uint8_t c = 0; c < 32; c++) {
    t.cycle ();

    CHECK (t.get_active () == true);
  }

  // tapping further keys, we are still active
  t.press (1);
  t.release (1);
  CHECK (t.get_active () == true);

  // releasing cancels immediately
  t.release (42);
  CHECK (t.cnt_onDeactivate == 1);
  CHECK (t.get_active () == false);
}

TEST (OneShot, ignore_codes) {
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

  CHECK (t.cnt_onActivate == 1);
  CHECK (t.get_active () == true);
  CHECK (t.get_sticky () == false);

  // pressing a non-ignored one does cancel us
  t.press (1);
  t.release (1);
  t.cycle ();

  CHECK (t.get_active () == false);
}

TEST_GROUP (OneShotMod) {
};

TEST (OneShotMod, modifiers) {
  OneShotModifierTestKey t = OneShotModifierTestKey (42, new TestKey (132));

  t.press (42);
  CHECK (t.get_last_reg () == 132);
  t.release (42);
  t.cycle ();

  t.press (1);
  t.release (1);
  t.cycle ();

  CHECK (t.get_last_unreg () == 132);
}

TEST (OneShot, caller_signal) {
  OneShotTestKey t = OneShotTestKey (42);

  CHECK (t.press (1) == true);
  CHECK (t.press (42) == false);
  CHECK (t.release (42) == false);
  CHECK (t.release (1) == true);
}
