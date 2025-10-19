/*
 * custom_status_screen.c
 *
 * Минимальный рабочий экран для OLED через LVGL
 */

#include "custom_status_screen.h"
#include <lvgl.h>
#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

lv_style_t global_style;

lv_obj_t *zmk_display_status_screen(void) {
    // Получаем текущий экран
    lv_obj_t *screen = lv_scr_act();

    // Инициализация глобального стиля
    lv_style_init(&global_style);
    lv_style_set_bg_color(&global_style, lv_color_black());
    lv_style_set_border_width(&global_style, 0);
    lv_obj_add_style(screen, &global_style, LV_PART_MAIN);

    // Создаём белый квадрат 20x20 по центру
    lv_obj_t *rect = lv_obj_create(screen);
    lv_obj_set_size(rect, 20, 20);
    lv_obj_align(rect, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_color(rect, lv_color_white(), 0);
    lv_obj_clear_flag(rect, LV_OBJ_FLAG_SCROLLABLE);

    return screen;
}
