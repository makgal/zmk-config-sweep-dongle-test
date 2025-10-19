#include <lvgl.h>
#include <zephyr/logging/log.h>
#include <zmk/display.h>
#include <zmk/events/battery_state_changed.h>
#include <zmk/event_manager.h>
#include <zmk/usb.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#define NUM_PERIPHERALS 2

struct battery_object {
    lv_obj_t *canvas;
    lv_obj_t *label;
};

static struct battery_object battery_objs[NUM_PERIPHERALS];
static lv_color_t battery_buffers[NUM_PERIPHERALS][5*8];

static void draw_battery(lv_obj_t *canvas, uint8_t level, bool usb_present) {
    lv_canvas_fill_bg(canvas, lv_color_black(), LV_OPA_COVER);
    lv_draw_rect_dsc_t rect_dsc;
    lv_draw_rect_dsc_init(&rect_dsc);
    if (usb_present) {
        rect_dsc.bg_opa = LV_OPA_TRANSP;
        rect_dsc.border_color = lv_color_white();
        rect_dsc.border_width = 1;
    }

    // Нарисуем простую «заполненную» часть батареи
    int fill_height = (level * 5) / 100;  // 5 пикселей высоты
    lv_canvas_draw_rect(canvas, 1, 5 - fill_height, 3, fill_height, &rect_dsc);
}

static void battery_update_cb(const struct zmk_event_t *eh) {
    const struct zmk_battery_state_changed *ev = as_zmk_battery_state_changed(eh);
    if (!ev) return;

    for (int i = 0; i < NUM_PERIPHERALS; i++) {
        uint8_t level = ev->state_of_charge;  // для примера одинаковый уровень
        bool usb_present = zmk_usb_is_powered();
        draw_battery(battery_objs[i].canvas, level, usb_present);
        lv_label_set_text_fmt(battery_objs[i].label, "%u%%", level);
    }
}

lv_obj_t *zmk_display_status_screen(void) {
    lv_obj_t *screen = lv_obj_create(NULL);

    for (int i = 0; i < NUM_PERIPHERALS; i++) {
        battery_objs[i].canvas = lv_canvas_create(screen);
        lv_canvas_set_buffer(battery_objs[i].canvas, battery_buffers[i], 5, 8, LV_IMG_CF_TRUE_COLOR);
        lv_obj_align(battery_objs[i].canvas, LV_ALIGN_TOP_RIGHT, -10, i * 12);

        battery_objs[i].label = lv_label_create(screen);
        lv_obj_align_to(battery_objs[i].label, battery_objs[i].canvas, LV_ALIGN_OUT_RIGHT_MID, 2, 0);
    }

    return screen;
}

// Подписка на события батареи
ZMK_DISPLAY_WIDGET_LISTENER(widget_dongle_battery_status, struct zmk_battery_state_changed,
    battery_update_cb, NULL)
ZMK_SUBSCRIPTION(widget_dongle_battery_status, zmk_battery_state_changed);
#if IS_ENABLED(CONFIG_USB_DEVICE_STACK)
ZMK_SUBSCRIPTION(widget_dongle_battery_status, zmk_usb_conn_state_changed);
#endif
