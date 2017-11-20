/* Copyright 2017 Ben Ridder
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
#include "xd75.h"
#include "print.h"  


// Fillers to make layering more clear
#define _______ KC_TRNS
#define ___T___ KC_TRNS
#define XXXXXXX KC_NO

// Layer shorthand
#define _QW 0
#define _CM 1
#define _DV 2
#define _LW 3
#define _RS 4
#define _FN 5

enum xd75_layers {
  QWERTY_LAYER,
  NAV_LAYER,
  LOWER,
  RAISE,
};

enum tap_dance_t {
    HYPER_CURLY_BRACKET_RIGHT = 0,
    HYPER_CURLY_BRACKET_LEFT  = 1,
};

enum {
    SINGLE_TAP = 1,
    SINGLE_HOLD,
    DOUBLE_SINGLE_TAP,
    INVALID_TAP_STATE,
};

typedef struct {
    uint32_t state;
    uint32_t single_tap_keycode;
    uint32_t single_tap_mod;
    uint32_t single_hold_keycode_1;
    uint32_t single_hold_keycode_2;
    uint32_t single_hold_keycode_3;
} tap_user_data_t;

int cur_tap_dance_state(qk_tap_dance_state_t *state) {
    if (state->count == 1) {
        return (state->interrupted || state->pressed == 0) ? SINGLE_TAP : SINGLE_HOLD;
    } else if (state->count == 2) {
        return (state->interrupted) ? DOUBLE_SINGLE_TAP : INVALID_TAP_STATE;
    }
    return INVALID_TAP_STATE;
}

static tap_user_data_t left_hyper_state = {
    .state = 0,
    .single_tap_keycode = KC_LCBR,
    .single_tap_mod = KC_LSFT,
    .single_hold_keycode_1 = KC_LALT,
    .single_hold_keycode_2 = KC_LCTL,
    .single_hold_keycode_3 = KC_LGUI,
};

static tap_user_data_t right_hyper_state = {
    .state = 0,
    .single_tap_keycode = KC_RCBR,
    .single_tap_mod = KC_RSFT,
    .single_hold_keycode_1 = KC_RALT,
    .single_hold_keycode_2 = KC_RCTL,
    .single_hold_keycode_3 = KC_RGUI,
};

static void tap_finished(qk_tap_dance_state_t *state, void *user_data)
{
    tap_user_data_t *tap_state = (tap_user_data_t *)user_data;
    tap_state->state = cur_tap_dance_state(state);
    switch (tap_state->state) {
        case SINGLE_TAP:
            register_code(tap_state->single_tap_mod);
            register_code(tap_state->single_tap_keycode);
            unregister_code(tap_state->single_tap_mod);
            break;
        case SINGLE_HOLD:
            register_code(tap_state->single_hold_keycode_1);
            register_code(tap_state->single_hold_keycode_2);
            register_code(tap_state->single_hold_keycode_3);
            break;
        case DOUBLE_SINGLE_TAP:
            register_code(tap_state->single_tap_mod);
            register_code(tap_state->single_tap_keycode);
            unregister_code(tap_state->single_tap_keycode);
            register_code(tap_state->single_tap_keycode);
            unregister_code(tap_state->single_tap_mod);
            break;
        default:
            break;
    }
}

static void tap_reset(qk_tap_dance_state_t *state, void *user_data)
{
    tap_user_data_t *tap_state = (tap_user_data_t *)user_data;

    switch (tap_state->state) {
        case SINGLE_TAP:
            unregister_code(tap_state->single_tap_keycode);
            break;
        case INVALID_TAP_STATE:
            break;
        case SINGLE_HOLD:
            unregister_code(tap_state->single_hold_keycode_3);
            unregister_code(tap_state->single_hold_keycode_2);
            unregister_code(tap_state->single_hold_keycode_1);
            break;
        case DOUBLE_SINGLE_TAP:
            unregister_code(tap_state->single_tap_keycode);
            break;
        default:
            xprintf("LOL WUT\n");
            break;
    }
}

qk_tap_dance_action_t tap_dance_actions[] = {
  [HYPER_CURLY_BRACKET_RIGHT] = { .fn = { NULL, tap_finished, tap_reset }, .user_data = &right_hyper_state },
  [HYPER_CURLY_BRACKET_LEFT]  = { .fn = { NULL, tap_finished, tap_reset }, .user_data = &left_hyper_state },
};

#define EscNav LT(NAV_LAYER, KC_ESC)
#define LCTL_BR CTL_T(KC_LBRC)
#define RCTL_BR CTL_T(KC_RBRC)
#define RHYP_BR TD(HYPER_CURLY_BRACKET_RIGHT)
#define LHYP_BR TD(HYPER_CURLY_BRACKET_LEFT)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* QWERTY - MIT ENHANCED / GRID COMPATIBLE
 * .-----------------------------------------------------------------------------------------------------------------------------------------.
 * | `         | 1      | 2      | 3      | 4      | 5      | 6      | 7      | 8      | 9      | 0      | BKSPC  | BKSPC  |        |        |
 * |-----------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-----------------|
 * | TAB       | Q      | W      | E      | R      | T      | Y      | U      | I      | O      | P      | ENT    | ENT    |        |        |
 * |-----------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-----------------+--------|
 * | ESC-F(Nav)| A      | S      | D      | F      | G      | H      | J      | K      | L      | ;      | '      |        |        |        |
 * |-----------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------------------------+--------|
 * | LSHIFT-(  | Z      | X      | C      | V      | B      | N      | M      | ,      | .      | /      |RSHIFT-)|        | UP     |        |
 * |-----------+--------+--------+--------+--------+- 2u ------------+--------+--------+--------+--------+-----------------+--------+--------|
 * | CTRL-'['  | LHYPER | LALT   | LGUI   | LOWER  | XXXXXX . SPACE  | RAISE  | RGUI   | RALT   | RCTRL  | FN     | LEFT   | DOWN   | RIGHT  |
 * '-----------------------------------------------------------------------------------------------------------------------------------------'
 */

 [QWERTY_LAYER] = { /* QWERTY */
  { KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,      KC_5,    KC_6,    KC_7,      KC_8,    KC_9,    KC_0,    KC_BSPC, KC_BSPC, _______, MO(_FN)  },
  { KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,      KC_T,    KC_Y,    KC_U,      KC_I,    KC_O,    KC_P,    KC_ENT , KC_ENT , _______, _______  },
  { EscNav,  KC_A,    KC_S,    KC_D,    KC_F,      KC_G,    KC_H,    KC_J,      KC_K,    KC_L,    KC_SCLN, KC_QUOT, _______, _______, _______  },
  { KC_LSPO, KC_Z,    KC_X,    KC_C,    KC_V,      KC_B,    KC_N,    KC_M,      KC_COMM, KC_DOT,  KC_SLSH, KC_RSPC, _______, KC_UP,   _______  },
  { LCTL_BR, LHYP_BR, KC_LALT, KC_LGUI, MO(LOWER), KC_SPC,  KC_SPC,  MO(RAISE), KC_RGUI, KC_RALT, RHYP_BR, RCTL_BR, KC_LEFT, KC_DOWN, KC_RGHT  },
 },

