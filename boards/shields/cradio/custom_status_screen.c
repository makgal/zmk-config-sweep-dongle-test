#include <zephyr/logging/log.h>
#include <zmk/display.h>
#include <zmk/events/battery_state_changed.h>
#include <zmk/event_manager.h>
#include <zmk/usb.h>
#include <lvgl.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#define NUM_PERIPHERALS 2

struct peripheral_status {
    uint8_t level;
    bool connected;
};

static struct peripheral_status periph[NUM_PERIPHERALS] = {0};

static void update_display(void) {
    char buf[32];

    // Формируем строку: L:100% R:100% или L:- R:100%
    snprintf(buf, sizeof(buf),
        "L:%s R:%s",
        periph[0].connected ? (char [4]){0} : "-",  // если подключена, заменим ниже
        periph[1].connected ? (char [4]){0} : "-"
    );

    if (periph[0].connected) {
        snprintf(buf + 2, sizeof(buf) - 2, "%u%%", periph[0].level);
    }
    if (periph[1].connected) {
        snprintf(buf + 9, sizeof(buf) - 9, "%u%%", periph[1].level);
    }

    // Обновляем виджет на экране
    lv_label_set_text(zmk_display_get_label(), buf);
}

// Событие обновления батареи
static void battery_update_cb(const struct zmk_event_t *eh) {
    const struct zmk_battery_state_changed *ev = as_zmk_battery_state_changed(eh);
    if (!ev) return;

    // Пример: ev->peripheral_index = 0 или 1
    int idx = ev->peripheral_index;
    if (idx >= 0 && idx < NUM_PERIPHERALS) {
        periph[idx].level = ev->state_of_charge;
        periph[idx].connected = true;
    }

    update_display();
}

lv_obj_t *zmk_display_status_screen(void) {
    lv_obj_t *screen = lv_obj_create(NULL);

    // создаём один общий label
    lv_obj_t *label = lv_label_create(screen);
    lv_obj_center(label);

    zmk_display_set_label(label);

    return screen;
}

// Подписка на событие батареи
ZMK_DISPLAY_WIDGET_LISTENER(widget_battery_status,
    struct zmk_battery_state_changed, battery_update_cb, NULL)
ZMK_SUBSCRIPTION(widget_battery_status, zmk_battery_state_changed);
