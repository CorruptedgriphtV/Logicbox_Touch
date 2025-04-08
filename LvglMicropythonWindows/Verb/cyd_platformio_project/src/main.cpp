#include <Arduino.h>
// Include lv_conf.h before lvgl.h when LV_CONF_INCLUDE_SIMPLE is used
#include "lv_conf.h"
#include <lvgl.h>
// TFT_eSPI setup is now handled by src/User_Setup.h (auto-included)
#include <TFT_eSPI.h> // Hardware-specific library for display
#include "ui.h"       // Include the UI header file

/* Define screen dimensions (Must match TFT_WIDTH/TFT_HEIGHT in build flags) */
#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240

/* LVGL Display Buffers */
static lv_disp_draw_buf_t draw_buf;
// Use internal RAM for smaller buffers, PSRAM for larger ones if available
// Option 1: Static buffer in internal RAM
static lv_color_t buf1[SCREEN_WIDTH * 10]; // Declare a buffer for 10 lines
// Option 2: If you have PSRAM, you might allocate larger buffers dynamically
// static lv_color_t *buf1 = (lv_color_t *)ps_malloc(SCREEN_WIDTH * SCREEN_HEIGHT / 10 * sizeof(lv_color_t));
// static lv_color_t *buf2 = (lv_color_t *)ps_malloc(SCREEN_WIDTH * SCREEN_HEIGHT / 10 * sizeof(lv_color_t));

/* TFT_eSPI display driver instance */
TFT_eSPI tft = TFT_eSPI();

/* LVGL Display Flush Callback */
void my_disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t *)color_p, w * h, true); // true for byte swap if LV_COLOR_16_SWAP is enabled
    tft.endWrite();

    lv_disp_flush_ready(disp_drv); // Inform LVGL that flushing is done
}

/* LVGL Touchpad Read Callback */
void my_touchpad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data) {
    uint16_t touchX, touchY;
    bool touched = tft.getTouch(&touchX, &touchY, 600); // 600 is pressure threshold (adjust if needed)

    if (!touched) {
        data->state = LV_INDEV_STATE_REL;
    } else {
        // Optional: Apply calibration/mapping if necessary
        data->point.x = touchX;
        data->point.y = touchY;
        data->state = LV_INDEV_STATE_PR;
        // Serial.printf("Touch: x=%d, y=%d\n", touchX, touchY); // Debug print
    }
}

// Placeholder for UI creation function is now removed, replaced by functions in ui.cpp

void setup() {
    Serial.begin(115200);
    Serial.println("LVGL Arduino Demo for CYD");

    // Initialize LVGL
    lv_init();

    // Initialize TFT_eSPI
    tft.begin();
    tft.setRotation(3); // Set rotation based on ejemplobutton

    // Initialize LVGL display buffers
    // Option 1 (Static buffer):
    lv_disp_draw_buf_init(&draw_buf, buf1, NULL, SCREEN_WIDTH * 10);
    // Option 2 (Dynamic buffer in PSRAM - ensure PSRAM is initialized if used):
    // lv_disp_draw_buf_init(&draw_buf, buf1, buf2, SCREEN_WIDTH * SCREEN_HEIGHT / 10);

    // Initialize LVGL display driver
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = SCREEN_WIDTH;  // Use the corrected dimensions
    disp_drv.ver_res = SCREEN_HEIGHT; // Use the corrected dimensions
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    // Initialize LVGL input device driver (touch)
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register(&indev_drv);

    // Backlight should be controlled by TFT_BL build flag if defined
    // pinMode(TFT_BL, OUTPUT); // Remove explicit control
    // digitalWrite(TFT_BL, HIGH); // Remove explicit control

    // Create the UI elements using the function from ui.cpp
    setup_ui();

    Serial.println("Setup complete.");
}

void loop() {
    // Let LVGL handle tasks (timers, animations, input processing)
    lv_timer_handler();
    delay(5); // Allow ESP32 background tasks to run
}
