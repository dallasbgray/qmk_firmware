
// declare custom effects
RGB_MATRIX_EFFECT(SOLID_REACTIVE_SIMPLE_RAINDROPS) // RGB_MATRIX_CUSTOM_SOLID_REACTIVE_SIMPLE_RAINDROPS

// define effects in this block
#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

typedef struct {
    uint8_t idx;
    bool init;
    uint16_t time_offset;
    HSV hsv;
} led_state_t;

#define MAX_LED_INDEX 68
static const uint8_t leds_backlight[] = {1, 4, 7, 24, 27, 31, 35, 38, 41, 58, 61, 65};
static led_state_t led_state[MAX_LED_INDEX];

static void set_hsv_custom(uint8_t i, HSV hsv) {
    RGB rgb = rgb_matrix_hsv_to_rgb(hsv);
    rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
}

static HSV raindrops_get_color_mine(uint16_t time, led_state_t* led, effect_params_t* params) {
    HSV hsv = {led->hsv.h, rgb_matrix_config.hsv.s, rgb_matrix_config.hsv.v};
    hsv.v = scale8(abs8(sin8(time - led->time_offset) - 128) * 2, hsv.v);

    // if light is off or being initialized, change the color once
    if (hsv.v == 0 || led->init) {
        // Take the shortest path between hues
        int16_t deltaH = ((rgb_matrix_config.hsv.h + 180) % 360 - rgb_matrix_config.hsv.h) / 4;
        if (deltaH > 127) {
            deltaH -= 256;
        } else if (deltaH < -127) {
            deltaH += 256;
        }
        hsv.h   = rgb_matrix_config.hsv.h + (deltaH * (random8() & 0x03));
    }
    return hsv;
}
static HSV SOLID_REACTIVE_SIMPLE_math(HSV hsv, uint16_t offset) {
#            ifdef RGB_MATRIX_SOLID_REACTIVE_GRADIENT_MODE
    hsv.h = scale16by8(g_rgb_timer, qadd8(rgb_matrix_config.speed, 8) >> 4);
#            endif
    hsv.v = scale8(255 - offset, hsv.v);
    return hsv;
}
typedef HSV (*reactive_f)(HSV hsv, uint16_t offset);
static bool effect_runner_reactive_mine(effect_params_t* params, reactive_f effect_func) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    // move the extra breathing variables here
    uint16_t max_tick = 65535 / qadd8(rgb_matrix_config.speed, 1);
    uint16_t time = scale16by8(g_rgb_timer, rgb_matrix_config.speed / 8);

    // only needed during led state init
    uint16_t scaled = scale16by8(g_rgb_timer, qadd8(rgb_matrix_config.speed, 16));
    uint8_t randIdx = random8_max(sizeof leds_backlight / sizeof leds_backlight[0]);


    if (params->init) {
        for(uint8_t i = 0; i < sizeof leds_backlight / sizeof leds_backlight[0]; i++) {
            led_state[leds_backlight[i]] = (led_state_t){
                .idx = leds_backlight[i],
                .init = true,
                .time_offset = 0,
                .hsv = {0, 0, 0}
            };
        }
    }

    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();
        
        // dg added this if statement to split effects
        if(HAS_FLAGS(g_led_config.flags[i], LED_FLAG_UNDERGLOW)) { // raindrop breathing effect for underglow rgb
            led_state_t *l = &led_state[i]; //TODO make dynamic later, check if pointer exists
    
            if (l->init) {
                if (scaled % 10 == 0 && l->idx == leds_backlight[randIdx]) { // add one random breathing LED every tick, make sure speed is not 0
                    l->hsv = raindrops_get_color_mine(0, l, params);
                    l->time_offset = time;
                    dprintf("time offset: %u\n", l->time_offset);
                    l->init = false;
                }
                // set_hsv_custom(l->idx, l->hsv);
            } else {
                l->hsv = raindrops_get_color_mine(time, l, params);
                set_hsv_custom(l->idx, l->hsv);
            }
        } else if (HAS_FLAGS(g_led_config.flags[i], LED_FLAG_KEYLIGHT)) { // reactive effect for per-key rgb
            uint16_t tick = max_tick;
            // Reverse search to find most recent key hit
            for (int8_t j = g_last_hit_tracker.count - 1; j >= 0; j--) {
                if (g_last_hit_tracker.index[j] == i && g_last_hit_tracker.tick[j] < tick) {
                    tick = g_last_hit_tracker.tick[j];
                    break;
                }
            }

            uint16_t offset = scale16by8(tick, qadd8(rgb_matrix_config.speed, 1));
            set_hsv_custom(i, effect_func(rgb_matrix_config.hsv, offset));
        }
    }
    return rgb_matrix_check_finished_leds(led_max);
}

static bool SOLID_REACTIVE_SIMPLE_RAINDROPS(effect_params_t* params) {
    return effect_runner_reactive_mine(params, &SOLID_REACTIVE_SIMPLE_math);
}



#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
