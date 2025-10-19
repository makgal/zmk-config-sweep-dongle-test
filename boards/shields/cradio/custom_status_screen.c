#include "custom_status_screen.h"

lv_style_t global_style;

lv_obj_t *zmk_display_status_screen(void) {
    lv_obj_t *screen;

    // Создаем основной экран
    screen = lv_obj_create(NULL);

    // Инициализация глобального стиля
    lv_style_init(&global_style);
    lv_style_set_text_font(&global_style, &lv_font_unscii_8);
    lv_style_set_text_letter_space(&global_style, 1);
    lv_style_set_text_line_space(&global_style, 1);
    lv_obj_add_style(screen, &global_style, LV_PART_MAIN);

    // Создаем простой квадрат
    lv_obj_t *square = lv_obj_create(screen);
    lv_obj_set_size(square, 10, 10);               // размер квадрата
    lv_obj_align(square, LV_ALIGN_CENTER, 0, 0);   // выравнивание по центру

    // Цвет квадрата (белый для OLED)
    lv_obj_set_style_bg_color(square, lv_color_white(), 0);
    lv_obj_set_style_bg_opa(square, LV_OPA_COVER, 0);

    return screen;
}
