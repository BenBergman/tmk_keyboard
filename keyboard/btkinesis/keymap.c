/*
Copyright 2011 Jun Wako <wakojun@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/* 
 * Keymap for PFU HHKB Pro
 */
#include <stdint.h>
#include <stdbool.h>
#include <avr/pgmspace.h>
#include "host.h"
#include "keycode.h"
#include "print.h"
#include "debug.h"
#include "util.h"
#include "keymap.h"

/*
 * ,-----------------------------------.   ,-----------------------------------.
 * |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
 * `-----------------------------------'   `-----------------------------------'
 *    ,------------------------.                   ,------------------------.
 *    |    |   |   |   |   |   |                   |   |   |   |   |   |    |
 *    |----+---+---+---+---+---|                   |---+---+---+---+---+----|
 *    |    |   |   |   |   |   |                   |   |   |   |   |   |    |
 *    |----+---+---+---+---+---|                   |---+---+---+---+---+----|
 *    |    |   |   |   |   |   |                   |   |   |   |   |   |    |
 *    |----+---+---+---+---+---|                   |---+---+---+---+---+----|
 *    |    |   |   |   |   |   |                   |   |   |   |   |   |    |
 *    `----+---+---+---+---+---'                   `---+---+---+---+---+----'
 *         |   |   |   |   |                           |   |   |   |   |
 *         `---------------'   ,-------.   ,-------.   `---------------'
 *                             |   |   |   |   |   |
 *                         ,---+---+---|   |---+---+---.
 *                         |   |   |   |   |   |   |   |
 *                         | - | - |---|   |---| - | - |
 *                         | - | - |   |   |   | - | - |
 *                         `-----------'   `-----------'
 */

/*
 * Condensed Kinesis matrix (original qwerty labelling)
 *   | 0     1     2     3     4     5     6     7     8     9     10    11
 * --+-----------------------------------------------------------------------
 * 0 | =     1     2     3     4     5     6     7     8     9     0     -
 * 1 | TB    Q     W     E     R     T     Y     U     I     O     P     \
 * 2 | CP    A     S     D     F     G     H     J     K     L     ;     ‘
 * 3 | LS    Z     X     C     V     B     N     M     ,     .     /     RS
 * 4 |       `     IN    LT          RT    UP          DN    [     ]
 * 5 |       F6    F7    F8                            F9    F10   F11
 * 6 | HOME  F3    F4    F5    END   LCTRL SPACE ENTER F12   PS    SL    RALT
 * 7 | BKSP  ESC   F1    F2    DEL   LALT  PGDN  RCTRL PB    PG    KY    PGUP
 */

