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

// Convert physical keyboard layout to matrix array.
// This is a macro to define keymap easily in keyboard layout form.
#define	KEYMAP( \
	K212, K213, K214, K112, K113, K114, K012, K013, K014, K312, K313, K314, K412, K413, K414, K512, K513, K514, \
	K000, K001, K002, K003, K004, K005,                                     K506, K507, K508, K509, K510, K511, \
	K100, K101, K102, K103, K104, K105,                                     K606, K607, K608, K609, K610, K611, \
	K200, K201, K202, K203, K204, K205,                                     K706, K707, K708, K709, K710, K711, \
	K300, K301, K302, K303, K304, K305,                                     K806, K807, K808, K809, K810, K811, \
	      K401, K402, K403, K405,                                                 K306, K308, K309, K310, \
	                                    K605, K505,             K704, K602, \
	                              K603, K604, K503,             K702, K601, K600, \
										      K501,             K700 \
) \
{ \
	{ KC_##K000, KC_##K001, KC_##K002, KC_##K003, KC_##K004, KC_##K005, KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_##K012, KC_##K013, KC_##K014 }, \
	{ KC_##K100, KC_##K101, KC_##K102, KC_##K103, KC_##K104, KC_##K105, KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_##K112, KC_##K113, KC_##K114 }, \
	{ KC_##K200, KC_##K201, KC_##K202, KC_##K203, KC_##K204, KC_##K205, KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_##K212, KC_##K213, KC_##K214 }, \
	{ KC_##K300, KC_##K301, KC_##K302, KC_##K303, KC_##K304, KC_##K305, KC_##K306, KC_NO,     KC_##K308, KC_##K309, KC_##K310, KC_##K311, KC_##K312, KC_##K313, KC_##K314 }, \
	{ KC_##K400, KC_##K401, KC_##K402, KC_##K403, KC_NO,     KC_##K405, KC_##K406, KC_##K407, KC_##K408, KC_##K409, KC_##K410, KC_##K411, KC_##K412, KC_##K413, KC_##K414 }, \
	{ KC_NO,     KC_##K501, KC_NO,     KC_##K503, KC_NO,     KC_##K505, KC_##K506, KC_##K507, KC_##K508, KC_##K509, KC_##K510, KC_NO,     KC_##K512, KC_##K513, KC_##K514 }, \
	{ KC_##K600, KC_##K601, KC_##K602, KC_##K603, KC_##K604, KC_##K605, KC_##K606, KC_##K607, KC_##K608, KC_##K609, KC_##K610, KC_##K611, KC_NO,     KC_NO,     KC_NO     }, \
	{ KC_##K700, KC_NO,     KC_##K702, KC_NO,     KC_##K704, KC_NO,     KC_##K706, KC_##K707, KC_##K708, KC_##K709, KC_##K710, KC_##K711, KC_NO,     KC_NO,     KC_NO     }, \
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
    /* Layer 0: Default Layer
     * ,-----------------------------------------------------------.
     * |Esc|  1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|  =|  \|  `|
     * |-----------------------------------------------------------|
     * |Tab  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  [|  ]|Backs|
     * |-----------------------------------------------------------|
     * |Contro|  A|  S|  D|  F|  G|  H|  J|  K|  L|Fn3|  '|Return  |
     * |-----------------------------------------------------------|
     * |Shift   |  Z|  X|  C|  V|  B|  N|  M|  ,|  .|Fn2|Shift |Fn1|
     * `-----------------------------------------------------------'
     *       |Gui|Alt  |Fn5                    |Alt  |Fn4|
     *       `-------------------------------------------'
     */
    KEYMAP(ESC, 1,   2,   3,   4,   5,   6,   7,   8,   9,   0,   MINS,EQL, BSLS,GRV, \
           TAB, Q,   W,   E,   R,   T,   Y,   U,   I,   O,   P,   LBRC,RBRC,BSPC, \
           LCTL,A,   S,   D,   F,   G,   H,   J,   K,   L,   FN3, QUOT,ENT, \
           LSFT,Z,   X,   C,   V,   B,   N,   M,   COMM,DOT, FN2, RSFT,FN1, \
                LGUI,LALT,          FN5,                RALT,FN4),

    /* Layer 1: HHKB mode (HHKB Fn)
     * ,-----------------------------------------------------------.
     * |Esc| F1| F2| F3| F4| F5| F6| F7| F8| F9|F10|F11|F12|Ins|Del|
     * |-----------------------------------------------------------|
     * |Caps |   |   |   |   |   |   |   |Psc|Slk|Pus|Up |   |Backs|
     * |-----------------------------------------------------------|
     * |Contro|VoD|VoU|Mut|   |   |  *|  /|Hom|PgU|Lef|Rig|Enter   |
     * |-----------------------------------------------------------|
     * |Shift   |   |   |   |   |   |  +|  -|End|PgD|Dow|Shift |xxx|
     * `-----------------------------------------------------------'
     *      |Gui |Alt  |Space                  |Alt  |xxx|
     *      `--------------------------------------------'
     */ 
    KEYMAP(PWR, F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, F12, INS, DEL, \
           CAPS,NO,  NO,  NO,  NO,  NO,  NO,  NO,  PSCR,SLCK,BRK, UP,  NO,  BSPC, \
           LCTL,VOLD,VOLU,MUTE,NO,  NO,  PAST,PSLS,HOME,PGUP,LEFT,RGHT,ENT, \
           LSFT,NO,  NO,  NO,  NO,  NO,  PPLS,PMNS,END, PGDN,DOWN,RSFT,FN1, \
                LGUI,LALT,          SPC,                RALT,FN7),

    /* Layer 2: Vi mode (Slash)
     * ,-----------------------------------------------------------.
     * |Esc| F1| F2| F3| F4| F5| F6| F7| F8| F9|F10|F11|F12|Ins|Del|
     * |-----------------------------------------------------------|
     * |Tab  |Hom|PgD|Up |PgU|End|Hom|PgD|PgUlEnd|   |   |   |Backs|
     * |-----------------------------------------------------------|
     * |Contro|   |Lef|Dow|Rig|   |Lef|Dow|Up |Rig|   |   |Return  |
     * |-----------------------------------------------------------|
     * |Shift   |   |   |   |   |   |Hom|PgD|PgUlEnd|xxx|Shift |   |
     * `-----------------------------------------------------------'
     *       |Gui|Alt  |Space                  |Alt  |Gui|
     *       `-------------------------------------------'
     */
    KEYMAP(ESC, F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, F12, INS, DEL, \
           TAB, HOME,PGDN,UP,  PGUP,END, HOME,PGDN,PGUP,END, NO,  NO,  NO,  BSPC, \
           LCTL,NO,  LEFT,DOWN,RGHT,NO,  LEFT,DOWN,UP,  RGHT,NO,  NO,  ENT, \
           LSFT,NO,  NO,  NO,  NO,  NO,  HOME,PGDN,PGUP,END, FN2, RSFT,NO, \
                LGUI,LALT,          SPC,                RALT,RGUI),

    /* Layer 3: Mouse mode (Semicolon)
     * ,-----------------------------------------------------------.
     * |Esc| F1| F2| F3| F4| F5| F6| F7| F8| F9|F10|F11|F12|Ins|Del|
     * |-----------------------------------------------------------|
     * |Tab  |MwL|MwU|McU|MwD|MwR|MwL|MwD|MwU|MwR|   |   |   |Backs|
     * |-----------------------------------------------------------|
     * |Contro|   |McL|McD|McR|   |McL|McD|McU|McR|xxx|   |Return  |
     * |-----------------------------------------------------------|
     * |Shift   |Mb4|Mb5|Mb1|Mb2|Mb3|Mb2|Mb1|Mb4|Mb5|   |Shift |   |
     * `-----------------------------------------------------------'
     *      |Gui |Alt  |Mb1                    |Alt  |Gui|
     *      `--------------------------------------------'
     * Mc: Mouse Cursor / Mb: Mouse Button / Mw: Mouse Wheel 
     */
#ifdef HOST_IWRAP
// iWRAP does not support mouse wheel, use these keycodes to remap as wheel
#define KC_KPPL KC_KP_PLUS
#define KC_KPMI KC_KP_MINUS
#define KC_KPAS KC_KP_ASTERISK
#define KC_KPSL KC_KP_SLASH
    KEYMAP(ESC, F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, F12, INS, DEL, \
           TAB, KPAS,KPPL,MS_U,KPMI,KPSL,KPAS,KPPL,KPMI,KPSL,NO,  NO,  NO,  BSPC, \
           LCTL,NO,  MS_L,MS_D,MS_R,NO,  MS_L,MS_D,MS_U,MS_R,FN3, NO,  ENT, \
           LSFT,BTN4,BTN5,BTN1,BTN2,BTN3,BTN2,BTN1,NO,  NO,  NO,  RSFT,NO, \
                LGUI,LALT,          BTN1,               RALT,FN4),
#else
    KEYMAP(ESC, F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, F12, INS, DEL, \
           TAB, NO,  NO,  NO,  NO,  NO,  WH_L,WH_D,WH_U,WH_R,NO,  NO,  NO,  BSPC, \
           LCTL,NO,  ACL0,ACL1,ACL2,NO,  MS_L,MS_D,MS_U,MS_R,FN3, NO,  ENT, \
           LSFT,NO,  NO,  NO,  NO,  BTN3,BTN2,BTN1,BTN4,BTN5,NO,  RSFT,NO, \
                LGUI,LALT,          BTN1,               RALT,FN4),
#endif

    /* Layer 4: Matias half keyboard style (Space)
     * ,-----------------------------------------------------------.
     * |  -|  0|  9|  8|  7|  6|  5|  4|  3|  2|  1|   |   |   |Esc|
     * |-----------------------------------------------------------|
     * |Backs|  P|  O|  I|  U|  Y|  T|  R|  E|  W|  Q|   |   |Tab  |
     * |-----------------------------------------------------------|
     * |Contro|  ;|  L|  K|  J|  H|  G|  F|  D|  S|  A|Con|Control |
     * |-----------------------------------------------------------|
     * |Shift   |  /|  .|  ,|  M|  N|  B|  V|  C|  X|  Z|Shift |   |
     * `-----------------------------------------------------------'
     *      |Gui |Alt  |xxxxxxxxxxxxxxxxxxxxxxx|Alt  |Gui|
     *      `--------------------------------------------'
     */
    KEYMAP(MINS,0,   9,   8,   7,   6,   5,   4,   3,   2,   1,   NO,  NO,  NO,  ESC, \
           BSPC,P,   O,   I,   U,   Y,   T,   R,   E,   W,   Q,   NO,  NO,  TAB, \
           LCTL,SCLN,L,   K,   J,   H,   G,   F,   D,   S,   A,   RCTL,RCTL, \
           LSFT,SLSH,DOT, COMM,M,   N,   B,   V,   C,   X,   Z,   RSFT,NO, \
                LGUI,LALT,          FN5,                RALT,RGUI),

    /* Layer5: another Mouse mode (Space) */
#ifdef HOST_IWRAP
    KEYMAP(ESC, F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, F12, INS, DEL, \
           TAB, KPAS,KPPL,MS_U,KPMI,KPSL,KPAS,KPPL,KPMI,KPSL,NO,  NO,  NO,  BSPC, \
           LCTL,NO,  MS_L,MS_D,MS_R,NO,  MS_L,MS_D,MS_U,MS_R,FN3, NO,  ENT, \
           LSFT,BTN4,BTN5,BTN1,BTN2,BTN3,BTN2,BTN1,BTN4,BTN5,NO,  RSFT,NO, \
                LGUI,LALT,          FN5,                RALT,RGUI),
#else
    KEYMAP(ESC, F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, F12, INS, DEL, \
           TAB, NO,  NO,  NO,  NO,  NO,  WH_L,WH_D,WH_U,WH_R,NO,  NO,  NO,  BSPC, \
           LCTL,NO,  ACL0,ACL1,ACL2,NO,  MS_L,MS_D,MS_U,MS_R,FN3, NO,  ENT, \
           LSFT,NO,  NO,  NO,  NO,  BTN3,BTN2,BTN1,BTN4,BTN5,NO,  RSFT,NO, \
                LGUI,LALT,          FN5,                RALT,RGUI),
#endif
    /* 3: Dvorak http://en.wikipedia.org/wiki/Dvorak_Simplified_Keyboard
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
	KEYMAP(ESC, F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, F12, PSCR,SLCK,BRK, FN1, FN2,
	       EQL, 1,   2,   3,   4,   5,                                 6,   7,   8,   9,   0,   MINS,
           TAB, QUOT,COMM,DOT, P,   Y,                                 F,   G,   C,   R,   L,   SLSH,
           CAPS,A,   O,   E,   U,   I,                                 D,   H,   T,   N,   S,   BSLS,
           LSFT,SCLN,Q,   J,   K,   X,                                 B,   M,   W,   V,   Z,   RSFT,
	            GRV, BSLS,DOWN,UP,                                          LEFT,RGHT,LBRC,RBRC,
	                                     LALT,LGUI,          RCTL,RALT,
	                                BSPC,DEL, HOME,          PGUP,ENT, SPC,
	                                          END,           PGDN
	       ),

	 /*
     * ,-----------------------------------------------------------.
     * |  `|  1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  [|  ]|Backspa|
     * |-----------------------------------------------------------|
     * |Tab  |  '|  ,|  .|  P|  Y|  F|  G|  C|  R|  L|  /|  =|    \|
     * |-----------------------------------------------------------|
     * |BackSp|  A|  O|  E|  U|  I|  D|  H|  T|  N|  S|  -|Return  |
     * |-----------------------------------------------------------|
     * |Shift   |  ;|  Q|  J|  K|  X|  B|  M|  Wl  V|  Z|Shift     |
     * |-----------------------------------------------------------|
     * |Ctrl |Gui |Alt |         Space         |Alt |Gui |Menu|Ctrl|
     * `-----------------------------------------------------------'
     */
    KEYMAP(
    ESC, F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, F12,           PSCR,SLCK,BRK,
    GRV, 1,   2,   3,   4,   5,   6,   7,   8,   9,   0,   LBRC,RBRC,BSPC,     INS, HOME,PGUP,    NLCK,PSLS,PAST,PMNS,
    TAB, QUOT,COMM,DOT, P,   Y,   F,   G,   C,   R,   L,   SLSH,EQL, BSLS,     DEL, END, PGDN,    P7,  P8,  P9,
    CAPS,A,   O,   E,   U,   I,   D,   H,   T,   N,   S,   MINS,     ENT,                         P4,  P5,  P6,  PPLS,
    LSFT,SCLN,Q,   J,   K,   X,   B,   M,   W,   V,   Z,             RSFT,          UP,           P1,  P2,  P3,
    LCTL,LGUI,LALT,          SPC,                     RALT,RGUI,APP, RCTL,     LEFT,DOWN,RGHT,    P0,       PDOT,PENT
    ),

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
