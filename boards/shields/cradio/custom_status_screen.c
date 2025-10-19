/*
 * Кастомный экран для донгла ZMK.
 * Показывает заряд батареи левой (L) и правой (R) половинок.
 * Если половинка не подключена — выводится "-".
 */

#include <zephyr/logging/log.h>
#include <zmk/event_manager.h>
#include <zmk/events/battery_state_changed.h> // новый include для события батареи
#include <lvgl.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

// Текущий уровень заряда половинок (-1 = нет данных/отключена)
static int8_t left_level = -1;
static int8_t right_level = -1;

// LVGL элемент для вывода текста
static lv_obj_t *label;

/**
 * @brief Обновляет текст на дисплее
 */
static void refresh_screen(void) {
    char buf[32];
    char left_str[8];
    char right_str[8];

    if (left_level >= 0)
        snprintf(left_str, sizeof(left_str), "%d%%", left_level);
    else
        snprintf(left_str, sizeof(left_str), "-");

    if (right_level >= 0)
        snprintf(right_str, sizeof(right_str), "%d%%", right_level);
    else
        snprintf(right_str, sizeof(right_str), "-");

    snprintf(buf, sizeof(buf), "L: %s  R: %s", left_str, right_str);

    lv_label_set_text(label, buf);
}

/**
 * @brief Обработчик события батареи
 */
static void battery_event_handler(const struct zmk_event_t *eh) {
    const struct zmk_battery_state_changed *ev = as_zmk_battery_state_changed(eh);
    if (!ev)
        return;

    // source: 0 = левая половинка, 1 = правая
    if (ev->source == 0)
        left_level = ev->state_of_charge;
    else if (ev->source == 1)
        right_level = ev->state_of_charge;

    refresh_screen();
}

// Подписываемся на событие батареи
ZMK_SUBSCRIPTION(custom_display_battery, zmk_battery_state_changed);

/**
 * @brief Создаёт экран для дисплея
 */
lv_obj_t *zmk_display_status_screen(void) {
    lv_obj_t *screen = lv_obj_create(NULL);

    // Настройка простого текста для монохромного экрана
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_text_font(&style, &lv_font_unscii_8);
    lv_obj_add_style(screen, &style, LV_PART_MAIN);

    // Создаём метку по центру
    label = lv_label_create(screen);
    lv_label_set_text(label, "L: -  R: -");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    return screen;
}
