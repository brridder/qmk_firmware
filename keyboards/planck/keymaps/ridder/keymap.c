/* Copyright 2015-2017 Jack Humbert
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
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

#include "planck.h"
#include "action_layer.h"

extern keymap_config_t keymap_config;

#define ___x___ KC_NO

enum planck_layers {
  BASE_QWERTY_LAYER,
  _NUMERIC,
  _LOWER,
  _RAISE,
  _ADJUST,
  _NAVIGATION_LAYER,
  GUI_LAYER
};

enum planck_macros {
    M_VERSION,
    LCTL_BRACE,
    RCTL_BRACE,
};

enum planck_keycodes {
  QWERTY = SAFE_RANGE,
  LOWER,
  RAISE,
  BACKLIT,
};

#define ESC_NAV_LY LT(_NAVIGATION_LAYER, KC_ESC)
#define SCLN_NAV_LY LT(_NAVIGATION_LAYER, KC_SCLN)
#define LBRT LT(_ADJUST, KC_LBRACKET)
#define RBRT LT(_ADJUST, KC_RBRACKET)
#define LCTL_ CTL_T(KC_LCBR)
#define RCTL_ CTL_T(KC_RCBR)

#define WM_FULL LALT(LGUI(KC_F))
#define WM_NEXT LCTL(LALT(LGUI(KC_RGHT)))
#define WM_PREV LCTL(LALT(LGUI(KC_LEFT)))
#define WM_NW   LCTL(LGUI(KC_LEFT))
#define WM_N    LALT(LGUI(KC_UP))
#define WM_NE   LCTL(LGUI(KC_RGHT))
#define WM_E    LALT(LGUI(KC_RGHT))
#define WM_SE   S(LCTL(LGUI(KC_RGHT)))
#define WM_S    LALT(LGUI(KC_DOWN))
#define WM_SW   S(LCTL(LGUI(KC_LEFT)))
#define WM_W    LALT(LGUI(KC_LEFT))
#define WM_CNTR LALT(LGUI(KC_C))

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Qwerty
 * ,------------------------------------------------------------------------------------------------------------.
 * | Tab        |   Q      |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P          | ENT         |
 * |------------+----------+------+------+------+-------------+------+------+------+--------------+-------------|
 * | Esc-F(Nav) |   A      |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  | ;-F(Nav)     |  "          |
 * |------------+----------+------+------+------+------|------+------+------+------+--------------+-------------|
 * | Shift - '('|   Z      |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |   /          | SHIFT - ')' |
 * |------------+----------+------+------+------+------+------+------+------+------+--------------+-------------|
 * | F(3) - '[  | Ctrl-'{' | Alt  | GUI  |Lower |    Space    |Raise | GUI  | Alt  | Ctrl-'}'     | F(3) - ']'  |
 * `------------------------------------------------------------------------------------------------------------'
 */
[BASE_QWERTY_LAYER] = {
  {KC_TAB,       KC_Q,   KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,           KC_ENT },
  {ESC_NAV_LY,   KC_A,   KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    SCLN_NAV_LY,    KC_QUOT},
  {KC_LSPO,      KC_Z,   KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,        KC_RSPC},
  {LCTL_,        LCTL_,  KC_LALT, KC_LGUI, LOWER,   KC_SPC,  KC_SPC,  RAISE,   KC_RGUI, KC_RALT, RCTL_,          RCTL_  }
},

/* Numeric layer
*                ,-----------------------------------------------------------------------.
* Application -- |D-Grv|  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  10 | #   |
*      window    |-----------------------------------------------------------------------|
*    switcher    |     | F1  | F2  | F3  | F4  | F5  | F6  | F7  | F8  | F9  | F10 |     |
*                |-----------------------------------------------------------------------|
*                |     | -   | =   | `   | \   |     |     |     | ,   | .   | /   |     |
*                |-----------------------------------------------------------------------|
*                |     |     |     |     |     | Backspace |     |     |     |     |     |
*                `-----------------------------------------------------------------------'
*/
[_LOWER] = {
    {KC_GRV , KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    _______},
    {_______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  _______},
    {_______, KC_MINS, KC_EQL,  KC_GRV,  KC_BSLS, ___x___, ___x___, ___x___, KC_COMM, KC_DOT,  KC_SLSH, _______},
    {_______, _______, _______, _______, _______, KC_BSPC, KC_BSPC, _______, _______, _______, _______, _______}
},

