
// declare custom effects
RGB_MATRIX_EFFECT(SOLID_TWINKLE)                                // RGB_MATRIX_CUSTOM_SOLID_TWINKLE
RGB_MATRIX_EFFECT(SOLID_TWINKLE_RAINDROPS)                      // RGB_MATRIX_CUSTOM_SOLID_TWINKLE_RAINDROPS
RGB_MATRIX_EFFECT(SOLID_TWINKLE_JELLYBEAN_RAINDROPS)            // RGB_MATRIX_CUSTOM_SOLID_TWINKLE_JELLYBEAN_RAINDROPS

RGB_MATRIX_EFFECT(SOLID_REACTIVE_SIMPLE)                        // RGB_MATRIX_CUSTOM_SOLID_REACTIVE_SIMPLE
RGB_MATRIX_EFFECT(SOLID_REACTIVE_SIMPLE_RAINDROPS)              // RGB_MATRIX_CUSTOM_SOLID_REACTIVE_SIMPLE_RAINDROPS
RGB_MATRIX_EFFECT(SOLID_REACTIVE_SIMPLE_JELLYBEAN_RAINDROPS)    // RGB_MATRIX_CUSTOM_SOLID_REACTIVE_SIMPLE_JELLYBEAN_RAINDROPS


// define effects in this block
#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

typedef struct {
    uint8_t idx;
    bool init;
    uint16_t time_offset;
    HSV hsv;
} led_state_t;

typedef enum {
    RAINDROPS,
    JELLYBEANRAINDROPS,
    CHRISTMASWHITE,
    DEFAULTCOLOR
} color_palette_t;

// more HSV colors
#define CANDLE          21, 212, 255    // 1900 Kelvin
#define CANDLEMINE      21, 235, 255
#define TUNGSTEN40W     20, 110, 255    // 2600 Kelvin
#define TUNGSTEN100W    22, 84, 255     // 2850 Kelvin
#define CARBONARC       23, 10, 255     // 5200 Kelvin

#define MAX_LED_INDEX 68

static const uint8_t leds_backlight[] = {1, 4, 7, 24, 27, 31, 35, 38, 41, 58, 61, 65};
static led_state_t led_state[MAX_LED_INDEX];
static color_palette_t current_palette = DEFAULTCOLOR;

static void set_hsv_custom(uint8_t i, HSV hsv) {
    RGB rgb = rgb_matrix_hsv_to_rgb(hsv);
    rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
}

// colors
static uint8_t get_palette_hue(color_palette_t palette) {
    uint8_t hue;
    switch (palette) {
        case RAINDROPS:
            // Take the shortest path between hues
            int16_t deltaH = ((rgb_matrix_config.hsv.h + 180) % 360 - rgb_matrix_config.hsv.h) / 4;
            if (deltaH > 127) {
                deltaH -= 256;
            } else if (deltaH < -127) {
                deltaH += 256;
            }
            hue = rgb_matrix_config.hsv.h + (deltaH * (random8() & 0x03));
            break;
        case JELLYBEANRAINDROPS:
            hue = random8();
            break;
        case DEFAULTCOLOR:
        default:
            hue = rgb_matrix_config.hsv.h;
            break;


    }
    return hue;
}
static uint8_t get_palette_saturation(color_palette_t palette) {
    uint8_t saturation;
    switch (palette) {
        case JELLYBEANRAINDROPS:
            saturation = random8_min_max(127, 255);
            break;
        case RAINDROPS:
        case DEFAULTCOLOR:
        default:
            saturation = rgb_matrix_config.hsv.s;
            break;
    }
    return saturation;
}

static HSV get_color_twinkle(
    uint16_t time,
    led_state_t* led,
    effect_params_t* params,
    bool partialBrightnessFade) {

    HSV hsv = {led->hsv.h, led->hsv.s, rgb_matrix_config.hsv.v};

    // fade in and out, can only set partialBrightnessFade = true if using DEFAULTCOLOR
    if (partialBrightnessFade) {
        hsv.v = scale8(abs8(sin8(time - led->time_offset) - 128) * 1.5 + 63, hsv.v);    // does not hit 0 to avoid reddish shift when brightness approaches 0
        // hsv.v = scale8(abs8(sin8(time - led->time_offset) - 128) + 127, hsv.v);      // alt with smaller amplitude
    } else {
        hsv.v = scale8(abs8(sin8(time - led->time_offset) - 128) * 2, hsv.v);
    }

    if (hsv.v == 0 || led->init) {
        hsv.h = get_palette_hue(current_palette);
        hsv.s = get_palette_saturation(current_palette);
    }

    return hsv;
}

static void set_color_twinkle(led_state_t* led, uint16_t timeOffset, effect_params_t* params, uint16_t scaledTime, uint8_t modFactor, uint8_t randomIndex, bool partialBrightnessFade) {
    if (led->init) {
        if (scaledTime % 10 == 0 && led->idx == randomIndex) { // add one random breathing LED every tick, make sure speed is not 0
            led->hsv          = get_color_twinkle(0, led, params, partialBrightnessFade);
            led->time_offset  = timeOffset;
            led->init         = false;
        }
    } else {
        led->hsv = get_color_twinkle(timeOffset, led, params, partialBrightnessFade);
    }
    set_hsv_custom(led->idx, led->hsv);
}

