// In case you forgor:
// From QMK's docs:
// The modifiers (mod) argument to the MT() macro are prefixed with MOD_, not KC_

#include QMK_KEYBOARD_H

// For oneshots check callum's github
// https://github.com/callum-oakley/qmk_firmware/tree/master/users/callum
#include "oneshot.h"

#define HOME G(KC_LEFT)
#define END G(KC_RGHT)
#define FWD G(KC_RBRC)
#define BACK G(KC_LBRC)
#define TAB_L G(S(KC_LBRC))
#define TAB_R G(S(KC_RBRC))
#define SPACE_L A(G(KC_LEFT))
#define SPACE_R A(G(KC_RGHT))
#define LA_SYM MO(SYM)
#define LA_NAV MO(NAV)

enum layer {
    BASE,
    NAV,
    NUM,
    SYM,
    // _BASE,
    // _NUM,
    // _VIM,
    // _MEDIA,
    // _MOUSE,
    // _GAME,
    // _GAME_NUM,
    // _FG,
    // Mozna sprobowac zrobic tak, zeby na jednej rece po przytrzymaniu danego hmr dostepne byly tylko kolejne modifiery
    // w sensie chodzi o to ze jak przytrzymam gui to na lewej polowie nie dziala zadna litera, a dzialaja tylko kolejne modifiery
    // _LSFT,
    // _LALT,
    // _LCTL,
    // _LGUI,
    // _RSFT,
    // _RALT,
    // _RCTL,
    // _RGUI,
};

// See:
// https://docs.qmk.fm/feature_macros

enum custom_keycodes {
    // OS for one shot
    OS_SHFT = SAFE_RANGE,
    OS_CTRL,
    OS_ALT,
    OS_GUI,
    REDO,
    MEH_CLR,
    LT_MDIA,
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // static uint16_t media_timer;

    update_oneshot(
        &os_shft_state, KC_LSFT, OS_SHFT,
        keycode, record
    );
    update_oneshot(
        &os_ctrl_state, KC_LCTL, OS_CTRL,
        keycode, record
    );
    update_oneshot(
        &os_alt_state, KC_LALT, OS_ALT,
        keycode, record
    );
    update_oneshot(
        &os_cmd_state, KC_LCMD, OS_GUI,
        keycode, record
    );

    switch (keycode) {
        case REDO:
            if (record->event.pressed) {
                SEND_STRING(SS_DOWN(X_LCTL)SS_DOWN(X_LSFT)SS_TAP(X_Z)SS_UP(X_LSFT)SS_UP(X_LCTL));
            } else {
            }
            return false;

        case MEH_CLR:
            if (record->event.pressed) {
                layer_move(BASE);
            } else {
            }
            return false;

        // case LT_MDIA:
        //     if(record->event.pressed) {
        //         media_timer = timer_read();
        //         layer_move(_MEDIA);
        //     }
        //     else {
        //         layer_move(_BASE);
        //         if (timer_elapsed(media_timer) < TAPPING_TERM) {
        //             caps_word_on();
        //         }
        //     }
        //     return false;
    }
    return true;
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    // [TEMPLATE] = LAYOUT_split_3x6_3(
    // KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,             KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
    // KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,             KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
    // KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,             KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
    //                     KC_NO, KC_NO, KC_NO,        KC_NO, KC_NO, KC_NO
    // ),

    // [BASE] = LAYOUT_split_3x6_3(
    // KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,             KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
    // KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,             KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
    // KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,             KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
    //                     KC_NO, KC_NO, KC_NO,        KC_NO, KC_NO, KC_NO
    // ),

    [BASE] = LAYOUT_split_3x6_3(
     KC_NO,   KC_Q,    KC_W,    KC_F,    KC_P,    KC_G,    KC_J,    KC_L,    KC_U,    KC_Y,    KC_QUOT, KC_NO,
     KC_NO,   KC_A,    KC_R,    KC_S,    KC_T,    KC_D,    KC_H,    KC_N,    KC_E,    KC_I,    KC_O,    KC_NO,
     KC_NO,   KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_K,    KC_M,    KC_COMM, KC_DOT,  KC_SCLN, KC_NO,
                                         KC_NO, LA_NAV,  KC_LSFT, KC_SPC,  LA_SYM, KC_NO
    ),

