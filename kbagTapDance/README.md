Generic multi-use key implementation
====================================

[![Build Status](https://travis-ci.org/algernon/arduino-kbd-tap-dance.svg?branch=master)](https://travis-ci.org/algernon/arduino-kbd-tap-dance)

Primarily aimed at the [Keyboard.io Model 01][kbdio], but likely usable by any
other keyboard built with an Arduino at heart. But before we go and look at the
details, let me first say that this has not been tested on any real hardware,
and as such, is more a proof of concept, than anything else.

 [kbdio]: https://shop.keyboard.io/

Features
--------

### Tap-dance keys

Tap-dance keys are generic multi-purpose keys, that do a different action
depending on how many times they were tapped before the action triggered.
Actions trigger when they time out, or when another key is pressed (before the
interrupting key is handled).
  
These are perfect for adding less used behaviour to some keys, like making the
`Tab` key act as a temporary layer switch while held, and a layer toggle when
tapped twice.
  
### One-shot keys

These keys register immediately, and remain registered until they are canceled,
either by timing out, or after pressing another key.
  
These are perfect for modifiers, allowing you to release the modifier, tap
another key, and still have the modifier applied.
  
### Leader keys

These use a lookup method to figure out what action to do. Once a leader key is
tapped, all other keys will stop doing their own function, and contribute to the
*leader sequence*. If there is an action for a given sequence, that is run
instead of the keys pressed previously. If there is no such action, or the key
times out, the pressed keys are silently ignored.
  
This can be used to input longer command sequences, as individual taps, instead
of chording. Perfect for those occasions where one may want a mnemonic shortcut
to do something complicated, and rarely used. Such as enabling firmware
debugging, or typing a pre-recorded message, or switching the default layer.

License
=======

The code is released under the terms of the GNU GPL, version 3 or later. See the
COPYING file for details.
