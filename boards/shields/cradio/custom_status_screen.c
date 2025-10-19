#include <zephyr/kernel.h>
#include <zmk/display.h>
#include "custom_status_screen.h"

lv_obj_t *zmk_display_status_screen(void) {
    // Создаём экран
    lv_obj_t *screen = lv_obj_create(NULL);

    // Создаём "квадратик"
    lv_obj_t *rect = lv_obj_create(screen);
    lv_obj_set_size(rect, 10, 10);                       // размер квадрата
    lv_obj_align(rect, LV_ALIGN_CENTER, 0, 0);           // по центру экрана
    lv_obj_set_style_bg_color(rect, lv_color_white(), 0); // цвет заливки белый
    lv_obj_set_style_bg_opa(rect, LV_OPA_COVER, 0);      // полностью непрозрачный

    return screen;
}
