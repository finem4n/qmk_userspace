// In case you forgor:
// From QMK's docs:
// The modifiers (mod) argument to the MT() macro are prefixed with MOD_, not KC_

#include QMK_KEYBOARD_H

enum layer {
    _BASE,
    _NUM,
    _VIM,
    _MEDIA,
    _MOUSE,
    _GAME,
    _GAME_NUM,
    _FG,
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
    REDO = SAFE_RANGE,
    LPAR_HMR,
    RPAR_HMR,
    MEH_CLEAR,
    MEDIA_LT,
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    static uint16_t lpar_hmr_timer;
    static uint16_t rpar_hmr_timer;
    static uint16_t media_timer;

    switch (keycode) {
        case REDO:
            if (record->event.pressed) {
                SEND_STRING(SS_DOWN(X_LCTL)SS_DOWN(X_LSFT)SS_TAP(X_Z)SS_UP(X_LSFT)SS_UP(X_LCTL));
            } else {
            }
            return false;

        case LPAR_HMR:
        // Source: https://thomasbaart.nl/2018/12/09/qmk-basics-tap-and-hold-actions/
            if(record->event.pressed) {
                lpar_hmr_timer = timer_read();
                register_code(KC_RSFT);

            }
            else {
                unregister_code(KC_RSFT);
                if (timer_elapsed(lpar_hmr_timer) < TAPPING_TERM) {
                    tap_code16(S(KC_9));
                }
            }
            return false;

        case RPAR_HMR:
            if(record->event.pressed) {
                rpar_hmr_timer = timer_read();
                register_code(KC_LALT);
            }
            else {
                unregister_code(KC_LALT);
                if (timer_elapsed(rpar_hmr_timer) < TAPPING_TERM) {
                    tap_code16(S(KC_0));
                }
            }
            return false;

        case MEH_CLEAR:
            if (record->event.pressed) {
                layer_move(_BASE);
            } else {
            }
            return false;

        case MEDIA_LT:
            if(record->event.pressed) {
                media_timer = timer_read();
                layer_move(_MEDIA);
            }
            else {
                layer_move(_BASE);
                if (timer_elapsed(media_timer) < TAPPING_TERM) {
                    caps_word_on();
                }
            }
            return false;
    }
    return true;
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    /*[_TEMPLATE] = LAYOUT_split_3x6_3(*/
    /*KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,             KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,*/
    /*KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,             KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,*/
    /*KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,             KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,*/
    /*                    KC_NO, KC_NO, KC_NO,        KC_NO, KC_NO, KC_NO*/
    /*),*/

    // TODO check media lt if its working
    // TODO add shfit o tg mouse
    [_BASE] = LAYOUT_split_3x6_3(
        KC_ESC,     KC_Q,         KC_W,         KC_E,         KC_R,         KC_T,       KC_Y, KC_U,         KC_I,         KC_O,         KC_P,            KC_BSLS,
        KC_TAB,     LALT_T(KC_A), LSFT_T(KC_S), LCTL_T(KC_D), LGUI_T(KC_F), KC_G,       KC_H, RGUI_T(KC_J), RCTL_T(KC_K), RSFT_T(KC_L), LALT_T(KC_SCLN), KC_QUOT,
        MEDIA_LT,   KC_Z,         KC_X,         KC_C,         KC_V,         KC_B,       KC_N, KC_M,         KC_COMM,      KC_DOT,       KC_SLSH,         KC_DEL,
                                                      KC_MEH, KC_SPC, TG(_MOUSE),       TG(_VIM), LT(_NUM, KC_ENT), RALT_T(KC_BSPC)
    ),

    // TODO h i g jako @ i *
    [_NUM] = LAYOUT_split_3x6_3(
        KC_GRV,     KC_1,    KC_2,    KC_3,            KC_4,           KC_5,            KC_6,    KC_7,            KC_8,            KC_9,     KC_0,     KC_NO,
        KC_TAB,     KC_LALT, KC_LSFT, LCTL_T(KC_MINS), LGUI_T(KC_EQL), S(KC_2),         S(KC_8), RGUI_T(KC_LBRC), RCTL_T(KC_RBRC), LPAR_HMR, RPAR_HMR, KC_F12,
        CW_TOGG,    KC_F1,   KC_F2,   KC_F3,           KC_F4,          KC_F5,           KC_F6,   KC_F7,           KC_F8,           KC_F9,    KC_F10,   KC_F11,
                                                    MEH_CLEAR, KC_SPC, KC_NO,           KC_NO, KC_TRNS, KC_NO
    ),

    // TODO press shift and a to get sth like end and then MEH_CLEAR aka base layer
    // shift i do end and meh
    // ctrl and r to redo instead of singular redo
    [_VIM] = LAYOUT_split_3x6_3(
        KC_ESC, KC_NO,      KC_NO,      C(KC_RGHT), REDO,       KC_NO,                      C(KC_C),    C(KC_Z),    KC_NO,      KC_NO,      C(KC_V),    KC_NO,
        KC_TAB, KC_LALT,    KC_LSFT,    KC_LCTL,    KC_LGUI,    KC_NO,                      KC_LEFT,    KC_DOWN,    KC_UP,      KC_RGHT,    KC_NO,      KC_NO,
        KC_NO,  KC_NO,      C(KC_X),    KC_NO,      KC_NO,      C(KC_LEFT),                 KC_HOME,    KC_PGDN,    KC_PGUP,    KC_END,     C(KC_F),    KC_NO,
                                             MEH_CLEAR, KC_SPC, KC_NO,                      KC_TRNS, KC_ENT, KC_BSPC
    ),


