#include "qosa_def.h"
#include "qosa_lcd.h"
#include "qosa_log.h"
#include "qosa_pinctrl.h"
#include "qosa_sys.h"
#include "unirtos_app_init_registry.h"

#include "lvgl.h"

#define QOS_LOG_TAG                LOG_TAG_LCD_API
#define UNIR_LVGL_LCD_PORT          QOSA_LCD_SPECIAL_PORT
#define UNIR_LVGL_LCD_RST_PIN       49
#define UNIR_LVGL_LCD_RST_FUNC      0
#define UNIR_LVGL_LCD_DS_PIN        51
#define UNIR_LVGL_LCD_DS_FUNC       2
#define UNIR_LVGL_LCD_CS_PIN        52
#define UNIR_LVGL_LCD_CS_FUNC       1
#define UNIR_LVGL_LCD_DOUT_PIN      50
#define UNIR_LVGL_LCD_DOUT_FUNC     1
#define UNIR_LVGL_LCD_CLK_PIN       53
#define UNIR_LVGL_LCD_CLK_FUNC      1
#define UNIR_LVGL_TASK_STACK_SIZE   (12U * 1024U)
#define UNIR_LVGL_TASK_PRIORITY     4
#define UNIR_LVGL_TICK_MS           5

extern qosa_lcd_drv_cfg_t g_unir_lcd_drv_st7796s_info;
qosa_bool_t unir_lvgl_port_display_init(void);
void unir_lvgl_port_paint_base(void);

static qosa_task_t g_unir_lvgl_task = QOSA_NULL;
static lv_obj_t *g_progress_bar = QOSA_NULL;
static lv_obj_t *g_status_label = QOSA_NULL;

static void unir_lvgl_update_ui(lv_timer_t *timer)
{
    static qosa_uint8_t progress;

    (void)timer;
    progress = (qosa_uint8_t)((progress + 2U) % 101U);
    lv_bar_set_value(g_progress_bar, progress, LV_ANIM_OFF);
    lv_label_set_text_fmt(g_status_label, "Partial refresh: %u%%", progress);
}

