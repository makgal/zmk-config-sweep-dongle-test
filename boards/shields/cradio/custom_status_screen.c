#include <lvgl.h>
#include "custom_status_screen.h"

lv_obj_t *zmk_display_status_screen(void) {
    // Создаём главный экран
    lv_obj_t *screen = lv_obj_create(NULL);

    // Инициализируем стиль для квадрата
    static lv_style_t square_style;
    lv_style_init(&square_style);
    lv_style_set_bg_color(&square_style, lv_color_white());
    lv_style_set_bg_opa(&square_style, LV_OPA_COVER);

    // Создаём квадрат на экране
    lv_obj_t *square = lv_obj_create(screen);
    lv_obj_set_size(square, 20, 20);           // размеры квадрата 20x20 px
    lv_obj_add_style(square, &square_style, 0);
    lv_obj_align(square, LV_ALIGN_CENTER, 0, 0); // по центру экрана

    return screen;
}
