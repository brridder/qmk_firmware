#pragma once

#include "quantum.h"

enum current_tap_state_t {
    SINGLE_TAP = 1,
    SINGLE_HOLD,
    DOUBLE_SINGLE_TAP,
    INVALID_TAP_STATE,
};


typedef struct {
    enum current_tap_state_t state;
    uint32_t single_tap_keycode;
    uint32_t single_tap_mod;
    uint32_t single_hold_keycodes[3];
} tap_user_data_t;

void tap_finished(qk_tap_dance_state_t *state, void *user_data);
void tap_reset(qk_tap_dance_state_t *state, void *user_data);

#define SINGLE_HOLD_TAP(in_user_data) { \
    .fn = { NULL, tap_finished, tap_reset }, \
    .user_data = in_user_data, \
}
