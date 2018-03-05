// This is the Dvorak-friendly layout for the Mitosis by CarVac (/u/CarVac)
// It features space on the left thumb, shift on the right thumb, a
//  number layer with all the numbers on the home row, and a function layer
//  that provides mouse keys among other things.

#include "mitosis.h"
#include "ridder.h"

enum mitosis_layers
{
    _STD,
    _VGC,
    _LWR, // Lower
    _UPR, // Upper
    _FN
};


//Mousekeys
#define MOUSEKEY_DELAY 300
#define MOUSEKEY_INTERNAL 50
#define MOUSEKEY_MAX_SPEED 20
#define MOUSEKEY_TIME_TO_MAX 30
#define MOUSEKEY_WHEEL_MAX_SPEED 8
#define MOUSEKEY_WHEEL_TIME_TO_MAX 40

// Fillers to make layering more clear
#define _______ KC_TRNS
#define XXXXXXX KC_NO

enum tap_dance_t {
    HYPER_CURLY_BRACKET_RIGHT = 0,
    HYPER_CURLY_BRACKET_LEFT  = 1,
};

#define EscNav LT(NAV_LAYER, KC_ESC)
#define LCTL_BR CTL_T(KC_LBRC)
#define RCTL_BR CTL_T(KC_RBRC)
#define RHYP_BR TD(HYPER_CURLY_BRACKET_RIGHT)
#define LHYP_BR TD(HYPER_CURLY_BRACKET_LEFT)

static tap_user_data_t left_hyper_state = {
    .state = 0,
    .single_tap_keycode = KC_LCBR,
    .single_tap_mod = KC_LSFT,
    .single_hold_keycodes = { KC_LALT, KC_LCTL, KC_LGUI },
};

static tap_user_data_t right_hyper_state = {
    .state = 0,
    .single_tap_keycode = KC_RCBR,
    .single_tap_mod = KC_RSFT,
    .single_hold_keycodes = { KC_RALT, KC_RCTL, KC_RGUI },
};

