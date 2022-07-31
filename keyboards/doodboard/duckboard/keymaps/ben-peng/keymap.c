/* Copyright 2020 doodboard
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

#include "kb.h"

void suspend_power_down_user(void) {
    oled_off();
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

	KEYMAP(
		         TG(1),   KC_PSLS, KC_PAST, KC_PMNS,
		         KC_P7,    KC_P8,    KC_P9,    KC_PPLS,
		         KC_P4,    KC_P5,    KC_P6,    KC_PPLS,
		KC_MUTE, KC_P1,    KC_P2,    KC_P3,    KC_ENT,
		KC_BSPC, KC_P0,    KC_P0,    KC_DOT,  KC_ENT),

	KEYMAP(
		         TG(1),   KC_TRNS, KC_TRNS, KC_TRNS,
		         KC_HOME, KC_UP,   KC_PGUP, KC_TRNS,
		         KC_LEFT, KC_TRNS, KC_RGHT, KC_TRNS,
		KC_TRNS, KC_END,  KC_DOWN, KC_PGDN, KC_TRNS,
		TG(2),   KC_TRNS, KC_INS,  KC_DEL,  KC_TRNS),

	KEYMAP(
		         KC_TRNS, RGB_TOG, RGB_MOD, KC_TRNS,
		         RGB_HUI, RGB_SAI, RGB_VAI, KC_TRNS,
		         RGB_HUD, RGB_SAD, RGB_VAD, KC_TRNS,
		RESET,   KC_TRNS, KC_TRNS, TG(3), KC_TRNS,
		TG(2),   RESET,   KC_TRNS, KC_TRNS, KC_TRNS),
	KEYMAP(
		         KC_TRNS, RGB_TOG, RGB_MOD, KC_TRNS,
		         RGB_HUI, RGB_SAI, RGB_VAI, KC_TRNS,
		         RGB_HUD, RGB_SAD, RGB_VAD, KC_TRNS,
		RESET,   LGUI(KC_C), LGUI(KC_V), KC_TRNS, KC_TRNS,
		TG(0),   RESET,   KC_TRNS, KC_TRNS, KC_TRNS),

};

void encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) { /* First encoder */
        if (clockwise) {
            tap_code(KC_VOLU);
        } else {
            tap_code(KC_VOLD);
        }
}
}


#ifdef OLED_DRIVER_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) { return OLED_ROTATION_270; }


// WPM-responsive animation stuff here
#define IDLE_FRAMES 2
#define IDLE_SPEED 40 // below this wpm value your animation will idle

#define ANIM_FRAME_DURATION 200 // how long each frame lasts in ms
// #define SLEEP_TIMER 60000 // should sleep after this period of 0 wpm, needs fixing
#define ANIM_SIZE 636 // number of bytes in array, minimize for adequate firmware size, max is 1024

uint32_t anim_timer = 0;
uint32_t anim_sleep = 0;
uint8_t current_idle_frame = 0;

// Credit to u/Pop-X- for the initial code. You can find his commit here https://github.com/qmk/qmk_firmware/pull/9264/files#diff-303f6e3a7a5ee54be0a9a13630842956R196-R333.
static void render_anim(void) {
    static const char PROGMEM idle[IDLE_FRAMES][ANIM_SIZE] = {
        {
            0,0,0,0,0,0,0,0,0,0,128,64,32,144,16,16,32,64,128,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,193,50,205,240,30,3,3,0,0,15,240,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,63,192,63,255,224,16,8,7,0,0,1,30,96,64,128,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,7,8,9,5,4,4,8,8,4,2,2,1,0,0,0,0,0,0,0,0,0
        },
        {
            0,0,0,0,0,0,0,0,0,0,128,64,32,144,16,16,32,64,128,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,193,50,205,240,30,3,3,128,0,15,240,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,63,192,63,255,230,8,16,32,19,12,1,30,224,128,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,1,6,9,9,4,8,8,4,4,2,2,2,1,0,0,0,0,0,0,0,0
        }
    };

    //assumes 1 frame prep stage
    void animation_phase(void) {
            current_idle_frame = (current_idle_frame + 1) % IDLE_FRAMES;
            oled_write_raw_P(idle[abs((IDLE_FRAMES-1)-current_idle_frame)], ANIM_SIZE);
    }

        if(timer_elapsed32(anim_timer) > ANIM_FRAME_DURATION) {
            anim_timer = timer_read32();
            animation_phase();
        }
    }

void oled_task_user(void) {
        render_anim();
        oled_set_cursor(0,6);
        oled_write_P(PSTR("NOOT\nNOOT\n"), false);
    oled_write_P(PSTR("-----\n"), false);
    // Host Keyboard Layer Status
    oled_write_P(PSTR("MODE\n"), false);
    oled_write_P(PSTR("\n"), false);

    switch (get_highest_layer(layer_state)) {
        case 0:
            oled_write_P(PSTR("BASE\n"), false);
            break;
        case 1:
            oled_write_P(PSTR("FUNC\n"), false);
            break;
        case 2:
            oled_write_P(PSTR("RGB\n"), false);
            break;
	case 3:
            oled_write_P(PSTR("COPY\n"), false);
            break;
    }
}
#endif

void keyboard_post_init_user(void) {
  //Customise these values to debug
  debug_enable=true;
  debug_matrix=true;
  //debug_keyboard=true;
  //debug_mouse=true;
}
