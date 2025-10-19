#include <lvgl.h>
#include <zephyr/logging/log.h>
#include <zmk/display.h>
#include <zmk/events/battery_state_changed.h>
#include <zmk/events/split_connection_changed.h>
#include <zmk/event_manager.h>
#include <zmk/usb.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#define NUM_PERIPHERALS 2  // 0 = Left, 1 = Right

static lv_obj_t *battery_label;
static bool peripheral_connected[NUM_PERIPHERALS] = { true, true };
static uint8_t battery_levels[NUM_PERIPHERALS] = { 100, 100 };

// Обновляем текст на экране
static void update_battery_label(void) {
    char buf[32];
    snprintf(buf, sizeof(buf), "L: %s R: %s",
             peripheral_connected[0] ? 
                 (char[5]){0} : "-",
                 peripheral_connected[1] ? 
                 (char[5]){0} : "-");
    // Заменяем буфер на проценты или "-"
    snprintf(buf, sizeof(buf), "L: %s R: %s",
             peripheral_connected[0] ? (char[8]){0} : "-",
             peripheral_connected[1] ? (char[8]){0} : "-");

    if (peripheral_connected[0]) snprintf(buf + 3, sizeof(buf)-3, "%u%%", battery_levels[0]);
    if (peripheral_connected[1]) snprintf(buf + 9, sizeof(buf)-9, "%u%%", battery_levels[1]);

    lv_label_set_text(battery_label, buf);
}

// Callback на события батареи
static void battery_update_cb(const struct zmk_event_t *eh) {
    const struct zmk_battery_state_changed *ev = as_zmk_battery_state_changed(eh);
    if (!ev) return;

    battery_levels[0] = ev->state_of_charge;
    battery_levels[1] = ev->state_of_charge;

    update_battery_label();
}

// Callback на события подключения половинки
static void split_connection_cb(const struct zmk_event_t *eh) {
    const struct zmk_split_connection_changed *ev = as_zmk_split_connection_changed(eh);
    if (!ev) return;

    peripheral_connected[0] = ev->left_connected;
    peripheral_connected[1] = ev->right_connected;

    update_battery_label();
}

// Создание экрана
lv_obj_t *zmk_display_status_screen(void) {
    lv_obj_t *screen = lv_obj_create(NULL);

    battery_label = lv_label_create(screen);
    lv_obj_align(battery_label, LV_ALIGN_TOP_RIGHT, -10, 5);

    update_battery_label();
    return screen;
}

// Подписки на события
ZMK_DISPLAY_WIDGET_LISTENER(widget_battery_status, struct zmk_battery_state_changed,
                            battery_update_cb, NULL)
ZMK_SUBSCRIPTION(widget_battery_status, zmk_battery_state_changed);

ZMK_DISPLAY_WIDGET_LISTENER(widget_split_status, struct zmk_split_connection_changed,
                            split_connection_cb, NULL)
ZMK_SUBSCRIPTION(widget_split_status, zmk_split_connection_changed);
