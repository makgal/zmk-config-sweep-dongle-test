if(CONFIG_ZMK_DISPLAY AND CONFIG_ZMK_DISPLAY_STATUS_SCREEN_CUSTOM)
    # Создаём библиотеку для нашего файла
    zephyr_library()
    zephyr_library_sources(custom_status_screen.c)

    # Пути для заголовков LVGL
    zephyr_library_include_directories(
        ${ZEPHYR_BASE}/modules/lvgl/include
        ${ZEPHYR_BASE}/lib/gui/lvgl
    )

    # Линкуем с LVGL
    zephyr_library_link_libraries(lvgl)
endif()
