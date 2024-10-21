#include QMK_KEYBOARD_H
#if __has_include("keymap.h")
#    include "keymap.h"
#endif

// debugging
// #include "print.h"

// Tap dance
typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP,
    TD_DOUBLE_HOLD,
    TD_DOUBLE_SINGLE_TAP, // useful to send two single taps
    TD_TRIPLE_TAP,
    TD_TRIPLE_HOLD
} td_state_t;

typedef struct {
    bool is_press_action;
    td_state_t state;
} td_tap_t;

typedef struct {
    bool is_oneshot_activated;
} td_user_t;

enum tap_dance_keycodes {
    DANCE1,
    DANCE2,
    DANCE3
};

td_state_t cur_dance(tap_dance_state_t *state);
void dance1_finished(tap_dance_state_t *state, void *user_data);
void dance1_reset(tap_dance_state_t *state, void *user_data);
void dance2_finished(tap_dance_state_t *state, void *user_data);
void dance2_reset(tap_dance_state_t *state, void *user_data);
void dance3_finished(tap_dance_state_t *state, void *user_data);
void dance3_reset(tap_dance_state_t *state, void *user_data);

// Layouts
enum layer_names {
    _QWERTY,
    _GAME,
    _SYMBOL,
    _ADJUST
};

// shorter keycodes 
#define LEFTALT     LALT_T(KC_LBRC)
#define RIGHTALT    RALT_T(KC_RBRC)
#define TD_1        TD(DANCE1)
#define TD_2        TD(DANCE2)
#define TD_3        TD(DANCE3)
#define MO_SYM      MO(_SYMBOL)

enum key_group {
    _NUMPAD,
    _FUNCTION,
    _WASD,
    _BACKLIGHT,
    _WARNING,
    _SETTINGS,
};

