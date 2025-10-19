/*
 * custom_status_screen.c
 *
 * Минимальный рабочий экран для OLED через ZMK
 */

#include "custom_status_screen.h"
#include <lvgl.h>
#include <zephyr/logging/log.h>
#include <zmk/display.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

// Функция создаёт экран с простым квадратом
lv_obj_t *zmk_display_status_screen(void) {
    lv_obj_t *screen = lv_scr_act();  // используем текущий экран

    // Очистка экрана
    lv_obj_clean(screen);

    // Создаем прямоугольник
    lv_obj_t *rect = lv_obj_create(screen);
    lv_obj_set_size(rect, 20, 20);                  // 20x20 пикселей
    lv_obj_align(rect, LV_ALIGN_CENTER, 0, 0);      // по центру
    lv_obj_set_style_bg_color(rect, lv_color_white(), 0);
    lv_obj_clear_flag(rect, LV_OBJ_FLAG_SCROLLABLE);

    return screen;
}

// Автоматическая инициализация дисплея
static int custom_status_screen_init(const struct device *dev) {
    ARG_UNUSED(dev);

    zmk_display_set_screen(zmk_display_status_screen());

    return 0;
}

SYS_INIT(custom_status_screen_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