qk_tap_dance_action_t tap_dance_actions[] = {
  [HYPER_CURLY_BRACKET_RIGHT] = SINGLE_HOLD_TAP(&right_hyper_state),
  [HYPER_CURLY_BRACKET_LEFT]  = SINGLE_HOLD_TAP(&left_hyper_state),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* QWERTY
 * .--------------------------------------------..--------------------------------------------.
 * | Q      | W      | E      | R      | T      || Y      | U      | I      | O      | P      |
 * |--------+--------+--------+--------+--------||--------+--------+--------+--------+--------|
 * | A      | S      | D      | F      | G      || J      | H      | K      | L      | ;      |
 * |--------+--------+--------+--------+--------||--------+--------+--------+--------+--------|
 * | Z      | X      | C      | V      | B      || N      | M      | ,      | .      | /      |
 * '--------+--------+--------+--------+--------||--------+--------+--------+--------+--------'
 *          | Shft-( | FN     |        |        ||        |        | '      | Shft-) |
 *          |--------+--------+--------+--------||--------+--------+--------+--------|
 *          | CTRL-[ | META-{ | LOWER  | SPACE  || ENTER  | UPPER  | META-} | CTRL-] |
 *          '-----------------------------------''-----------------------------------'
 */

[_STD] = { /* Standard; as compatible with dvorak and qwerty as possible */
  {KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,           KC_Y,    KC_U,    KC_I,    KC_O,    KC_P    },
  {KC_A,    KC_S,    KC_D,    KC_F,    KC_G,           KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN },
  {KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,           KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH },
  {XXXXXXX, KC_LSPO, MO(_FN), _______, _______,        _______, _______, KC_QUOT, KC_RSPC, XXXXXXX },
  {XXXXXXX, LCTL_BR, LHYP_BR, MO(_LWR), KC_SPC,        KC_ENT , MO(_UPR),RHYP_BR, RCTL_BR, XXXXXXX }
},

/* Video Games
 * .--------------------------------------------..--------------------------------------------.
 * | TAB    | Q      | W      | E      | R      || Y      | U      | I      | O      | P      |
 * |--------+--------+--------+--------+--------||--------+--------+--------+--------+--------|
 * | ESC    | A      | S      | D      | F      || J      | H      | K      | L      | ;      |
 * |--------+--------+--------+--------+--------||--------+--------+--------+--------+--------|
 * | SHIFT  | Z      | X      | C      | V      || N      | M      | ,      | .      | /      |
 * '--------+--------+--------+--------+--------||--------+--------+--------+--------+--------'
 *          | PGUP   | TAB    | LCTRL  | SPACE  || LSHIFT | ENTER  | UP     | PSCR   |
 *          |--------+--------+--------+--------||--------+--------+--------+--------|
 *          | LCTL   | LGUI   | LALT   | FN     || NUM    | LEFT   | DOWN   | RIGHT  |
 *          '-----------------------------------''-----------------------------------'
 */
// Video game config
[_VGC] = {
  {KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,           KC_Y,    KC_U,    KC_I,    KC_O,    KC_P    },
  {KC_ESC,  KC_A,    KC_S,    KC_D,    KC_F,           KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN },
  {KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,           KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH },
  {XXXXXXX, KC_PGUP, MO(_FN), KC_LCTL, MO(_LWR),       KC_LSFT, KC_ENT,  KC_UP,   KC_PSCR, XXXXXXX },
  {XXXXXXX, KC_LCTL, KC_LALT, KC_LALT, KC_SPC,         MO(_UPR), KC_LEFT, KC_DOWN, KC_RGHT, XXXXXXX }
},

/* Number layout, for data entry and programming purposes (Dvorak result in parens)
 * .--------------------------------------------..--------------------------------------------.
 * | 1      | 2      | 3      | 4      | 5      || 6      | 7      | 8      | 9      | 0      |
 * |--------+--------+--------+--------+--------||--------+--------+--------+--------+--------|
 * | F1     | F2     | F3     | F4     | F5     || F6     | F7     | F8     | F9     | F10    |
 * |--------+--------+--------+--------+--------||--------+--------+--------+--------+--------|
 * | -      | =      | `      | \      |        ||        |        |        |        |        |
 * '--------+--------+--------+--------+--------||--------+--------+--------+--------+--------'
 *          |        |        |        |        ||        |        |        |        |
 *          |--------+--------+--------+--------||--------+--------+--------+--------|
 *          |        |        |        |        ||        |        |        |        |
 *          '-----------------------------------''-----------------------------------'
 */

[_LWR] = { /* LOWER */
  {KC_1,    KC_2,    KC_3,    KC_4,    KC_5,          KC_6,    KC_7,    KC_8,    KC_9,    KC_0    },
  {KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,         KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10  },
  {KC_MINS, KC_EQL , KC_GRV , KC_BSLS, _______,       _______, _______, _______, _______, _______ },
  {XXXXXXX, _______, _______, _______, _______,       _______, _______, _______, _______, XXXXXXX },
  {XXXXXXX, _______, _______, _______, _______,       _______, _______, _______, _______, XXXXXXX }
},

/* Number layout, for data entry and programming purposes (Dvorak result in parens)
 * .--------------------------------------------..--------------------------------------------.
 * | !      | @      | #      | $      | %      || ^      | &      | *      | '      | "      |
 * |--------+--------+--------+--------+--------||--------+--------+--------+--------+--------|
 * |        |        |        |        |        || Left   | Down   | Up     | Right  |        |
 * |--------+--------+--------+--------+--------||--------+--------+--------+--------+--------|
 * | _      | +      | ~      | |      |        ||        |        |        |        |        |
 * '--------+--------+--------+--------+--------||--------+--------+--------+--------+--------'
 *          |        |        |        |        ||        |        |        |        |
 *          |--------+--------+--------+--------||--------+--------+--------+--------|
 *          |        |        |        |        ||        |        |        |        |
 *          '-----------------------------------''-----------------------------------'
 */

[_UPR] = {  /* Upper */
  {S(KC_1), S(KC_2), S(KC_3), S(KC_4), S(KC_5),       S(KC_6), S(KC_7), S(KC_8), KC_QUOT, S(KC_QUOT)},
  {_______, _______, _______, _______, _______,       KC_LEFT, KC_DOWN, KC_UP  , KC_RGHT, _______ },
  {S(KC_MINS),S(KC_EQL),S(KC_GRV),S(KC_BSLS),_______, _______, _______, _______, _______, _______ },
  {XXXXXXX, _______, _______, _______, _______,       _______, _______, _______, _______, XXXXXXX },
  {XXXXXXX, _______, _______, _______, _______,       _______, _______, _______, _______, XXXXXXX }
},


/* Fn layout, for typing purposes (Dvorak result in parens)
 * .--------------------------------------------..--------------------------------------------.
 * | `      |        | MS_U   |        |        || WH_U   | WH_L   | BTN3   | WH_R   | [ (/?) |
 * |--------+--------+--------+--------+--------||--------+--------+--------+--------+--------|
 * | ESC    | MS_L   | MS_D   | MS_R   |        || WH_D   | BTN1   | BTN2   |        | ' (-_) |
 * |--------+--------+--------+--------+--------||--------+--------+--------+--------+--------|
 * | APP    | MPRV   | MPLY   | MSTP   | MNXT   ||        | BSPC   | DEL    | INS    | \      |
 * '--------+--------+--------+--------+--------||--------+--------+--------+--------+--------'
 *          | VOLU   |        |        |        ||        |        | PGUP   |        |
 *          |--------+--------+--------+--------||--------+--------+--------+--------|
 *          | VOLD   |        |        |        ||        | HOME   | PGDN   | END    |
 *          '-----------------------------------''-----------------------------------'
 */

[_FN] = { /* Function Layer, primary alternative layer featuring numpad on right hand,
                                   cursor keys on left hand, and all symbols*/
  {KC_GRV,  _______, KC_MS_U, _______, TG(_VGC),      KC_WH_U, KC_WH_L, KC_BTN3, KC_WH_R, KC_LBRC },
  {KC_ESC,  KC_MS_L, KC_MS_D, KC_MS_R, _______,       KC_WH_D, KC_BTN1, KC_BTN2, _______, KC_QUOT },
  {KC_APP,  KC_MPRV, KC_MPLY, KC_MSTP, KC_MNXT,       _______, KC_BSPC, KC_DEL,  KC_INS,  KC_BSLS },
  {XXXXXXX, KC_VOLU, _______, _______, _______,       _______, _______, KC_PGUP, _______, XXXXXXX },
  {XXXXXXX, KC_VOLD, _______, _______, _______,       _______, KC_HOME, KC_PGDN, KC_END,  XXXXXXX }
},

/* blank key layout template
 * .--------------------------------------------..--------------------------------------------.
 * |        |        |        |        |        ||        |        |        |        |        |
 * |--------+--------+--------+--------+--------||--------+--------+--------+--------+--------|
 * |        |        |        |        |        ||        |        |        |        |        |
 * |--------+--------+--------+--------+--------||--------+--------+--------+--------+--------|
 * |        |        |        |        |        ||        |        |        |        |        |
 * '--------+--------+--------+--------+--------||--------+--------+--------+--------+--------'
 *          |        |        |        |        ||        |        |        |        |
 *          |--------+--------+--------+--------||--------+--------+--------+--------|
 *          |        |        |        |        ||        |        |        |        |
 *          '-----------------------------------''-----------------------------------'
 */

};

void matrix_scan_user(void) {
    uint8_t layer = biton32(layer_state);

    switch (layer) {
        //case _STD:
            //set_led_off;
            //break;
        case _FN:
            set_led_blue;
            break;
        case _UPR:
            set_led_red;
            break;
        default:
            break;
    }
};