/* Symbol layer
*                ,-----------------------------------------------------------------------.
*                |     | !   | @   | #   | $   | %   | ^   | &   | *   | '   | "   |     | \
*                |-----------------------------------------------------------------------|
*                |     | F11 | F12 | F13 | F14 | F15 | F16 | F17 | F18 | F19 | F20 | #   |
*                |-----------------------------------------------------------------------|  |-- Mostly shifted version
*                |     | _   | +   | ~   | |   |     |     |     | ,   | .   | /   |     | /    of lower layer
*                |-----------------------------------------------------------------------|
*                |     |     |     |     |     |  Delete   |     |     |     |     |     |
*                `-----------------------------------------------------------------------'
*/
[_RAISE] = {
    {KC_TILD, S(KC_1), S(KC_2), S(KC_3), S(KC_4), S(KC_5), S(KC_6), S(KC_7), S(KC_8), KC_QUOT, S(KC_QUOT), _______},
    {_______, KC_F11,  KC_F12,  KC_F13,  KC_F14,  KC_F15,  KC_F16,  KC_F17,  KC_F18,  KC_F19,  KC_F20,     _______},
    {_______, KC_UNDS, KC_PLUS, KC_TILD, KC_PIPE, ___x___, ___x___, ___x___, KC_COMM, KC_DOT,  KC_SLSH,    _______},
    {_______, _______, _______, _______, _______, KC_DEL,  KC_DEL,  _______, _______, _______, _______,    _______}
},

/* Directional navigation layer
*
*         Large movements -----/```````````````````\   /```````````````````\----- Vim-style arrow keys
*                ,-----------------------------------------------------------------------.
*                |     |     |     |     |     |     |     |     |     |     |     |     |
*                |-----------------------------------------------------------------------|
*                |     |     |Home |PgUp |PgDn | End |Left |Down | Up  |Right|     |     |
*                |-----------------------------------------------------------------------|
*                |     |     |     |     |     |     |     |     |     |     |     |     |
*                |-----------------------------------------------------------------------|
*                |     |     |     |     |     |           |     |     |     |     |     |
*                `-----------------------------------------------------------------------'
*/
[_NAVIGATION_LAYER] = {
    {___x___,    ___x___, ___x___, ___x___, ___x___, ___x___, ___x___, ___x___, ___x___, ___x___, ___x___, ___x___},
    {ESC_NAV_LY, ___x___, KC_HOME, KC_PGUP, KC_PGDN, KC_END,  KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, SCLN_NAV_LY, _______},
    {_______,    ___x___, ___x___, ___x___, ___x___, ___x___, ___x___, ___x___, ___x___, ___x___, ___x___, _______},
    {_______,    KC_MPRV, KC_MPLY, KC_MNXT, KC_SLCK, KC_SLEP, KC_SLEP, KC_PAUS, KC_MUTE, KC_VOLD, KC_VOLU, _______}
},

/* GUI (window management/mouse/media controls) layer
*
*        Mouse keys -----/```````````````````\               /```````````````````\----- Window manager
*                ,-----------------------------------------------------------------------.
*                |     |Ms B2|Ms Up|Ms B1|Ms WD|     |     |Prev | NW  |  N  | NE  |     |
*                |-----------------------------------------------------------------------|
*                |     |Ms L |Ms Dn|Ms R |Ms WU|     |     |Full |  W  |Centr|  E  |     |
*                |-----------------------------------------------------------------------|
*                |     |Ms WL|Ms B3|Ms WR|     |     |     |Next | SW  |  S  | SE  |     |
*                |-----------------------------------------------------------------------|
*                |     |Prev |Play |Next |Brig-|   Sleep   |Brig+|Mute |Vol- |Vol+ |     |
*                `-----------------------------------------------------------------------'
*                        \___ Media ___/   \___ Screen/sleep __/   \___ Volume __/
*/
[_ADJUST] = {
    {_______, KC_BTN2, KC_MS_U, KC_BTN1, KC_WH_D, ___x___, ___x___, WM_PREV, WM_NW,   WM_N,    WM_NE,   _______},
    {_______, KC_MS_L, KC_MS_D, KC_MS_R, KC_WH_U, ___x___, ___x___, WM_FULL, WM_W,    WM_CNTR, WM_E,    _______},
    {_______, KC_WH_L, KC_BTN3, KC_WH_R, ___x___, ___x___, ___x___, WM_NEXT, WM_SW,   WM_S,    WM_SE,   _______},
    {_______, KC_MPRV, KC_MPLY, KC_MNXT, KC_SLCK, KC_SLEP, KC_SLEP, KC_PAUS, KC_MUTE, KC_VOLD, KC_VOLU, _______}
},
};

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
    switch(id) {
        case M_VERSION:
            break;
    }
    return MACRO_NONE;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case QWERTY:
      if (record->event.pressed) {
        print("mode just switched to qwerty and this is a huge string\n");
        set_single_persistent_default_layer(BASE_QWERTY_LAYER);
      }
      return false;
      break;
    case LOWER:
      if (record->event.pressed) {
        layer_on(_LOWER);
      } else {
        layer_off(_LOWER);
      }
      return false;
      break;
    case RAISE:
      if (record->event.pressed) {
        layer_on(_RAISE);
      } else {
        layer_off(_RAISE);
      }
      return false;
      break;
    case BACKLIT:
      if (record->event.pressed) {
        register_code(KC_RSFT);
        #ifdef BACKLIGHT_ENABLE
          backlight_step();
        #endif
      } else {
        unregister_code(KC_RSFT);
      }
      return false;
      break;
  }
  return true;
}