/* LOWERED
 * .-----------------------------------------------------------------------------------------------------------------------------------------.
 * |           | F1     | F2     | F3     | F4     | F5     | F6     | F7     | F8     | F9     | F10    | F11    | F12    |        |        |
 * |-----------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------------------------+--------|
 * |           | !      | @      | #      | $      | %      | ^      | &      | *      | (      | )      |        |        |        | INS    |
 * |-----------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------------------------+--------|
 * |           |        |        |        |        |        |        |        |        |        |        |        |        |        |        |
 * |-----------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------------------------+--------|
 * |           | -      | =      | `      | \      |        |        |        |        |        |        |        |        |        |        |
 * |-----------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------------------------+--------|
 * |           |        |        |        |        | XXXXXX .        |        |        |        |        |        |        |        |        |
 * |-----------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------------------------+--------|
 */

 [LOWER] = { /* LOWERED */
  { KC_GRV , KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, _______  },
  { _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_INS   },
  { _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______  },
  { _______, KC_MINS, KC_EQL , KC_GRV , KC_BSLS, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______  },
  { _______, _______, _______, _______, _______, KC_BSPC, KC_BSPC, _______, _______, _______, _______, _______, _______, _______, _______  },
 },

/* RAISED
 * .---------------------------------------------------------------------------------------------------------------------- 2u ------------.
 * |        | F1     | F2     | F3     | F4     | F5     | F6     | F7     | F8     | F9     | F10    | F11    | F12    | XXXXXX .        |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-----------------|
 * |        | 1      | 2      | 3      | 4      | 5      | 6      | 7      | 8      | 9      | 0      |        |        |        | INS    |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+- 2u ------------+--------|
 * |        | F1     | F2     | F3     | F4     | F5     | F6     | -      | =      | [      | ]      | \      | XXXXXX .        |        |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+- 2u ---------------------+--------|
 * |        | F7     | F8     | F9     | F10    | F11    | F12    |        |        |        |        | XXXXXX .        |        |        |
 * |--------+--------+--------+--------+--------+- 2u ------------+--------+--------+--------+--------+-----------------+--------+--------|
 * |        |        |        |        |        | XXXXXX .        |        |        |        |        |        |        |        |        |
 * '--------------------------------------------------------------------------------------------------------------------------------------'
 */

 [RAISE] = { /* RAISED */
  { _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  ___T___, ___T___  },
  { _______, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    _______, _______, _______, KC_INS   },
  { _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_MINS, KC_EQL,  KC_LBRC, KC_RBRC, KC_BSLS, ___T___, ___T___, _______  },
  { _______, KC_UNDS, KC_PLUS, KC_TILD, KC_PIPE, _______, _______, _______, _______, _______, _______, ___T___, ___T___, _______, _______  },
  { _______, _______, _______, _______, _______, KC_DEL , KC_DEL , _______, _______, _______, _______, _______, _______, _______, _______  },
 },

