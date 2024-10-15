// !!! DO NOT ADD #pragma once !!! //

// RGB_MATRIX_CUSTOM_KB = yes is required in rules.mk for this code to work
// inspiration and examples under quantum/rgb_matrix/animations/, docs here: https://docs.qmk.fm/features/rgb_matrix
// to use these effects programmatically, call it like this
// rgb_matrix_mode(RGB_MATRIX_CUSTOM_glitter2);

// declare custom effects
RGB_MATRIX_EFFECT(glitter1)
RGB_MATRIX_EFFECT(glitter2)

// define effects in this block
#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

// simple effect, in a single method
static bool glitter1(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    for (uint8_t i= led_min; i< led_max; i++) {
        rgb_matrix_set_color(i, 0xff, 0xff, 0x00);
    }
    return rgb_matrix_check_finished_leds(led_max);
}

// more complex effect, with external methods and state, with dedicated init and run methods
static uint8_t some_global_state;
static void glitter2_complex_init(effect_params_t* params) {
  some_global_state = 1;
}
static bool glitter2_complex_run(effect_params_t* params) {
  RGB_MATRIX_USE_LIMITS(led_min, led_max);
  for (uint8_t i = led_min; i < led_max; i++) {
    rgb_matrix_set_color(i, 0xff, some_global_state++, 0xff);
  }
  return rgb_matrix_check_finished_leds(led_max);
}
static bool glitter2(effect_params_t* params) {
  if (params->init) glitter2_complex_init(params);
  return glitter2_complex_run(params);
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