    [SYM] = LAYOUT_split_3x6_3(
     KC_NO,   KC_ESC,  KC_LBRC, KC_LCBR, KC_LPRN, KC_TILD, KC_CIRC, KC_RPRN, KC_RCBR, KC_RBRC, KC_GRV, KC_NO,
     KC_NO,   KC_MINS, KC_ASTR, KC_EQL,  KC_UNDS, KC_DLR,  KC_HASH, OS_GUI,  OS_ALT,  OS_CTRL, OS_SHFT,KC_NO,
     KC_NO,   KC_PLUS, KC_PIPE, KC_AT,   KC_SLSH, KC_PERC, XXXXXXX, KC_BSLS, KC_AMPR, KC_QUES, KC_EXLM,KC_NO,
                                         KC_NO,_______, _______, _______, _______, KC_NO
    ),

    [NAV] = LAYOUT_split_3x6_3(
     KC_NO,   KC_TAB,  KC_NO,  TAB_L,   TAB_R,   KC_VOLU, KC_NO,   HOME,    KC_UP,   END,     KC_DEL, KC_NO,
     KC_NO,   OS_SHFT, OS_CTRL, OS_ALT,  OS_GUI,  KC_VOLD, KC_CAPS, KC_LEFT, KC_DOWN, KC_RGHT, KC_BSPC,KC_NO,
     KC_NO,   SPACE_L, SPACE_R, BACK,    FWD,     KC_MPLY, XXXXXXX, KC_PGDN, KC_PGUP, KC_NO, KC_ENT, KC_NO,
                                         KC_NO,_______, _______, _______, _______, KC_NO
    ),

    [NUM] = LAYOUT_split_3x6_3(
     KC_NO,   KC_7,    KC_5,    KC_3,    KC_1,    KC_9,    KC_8,    KC_0,    KC_2,    KC_4,    KC_6,    KC_NO,
     KC_NO,   OS_SHFT, OS_CTRL, OS_ALT,  OS_GUI,  KC_F11,  KC_F10,  OS_GUI,  OS_ALT,  OS_CTRL, OS_SHFT, KC_NO,
     KC_NO,   KC_F7,   KC_F5,   KC_F3,   KC_F1,   KC_F9,   KC_F8,   KC_F12,  KC_F2,   KC_F4,   KC_F6,   KC_NO,
                                         KC_NO,_______, _______, _______, _______, KC_NO
    ),