/* FUNCTION
 * .---------------------------------------------------------------------------------------------------------------------- 2u ------------.
 * | NUM LK | F1     | F2     | F3     | F4     | F5     | F6     | F7     | F8     | F9     | F10    | F11    | F12    | XXXXXX .        |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-----------------|
 * | SCR LK | F13    | F14    | F15    | F16    | F17    | F18    | F19    | F20    | F21    | F22    | F23    | F24    | PAUSE  | PR SCR |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+- 2u ------------+--------|
 * | CAP LK | MS BT5 | MS BT4 | MS BT3 | MS BT2 | SLOW M | FAST M | NEXT   | VOL+   | VOL-   | PLAY   |        | XXXXXX .        | WHEEL+ |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+- 2u ---------------------+--------|
 * | RGB TG | RGB MD | RGB HI | RGB HD | RGB SI | RGB SD | RGB VI | RGB VD | BL TOG | BL INC | BL DEC | XXXXXX .        | MOUS U | WHEEL- |
 * |--------+--------+--------+--------+--------+-- 2u -----------+--------+--------+--------+--------+-----------------+--------+--------|
 * | RESET  |        | QWERTY | COLEMK | DVORAK | XXXXXX . MS BT1 |        |        |        |        |        | MOUS L | MOUS D | MOUS R |
 * '--------------------------------------------------------------------------------------------------------------------------------------'
 */

 [_FN] = { /* FUNCTION */
  { KC_NLCK, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  ___T___, ___T___  },
  { KC_SLCK, KC_F13,  KC_F14,  KC_F15,  KC_F16,  KC_F17,  KC_F18,  KC_F19,  KC_F20,  KC_F21,  KC_F22,  KC_F23,  KC_F24,  KC_PAUS, KC_PSCR  },
  { KC_CAPS, KC_BTN5, KC_BTN4, KC_BTN3, KC_BTN2, KC_ACL0, KC_ACL2, KC_MNXT, KC_VOLD, KC_VOLU, KC_MPLY, _______, ___T___, ___T___, KC_WH_U  },
  { RGB_TOG, RGB_MOD, RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD, RGB_VAI, RGB_VAD, BL_TOGG, BL_INC,  BL_DEC,   ___T___, ___T___, KC_MS_U, KC_WH_D  },
  { RESET  , _______, DF(_QW), DF(_CM), DF(_DV), KC_BTN1, KC_BTN1, _______, _______, _______, _______, _______, KC_MS_L, KC_MS_D, KC_MS_R  },
 },

/* NAV LAYER
 * .--------------------------------------------------------------------------------------------------------------------------------------.
 * |        |        |        |        |        |        |        |        |        |        |        |        |        |        |        |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-----------------|
 * |        |        |        |        |        |        |        |        |        |        |        |        |        |        |        |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-----------------+--------|
 * |        |        | HOME   | PgUp   | PgDn   | END    | LEFT   | DOWN   | UP     | RIGHT  |        |        |        |        |        |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------------------------+--------|
 * |        |        |        |        |        |        |        |        |        |        |        |        |        |        |        |
 * |--------+--------+--------+--------+--------+-----------------+--------+--------+--------+--------+-----------------+--------+--------|
 * |        |        |        |        |        |        |        |        |        |        |        |        |        |        |        |
 * '--------------------------------------------------------------------------------------------------------------------------------------'
 */
 [NAV_LAYER] = {
  { KC_GRV , _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______ },
  { _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______ },
  { _______, _______, KC_HOME, KC_PGDN, KC_PGUP, KC_END , KC_LEFT, KC_DOWN, KC_UP  , KC_RGHT, _______, _______, _______, _______, _______ },
  { _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______ },
  { _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______ },
 },
};

#if 0
/* EMPTY
 * .--------------------------------------------------------------------------------------------------------------------------------------.
 * |        |        |        |        |        |        |        |        |        |        |        |        |        |        |        |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-----------------|
 * |        |        |        |        |        |        |        |        |        |        |        |        |        |        |        |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-----------------+--------|
 * |        |        |        |        |        |        |        |        |        |        |        |        |        |        |        |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------------------------+--------|
 * |        |        |        |        |        |        |        |        |        |        |        |        |        |        |        |
 * |--------+--------+--------+--------+--------+-----------------+--------+--------+--------+--------+-----------------+--------+--------|
 * |        |        |        |        |        |        |        |        |        |        |        |        |        |        |        |
 * '--------------------------------------------------------------------------------------------------------------------------------------'
 */
 [EMPTY] = {
  { _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______ },
  { _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______ },
  { _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______ },
  { _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______ },
  { _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______ },
 },
#endif

const uint16_t PROGMEM fn_actions[] = {

};

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
  // MACRODOWN only works in this function
      switch(id) {
        case 0:
          if (record->event.pressed) {
            register_code(KC_RSFT);
            #ifdef BACKLIGHT_ENABLE
              backlight_step();
            #endif
          } else {
            unregister_code(KC_RSFT);
          }
        break;
      }
    return MACRO_NONE;
};
