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

#include "kbagLayout.h"

#include <iostream>
#include <iomanip>
#include <assert.h>

static uint16_t simple_keymap[] = {
   KC_F11   ,KC_9     ,KC_7     ,KC_5     ,KC_3     ,KC_1     ,KC_NO
  ,KC_BSLS  ,KC_X     ,KC_W     ,KC_C     ,KC_H     ,KC_F
  ,KC_TAB   ,KC_A     ,KC_O     ,KC_E     ,KC_I     ,KC_U     ,LSFT(KC_9)
  ,KC_MPLY  ,KC_Z     ,KC_Q     ,KC_QUOT  ,KC_COMM  ,KC_DOT   ,KC_NO

                                            ,KC_NO    ,KC_BSPC  ,KC_NO    ,KC_NO
                                                          ,KC_NO


                                                    ,KC_NO     ,KC_0     ,KC_2     ,KC_4     ,KC_6     ,KC_8     ,KC_NO
                                                               ,KC_M     ,KC_G     ,KC_L     ,KC_P     ,KC_SLSH  ,KC_GRV
                                                    ,RSFT(KC_0),KC_D     ,KC_R     ,KC_T     ,KC_N     ,KC_S     ,KC_EQL
                                                    ,KC_NO     ,KC_B     ,KC_K     ,KC_V     ,KC_Y     ,KC_J     ,KC_MSTP

                                            ,KC_NO    ,KC_SPC   ,KC_ENT   ,KC_MINS
                                                          ,KC_NO
};

static uint16_t mod_keymap[] = {
   LCTL(RCTL(KC_0)),KC_NO ,KC_NO      ,KC_NO      ,KC_NO      ,KC_NO      ,KC_NO
  ,KC_NO      ,KC_NO      ,KC_NO      ,KC_NO      ,KC_NO      ,KC_NO
  ,KC_NO      ,KC_NO      ,KC_NO      ,KC_NO      ,KC_NO      ,KC_NO      ,KC_NO
  ,KC_NO      ,KC_NO      ,KC_NO      ,KC_NO      ,KC_NO      ,KC_NO      ,KC_NO

                                                    ,KC_NO      ,KC_NO      ,KC_NO      ,KC_NO
                                                                    ,KC_NO


                          ,KC_NO      ,KC_NO      ,KC_NO      ,KC_NO      ,KC_NO      ,KC_NO      ,KC_NO
                                      ,KC_NO      ,KC_NO      ,KC_NO      ,KC_NO      ,KC_NO      ,KC_NO
                          ,KC_NO      ,KC_NO      ,KC_NO      ,KC_NO      ,KC_NO      ,KC_NO      ,KC_NO
                          ,KC_NO      ,KC_NO      ,KC_NO      ,KC_NO      ,KC_NO      ,KC_NO      ,KC_NO

            ,KC_NO      ,KC_NO      ,KC_NO      ,KC_NO
                                ,KC_NO
};

class LayoutTestKey : public BasicKey {
public:
  uint8_t pc, rc, spc, src, cc;
  BasicKey *k;

  LayoutTestKey (uint8_t index, BasicKey *k) : BasicKey (index) {
    this->k = k;
    cc = pc = rc = spc = src = 0;
  };

  ~LayoutTestKey (void) {
    delete (k);
  }

  virtual bool press (uint8_t index) {
    pc++;
    if (index == this->index)
      spc++;
    return k->press (index);
  }

  virtual bool release (uint8_t index) {
    rc++;
    if (index == this->index)
      src++;
    return k->release (index);
  }

  virtual void cycle (void) {
    cc++;
    k->cycle ();
  }

  uint8_t get_code (void) {
    return ((SelfKey *)this->k)->get_code ();
  }
};

class TestHWKey : public HWKey {
public:
  uint8_t pcache[10240], rcache[10240];
  uint16_t pidx, ridx;

  TestHWKey (void) {
    pidx = ridx = 0;
  };

  void register_code (uint8_t code) {
    if (code == 0)
      return;
    pcache[pidx++] = code;
  };
  void unregister_code (uint8_t code) {
    if (code == 0)
      return;
    rcache[ridx++] = code;
  };
};

class TestLayout : public Layout {
public:
  TestLayout (HWKey *hw, uint16_t layout[LAYOUT_SIZE]) : Layout (hw) {
    setup(layout);
  };

  virtual BasicKey *createKey (uint8_t index, uint16_t code) {
    return new LayoutTestKey (index, Layout::createKey (index, code));
  }

  LayoutTestKey *get_key (uint8_t index) {
    return (LayoutTestKey *)layout[index];
  }
};

TEST_GROUP (Layout) {
};

TEST (Layout, simple) {
  TestHWKey *hwk = new TestHWKey ();
  TestLayout *layout = new TestLayout (hwk, simple_keymap);

  //
  for (int i = 0; i < LAYOUT_SIZE; i++) {
    if (simple_keymap[i] >> 8 == 0)
      LONGS_EQUAL (simple_keymap[i] & ~0xff00, layout->get_key (i)->get_code());
  }

  //
  int cc = 0;
  layout->cycle ();
  for (int i = 0; i < LAYOUT_SIZE; i++) {
    cc += layout->get_key (i)->cc;
  }
  LONGS_EQUAL (64, cc);

  //
  for (int i = 0; i < LAYOUT_SIZE; i++) {
    layout->press (i);
  }
  for (int i = 0; i < LAYOUT_SIZE; i++) {
    LayoutTestKey *k = layout->get_key (i);

    LONGS_EQUAL (64, k->pc);
    LONGS_EQUAL (1, k->spc);
  }

  //
  for (int i = 0; i < LAYOUT_SIZE; i++) {
    layout->release (i);
  }
  for (int i = 0; i < LAYOUT_SIZE; i++) {
    LayoutTestKey *k = layout->get_key (i);

    LONGS_EQUAL (64, k->rc);
    LONGS_EQUAL (1, k->src);
  }

  delete (layout);
}

TEST (Layout, modifiers) {
  TestHWKey *hwk = new TestHWKey ();
  TestLayout *mod_layout = new TestLayout (hwk, mod_keymap);

  mod_layout->press (0);
  mod_layout->cycle ();
  mod_layout->release (0);

  CHECK (hwk->pcache[0] == KC_RCTL &&
         hwk->pcache[1] == KC_LCTL &&
         hwk->pcache[2] == KC_0);

  CHECK (hwk->rcache[0] == KC_0 &&
         hwk->rcache[1] == KC_LCTL &&
         hwk->rcache[2] == KC_RCTL);

  delete (mod_layout);
}
