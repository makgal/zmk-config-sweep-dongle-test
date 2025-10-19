#include <zephyr/kernel.h>
#include <zmk/display.h>
#include "custom_status_screen.h"

/**
 * @brief Создаёт экран с простым квадратиком (монохром OLED)
 */
lv_obj_t *zmk_display_status_screen(void) {
    // Создаём основной экран
    lv_obj_t *screen = lv_obj_create(NULL);

    // Создаём объект — квадрат
    lv_obj_t *rect = lv_obj_create(screen);

    // Задаём размер квадрата 20x20
    lv_obj_set_size(rect, 20, 20);

    // Размещаем в центре экрана
    lv_obj_align(rect, LV_ALIGN_CENTER, 0, 0);

    // Для монохромного экрана: включаем фон (белый квадрат)
    lv_obj_set_style_bg_color(rect, lv_color_white(), 0);
    lv_obj_set_style_bg_opa(rect, LV_OPA_COVER, 0);

    return screen;
}
