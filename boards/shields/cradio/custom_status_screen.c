#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zmk/display.h>
#include <zmk/events/battery_state_changed.h>
#include <zmk/event_manager.h>
#include "custom_status_screen.h"

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

/* Статические переменные для хранения состояния половинок */
static uint8_t left_level = 0;
static uint8_t right_level = 0;
static bool left_connected = false;
static bool right_connected = false;

static lv_obj_t *battery_label;

/* Обновление текста на дисплее */
static void update_text_display(void) {
    char buf[32];

    snprintf(buf, sizeof(buf),
             "L:%s R:%s",
             left_connected ? "" : "-",
             right_connected ? "" : "-");

    if (left_connected) {
        char left_buf[5];
        snprintf(left_buf, sizeof(left_buf), "%u%%", left_level);
        snprintf(buf + 2, sizeof(buf) - 2, "%s", left_buf);
    }

    if (right_connected) {
        char right_buf[5];
        snprintf(right_buf, sizeof(right_buf), "%u%%", right_level);
        snprintf(buf + 9, sizeof(buf) - 9, "%s", right_buf);
    }

    lv_label_set_text(battery_label, buf);
}

/* Callback события батареи */
static void battery_update_cb(const struct zmk_event_t *eh) {
    const struct zmk_battery_state_changed *ev = as_zmk_battery_state_changed(eh);
    if (!ev) return;

    if (ev->source == 0) {      /* левая половинка */
        left_level = ev->state_of_charge;
        left_connected = true;
    } else if (ev->source == 1) { /* правая половинка */
        right_level = ev->state_of_charge;
        right_connected = true;
    }

    update_text_display();
}

/* Инициализация виджета на экране */
int zmk_widget_dongle_battery_status_init(lv_obj_t *parent) {
    battery_label = lv_label_create(parent);
    lv_label_set_text(battery_label, "L:- R:-");
    lv_obj_align(battery_label, LV_ALIGN_TOP_RIGHT, 0, 0);

    return 0;
}

/* Подписка на события батареи */
ZMK_SUBSCRIPTION(widget_dongle_battery_status, zmk_battery_state_changed);
