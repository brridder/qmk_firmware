#include "ridder.h"

static int cur_tap_dance_state(qk_tap_dance_state_t *state) {
    if (state->count == 1) {
        return (state->interrupted || state->pressed == 0) ? SINGLE_TAP : SINGLE_HOLD;
    } else if (state->count == 2) {
        return (state->interrupted) ? DOUBLE_SINGLE_TAP : INVALID_TAP_STATE;
    }
    return INVALID_TAP_STATE;
}

void tap_finished(qk_tap_dance_state_t *state, void *user_data)
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
            for (int i = 0; i < sizeof(tap_state->single_hold_keycodes)/sizeof(tap_state->single_hold_keycodes[0]); i++) {
                register_code(tap_state->single_hold_keycodes[i]);
            }
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

void tap_reset(qk_tap_dance_state_t *state, void *user_data)
{
    tap_user_data_t *tap_state = (tap_user_data_t *)user_data;

    switch (tap_state->state) {
        case SINGLE_TAP:
            unregister_code(tap_state->single_tap_keycode);
            break;
        case INVALID_TAP_STATE:
            break;
        case SINGLE_HOLD:
            for (int i = 0; i < sizeof(tap_state->single_hold_keycodes)/sizeof(tap_state->single_hold_keycodes[0]); i++) {
                unregister_code(tap_state->single_hold_keycodes[i]);
            }
            break;
        case DOUBLE_SINGLE_TAP:
            unregister_code(tap_state->single_tap_keycode);
            break;
        default:
            xprintf("LOL WUT\n");
            break;
    }
}

