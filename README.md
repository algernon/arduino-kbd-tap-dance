A generic tap-dance implementation
==================================

Primarily aimed at the [Keyboard.io Model 01][kbdio], but likely usable by any
other keyboard built with an Arduino at heart. But before we go and look at the
details, let me first say that this has not been tested on any real hardware,
and as such, is more a proof of concept, than anything else.

 [kbdio]: https://shop.keyboard.io/

What is tap-dance?
------------------

Tap-dance is the name I gave to a feature I originally implemented for
[QMK][qmk] to be used on my [ErgoDox EZ][ez]. It's a funny name for
multi-purpose keys. The basic idea is that one can have multiple actions for a
single key, depending on whether it was tapped once, or twice, or held after one
or two taps - or any number of taps, really.

 [qmk]: https://github.com/jackhumbert/qmk_firmware
 [ez]: https://ergodox-ez.com/

To give you an example, with tap-dance, I can have a key that behaves the
following way:

- If I tap it once, it registers the `Tab` key.
- If I press it once, and keep holding it, it switches to another layer, while
  held.
- If I tap it twice, and release it, it switches to a layer, and remains there
  until toggled off.
  
There are many more things one can do, and the most common ones have a class on
their own, too. The most common one being the double-tap: tapping the key once
registers one keycode, tapping it twice, registers another.

Because tap-dance knows not only about the tap-dance keys, but receives all
other keypresses too, it can recognise when a sequence has been interrupted, and
call the appropriate action, before proceeding with the interrupting keypress.
In practice, this means that if you have a tap-dance key that registers `a` on
the first tap, `b` on the second, and you press this key, followed by - say -
`c` quickly, then the system will register `ac`. And if you keep holding the
key, it will register `a`, and let the OS trigger the auto-repeat.

How to use it?
--------------

*TODO*

License
-------

The code is released under the terms of the GNU GPL, version 3 or later. See the
COPYING file for details.
