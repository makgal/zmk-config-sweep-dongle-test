#pragma once

#include <zephyr/kernel.h>
#include <zmk/display.h>
#include "custom_status_screen.h"


/**
 * @brief Создаёт экран и показывает текст "Тест"
 */
lv_obj_t *zmk_display_status_screen(void) {
    lv_obj_t *screen = lv_obj_create(NULL);

    // Создаём надпись
    lv_obj_t *label = lv_label_create(screen);
    lv_label_set_text(label, "Тест");

    // Выравнивание по центру
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    return screen;
}