struct led_lights {
    enum layer_names layer;
    enum key_group group;
    HSV hsv;
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTY] = LAYOUT(
    // ┌────────┬────────┬────────┬────────┬────────┬────────┐                       ┌────────┬────────┬────────┬────────┬────────┬────────┐
         QK_GESC,  KC_1  ,  KC_2  ,  KC_3  ,  KC_4  ,  KC_5  ,                          KC_6  ,  KC_7  ,  KC_8  ,  KC_9  ,  KC_0  , KC_BSPC,
    // ├────────┼────────┼────────┼────────┼────────┼────────┤                       ├────────┼────────┼────────┼────────┼────────┼────────┤
         KC_TAB ,  KC_Q  ,  KC_W  ,  KC_E  ,  KC_R  ,  KC_T  ,                          KC_Y  ,  KC_U  ,  KC_I  ,  KC_O  ,  KC_P  , KC_ENT ,
    // ├────────┼────────┼────────┼────────┼────────┼────────┤                       ├────────┼────────┼────────┼────────┼────────┼────────┤
         SC_LSPO,  KC_A  ,  KC_S  ,  KC_D  ,  KC_F  ,  KC_G  ,                          KC_H  ,  KC_J  ,  KC_K  ,  KC_L  , KC_SCLN, SC_RSPC,
    // ├────────┼────────┼────────┼────────┼────────┼────────┼────────┐     ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
         KC_LCTL,  KC_Z  ,  KC_X  ,  KC_C  ,  KC_V  ,  KC_B  , LEFTALT,      RIGHTALT,  KC_N  ,  KC_M  , KC_COMM, KC_DOT , KC_SLSH, KC_QUOT,
    // └────────┴────────┴────────┴────┬───┴────┬───┴────┬───┴────┬───┘     └────┬───┴────┬───┴────┬───┴────┬───┴────────┴────────┴────────┘
                                         KC_LGUI, MO_SYM , KC_SPC ,               KC_SPC,   TD_1  , KC_ENT
                                    // └────────┴────────┴────────┘              └────────┴────────┴────────┘
    ),
    [_GAME] = LAYOUT(
    // ┌────────┬────────┬────────┬────────┬────────┬────────┐                       ┌────────┬────────┬────────┬────────┬────────┬────────┐
         KC_ESC , KC_1   , KC_2   , KC_3   , KC_4   , KC_5   ,                         KC_6   , KC_7   , KC_8   , KC_9   , KC_0   , KC_BSPC,
    // ├────────┼────────┼────────┼────────┼────────┼────────┤                       ├────────┼────────┼────────┼────────┼────────┼────────┤
         KC_TAB , KC_Q   , KC_W   , KC_E   , KC_R   , KC_T   ,                         KC_Y   , KC_U   , KC_I   , KC_O   , KC_P   , KC_ENT ,
    // ├────────┼────────┼────────┼────────┼────────┼────────┤                       ├────────┼────────┼────────┼────────┼────────┼────────┤
         KC_LSFT, KC_A   , KC_S   , KC_D   , KC_F   , KC_G   ,                         KC_H   , KC_J   , KC_K   , KC_L   , KC_NO  , KC_NO  , 
    // ├────────┼────────┼────────┼────────┼────────┼────────┼────────┐     ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
         KC_LCTL, KC_Z   , KC_X   , KC_C   , KC_V   , KC_B   , KC_LALT,       KC_NO  , KC_N   , KC_M   , KC_NO  , KC_NO  , KC_NO  , KC_NO  ,
    // └────────┴────────┴────────┴────┬───┴────┬───┴────┬───┴────┬───┘     └────┬───┴────┬───┴────┬───┴────┬───┴────────┴────────┴────────┘
                                         KC_LGUI, KC_SPC , KC_F1  ,                KC_NO  , TD_1   , KC_NO
                                    // └────────┴────────┴────────┘              └────────┴────────┴────────┘
    ),
    [_SYMBOL] = LAYOUT(
    // ┌────────┬────────┬────────┬────────┬────────┬────────┐                       ┌────────┬────────┬────────┬────────┬────────┬────────┐
         KC_F12 , KC_F1  , KC_F2  , KC_F3  , KC_F4  , KC_F5  ,                         KC_F6  , KC_F7  , KC_F8  , KC_F9  , KC_F1  , KC_F11 ,
    // ├────────┼────────┼────────┼────────┼────────┼────────┤                       ├────────┼────────┼────────┼────────┼────────┼────────┤
         KC_TRNS, KC_TRNS, KC_UP  , KC_TRNS, KC_LPRN, KC_RPRN,                         KC_AMPR, KC_P1  , KC_P2  , KC_P3  , KC_EQL , KC_TRNS,
    // ├────────┼────────┼────────┼────────┼────────┼────────┤                       ├────────┼────────┼────────┼────────┼────────┼────────┤
         KC_TRNS, KC_LEFT, KC_DOWN, KC_RGHT, KC_LCBR, KC_RCBR,                         KC_PIPE, KC_P4  , KC_P5  , KC_P6  , KC_MINS, KC_TRNS,
    // ├────────┼────────┼────────┼────────┼────────┼────────┼────────┐     ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
         KC_CAPS, KC_TRNS, KC_TRNS, KC_TRNS, KC_LBRC, KC_RBRC, KC_TRNS,       KC_TRNS, KC_EXLM, KC_P7  , KC_P8  , KC_P9  , KC_BSLS, KC_TRNS,
    // └────────┴────────┴────────┴────┬───┴────┬───┴────┬───┴────┬───┘     └────┬───┴────┬───┴────┬───┴────┬───┴────────┴────────┴────────┘
                                         KC_TRNS, KC_TRNS, KC_TRNS,                KC_TRNS, KC_TRNS, KC_P0
                                    // └────────┴────────┴────────┘              └────────┴────────┴────────┘
    ),
    [_ADJUST] = LAYOUT(
    // ┌────────┬────────┬────────┬────────┬────────┬────────┐                       ┌────────┬────────┬────────┬────────┬────────┬────────┐
         KC_NO  , KC_NO  , KC_NO  , TD_2   , TD_3   , NK_TOGG,                         NK_TOGG, TD_3  ,  TD_2   , KC_NO  , KC_NO  , KC_NO  ,
    // ├────────┼────────┼────────┼────────┼────────┼────────┤                       ├────────┼────────┼────────┼────────┼────────┼────────┤
         KC_NO  , KC_NO  , KC_NO  , KC_NO  , KC_NO  , KC_NO  ,                         KC_NO  , KC_NO  , KC_NO  , KC_NO  , KC_NO  , KC_NO  ,
    // ├────────┼────────┼────────┼────────┼────────┼────────┤                       ├────────┼────────┼────────┼────────┼────────┼────────┤
         KC_NO  , RGB_SPD, RGB_SPI, RGB_VAD, RGB_VAI, RGB_TOG,                         RGB_TOG, RGB_VAI, RGB_VAD, RGB_SPI, RGB_SPD, KC_NO  ,
    // ├────────┼────────┼────────┼────────┼────────┼────────┼────────┐     ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
         KC_NO  , KC_NO  , KC_NO  , KC_NO  , KC_NO  , KC_NO  , KC_NO  ,       KC_NO  , KC_NO  , KC_NO  , KC_NO  , KC_NO  , KC_NO  , KC_NO  ,
    // └────────┴────────┴────────┴────┬───┴────┬───┴────┬───┴────┬───┘     └────┬───┴────┬───┴────┬───┴────┬───┴────────┴────────┴────────┘
                                         KC_NO  , TD_1   , KC_NO  ,                KC_NO  ,  TD_1  , KC_NO
                                    // └────────┴────────┴────────┘              └────────┴────────┴────────┘
    )

    /*
    [_EMPTY] = LAYOUT(
    // ┌────────┬────────┬────────┬────────┬────────┬────────┐                       ┌────────┬────────┬────────┬────────┬────────┬────────┐
                ,        ,        ,        ,        ,        ,                                ,        ,        ,        ,        ,        ,
    // ├────────┼────────┼────────┼────────┼────────┼────────┤                       ├────────┼────────┼────────┼────────┼────────┼────────┤
                ,        ,        ,        ,        ,        ,                                ,        ,        ,        ,        ,        ,
    // ├────────┼────────┼────────┼────────┼────────┼────────┤                       ├────────┼────────┼────────┼────────┼────────┼────────┤
                ,        ,        ,        ,        ,        ,                                ,        ,        ,        ,        ,        ,
    // ├────────┼────────┼────────┼────────┼────────┼────────┼────────┐     ┌────────┼────────┼────────┼────────┼────────┼────────┼────────┤
                ,        ,        ,        ,        ,        ,        ,              ,        ,        ,        ,        ,        ,        ,
    // └────────┴────────┴────────┴────┬───┴────┬───┴────┬───┴────┬───┘     └────┬───┴────┬───┴────┬───┴────┬───┴────────┴────────┴────────┘
                                                ,        ,        ,                       ,        ,
                                    // └────────┴────────┴────────┘              └────────┴────────┴────────┘
    ),
    */
};