// Convert physical keyboard layout to matrix array.
// This is a macro to define keymap easily in keyboard layout form.
#define	KEYMAP( \
	K701, K702, K703, K601, K602, K603, K501, K502, K503, K508, K509, K510, K608, K609, K610, K708, K709, K710, \
	K000, K001, K002, K003, K004, K005,                                     K006, K007, K008, K009, K010, K011, \
	K100, K101, K102, K103, K104, K105,                                     K106, K107, K108, K109, K110, K111, \
	K200, K201, K202, K203, K204, K205,                                     K206, K207, K208, K209, K210, K211, \
	K300, K301, K302, K303, K304, K305,                                     K306, K307, K308, K309, K310, K311, \
	      K401, K402, K403, K405,                                                 K406, K408, K409, K410, \
	                                    K605, K705,             K611, K707, \
	                              K700, K704, K600,             K706, K607, K606, \
	                                          K604,             K711 \
) \
{ \
	{ KC_##K000, KC_##K100, KC_##K200, KC_##K300, KC_NO,     KC_NO,     KC_##K600, KC_##K700 }, \
	{ KC_##K001, KC_##K101, KC_##K201, KC_##K301, KC_##K401, KC_##K501, KC_##K601, KC_##K701 }, \
	{ KC_##K002, KC_##K102, KC_##K202, KC_##K302, KC_##K402, KC_##K502, KC_##K602, KC_##K702 }, \
	{ KC_##K003, KC_##K103, KC_##K203, KC_##K303, KC_##K403, KC_##K503, KC_##K603, KC_##K703 }, \
	{ KC_##K004, KC_##K104, KC_##K204, KC_##K304, KC_NO,     KC_NO,     KC_##K604, KC_##K704 }, \
	{ KC_##K005, KC_##K105, KC_##K205, KC_##K305, KC_##K405, KC_NO,     KC_##K605, KC_##K705 }, \
	{ KC_##K006, KC_##K106, KC_##K206, KC_##K306, KC_##K406, KC_NO,     KC_##K606, KC_##K706 }, \
	{ KC_##K007, KC_##K107, KC_##K207, KC_##K307, KC_NO,     KC_NO,     KC_##K607, KC_##K707 }, \
	{ KC_##K008, KC_##K108, KC_##K208, KC_##K308, KC_##K408, KC_##K508, KC_##K608, KC_##K708 }, \
	{ KC_##K009, KC_##K109, KC_##K209, KC_##K309, KC_##K409, KC_##K509, KC_##K609, KC_##K709 }, \
	{ KC_##K010, KC_##K110, KC_##K210, KC_##K310, KC_##K410, KC_##K510, KC_##K610, KC_##K710 }, \
	{ KC_##K011, KC_##K111, KC_##K211, KC_##K311, KC_NO,     KC_NO,     KC_##K611, KC_##K711 } \
}

#define KEYCODE(layer, row, col) (pgm_read_byte(&keymaps[(layer)][(row)][(col)]))


// Assign Fn key(0-7) to a layer to which switch with the Fn key pressed.
static const uint8_t PROGMEM fn_layer[] = {
	0,              // Fn0
	1,              // Fn1
	2,              // Fn2
	3,              // Fn3
	3,              // Fn4
	5,              // Fn5
	0,              // Fn6
	0               // Fn7
};

// Assign Fn key(0-7) to a keycode sent when release Fn key without use of the layer.
// See layer.c for details.
static const uint8_t PROGMEM fn_keycode[] = {
	KC_NO,          // Fn0
	KC_NO,          // Fn1
	KC_SLSH,        // Fn2
	KC_SCLN,        // Fn3
	KC_NO,          // Fn4
	KC_SPC,         // Fn5
	KC_NO,          // Fn6
	KC_NO           // Fn7
};

static const uint8_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	/* 0: Qwerty
	 * ,-----------------------------------.   ,-----------------------------------.
	 * |ESC| F1| F2| F3| F4| F5| F6| F7| F8|   | F9|F10|F11|F12| PS| SL|Brk|Key|Prg|
	 * `-----------------------------------'   `-----------------------------------'
	 *     ,-----------------------.                   ,-----------------------.
	 *     |  =|  1|  2|  3|  4|  5|                   |  6|  7|  8|  9|  0|  -|
	 *     |---+---+---+---+---+---|                   |---+---+---+---+---+---|
	 *     |Tab|  Q|  W|  E|  R|  T|                   |  Y|  U|  I|  O|  P|  \|
	 *     |---+---+---+---+---+---|                   |---+---+---+---+---+---|
	 *     |Cap|  A|  S|  D|  F|  G|                   |  H|  J|  K|  L|  ;|  '|
	 *     |---+---+---+---+---+---|                   |---+---+---+---+---+---|
	 *     |Sft|  Z|  X|  C|  V|  B|                   |  N|  M|  ,|  .|  /|Sft|
	 *     `---+---+---+---+---+---'                   `---+---+---+---+---+---'
	 *         |  `|  \| Dn| Up|                           |Lft|Rgt|  [|  ]|
	 *         `---------------'   ,-------.   ,-------.   `---------------'
	 *                             |Alt|Win|   |Ctl|Alt|
	 *                         ,---+---+---|   |---+---+---.
	 *                         |BSp|Del|Hom|   |PgU|Ent|Spc|
	 *                         | - | - |---|   |---| - | - |
	 *                         | - | - |End|   |PgD| - | - |
	 *                         `-----------'   `-----------'
	 */
	KEYMAP(
	ESC, F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, F12, PSCR,SLCK,BRK, FN1, FN2,
	EQL, 1,   2,   3,   4,   5,                                 6,   7,   8,   9,   0,   MINS,
	TAB, Q,   W,   E,   R,   T,                                 Y,   U,   I,   O,   P,   BSLS,
	CAPS,A,   S,   D,   F,   G,                                 H,   J,   K,   L,   SCLN,QUOT,
	LSFT,Z,   X,   C,   V,   B,                                 N,   M,   COMM,DOT, SLSH,RSFT,
	     GRV, BSLS,DOWN,UP,                                          LEFT,RGHT,LBRC,RBRC,
	                              LALT,LGUI,          RCTL,RALT,
	                         BSPC,DEL, HOME,          PGUP,ENT, SPC,
	                                   END,           PGDN
	),

	/* 1: Dvorak http://en.wikipedia.org/wiki/Dvorak_Simplified_Keyboard
	 * ,-----------------------------------.   ,-----------------------------------.
	 * |ESC| F1| F2| F3| F4| F5| F6| F7| F8|   | F9|F10|F11|F12| PS| SL|Brk|Key|Prg|
	 * `-----------------------------------'   `-----------------------------------'
	 *    ,------------------------.                   ,------------------------.
	 *    |   =|  1|  2|  3|  4|  5|                   |  6|  7|  8|  9|  0|   -|
	 *    |----+---+---+---+---+---|                   |---+---+---+---+---+----|
	 *    | Tab|  '|  ,|  .|  P|  Y|                   |  F|  G|  C|  R|  L|   /|
	 *    |----+---+---+---+---+---|                   |---+---+---+---+---+----|
	 *    |Caps|  A|  O|  E|  U|  I|                   |  D|  H|  T|  N|  S|   \|
	 *    |----+---+---+---+---+---|                   |---+---+---+---+---+----|
	 *    |Shft|  ;|  Q|  J|  K|  X|                   |  B|  M|  W|  V|  Z|Shft|
	 *    `----+---+---+---+---+---'                   `---+---+---+---+---+----'
	 *         |  `|  \| Dn| Up|                           |Lft|Rgt|  [|  ]|
	 *         `---------------'   ,-------.   ,-------.   `---------------'
	 *                             |Alt|Win|   |Ctl|Alt|
	 *                         ,---+---+---|   |---+---+---.
	 *                         |BSp|Del|Hom|   |PgU|Ent|Spc|
	 *                         | - | - |---|   |---| - | - |
	 *                         | - | - |End|   |PgD| - | - |
	 *                         `-----------'   `-----------'
	 */
	KEYMAP(
	ESC, F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, F12, PSCR,SLCK,BRK, FN1, FN2,
	EQL, 1,   2,   3,   4,   5,                                 6,   7,   8,   9,   0,   MINS,
	TAB, QUOT,COMM,DOT, P,   Y,                                 F,   G,   C,   R,   L,   SLSH,
	CAPS,A,   O,   E,   U,   I,                                 D,   H,   T,   N,   S,   BSLS,
	LSFT,SCLN,Q,   J,   K,   X,                                 B,   M,   W,   V,   Z,   RSFT,
	     GRV, BSLS,DOWN,UP,                                          LEFT,RGHT,LBRC,RBRC,
	                              LALT,LGUI,          RCTL,RALT,
	                         BSPC,DEL, HOME,          PGUP,ENT, SPC,
	                                   END,           PGDN
	),

	/* 2: Mouse/keypad mode
	 * NOTE: see hhkb keymap.c for iWRAP mouse wheel info
	 * ,-----------------------------------.   ,-----------------------------------.
	 * |ESC| F1| F2| F3| F4| F5| F6| F7| F8|   | F9|F10|F11|F12| PS| SL|Brk|Key|Prg|
	 * `-----------------------------------'   `-----------------------------------'
	 *    ,------------------------.                   ,------------------------.
	 *    |   =|  1|  2|  3|  4|  5|                   |  6|NmL|  =|  /|  *|   -|
	 *    |----+---+---+---+---+---|                   |---+---+---+---+---+----|
	 *    | Tab|MwL|MwD|McU|MWU|MwR|                   |   |Nm7|Nm8|Nm9|Nm-|   /|
	 *    |----+---+---+---+---+---|                   |---+---+---+---+---+----|
	 *    |Caps|   |McL|McD|McR|   |                   |   |Nm4|Nm5|Nm6|Nm+|   \|
	 *    |----+---+---+---+---+---|                   |---+---+---+---+---+----|
	 *    |Shft|   |Mb1|Mb2|Mb3|   |                   |   |Nm1|Nm2|Nm3|Ent|Shft|
	 *    `----+---+---+---+---+---'                   `---+---+---+---+---+----'
	 *         |  `|  \| Dn| Up|                           |Lft|Rgt|Nm.|Ent|
	 *         `---------------'   ,-------.   ,-------.   `---------------'
	 *                             |Alt|Win|   |Ctl|Alt|
	 *                         ,---+---+---|   |---+---+---.
	 *                         |BSp|Del|Hom|   |PgU|Ent|Nm0|
	 *                         | - | - |---|   |---| - | - |
	 *                         | - | - |End|   |PgD| - | - |
	 *                         `-----------'   `-----------'
	 */
	KEYMAP(
	ESC, F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, F12, PSCR,SLCK,BRK, FN1, FN2,
	EQL, 1,   2,   3,   4,   5,                                 6,   NLCK,PEQL,PSLS,PAST,MINS,
	TAB, WH_L,WH_D,MS_U,WH_U,WH_R,                              NO,  P7,  P8,  P9,  PMNS,BSLS,
	CAPS,NO,  MS_L,MS_D,MS_R,NO,                                NO,  P4,  P5,  P6,  PPLS,QUOT,
	LSFT,NO,  BTN1,BTN2,BTN3,NO,                                NO,  P1,  P2,  P3,  PENT,RSFT,
	     GRV, BSLS,DOWN,UP,                                          LEFT,RGHT,PDOT,PENT,
	                              LALT,LGUI,          RCTL,RALT,
	                         BSPC,DEL, HOME,          PGUP,ENT, P0,
	                                   END,           PGDN
	)

};


uint8_t keymap_get_keycode(uint8_t layer, uint8_t row, uint8_t col)
{
	return KEYCODE(layer, row, col);
}

uint8_t keymap_fn_layer(uint8_t index)
{
	return pgm_read_byte(&fn_layer[index]);
}

uint8_t keymap_fn_keycode(uint8_t index)
{
	return pgm_read_byte(&fn_keycode[index]);
}