    [_MEDIA] = LAYOUT_split_3x6_3(
    KC_NO,   KC_NO, KC_NO,   KC_NO,   KC_NO,   KC_NO,             KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
    KC_NO,   KC_NO, KC_MUTE, KC_VOLD, KC_VOLU, KC_F20,             KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
    KC_TRNS, KC_NO, KC_NO,   KC_MPRV, KC_MNXT, KC_MPLY,             KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
                        KC_NO, KC_NO, KC_NO,        KC_NO, KC_NO, KC_NO
    ),



    // [_MEDIA] = LAYOUT_split_3x6_3(
    //     KC_NO,      KC_NO,  KC_NO,      KC_MPRV,    KC_MNXT,    KC_MPLY,                KC_APP,     KC_NO,      KC_NO,      KC_NO,      KC_PSCR,    KC_PWR,
    //     KC_LALT,    KC_NO,  KC_MUTE,    KC_VOLD,    KC_VOLU,    KC_F20,                 MS_LEFT,    MS_DOWN,    MS_UP,      MS_RGHT,    TG(_GAME),  TG(_FG),
    //     KC_TRNS,    KC_NO,  KC_NO,      KC_BRID,    KC_BRIU,    KC_NO,                  MS_WHLL,    MS_WHLD,    MS_WHLU,    MS_WHLR,    KC_NO,      KC_RSFT,
    //                                         KC_LGUI,   KC_TRNS,     KC_LCTL,        MS_BTN3,   MS_BTN1,  MS_BTN2
    // ),

    // TODO add hrm
    [_MOUSE] = LAYOUT_split_3x6_3(
        KC_NO,      KC_NO,  KC_NO,      KC_MPRV,    KC_MNXT,    KC_MPLY,                KC_APP,     KC_NO,      KC_NO,      KC_NO,      KC_PSCR,    KC_PWR,
        KC_LALT,    KC_NO,  KC_MUTE,    KC_VOLD,    KC_VOLU,    KC_F20,                 MS_LEFT,    MS_DOWN,    MS_UP,      MS_RGHT,    TG(_GAME),  TG(_FG),
        KC_LSFT,    KC_NO,  KC_NO,      KC_BRID,    KC_BRIU,    KC_NO,                  MS_WHLL,    MS_WHLD,    MS_WHLU,    MS_WHLR,    KC_NO,      KC_RSFT,
                                            MEH_CLEAR,   KC_TRNS,     KC_TRNS,        MS_BTN3,   MS_BTN1,  MS_BTN2
    ),

    // normie layer and game should be seperate????
    // zmienic tutaj te controle taby i spacje na cos inszego chyba
    [_GAME] = LAYOUT_split_3x6_3(
        KC_ESC,     KC_Q,   KC_W,   KC_E,   KC_R,   KC_T,                               KC_Y,   KC_U,   KC_I,       KC_O,    KC_P,      KC_BSLS,
        KC_LCTL,    KC_A,   KC_S,   KC_D,   KC_F,   KC_G,                               KC_H,   KC_J,   KC_K,       KC_L,    KC_SCLN,   KC_ENT,
        KC_LSFT,    KC_Z,   KC_X,   KC_C,   KC_V,   KC_B,                               KC_N,   KC_M,   KC_COMM,    KC_DOT,  KC_SLSH,   KC_RSFT,
                                    KC_TAB, KC_SPC, MO(_GAME_NUM),              KC_LGUI,    KC_ENT,     KC_LALT
    ),

    [_GAME_NUM] = LAYOUT_split_3x6_3(
        KC_GRV,     KC_1,   KC_2,   KC_3,   KC_4,   KC_5,                               KC_NO,  KC_NO,      KC_UP,      KC_NO,      KC_NO,      KC_NO,
        KC_LCTL,    KC_6,   KC_7,   KC_8,   KC_9,   KC_0,                               KC_NO,  KC_LEFT,    KC_DOWN,    KC_RGHT,    TG(_GAME),  KC_NO,
        KC_LSFT,    KC_F1,  KC_F2,  KC_F3,  KC_F4,  KC_F5,                              KC_F6,  KC_F7,      KC_F8,      KC_F9,      KC_F12,     KC_RSFT,
                                        KC_NO,  KC_NO,   KC_TRNS,               KC_LGUI,  KC_ENT,    KC_LALT
    ),

    // Layout for fighting games
    [_FG] = LAYOUT_split_3x6_3(
        KC_ESC,     KC_NO,  KC_NO,      KC_UP,      KC_NO,      KC_NO,                  KC_ENT, KC_U,  KC_I,    KC_O,   KC_NO,      KC_BSLS,
        KC_TAB,     KC_NO,  KC_LEFT,    KC_DOWN,    KC_RGHT,    KC_NO,                  KC_NO,  KC_J,  KC_K,    KC_L,   KC_SCLN,    TG(_FG),
        KC_LSFT,    KC_NO,  KC_NO,      KC_NO,      KC_NO,      KC_NO,                  KC_NO,  KC_M,  KC_COMM, KC_DOT, KC_SLSH,    KC_RSFT,
                                            KC_UP,      KC_SPC,     KC_LCTL,        KC_1,   KC_2,   KC_3
    ),
};