/* RGB SECTION */

const uint8_t leds_numpad[] = {44, 45, 46, 51, 52, 53, 56, 57,59, 63};
const uint8_t leds_function[] = {0, 2, 3, 5, 6, 8, 34, 36, 37, 39, 40, 42};
const uint8_t leds_wasd[] = {12, 16, 17, 18};
const uint8_t leds_backlight[] = {1, 4, 7, 24, 27, 31, 35, 38, 41, 58, 61, 65};
const uint8_t leds_warning[] = {5, 6, 39, 40};
const uint8_t leds_settings[] = {8, 16, 17, 18, 19, 20, 42, 50, 51, 52, 53, 54};
const uint8_t led_caps = 28;

// when using WS2812 driver, hsv is important because it allows the brightness to be limited
const struct led_lights led_configs[] = {
    { _SYMBOL,  _NUMPAD,    {0,   0, 255}       },
    { _SYMBOL,  _FUNCTION,  {0,   0, 255}       },
    { _SYMBOL,  _WASD,      {0,   0, 255}       },
    { _GAME,    _WASD,      {0,   0, 255}       },
    { _ADJUST,  _WARNING,   {HSV_RED}           },
    { _ADJUST,  _SETTINGS,  {HSV_CHARTREUSE}    }
};

/* MY FUNCTIONS */

// limit hsv brightness based on current setting
void hsv_set_brightness(HSV *hsv) {
    if (hsv->v > rgb_matrix_get_val())
        hsv->v = rgb_matrix_get_val();
}

// set jsv
RGB hsv_to_rgb_custom(HSV hsv) {
    hsv_set_brightness(&hsv);
    return hsv_to_rgb((HSV){hsv.h, hsv.s, hsv.v});
}

void set_rgb_defaults(void) {
    (void)rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_REACTIVE_SIMPLE);
}

// sets color for intersection of led group and current led batch from rgb_matrix_indicators
// leds are updated in batches, need led_min and led_max in scope for this function
void set_color_for_group(const uint8_t *leds, size_t leds_size, uint8_t led_min, uint8_t led_max, RGB *rgb) {
    for (uint8_t i = 0; i < leds_size; i++) {
        RGB_MATRIX_INDICATOR_SET_COLOR(leds[i], rgb->r, rgb->g, rgb->b);
    }
}

