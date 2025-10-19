#include <lvgl.h>
#include <zmk/display.h>
#include <zmk/events/battery_state_changed.h>
#include <zmk/event_manager.h>

static lv_obj_t *battery_label;

static void battery_update_cb(const zmk_event_t *eh) {
    const struct zmk_battery_state_changed *ev = as_zmk_battery_state_changed(eh);
    if (!ev) return;

    char buf[32];

    // Проверяем состояние каждой половинки
    const char *left  = ev->peripheral_connected[0] ? 
                        (char [4]){0} : "-"; // временный буфер для процента
    const char *right = ev->peripheral_connected[1] ? 
                        (char [4]){0} : "-";

    if (ev->peripheral_connected[0])
        snprintf((char *)left, 4, "%u%%", ev->state_of_charge[0]);

    if (ev->peripheral_connected[1])
        snprintf((char *)right, 4, "%u%%", ev->state_of_charge[1]);

    // Формируем строку "L: xx% R: xx%" или с "-"
    snprintf(buf, sizeof(buf), "L: %s R: %s", left, right);

    lv_label_set_text(battery_label, buf);
}

lv_obj_t *zmk_display_status_screen(void) {
    lv_obj_t *screen = lv_obj_create(NULL);

    battery_label = lv_label_create(screen);
    lv_obj_align(battery_label, LV_ALIGN_TOP_LEFT, 10, 10);
    lv_label_set_text(battery_label, "L: - R: -"); // начальный текст

    return screen;
}

// Подписка на события батарей
ZMK_DISPLAY_WIDGET_LISTENER(widget_battery_status, struct zmk_battery_state_changed,
    battery_update_cb, NULL)
ZMK_SUBSCRIPTION(widget_battery_status, zmk_battery_state_changed);
