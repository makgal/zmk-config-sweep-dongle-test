#include "custom_status_screen.h"
#include <lvgl.h>

static lv_style_t global_style;
static lv_color_t canvas_buffer[20 * 20]; // буфер для 20x20 пикселей

lv_obj_t *zmk_display_status_screen(void) {
    lv_obj_t *screen = lv_obj_create(NULL);

    lv_style_init(&global_style);
    lv_style_set_text_font(&global_style, &lv_font_unscii_8);
    lv_style_set_text_letter_space(&global_style, 1);
    lv_style_set_text_line_space(&global_style, 1);
    lv_obj_add_style(screen, &global_style, LV_PART_MAIN);

    // Canvas вместо обычного объекта
    lv_obj_t *canvas = lv_canvas_create(screen);
    lv_canvas_set_buffer(canvas, canvas_buffer, 20, 20, LV_IMG_CF_TRUE_COLOR);
    lv_obj_align(canvas, LV_ALIGN_CENTER, 0, 0);

    lv_canvas_fill_bg(canvas, lv_color_black(), LV_OPA_COVER);

    // Рисуем белый квадрат
    lv_draw_rect_dsc_t rect_dsc;
    lv_draw_rect_dsc_init(&rect_dsc);
    rect_dsc.bg_color = lv_color_white();
    rect_dsc.bg_opa = LV_OPA_COVER;
    lv_canvas_draw_rect(canvas, 0, 0, 10, 10, &rect_dsc);

    return screen;
}