/* QMK SYSTEM FUNCTIONS */
// override rgb effects for individual keys
bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {

    uint8_t layer = get_highest_layer(layer_state);
    size_t confArrSize = sizeof led_configs / sizeof led_configs[0];
    
    // debugging
    // dprintf("led_min: %u, led_max: %u\n", led_min, led_max);

    for (uint8_t i = 0; i < confArrSize; i++) {
        if (led_configs[i].layer == layer) {
            RGB rgb = hsv_to_rgb_custom(led_configs[i].hsv);
            size_t size;

            switch(led_configs[i].group) {
                case _NUMPAD:   // can use either arr or &arr[0]
                    size = sizeof leds_numpad / sizeof leds_numpad[0];
                    (void)set_color_for_group(leds_numpad, size, led_min, led_max, &rgb);
                    break;
                case _FUNCTION:
                    size = sizeof leds_function / sizeof leds_function[0] ;
                    (void)set_color_for_group(leds_function, size, led_min, led_max, &rgb);
                    break;
                case _WASD:
                    size = sizeof(leds_wasd) / sizeof leds_wasd[0];
                    (void)set_color_for_group(leds_wasd, size, led_min, led_max, &rgb);
                    break;
                case _WARNING:
                    size = sizeof leds_warning / sizeof leds_warning[0];
                    (void)set_color_for_group(leds_warning, size, led_min, led_max, &rgb);
                    break;
                case _SETTINGS:
                    size = sizeof leds_settings / sizeof leds_settings[0];
                    (void)set_color_for_group(leds_settings, size, led_min, led_max, &rgb);
                    break;
                default:
                    break;
            }
        }
    }

    if (layer != _ADJUST) {
        if (host_keyboard_led_state().caps_lock) {
            rgb_matrix_set_color(led_caps, RGB_RED); // caps lock LED, on for all regular layers
        }
    }

    return false;
}

// set initial rgb here
void keyboard_post_init_user(void) {
    if(!rgb_matrix_is_enabled()) {
        (void)rgb_matrix_enable_noeeprom();
    }
    (void)set_rgb_defaults();

    // debugging
    // debug_enable = true;
    // debug_matrix = true;

    // debugging, probably don't need these
    // debug_keyboard=true;
    // debug_mouse=true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    // layer indicator on all keys
    switch(get_highest_layer(state)) {
        case _GAME:
            (void)rgb_matrix_mode_noeeprom(RGB_MATRIX_BREATHING);
            HSV p = {HSV_PURPLE};
            hsv_set_brightness(&p);
            (void)rgb_matrix_sethsv_noeeprom(p.h, p.s, p.v);
            break;
        case _ADJUST:
            (void)rgb_matrix_mode_noeeprom(RGB_MATRIX_BREATHING);
            HSV a = {HSV_AZURE};
            hsv_set_brightness(&a);
            (void)rgb_matrix_sethsv_noeeprom(a.h, a.s, a.v);
            break;
        case _QWERTY:
        case _SYMBOL:
        default:
            (void)set_rgb_defaults();
            break;
    }
    return state;
}


/* TAP DANCE SECTION */

td_state_t cur_dance(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return TD_SINGLE_TAP;
        // Key has not been interrupted, but the key is still held. Means you want to send a 'HOLD'.
        else return TD_SINGLE_HOLD;
    } else if (state->count == 2) {
        // TD_DOUBLE_SINGLE_TAP is to distinguish between typing "pepper", and actually wanting a double tap
        // action when hitting 'pp'. Suggested use case for this return value is when you want to send two
        // keystrokes of the key, and not the 'double tap' action/macro.
        // if (state->interrupted) return TD_DOUBLE_SINGLE_TAP;
        if (state->pressed) return TD_DOUBLE_HOLD;
        else return TD_DOUBLE_TAP;
    }

    // Assumes no one is trying to type the same letter three times (at least not quickly).
    // If your tap dance key is 'KC_W', and you want to type "www." quickly - then you will need to add
    // an exception here to return a 'TD_TRIPLE_SINGLE_TAP', and define that enum just like 'TD_DOUBLE_SINGLE_TAP'
    if (state->count == 3) {
        if (state->interrupted || !state->pressed) return TD_TRIPLE_TAP;
        else return TD_TRIPLE_HOLD;
    } else return TD_UNKNOWN;
}

// Create an instance of 'td_tap_t' for each tap dance
static td_tap_t dance1_tap_state = { .is_press_action = true, .state = TD_NONE };
static td_tap_t dance2_tap_state = { .is_press_action = true, .state = TD_NONE };
static td_tap_t dance3_tap_state = { .is_press_action = true, .state = TD_NONE };

