#include "custom_status_screen.h"
#include <zmk/battery.h>
#include <zephyr/kernel.h>

static lv_obj_t *status_screen;
static lv_obj_t *battery_label;

lv_obj_t *zmk_display_status_screen(void) {
    if (status_screen) {
        return status_screen; // уже создано
    }

    status_screen = lv_obj_create(lv_scr_act()); // родитель – текущий экран
    lv_obj_set_size(status_screen, LV_SIZE_CONTENT, LV_SIZE_CONTENT);

    battery_label = lv_label_create(status_screen);
    lv_label_set_text(battery_label, ""); // пока пусто
    lv_obj_align(battery_label, LV_ALIGN_TOP_LEFT, 0, 0);

    // Первое обновление
    int offset = 0;
    char buf[64] = {0};
    int central = zmk_battery_state_of_charge();
    offset += snprintf(buf + offset, sizeof(buf) - offset, "%d%%", central);

#if IS_ENABLED(CONFIG_ZMK_SPLIT)
    for (int i = 0; i < ZMK_SPLIT_BLE_PERIPHERAL_COUNT; i++) {
        int periph = zmk_peripheral_battery_state_of_charge(i);
        offset += snprintf(buf + offset, sizeof(buf) - offset, " %d%%", periph);
    }
#endif

    lv_label_set_text(battery_label, buf);

    return status_screen;
}
