#include <zephyr/kernel.h>
#include <zmk/display.h>
#include "custom_status_screen.h"

/**
 * @brief Создаёт экран с простым квадратиком (монохром OLED)
 */
lv_obj_t *zmk_display_status_screen(void) {
    // Создаём новый экран
    lv_obj_t *screen = lv_obj_create(NULL);

    // Создаём квадрат 20x20 пикселей
    lv_obj_t *rect = lv_obj_create(screen);
    lv_obj_set_size(rect, 20, 20);
    lv_obj_align(rect, LV_ALIGN_CENTER, 0, 0);

    // Устанавливаем белый фон (для монохромного OLED это "включённый" пиксель)
    lv_obj_set_style_bg_color(rect, lv_color_white(), 0);
    lv_obj_set_style_bg_opa(rect, LV_OPA_COVER, 0);

    // Делаем экран активным
    lv_scr_load(screen);

    // Принудительно обновляем дисплей
    lv_task_handler();

    return screen;
}