// not using *user_data since we want to pass info between separate tap dance instances
static td_user_t dance1_user_data = { .is_oneshot_activated = false };

// DANCE1 is for combining a oneshot layer key and toggling to other layers
void dance1_finished(tap_dance_state_t *state, void *user_data) {
    dance1_tap_state.state = cur_dance(state);
    uint8_t layer = get_highest_layer(layer_state|default_layer_state);

    switch (dance1_tap_state.state) {
        case TD_SINGLE_TAP:
            // one shot layer to _SYMBOL layer
            if (!dance1_user_data.is_oneshot_activated && (layer == _QWERTY || layer == _GAME)) {
                set_oneshot_layer(_SYMBOL, ONESHOT_START);
                dance1_user_data.is_oneshot_activated = true;
            } else {
                // allows you to tap the one shot layer key again to turn it off
                reset_oneshot_layer();
                dance1_user_data.is_oneshot_activated = false;
            }
            break;
        case TD_SINGLE_HOLD:    layer_move(_QWERTY);    break;
        case TD_DOUBLE_HOLD:                            break; // do nothing
        case TD_DOUBLE_TAP:     layer_move(_GAME);      break;
        case TD_TRIPLE_TAP:     layer_move(_ADJUST);    break;
        // Last case is for fast typing. Assuming your key is `f`:
        // For example, when typing the word `buffer`, and you want to make sure that you send `ff` and not `Esc`.
        // In order to type `ff` when typing fast, the next character will have to be hit within the `TAPPING_TERM`, which by default is 200ms.
        // case TD_DOUBLE_SINGLE_TAP: tap_code(KC_Z); register_code(KC_Z); break; // nothing
        default: break;
    }
}

void dance1_reset(tap_dance_state_t *state, void *user_data) {
    switch (dance1_tap_state.state) {
        case TD_SINGLE_TAP: 
            if (dance1_user_data.is_oneshot_activated)
                clear_oneshot_layer_state(ONESHOT_PRESSED);
            break;
        case TD_SINGLE_HOLD:    break; // do nothing
        case TD_DOUBLE_HOLD:    break; // do nothing
        case TD_DOUBLE_TAP:     break; // do nothing
        case TD_TRIPLE_TAP:     break; // do nothing
        default: break;
    }
    dance1_tap_state.state = TD_NONE;
}

// DANCE2 is for ensuring intentional keypresses of power/wake/sleep
void dance2_finished(tap_dance_state_t *state, void *user_data) {
    dance2_tap_state.state = cur_dance(state);
    switch (dance2_tap_state.state) {
        case TD_SINGLE_TAP:     register_code(KC_SYSTEM_WAKE);  break;  // wake
        case TD_DOUBLE_TAP:     register_code(KC_SYSTEM_SLEEP); break;  // sleep
        default: break;
    }
}

void dance2_reset(tap_dance_state_t *state, void *user_data) {
    switch (dance2_tap_state.state) {
        case TD_SINGLE_TAP:     unregister_code(KC_SYSTEM_WAKE);  break;
        case TD_DOUBLE_TAP:     unregister_code(KC_SYSTEM_SLEEP); break;
        default: break;
    }
    dance2_tap_state.state = TD_NONE;
}

// DANCE3 is for ensuring intentional keypresses of keyboard reset/eeprom resert
void dance3_finished(tap_dance_state_t *state, void *user_data) {
    dance3_tap_state.state = cur_dance(state);
    switch (dance3_tap_state.state) {
        case TD_SINGLE_TAP: soft_reset_keyboard();  break;  // keyboard restart
        case TD_DOUBLE_TAP: reset_keyboard();       break;  // keyboard reset
        case TD_TRIPLE_TAP: eeconfig_init();        break;  // eeprom clear
        default: break;
    }
}

void dance3_reset(tap_dance_state_t *state, void *user_data) {
    switch (dance3_tap_state.state) {
        case TD_SINGLE_TAP: break;  // do nothing
        case TD_DOUBLE_TAP: break;  // do nothing
        case TD_TRIPLE_TAP: break;  // do nothing
        default: break;
    }
    dance3_tap_state.state = TD_NONE;
}

// Tap dance definitions, to access use the TD(index) macro in layouts
tap_dance_action_t tap_dance_actions[] = {
    [DANCE1] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, dance1_finished, dance1_reset),
    [DANCE2] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, dance2_finished, dance2_reset),
    [DANCE3] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, dance3_finished, dance3_reset)
};
