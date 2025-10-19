#include <lvgl.h>
#include <zmk/display.h>
#include <zmk/events/battery_state_changed.h>
#include <zmk/event_manager.h>
#include <zmk/usb.h>

#define NUM_PERIPHERALS 2

static lv_obj_t *battery_labels[NUM_PERIPHERALS];

static void battery_update_cb(const zmk_event_t *eh) {
    const struct zmk_battery_state_changed *ev = as_zmk_battery_state_changed(eh);
    if (!ev) return;

    for (int i = 0; i < NUM_PERIPHERALS; i++) {
        bool connected = ev->peripheral_connected[i];  // true если половинка подключена
        char buf[8];

        if (connected) {
            snprintf(buf, sizeof(buf), "%u%%", ev->state_of_charge[i]);
        } else {
            snprintf(buf, sizeof(buf), "-");
        }

        lv_label_set_text(battery_labels[i], buf);
    }
}

lv_obj_t *zmk_display_status_screen(void) {
    lv_obj_t *screen = lv_obj_create(NULL);

    for (int i = 0; i < NUM_PERIPHERALS; i++) {
        battery_labels[i] = lv_label_create(screen);
        lv_obj_align(battery_labels[i], LV_ALIGN_TOP_LEFT, 10 + i * 40, 10);
        lv_label_set_text(battery_labels[i], "-");
    }

    return screen;
}

// Подписка на события батарей
ZMK_DISPLAY_WIDGET_LISTENER(widget_battery_status, struct zmk_battery_state_changed,
    battery_update_cb, NULL)
ZMK_SUBSCRIPTION(widget_battery_status, zmk_battery_state_changed);