    // TODO check media lt if its working
    // TODO add shfit o tg mouse
    // [_BASE] = LAYOUT_split_3x6_3(
    //     KC_ESC,     KC_Q,         KC_W,         KC_E,         KC_R,         KC_T,       KC_Y, KC_U,         KC_I,         KC_O,         KC_P,            KC_BSLS,
    //     KC_TAB,     LALT_T(KC_A), LSFT_T(KC_S), LCTL_T(KC_D), LGUI_T(KC_F), KC_G,       KC_H, RGUI_T(KC_J), RCTL_T(KC_K), RSFT_T(KC_L), LALT_T(KC_SCLN), KC_QUOT,
    //     MEDIA_LT,   KC_Z,         KC_X,         KC_C,         KC_V,         KC_B,       KC_N, KC_M,         KC_COMM,      KC_DOT,       KC_SLSH,         KC_DEL,
    //                                                   KC_MEH, KC_SPC, TG(_MOUSE),       TG(_VIM), LT(_NUM, KC_ENT), RALT_T(KC_BSPC)
    // ),
    //
    // // TODO h i g jako @ i *
    // [_NUM] = LAYOUT_split_3x6_3(
    //     KC_GRV,     KC_1,    KC_2,    KC_3,            KC_4,           KC_5,            KC_6,    KC_7,            KC_8,            KC_9,     KC_0,     KC_NO,
    //     KC_TAB,     KC_LALT, KC_LSFT, LCTL_T(KC_MINS), LGUI_T(KC_EQL), S(KC_2),         S(KC_8), RGUI_T(KC_LBRC), RCTL_T(KC_RBRC), LPAR_HMR, RPAR_HMR, KC_F12,
    //     CW_TOGG,    KC_F1,   KC_F2,   KC_F3,           KC_F4,          KC_F5,           KC_F6,   KC_F7,           KC_F8,           KC_F9,    KC_F10,   KC_F11,
    //                                                 MEH_CLEAR, KC_SPC, KC_NO,           KC_NO, KC_TRNS, KC_NO
    // ),
    //
    // // TODO press shift and a to get sth like end and then MEH_CLEAR aka base layer
    // // shift i do end and meh
    // // ctrl and r to redo instead of singular redo
    // [_VIM] = LAYOUT_split_3x6_3(
    //     KC_ESC, KC_NO,      KC_NO,      C(KC_RGHT), REDO,       KC_NO,                      C(KC_C),    C(KC_Z),    KC_NO,      KC_NO,      C(KC_V),    KC_NO,
    //     KC_TAB, KC_LALT,    KC_LSFT,    KC_LCTL,    KC_LGUI,    KC_NO,                      KC_LEFT,    KC_DOWN,    KC_UP,      KC_RGHT,    KC_NO,      KC_NO,
    //     KC_NO,  KC_NO,      C(KC_X),    KC_NO,      KC_NO,      C(KC_LEFT),                 KC_HOME,    KC_PGDN,    KC_PGUP,    KC_END,     C(KC_F),    KC_NO,
    //                                          MEH_CLEAR, KC_SPC, KC_NO,                      KC_TRNS, KC_ENT, KC_BSPC
    // ),
    //
    //
    // [_MEDIA] = LAYOUT_split_3x6_3(
    // KC_NO,   KC_NO, KC_NO,   KC_NO,   KC_NO,   KC_NO,             KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
    // KC_NO,   KC_NO, KC_MUTE, KC_VOLD, KC_VOLU, KC_F20,             KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
    // KC_TRNS, KC_NO, KC_NO,   KC_MPRV, KC_MNXT, KC_MPLY,             KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
    //                     KC_NO, KC_NO, KC_NO,        KC_NO, KC_NO, KC_NO
    // ),
    //
    //
    //
    // // [_MEDIA] = LAYOUT_split_3x6_3(
    // //     KC_NO,      KC_NO,  KC_NO,      KC_MPRV,    KC_MNXT,    KC_MPLY,                KC_APP,     KC_NO,      KC_NO,      KC_NO,      KC_PSCR,    KC_PWR,
    // //     KC_LALT,    KC_NO,  KC_MUTE,    KC_VOLD,    KC_VOLU,    KC_F20,                 MS_LEFT,    MS_DOWN,    MS_UP,      MS_RGHT,    TG(_GAME),  TG(_FG),
    // //     KC_TRNS,    KC_NO,  KC_NO,      KC_BRID,    KC_BRIU,    KC_NO,                  MS_WHLL,    MS_WHLD,    MS_WHLU,    MS_WHLR,    KC_NO,      KC_RSFT,
    // //                                         KC_LGUI,   KC_TRNS,     KC_LCTL,        MS_BTN3,   MS_BTN1,  MS_BTN2
    // // ),
    //
    // // TODO add hrm
    // [_MOUSE] = LAYOUT_split_3x6_3(
    //     KC_NO,      KC_NO,  KC_NO,      KC_MPRV,    KC_MNXT,    KC_MPLY,                KC_APP,     KC_NO,      KC_NO,      KC_NO,      KC_PSCR,    KC_PWR,
    //     KC_LALT,    KC_NO,  KC_MUTE,    KC_VOLD,    KC_VOLU,    KC_F20,                 MS_LEFT,    MS_DOWN,    MS_UP,      MS_RGHT,    TG(_GAME),  TG(_FG),
    //     KC_LSFT,    KC_NO,  KC_NO,      KC_BRID,    KC_BRIU,    KC_NO,                  MS_WHLL,    MS_WHLD,    MS_WHLU,    MS_WHLR,    KC_NO,      KC_RSFT,
    //                                         MEH_CLEAR,   KC_TRNS,     KC_TRNS,        MS_BTN3,   MS_BTN1,  MS_BTN2
    // ),
    //
    // // normie layer and game should be seperate????
    // // zmienic tutaj te controle taby i spacje na cos inszego chyba
    // [_GAME] = LAYOUT_split_3x6_3(
    //     KC_ESC,     KC_Q,   KC_W,   KC_E,   KC_R,   KC_T,                               KC_Y,   KC_U,   KC_I,       KC_O,    KC_P,      KC_BSLS,
    //     KC_LCTL,    KC_A,   KC_S,   KC_D,   KC_F,   KC_G,                               KC_H,   KC_J,   KC_K,       KC_L,    KC_SCLN,   KC_ENT,
    //     KC_LSFT,    KC_Z,   KC_X,   KC_C,   KC_V,   KC_B,                               KC_N,   KC_M,   KC_COMM,    KC_DOT,  KC_SLSH,   KC_RSFT,
    //                                 KC_TAB, KC_SPC, MO(_GAME_NUM),              KC_LGUI,    KC_ENT,     KC_LALT
    // ),
    //
    // [_GAME_NUM] = LAYOUT_split_3x6_3(
    //     KC_GRV,     KC_1,   KC_2,   KC_3,   KC_4,   KC_5,                               KC_NO,  KC_NO,      KC_UP,      KC_NO,      KC_NO,      KC_NO,
    //     KC_LCTL,    KC_6,   KC_7,   KC_8,   KC_9,   KC_0,                               KC_NO,  KC_LEFT,    KC_DOWN,    KC_RGHT,    TG(_GAME),  KC_NO,
    //     KC_LSFT,    KC_F1,  KC_F2,  KC_F3,  KC_F4,  KC_F5,                              KC_F6,  KC_F7,      KC_F8,      KC_F9,      KC_F12,     KC_RSFT,
    //                                     KC_NO,  KC_NO,   KC_TRNS,               KC_LGUI,  KC_ENT,    KC_LALT
    // ),
    //
    // // Layout for fighting games
    // [_FG] = LAYOUT_split_3x6_3(
    //     KC_ESC,     KC_NO,  KC_NO,      KC_UP,      KC_NO,      KC_NO,                  KC_ENT, KC_U,  KC_I,    KC_O,   KC_NO,      KC_BSLS,
    //     KC_TAB,     KC_NO,  KC_LEFT,    KC_DOWN,    KC_RGHT,    KC_NO,                  KC_NO,  KC_J,  KC_K,    KC_L,   KC_SCLN,    TG(_FG),
    //     KC_LSFT,    KC_NO,  KC_NO,      KC_NO,      KC_NO,      KC_NO,                  KC_NO,  KC_M,  KC_COMM, KC_DOT, KC_SLSH,    KC_RSFT,
    //                                         KC_UP,      KC_SPC,     KC_LCTL,        KC_1,   KC_2,   KC_3
    // ),
};


bool is_oneshot_cancel_key(uint16_t keycode) {
    switch (keycode) {
    case LA_SYM:
    case LA_NAV:
        return true;
    default:
        return false;
    }
}

bool is_oneshot_ignored_key(uint16_t keycode) {
    switch (keycode) {
    case LA_SYM:
    case LA_NAV:
    case KC_LSFT:
    case OS_SHFT:
    case OS_CTRL:
    case OS_ALT:
    case OS_GUI:
        return true;
    default:
        return false;
    }
}

oneshot_state os_shft_state = os_up_unqueued;
oneshot_state os_ctrl_state = os_up_unqueued;
oneshot_state os_alt_state = os_up_unqueued;
oneshot_state os_cmd_state = os_up_unqueued;


layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, SYM, NAV, NUM);
}