static void unir_lvgl_create_screen(void)
{
    lv_obj_t *screen = lv_scr_act();
    lv_obj_t *title;
    lv_obj_t *button;
    lv_obj_t *button_label;
    lv_obj_t *red;
    lv_obj_t *green;
    lv_obj_t *blue;

    lv_obj_set_style_bg_color(screen, lv_color_hex(0x101820), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, LV_PART_MAIN);

    title = lv_label_create(screen);
    lv_label_set_text(title, "UniRTOS LVGL / ST7796S");
    lv_obj_set_style_text_color(title, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_pos(title, 48, 28);

    g_status_label = lv_label_create(screen);
    lv_label_set_text(g_status_label, "Partial refresh: 0%");
    lv_obj_set_style_text_color(g_status_label, lv_color_hex(0xD0D7DE), LV_PART_MAIN);
    lv_obj_set_pos(g_status_label, 82, 70);

    g_progress_bar = lv_bar_create(screen);
    lv_obj_set_size(g_progress_bar, 230, 16);
    lv_obj_set_pos(g_progress_bar, 45, 104);
    lv_bar_set_range(g_progress_bar, 0, 100);
    lv_obj_set_style_bg_color(g_progress_bar, lv_color_hex(0x30363D), LV_PART_MAIN);
    lv_obj_set_style_bg_color(g_progress_bar, lv_color_hex(0x2F81F7), LV_PART_INDICATOR);

    button = lv_obj_create(screen);
    lv_obj_set_size(button, 150, 52);
    lv_obj_set_pos(button, 85, 150);
    lv_obj_set_style_bg_color(button, lv_color_hex(0x238636), LV_PART_MAIN);
    button_label = lv_label_create(button);
    lv_label_set_text(button_label, "LVGL Ready");
    lv_obj_set_style_text_color(button_label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_pos(button_label, 32, 18);

    red = lv_obj_create(screen);
    green = lv_obj_create(screen);
    blue = lv_obj_create(screen);
    lv_obj_set_size(red, 72, 72);
    lv_obj_set_size(green, 72, 72);
    lv_obj_set_size(blue, 72, 72);
    lv_obj_set_pos(red, 38, 238);
    lv_obj_set_pos(green, 124, 238);
    lv_obj_set_pos(blue, 210, 238);
    lv_obj_set_style_bg_color(red, lv_color_hex(0xF85149), LV_PART_MAIN);
    lv_obj_set_style_bg_color(green, lv_color_hex(0x3FB950), LV_PART_MAIN);
    lv_obj_set_style_bg_color(blue, lv_color_hex(0x58A6FF), LV_PART_MAIN);
    lv_obj_set_style_border_width(red, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(green, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(blue, 0, LV_PART_MAIN);
    lv_obj_set_style_radius(red, 8, LV_PART_MAIN);
    lv_obj_set_style_radius(green, 8, LV_PART_MAIN);
    lv_obj_set_style_radius(blue, 8, LV_PART_MAIN);
    (void)lv_timer_create(unir_lvgl_update_ui, 100, QOSA_NULL);
}

static void unir_lvgl_demo_process(void *ctx)
{
    qosa_lcd_error_e ret;

    (void)ctx;
    qosa_task_sleep_ms(3000);
    qosa_pin_set_func(UNIR_LVGL_LCD_RST_PIN, UNIR_LVGL_LCD_RST_FUNC);
    qosa_pin_set_func(UNIR_LVGL_LCD_CS_PIN, UNIR_LVGL_LCD_CS_FUNC);
    qosa_pin_set_func(UNIR_LVGL_LCD_DS_PIN, UNIR_LVGL_LCD_DS_FUNC);
    qosa_pin_set_func(UNIR_LVGL_LCD_DOUT_PIN, UNIR_LVGL_LCD_DOUT_FUNC);
    qosa_pin_set_func(UNIR_LVGL_LCD_CLK_PIN, UNIR_LVGL_LCD_CLK_FUNC);
    ret = qosa_lcd_ioctl(UNIR_LVGL_LCD_PORT, QOSA_LCD_IOCTL_SET_DRV_CFG, &g_unir_lcd_drv_st7796s_info);
    if (ret != QOSA_LCD_SUCCESS) {
        QLOGE("Unable to configure ST7796S: %d", ret);
        return;
    }
    ret = qosa_lcd_init(UNIR_LVGL_LCD_PORT);
    if (ret != QOSA_LCD_SUCCESS) {
        QLOGE("Unable to initialize ST7796S: %d", ret);
        return;
    }
    lv_init();
    if (unir_lvgl_port_display_init() == QOSA_FALSE) {
        QLOGE("Unable to create LVGL DMA draw buffer");
        return;
    }
    unir_lvgl_port_paint_base();
    unir_lvgl_create_screen();

    /* Do not wait for LVGL's periodic refresh timer for the first image. */
    lv_obj_invalidate(lv_scr_act());
    lv_refr_now(QOSA_NULL);
    QLOGI("LVGL demo initialized; RGB blocks validate byte order");
    for (;;) {
        lv_tick_inc(UNIR_LVGL_TICK_MS);
        (void)lv_timer_handler();
        qosa_task_sleep_ms(UNIR_LVGL_TICK_MS);
    }
}

void unir_lvgl_demo_init(void)
{
    if (g_unir_lvgl_task == QOSA_NULL) {
        qosa_task_create(&g_unir_lvgl_task,
                         UNIR_LVGL_TASK_STACK_SIZE,
                         UNIR_LVGL_TASK_PRIORITY,
                         "lvgl_demo",
                         unir_lvgl_demo_process,
                         QOSA_NULL,
                         1);
        QLOGI("LVGL demo task created");
    }
}

UNIRTOS_APP_EXPORT(327, "lvgl_demo", unir_lvgl_demo_init);