static bool effect_runner_twinkle(effect_params_t* params, bool partialBrightnessFade, color_palette_t palette) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    uint16_t time       = scale16by8(g_rgb_timer, rgb_matrix_config.speed / 8);
    uint16_t scaledTime = scale16by8(g_rgb_timer, qadd8(rgb_matrix_config.speed, 16));
    uint8_t  randIdx    = random8_max(MAX_LED_INDEX);

    if (params->init) {
        current_palette = palette;
        for (uint8_t i = 0; i < MAX_LED_INDEX; i++) {
            led_state[i] = (led_state_t){
                .idx = i,
                .init = true,
                .time_offset = 0,
                .hsv = {0, 0, 0}
            };
        }
    }

    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS(); // MACRO if (!HAS_ANY_FLAGS(g_led_config.flags[i], params->flags)) return;

        set_color_twinkle(&led_state[i], time, params, scaledTime, 10, randIdx, partialBrightnessFade);
    }
    return rgb_matrix_check_finished_leds(led_max);
}

static void set_color_reactive(led_state_t* led, uint16_t timeOffset) {
    HSV hsv = led->hsv;
    uint8_t hsvValue = scale8(255 - timeOffset, rgb_matrix_config.hsv.v);

    if (hsvValue > hsv.v) {// == 0 &&  > 0
        #ifdef RGB_MATRIX_SOLID_REACTIVE_GRADIENT_MODE
            hsv.h = scale16by8(g_rgb_timer, qadd8(rgb_matrix_config.speed, 8) >> 4);
        #else
            hsv.h = get_palette_hue(current_palette); // use this line outside of the if-statement for a fun electric-looking effect
            hsv.s = get_palette_saturation(current_palette);
        #endif
    }
    hsv.v = hsvValue;
    led->hsv = hsv;

    set_hsv_custom(led->idx, led->hsv);
}

static bool effect_runner_reactive_twinkle_backlights(effect_params_t* params, color_palette_t palette) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    // move the extra breathing variables here
    uint16_t max_tick = 65535 / qadd8(rgb_matrix_config.speed, 1);

    uint16_t time     = scale16by8(g_rgb_timer, rgb_matrix_config.speed / 8);
    uint16_t scaledTime = scale16by8(g_rgb_timer, qadd8(rgb_matrix_config.speed, 16));
    uint8_t  randIdx    = random8_max(MAX_LED_INDEX);

    if (params->init) {
        current_palette = palette;
        for (uint8_t i = 0; i < MAX_LED_INDEX; i++) {
            led_state[i] = (led_state_t){
                .idx = i,
                .init = true,
                .time_offset = 0,
                .hsv = {0, 0, 0}
            };
        }
    }

    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();

        // dg added this if statement to split effects
        if (HAS_FLAGS(g_led_config.flags[i], LED_FLAG_KEYLIGHT)) {
            uint16_t tick = max_tick;
            // Reverse search to find most recent key hit
            for (int8_t j = g_last_hit_tracker.count - 1; j >= 0; j--) {
                if (g_last_hit_tracker.index[j] == i && g_last_hit_tracker.tick[j] < tick) {
                    tick = g_last_hit_tracker.tick[j];
                    break;
                }
            }

            uint16_t offset = scale16by8(tick, qadd8(rgb_matrix_config.speed, 1));

            set_color_reactive(&led_state[i], offset);
            // set_hsv_custom(i, effect_func(rgb_matrix_config.hsv, offset));
        } else if (HAS_FLAGS(g_led_config.flags[i], LED_FLAG_UNDERGLOW)) {
            set_color_twinkle(&led_state[i], time, params, scaledTime, 50, randIdx, false); // twinkle breathing effect for underglow rgb
        }
    }
    return rgb_matrix_check_finished_leds(led_max);
}

static bool SOLID_TWINKLE(effect_params_t* params) {
    return effect_runner_twinkle(params, true, DEFAULTCOLOR);
}
static bool SOLID_TWINKLE_RAINDROPS(effect_params_t* params) {
    return effect_runner_twinkle(params, false, RAINDROPS);
}
static bool SOLID_TWINKLE_JELLYBEAN_RAINDROPS(effect_params_t* params) {
    return effect_runner_twinkle(params, false, JELLYBEANRAINDROPS);
}

static bool SOLID_REACTIVE_SIMPLE(effect_params_t* params) {
    return effect_runner_reactive_twinkle_backlights(params, DEFAULTCOLOR);
}
static bool SOLID_REACTIVE_SIMPLE_RAINDROPS(effect_params_t* params) {
    return effect_runner_reactive_twinkle_backlights(params, RAINDROPS);
}
static bool SOLID_REACTIVE_SIMPLE_JELLYBEAN_RAINDROPS(effect_params_t* params) {
    return effect_runner_reactive_twinkle_backlights(params, JELLYBEANRAINDROPS);
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
