/*
 * Translated Arduino Sketch from ui_combined.py
 * Target: ESP32 with LVGL, TFT_eSPI, XPT2046_Touchscreen
 * Screen Resolution: 320x240 (Adjusted from Python script)
 *
 * NOTE: File I/O operations (Shell Viewer, Code Editor) require
 *       implementing SPIFFS or LittleFS handling separately.
 */
#include <Arduino.h> // Add Arduino header
#include <lvgl.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
// #include <SPIFFS.h> // Include if using SPIFFS for file operations
// #include <LittleFS.h> // Include if using LittleFS

// Insertar definiciones de resolución del tutorial
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320

// --- Hardware Configuration (from original sketch) ---
// Se eliminan las definiciones duplicadas ya presentes en el driver de la librería
#define XPT2046_IRQ 36
#define XPT2046_MOSI 32
define XPT2046_MISO 39
#define XPT2046_CLK 25
#define XPT2046_CS 33

// Se eliminan las instancias duplicadas del touchscreen
SPIClass touchscreenSPI = SPIClass(VSPI);
XPT2046_Touchscreen touchscreen(XPT2046_CS, XPT2046_IRQ);

#define DRAW_BUF_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT / 10 * (LV_COLOR_DEPTH / 8))
uint32_t draw_buf[DRAW_BUF_SIZE / 4];

// --- Global LVGL Objects ---
static lv_obj_t * main_scr;
static lv_obj_t * shell_viewer_scr;
static lv_obj_t * practicas_scr;
static lv_obj_t * practica1_scr;
static lv_obj_t * practica2_scr;
static lv_obj_t * herramientas_scr;
static lv_obj_t * simulador_scr;
static lv_obj_t * resistencias_scr;
static lv_obj_t * editor_scr;

// Widgets needing global access (example)
static lv_obj_t * shell_textarea;
static lv_obj_t * editor_code_ta;
static lv_obj_t * editor_kb;

// State for Practica 1
static lv_obj_t * practica1_led;
static lv_obj_t * practica1_switches[3];

// State for Practica 2
static lv_obj_t * practica2_led;
static bool practica2_switch_states[5]; // Corrected variable name

// State for Simulador
static lv_obj_t * simulador_led;
static lv_obj_t * simulador_sw1;
static lv_obj_t * simulador_sw2;
static const char * simulador_func_mode = "AND";
static lv_obj_t * simulador_gate_buttons[6];
const char * simulador_gate_names[] = {"AND", "OR", "XOR", "NAND", "NOR", "XNOR"};

// State for Resistencias
static lv_obj_t * resistencias_ta;
static lv_obj_t * resistencias_color_label;
static lv_obj_t * resistencias_smd_label;
static lv_obj_t * resistencias_color_bands[4]; // Max 4 bands shown

// --- Forward Declarations ---
// Screen creation functions
void create_main_screen_widgets();
void create_shell_viewer_screen();
void create_practicas_screen();
void create_practica1_screen();
void create_practica2_screen();
void create_herramientas_screen();
void create_simulador_screen();
void create_resistencias_screen();
void create_editor_screen();

// Event Handlers
static void main_button_event_handler(lv_event_t * e);
static void shell_refresh_handler(lv_event_t * e);
static void shell_back_handler(lv_event_t * e);
static void practicas_btn_handler(lv_event_t * e);
static void practicas_back_handler(lv_event_t * e);
static void practica1_switch_handler(lv_event_t * e);
static void practica1_back_handler(lv_event_t * e);
static void practica2_switch_handler(lv_event_t * e);
static void practica2_back_handler(lv_event_t * e);
static void herramientas_btn_handler(lv_event_t * e);
static void herramientas_back_handler(lv_event_t * e);
static void simulador_switch_handler(lv_event_t * e);
static void simulador_gate_btn_handler(lv_event_t * e);
static void simulador_back_handler(lv_event_t * e);
static void resistencias_update_handler(lv_event_t * e);
static void resistencias_back_handler(lv_event_t * e);
static void editor_save_handler(lv_event_t * e);
static void editor_back_handler(lv_event_t * e);

// Helper functions
static void update_shell_content();
static void update_simulador_led();
static void update_simulador_button_styles();
static void update_resistencias_display();
static void save_editor_code();


// --- LVGL Driver Functions (from original sketch) ---
void log_print(lv_log_level_t level, const char * buf) {
  LV_UNUSED(level);
  Serial.println(buf);
  Serial.flush();
}

// Revert to using XPT2046_Touchscreen instance directly
void touchscreen_read(lv_indev_t * indev, lv_indev_data_t * data) {
  if (touchscreen.tirqTouched() && touchscreen.touched()) {
    TS_Point p = touchscreen.getPoint();
    // Map coordinates like in the original sketch
    // Adjust these values if needed based on your specific screen calibration
    data->point.x = map(p.x, 200, 3700, 1, SCREEN_WIDTH);
    data->point.y = map(p.y, 240, 3800, 1, SCREEN_HEIGHT);
    data->state = LV_INDEV_STATE_PRESSED;
    /* Serial.print("X = ");
      Serial.print(data->point.x);
      Serial.print(" | Y = ");
      Serial.println(data->point.y); */
  } else {
    data->state = LV_INDEV_STATE_RELEASED;
  }
}


// --- Setup ---
void setup() {
  Serial.begin(115200);
  Serial.println("LVGL Arduino UI Translation Test");

  String LVGL_Arduino = String("LVGL Version: ") + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();
  Serial.println(LVGL_Arduino);

  lv_init();
  lv_log_register_print_cb(log_print);

  // Initialize TFT display - Handled by lv_tft_espi_create below

  // Initialize Touchscreen using XPT2046 library
  // Se eliminan las líneas duplicadas, ya que el driver se encarga de dichas definiciones
  // touchscreenSPI.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
  // touchscreen.begin(touchscreenSPI);
  // touchscreen.setRotation(1); // Match TFT rotation (0-3). Rotation 1 for 320x240 landscape usually.

  // Initialize LVGL display driver (This initializes TFT_eSPI internally)
  lv_display_t * disp = lv_tft_espi_create(SCREEN_WIDTH, SCREEN_HEIGHT, draw_buf, sizeof(draw_buf));
  lv_display_set_rotation(disp, LV_DISPLAY_ROTATION_270); // Set rotation if needed, ensure it matches TFT_eSPI

  // Initialize LVGL input device driver (Touch)
  lv_indev_t * indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(indev, touchscreen_read);

  // --- Initialize File System (Example for SPIFFS) ---
  /*
  if(!SPIFFS.begin(true)){ // Format SPIFFS if not mounted
       Serial.println("SPIFFS Mount Failed");
       return;
  }
  Serial.println("SPIFFS Mounted.");
  // You can list files here for debugging:
  // File root = SPIFFS.open("/");
  // File file = root.openNextFile();
  // while(file){ ... }
  */

  // Get the main screen reference
  main_scr = lv_screen_active();

  // Create the initial UI elements on the main screen
  create_main_screen_widgets();

  Serial.println("Setup complete.");
}

// --- Loop ---
void loop() {
  lv_task_handler();
  lv_tick_inc(5);
  delay(5);
}

// --- Screen Creation Functions ---

void create_main_screen_widgets() {
    lv_obj_t * cont = lv_obj_create(main_scr);
    lv_obj_set_size(cont, SCREEN_WIDTH, SCREEN_HEIGHT);
    lv_obj_center(cont);
    lv_obj_remove_style(cont, NULL, LV_PART_SCROLLBAR); // Remove scrollbars if they appear
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE); // Make container non-scrollable
    lv_obj_set_style_bg_color(cont, lv_color_hex(0xE0E0E0), 0); // Light grey background

    const char* button_texts[] = {
        "Visualizer", "Practicas", "Editor", "Herramientas" // Shortened for clarity
    };

    lv_coord_t btn_w = (SCREEN_WIDTH / 2) - 25; // Button width (fit 2 per row)
    lv_coord_t btn_h = (SCREEN_HEIGHT / 2) - 30; // Button height (fit 2 per row)

    for (int i = 0; i < 4; i++) {
        lv_obj_t * btn = lv_button_create(cont);
        lv_obj_set_size(btn, btn_w, btn_h);
        // Align in a 2x2 grid
        lv_obj_align(btn, LV_ALIGN_TOP_LEFT, // Use LV_ALIGN_...
                     (i % 2) * (btn_w + 15) + 15, // X position
                     (i / 2) * (btn_h + 15) + 15); // Y position
        lv_obj_add_event_cb(btn, main_button_event_handler, LV_EVENT_CLICKED, (void*)button_texts[i]); // Use LV_EVENT_...

        lv_obj_t * label = lv_label_create(btn);
        lv_label_set_text(label, button_texts[i]);
        lv_obj_center(label);
    }
}

void create_shell_viewer_screen() {
    if (!shell_viewer_scr) {
        shell_viewer_scr = lv_obj_create(NULL); // Create a new screen
    }
    lv_screen_load(shell_viewer_scr);
    lv_obj_clean(shell_viewer_scr); // Clear previous widgets if re-entering

    // Text area
    shell_textarea = lv_textarea_create(shell_viewer_scr);
    lv_obj_set_size(shell_textarea, SCREEN_WIDTH - 20, SCREEN_HEIGHT - 100);
    lv_obj_align(shell_textarea, LV_ALIGN_TOP_MID, 0, 10); // Use LV_ALIGN_...
    lv_textarea_set_placeholder_text(shell_textarea, "Loading shell output...");

    // Refresh Button
    lv_obj_t * btn_refresh = lv_button_create(shell_viewer_scr);
    lv_obj_set_size(btn_refresh, 120, 40);
    lv_obj_align(btn_refresh, LV_ALIGN_BOTTOM_RIGHT, -10, -10); // Use LV_ALIGN_...
    lv_obj_add_event_cb(btn_refresh, shell_refresh_handler, LV_EVENT_CLICKED, NULL); // Use LV_EVENT_...
    lv_obj_t * label_refresh = lv_label_create(btn_refresh);
    lv_label_set_text(label_refresh, "Actualizar");
    lv_obj_center(label_refresh);

    // Back Button
    lv_obj_t * btn_back = lv_button_create(shell_viewer_scr);
    lv_obj_set_size(btn_back, 120, 40);
    lv_obj_align(btn_back, LV_ALIGN_BOTTOM_LEFT, 10, -10); // Use LV_ALIGN_...
    lv_obj_add_event_cb(btn_back, shell_back_handler, LV_EVENT_CLICKED, NULL); // Use LV_EVENT_...
    lv_obj_t * label_back = lv_label_create(btn_back);
    lv_label_set_text(label_back, "Regresar");
    lv_obj_center(label_back);

    // Initial content load
    update_shell_content();
}

void create_practicas_screen() {
    if (!practicas_scr) {
        practicas_scr = lv_obj_create(NULL);
    }
    lv_screen_load(practicas_scr);
    lv_obj_clean(practicas_scr);

    lv_coord_t btn_w = SCREEN_WIDTH - 60;
    lv_coord_t btn_h = 60;

    // Button Practica 1
    lv_obj_t * btn1 = lv_button_create(practicas_scr);
    lv_obj_set_size(btn1, btn_w, btn_h);
    lv_obj_align(btn1, LV_ALIGN_CENTER, 0, -40); // Use LV_ALIGN_...
    lv_obj_add_event_cb(btn1, practicas_btn_handler, LV_EVENT_CLICKED, (void*)"Practica1"); // Use LV_EVENT_...
    lv_obj_t * label1 = lv_label_create(btn1);
    lv_label_set_text(label1, "Practica 1: Jueces");
    lv_obj_center(label1);

    // Button Practica 2
    lv_obj_t * btn2 = lv_button_create(practicas_scr);
    lv_obj_set_size(btn2, btn_w, btn_h);
    lv_obj_align(btn2, LV_ALIGN_CENTER, 0, 40); // Use LV_ALIGN_...
    lv_obj_add_event_cb(btn2, practicas_btn_handler, LV_EVENT_CLICKED, (void*)"Practica2"); // Use LV_EVENT_...
    lv_obj_t * label2 = lv_label_create(btn2);
    lv_label_set_text(label2, "Practica 2: Segurid"); // Shortened
    lv_obj_center(label2);

    // Back Button
    lv_obj_t * btn_back = lv_button_create(practicas_scr);
    lv_obj_set_size(btn_back, 100, 40);
    lv_obj_align(btn_back, LV_ALIGN_BOTTOM_LEFT, 10, -10); // Use LV_ALIGN_...
    lv_obj_add_event_cb(btn_back, practicas_back_handler, LV_EVENT_CLICKED, NULL); // Use LV_EVENT_...
    lv_obj_t * label_back = lv_label_create(btn_back);
    lv_label_set_text(label_back, "Regresar");
    lv_obj_center(label_back);
}

void create_practica1_screen() {
    if (!practica1_scr) {
        practica1_scr = lv_obj_create(NULL);
    }
    lv_screen_load(practica1_scr);
    lv_obj_clean(practica1_scr);

    // LED
    practica1_led = lv_led_create(practica1_scr);
    lv_obj_set_size(practica1_led, 50, 50);
    lv_obj_align(practica1_led, LV_ALIGN_TOP_MID, 0, 20); // Use LV_ALIGN_...
    lv_led_set_color(practica1_led, lv_color_hex(0xff0000)); // Red initially
    lv_led_off(practica1_led); // Start off

    // Switches
    lv_coord_t sw_y = 80;
    lv_coord_t sw_spacing = 80;
    lv_coord_t sw_start_x = (SCREEN_WIDTH - (2 * sw_spacing)) / 2 - (lv_obj_get_width(lv_switch_create(NULL))/2); // Center the group

    for (int i = 0; i < 3; i++) {
        practica1_switches[i] = lv_switch_create(practica1_scr);
        lv_obj_align(practica1_switches[i], LV_ALIGN_TOP_LEFT, sw_start_x + i * sw_spacing, sw_y); // Use LV_ALIGN_...
        lv_obj_add_event_cb(practica1_switches[i], practica1_switch_handler, LV_EVENT_VALUE_CHANGED, NULL); // Use LV_EVENT_...
    }

    // Back Button
    lv_obj_t * btn_back = lv_button_create(practica1_scr);
    lv_obj_set_size(btn_back, 100, 40);
    lv_obj_align(btn_back, LV_ALIGN_BOTTOM_LEFT, 10, -10); // Use LV_ALIGN_...
    lv_obj_add_event_cb(btn_back, practica1_back_handler, LV_EVENT_CLICKED, NULL); // Use LV_EVENT_...
    lv_obj_t * label_back = lv_label_create(btn_back);
    lv_label_set_text(label_back, "Regresar");
    lv_obj_center(label_back);

    // Initial LED state check
    practica1_switch_handler(NULL);
}

// Replacing entire function to ensure correctness
void create_practica2_screen() {
    if (!practica2_scr) {
        practica2_scr = lv_obj_create(NULL);
    }
    lv_screen_load(practica2_scr);
    lv_obj_clean(practica2_scr);
    lv_obj_set_style_bg_color(practica2_scr, lv_color_hex(0x40E0D0), 0); // Turquoise background

    // LED
    practica2_led = lv_led_create(practica2_scr);
    lv_obj_set_size(practica2_led, 40, 40);
    lv_obj_align(practica2_led, LV_ALIGN_TOP_MID, 0, 15); // Use LV_ALIGN_TOP_MID
    lv_led_set_color(practica2_led, lv_color_hex(0xaaaaaa)); // Grey initially
    lv_led_off(practica2_led);

    // Switches
    const char * switch_texts[] = {"Op1", "Op2", "Op3", "Op4", "Op5"}; // Shortened
    lv_coord_t sw_width = 50;
    lv_coord_t total_sw_width = 5 * sw_width;
    lv_coord_t spacing = (SCREEN_WIDTH - total_sw_width - 20) / 4; // Space between switches
    lv_coord_t start_x = 10;
    lv_coord_t y_pos = 70;

    for (int i = 0; i < 5; i++) {
        lv_obj_t * sw = lv_switch_create(practica2_scr);
        lv_obj_set_pos(sw, start_x + i * (sw_width + spacing), y_pos);
        practica2_switch_states[i] = lv_obj_has_state(sw, LV_STATE_CHECKED); // Init state
        // Pass index 'i' as user data
        lv_obj_add_event_cb(sw, practica2_switch_handler, LV_EVENT_VALUE_CHANGED, (void*)((intptr_t)i)); // Use LV_EVENT_VALUE_CHANGED

        lv_obj_t * lbl = lv_label_create(practica2_scr);
        lv_label_set_text(lbl, switch_texts[i]);
        lv_obj_align_to(lbl, sw, LV_ALIGN_OUT_BOTTOM_MID, 0, 5); // Use LV_ALIGN_OUT_BOTTOM_MID
    }

    // Back Button
    lv_obj_t * btn_back = lv_button_create(practica2_scr);
    lv_obj_set_size(btn_back, 100, 40);
    lv_obj_align(btn_back, LV_ALIGN_BOTTOM_LEFT, 10, -10); // Use LV_ALIGN_BOTTOM_LEFT
    lv_obj_add_event_cb(btn_back, practica2_back_handler, LV_EVENT_CLICKED, NULL); // Use LV_EVENT_CLICKED
    lv_obj_t * label_back = lv_label_create(btn_back);
    lv_label_set_text(label_back, "Regresar");
    lv_obj_center(label_back);

    // Initial LED state check
    practica2_switch_handler(NULL); // Call with NULL to check initial state
}


void create_herramientas_screen() {
    if (!herramientas_scr) {
        herramientas_scr = lv_obj_create(NULL);
    }
    lv_screen_load(herramientas_scr);
    lv_obj_clean(herramientas_scr);

    lv_coord_t btn_w = SCREEN_WIDTH - 60;
    lv_coord_t btn_h = 50;
    lv_coord_t spacing = 15;

    // Button Simulador
    lv_obj_t * btn_sim = lv_button_create(herramientas_scr);
    lv_obj_set_size(btn_sim, btn_w, btn_h);
    lv_obj_align(btn_sim, LV_ALIGN_TOP_MID, 0, 10); // Use LV_ALIGN_...
    lv_obj_add_event_cb(btn_sim, herramientas_btn_handler, LV_EVENT_CLICKED, (void*)"Simulador"); // Use LV_EVENT_...
    lv_obj_t * label_sim = lv_label_create(btn_sim);
    lv_label_set_text(label_sim, "Simulador Compuertas");
    lv_obj_center(label_sim);

    // Button Resistencias
    lv_obj_t * btn_res = lv_button_create(herramientas_scr);
    lv_obj_set_size(btn_res, btn_w, btn_h);
    lv_obj_align_to(btn_res, btn_sim, LV_ALIGN_OUT_BOTTOM_MID, 0, spacing); // Use LV_ALIGN_...
    lv_obj_add_event_cb(btn_res, herramientas_btn_handler, LV_EVENT_CLICKED, (void*)"Resistencias"); // Use LV_EVENT_...
    lv_obj_t * label_res = lv_label_create(btn_res);
    lv_label_set_text(label_res, "Resistencias Colores");
    lv_obj_center(label_res);

    // Button Editor (Removed from Python, re-added based on main screen)
    lv_obj_t * btn_edit = lv_button_create(herramientas_scr);
    lv_obj_set_size(btn_edit, btn_w, btn_h);
    lv_obj_align_to(btn_edit, btn_res, LV_ALIGN_OUT_BOTTOM_MID, 0, spacing); // Use LV_ALIGN_...
    lv_obj_add_event_cb(btn_edit, herramientas_btn_handler, LV_EVENT_CLICKED, (void*)"Editor"); // Use LV_EVENT_...
    lv_obj_t * label_edit = lv_label_create(btn_edit);
    lv_label_set_text(label_edit, "Editor de Codigo");
    lv_obj_center(label_edit);


    // Back Button
    lv_obj_t * btn_back = lv_button_create(herramientas_scr);
    lv_obj_set_size(btn_back, 100, 40);
    lv_obj_align(btn_back, LV_ALIGN_BOTTOM_LEFT, 10, -10); // Use LV_ALIGN_...
    lv_obj_add_event_cb(btn_back, herramientas_back_handler, LV_EVENT_CLICKED, NULL); // Use LV_EVENT_...
    lv_obj_t * label_back = lv_label_create(btn_back);
    lv_label_set_text(label_back, "Regresar");
    lv_obj_center(label_back);
}

void create_simulador_screen() {
    if (!simulador_scr) {
        simulador_scr = lv_obj_create(NULL);
    }
    lv_screen_load(simulador_scr);
    lv_obj_clean(simulador_scr);
    lv_obj_set_style_bg_color(simulador_scr, lv_color_hex(0xFFDAB9), 0); // Pastel orange

    // LED
    simulador_led = lv_led_create(simulador_scr);
    lv_obj_set_size(simulador_led, 40, 40);
    lv_obj_align(simulador_led, LV_ALIGN_TOP_MID, 0, 10); // Use LV_ALIGN_...
    lv_led_set_color(simulador_led, lv_color_hex(0xff0000)); // Red initially
    lv_led_off(simulador_led);

    // Switches
    simulador_sw1 = lv_switch_create(simulador_scr);
    lv_obj_align(simulador_sw1, LV_ALIGN_TOP_LEFT, 40, 60); // Use LV_ALIGN_...
    lv_obj_add_event_cb(simulador_sw1, simulador_switch_handler, LV_EVENT_VALUE_CHANGED, NULL); // Use LV_EVENT_...

    simulador_sw2 = lv_switch_create(simulador_scr);
    lv_obj_align(simulador_sw2, LV_ALIGN_TOP_RIGHT, -40, 60); // Use LV_ALIGN_...
    lv_obj_add_event_cb(simulador_sw2, simulador_switch_handler, LV_EVENT_VALUE_CHANGED, NULL); // Use LV_EVENT_...

    // Gate Buttons (2 rows of 3)
    lv_coord_t btn_w = 80;
    lv_coord_t btn_h = 35;
    lv_coord_t spacing = (SCREEN_WIDTH - 3 * btn_w) / 4;
    lv_coord_t y_pos1 = 110;
    lv_coord_t y_pos2 = 160;

    for (int i = 0; i < 6; i++) {
        simulador_gate_buttons[i] = lv_button_create(simulador_scr);
        lv_obj_set_size(simulador_gate_buttons[i], btn_w, btn_h);
        lv_coord_t current_y = (i < 3) ? y_pos1 : y_pos2;
        lv_coord_t current_x = spacing + (i % 3) * (btn_w + spacing);
        lv_obj_set_pos(simulador_gate_buttons[i], current_x, current_y);

        lv_obj_t * label = lv_label_create(simulador_gate_buttons[i]);
        lv_label_set_text(label, simulador_gate_names[i]);
        lv_obj_center(label);
        // Pass gate name string as user data
        lv_obj_add_event_cb(simulador_gate_buttons[i], simulador_gate_btn_handler, LV_EVENT_CLICKED, (void*)simulador_gate_names[i]); // Use LV_EVENT_...
    }

    // Back Button
    lv_obj_t * btn_back = lv_button_create(simulador_scr);
    lv_obj_set_size(btn_back, 100, 40);
    lv_obj_align(btn_back, LV_ALIGN_BOTTOM_LEFT, 10, -10); // Use LV_ALIGN_...
    lv_obj_add_event_cb(btn_back, simulador_back_handler, LV_EVENT_CLICKED, NULL); // Use LV_EVENT_...
    lv_obj_t * label_back = lv_label_create(btn_back);
    lv_label_set_text(label_back, "Regresar");
    lv_obj_center(label_back);

    // Initial state
    update_simulador_button_styles();
    update_simulador_led();
}

// Replacing entire function to ensure correctness
void create_resistencias_screen() {
    if (!resistencias_scr) {
        resistencias_scr = lv_obj_create(NULL);
    }
    lv_screen_load(resistencias_scr);
    lv_obj_clean(resistencias_scr);
    lv_obj_set_style_bg_color(resistencias_scr, lv_color_white(), 0); // White background

    // Text Area for input
    resistencias_ta = lv_textarea_create(resistencias_scr);
    lv_obj_set_size(resistencias_ta, SCREEN_WIDTH - 40, 40);
    lv_textarea_set_one_line(resistencias_ta, true);
    lv_textarea_set_accepted_chars(resistencias_ta, "0123456789"); // Only numbers
    lv_textarea_set_max_length(resistencias_ta, 6); // Limit length
    lv_obj_align(resistencias_ta, LV_ALIGN_TOP_MID, 0, 10); // Use LV_ALIGN_TOP_MID
    lv_textarea_set_placeholder_text(resistencias_ta, "Valor (ej. 220)");
    lv_obj_add_event_cb(resistencias_ta, resistencias_update_handler, LV_EVENT_VALUE_CHANGED, NULL); // Use LV_EVENT_VALUE_CHANGED

    // Label for Colors
    resistencias_color_label = lv_label_create(resistencias_scr);
    lv_label_set_long_mode(resistencias_color_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(resistencias_color_label, SCREEN_WIDTH - 20);
    lv_obj_align_to(resistencias_color_label, resistencias_ta, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10); // Use LV_ALIGN_OUT_BOTTOM_LEFT
    lv_label_set_text(resistencias_color_label, "Colores:");

    // Label for SMD Code
    resistencias_smd_label = lv_label_create(resistencias_scr);
    lv_obj_set_width(resistencias_smd_label, SCREEN_WIDTH - 20);
    lv_obj_align_to(resistencias_smd_label, resistencias_color_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5); // Use LV_ALIGN_OUT_BOTTOM_LEFT
    lv_label_set_text(resistencias_smd_label, "SMD:");

    // Color Band Placeholders (create objects, color set in update)
    lv_coord_t band_width = 30;
    lv_coord_t band_height = 60;
    lv_coord_t spacing = 8;
    lv_coord_t total_width = 4 * band_width + 3 * spacing;
    lv_coord_t start_x = (SCREEN_WIDTH - total_width) / 2;
    lv_coord_t y_pos = 120;

    for (int i = 0; i < 4; i++) {
        resistencias_color_bands[i] = lv_obj_create(resistencias_scr);
        lv_obj_set_size(resistencias_color_bands[i], band_width, band_height);
        lv_obj_set_pos(resistencias_color_bands[i], start_x + i * (band_width + spacing), y_pos);
        lv_obj_set_style_border_width(resistencias_color_bands[i], 1, 0);
        lv_obj_set_style_border_color(resistencias_color_bands[i], lv_color_black(), 0);
        lv_obj_set_style_bg_opa(resistencias_color_bands[i], LV_OPA_TRANSP, 0); // Initially transparent
    }

    // Back Button
    lv_obj_t * btn_back = lv_button_create(resistencias_scr);
    lv_obj_set_size(btn_back, 100, 40);
    lv_obj_align(btn_back, LV_ALIGN_BOTTOM_LEFT, 10, -10); // Use LV_ALIGN_BOTTOM_LEFT
    lv_obj_add_event_cb(btn_back, resistencias_back_handler, LV_EVENT_CLICKED, NULL); // Use LV_EVENT_CLICKED
    lv_obj_t * label_back = lv_label_create(btn_back);
    lv_label_set_text(label_back, "Regresar");
    lv_obj_center(label_back);

    // Initial update
    update_resistencias_display();
}

void create_editor_screen() {
    if (!editor_scr) {
        editor_scr = lv_obj_create(NULL);
    }
    lv_screen_load(editor_scr);
    lv_obj_clean(editor_scr);

    // Code Input Text Area
    editor_code_ta = lv_textarea_create(editor_scr);
    lv_obj_set_size(editor_code_ta, SCREEN_WIDTH - 20, SCREEN_HEIGHT - 130); // Smaller height to fit keyboard
    lv_obj_align(editor_code_ta, LV_ALIGN_TOP_MID, 0, 10); // Use LV_ALIGN_...
    lv_textarea_set_placeholder_text(editor_code_ta, "Escribe tu codigo aqui...");

    // Keyboard
    editor_kb = lv_keyboard_create(editor_scr);
    lv_obj_set_size(editor_kb, SCREEN_WIDTH, 110); // Adjusted size
    lv_obj_align(editor_kb, LV_ALIGN_BOTTOM_MID, 0, 0); // Use LV_ALIGN_...
    lv_keyboard_set_textarea(editor_kb, editor_code_ta);

    // Save Button
    lv_obj_t * btn_save = lv_button_create(editor_scr);
    lv_obj_set_size(btn_save, 100, 40);
    // Position above keyboard, right side
    lv_obj_align(btn_save, LV_ALIGN_BOTTOM_RIGHT, -10, -120); // Use LV_ALIGN_...
    lv_obj_add_event_cb(btn_save, editor_save_handler, LV_EVENT_CLICKED, NULL); // Use LV_EVENT_...
    lv_obj_t * label_save = lv_label_create(btn_save);
    lv_label_set_text(label_save, "Guardar");
    lv_obj_center(label_save);

    // Back Button
    lv_obj_t * btn_back = lv_button_create(editor_scr);
    lv_obj_set_size(btn_back, 100, 40);
     // Position above keyboard, left side
    lv_obj_align(btn_back, LV_ALIGN_BOTTOM_LEFT, 10, -120); // Use LV_ALIGN_...
    lv_obj_add_event_cb(btn_back, editor_back_handler, LV_EVENT_CLICKED, NULL); // Use LV_EVENT_...
    lv_obj_t * label_back = lv_label_create(btn_back);
    lv_label_set_text(label_back, "Regresar");
    lv_obj_center(label_back);
}


// --- Event Handlers ---

static void main_button_event_handler(lv_event_t * e) {
    const char* target_screen = (const char*)lv_event_get_user_data(e);

    if (strcmp(target_screen, "Visualizer") == 0) {
        create_shell_viewer_screen();
    } else if (strcmp(target_screen, "Practicas") == 0) {
        create_practicas_screen();
    } else if (strcmp(target_screen, "Editor") == 0) {
         create_editor_screen();
    } else if (strcmp(target_screen, "Herramientas") == 0) {
        create_herramientas_screen();
    }
}

static void shell_refresh_handler(lv_event_t * e) {
    update_shell_content();
}

static void shell_back_handler(lv_event_t * e) {
    lv_screen_load(main_scr); // Go back to main screen
}

static void practicas_btn_handler(lv_event_t * e) {
    const char* target = (const char*)lv_event_get_user_data(e);
    if (strcmp(target, "Practica1") == 0) {
        create_practica1_screen();
    } else if (strcmp(target, "Practica2") == 0) {
        create_practica2_screen();
    }
}

static void practicas_back_handler(lv_event_t * e) {
    lv_screen_load(main_scr);
}

static void practica1_switch_handler(lv_event_t * e) {
    int count = 0;
    for (int i = 0; i < 3; i++) {
        if (practica1_switches[i] && lv_obj_has_state(practica1_switches[i], LV_STATE_CHECKED)) {
            count++;
        }
    }

    if (practica1_led) {
        if (count >= 2) {
            lv_led_set_color(practica1_led, lv_color_hex(0x00ff00)); // Green
            lv_led_on(practica1_led);
            // TODO: Add GPIO control here if needed (e.g., digitalWrite(27, HIGH);)
        } else {
            lv_led_set_color(practica1_led, lv_color_hex(0xff0000)); // Red
            lv_led_on(practica1_led); // Keep LED visually on but red
             // TODO: Add GPIO control here if needed (e.g., digitalWrite(27, LOW);)
        }
    }
}


static void practica1_back_handler(lv_event_t * e) {
    create_practicas_screen(); // Go back to practicas menu
}

static void practica2_switch_handler(lv_event_t * e) {
    intptr_t idx = -1;
    lv_obj_t * sw = NULL; // Initialize sw to NULL
    if (e) { // If called by an event, get the index and target
       idx = (intptr_t)lv_event_get_user_data(e);
       sw = (lv_obj_t *)lv_event_get_target(e); // Cast void* to lv_obj_t*
       if (idx >= 0 && idx < 5 && sw) { // Check if sw is not NULL after cast
           practica2_switch_states[idx] = lv_obj_has_state(sw, LV_STATE_CHECKED);
           Serial.printf("Switch %d toggled, state: %d\n", (int)idx + 1, practica2_switch_states[idx]); // Print 1-based index
       }
    }

    // Update LED based on current states
    int checked_count = 0;
    for(int i=0; i<5; i++) {
        if(practica2_switch_states[i]) {
            checked_count++;
        }
    }

    if (practica2_led) {
        if (practica2_switch_states[2] || checked_count >= 2) { // Switch 3 (index 2) or 2+ switches
            lv_led_set_color(practica2_led, lv_color_hex(0x00ff00)); // Green
            lv_led_on(practica2_led);
        } else {
            lv_led_set_color(practica2_led, lv_color_hex(0xff0000)); // Red
             lv_led_on(practica2_led); // Keep LED visually on but red
        }
    }
}


static void practica2_back_handler(lv_event_t * e) {
    create_practicas_screen();
}

static void herramientas_btn_handler(lv_event_t * e) {
    const char* target = (const char*)lv_event_get_user_data(e);
    if (strcmp(target, "Simulador") == 0) {
        create_simulador_screen();
    } else if (strcmp(target, "Resistencias") == 0) {
        create_resistencias_screen();
    } else if (strcmp(target, "Editor") == 0) {
        create_editor_screen();
    }
}

static void herramientas_back_handler(lv_event_t * e) {
    lv_screen_load(main_scr);
}

static void simulador_switch_handler(lv_event_t * e) {
    update_simulador_led();
}

static void simulador_gate_btn_handler(lv_event_t * e) {
    simulador_func_mode = (const char*)lv_event_get_user_data(e);
    Serial.printf("Simulador mode set to: %s\n", simulador_func_mode);
    update_simulador_button_styles();
    update_simulador_led();
}

static void simulador_back_handler(lv_event_t * e) {
    create_herramientas_screen();
}

static void resistencias_update_handler(lv_event_t * e) {
    update_resistencias_display();
}

static void resistencias_back_handler(lv_event_t * e) {
    create_herramientas_screen();
}

static void editor_save_handler(lv_event_t * e) {
    save_editor_code();
    // Optionally, provide feedback to the user (e.g., temporary label)
    lv_obj_t * popup = lv_label_create(lv_screen_active()); // Create on current screen
    lv_label_set_text(popup, "Guardado!");
    lv_obj_align(popup, LV_ALIGN_CENTER, 0, 0); // Use LV_ALIGN_...
    // Simple deletion after delay using a timer might be more robust if available/implemented
    // For now, just fade out. User needs to manage deletion if object persists.
    lv_obj_fade_out(popup, 1500, 500); // Fade out after 0.5s, delay 1.5s
    // lv_obj_delete_delayed(popup, 2000); // Example if available
}


static void editor_back_handler(lv_event_t * e) {
    create_herramientas_screen();
}


// --- Helper Functions ---

static void update_shell_content() {
    if (!shell_textarea) return;

    const char* filename = "/shell_output.txt"; // Prepend with '/' for SPIFFS/LittleFS root
    String content = "Error: File system not implemented or file not found.";

    // --- SPIFFS/LittleFS Implementation Needed ---
    /*
    if (SPIFFS.exists(filename)) {
        File file = SPIFFS.open(filename, "r");
        if (file) {
            content = file.readString();
            file.close();
            if (content.length() == 0) {
                content = "(Archivo vacio)";
            }
        } else {
            content = "Error al abrir el archivo.";
        }
    } else {
         content = "shell_output.txt no encontrado.";
    }
    */
    // --- End File System Section ---

    lv_textarea_set_text(shell_textarea, content.c_str());
}

static void update_simulador_led() {
    if (!simulador_led || !simulador_sw1 || !simulador_sw2) return;

    bool s1 = lv_obj_has_state(simulador_sw1, LV_STATE_CHECKED);
    bool s2 = lv_obj_has_state(simulador_sw2, LV_STATE_CHECKED);
    bool result = false;

    if (strcmp(simulador_func_mode, "AND") == 0) result = s1 && s2;
    else if (strcmp(simulador_func_mode, "OR") == 0) result = s1 || s2;
    else if (strcmp(simulador_func_mode, "XOR") == 0) result = s1 ^ s2;
    else if (strcmp(simulador_func_mode, "NAND") == 0) result = !(s1 && s2);
    else if (strcmp(simulador_func_mode, "NOR") == 0) result = !(s1 || s2);
    else if (strcmp(simulador_func_mode, "XNOR") == 0) result = !(s1 ^ s2);

    if (result) {
        lv_led_set_color(simulador_led, lv_color_hex(0x00ff00)); // Green
        lv_led_on(simulador_led);
    } else {
        lv_led_set_color(simulador_led, lv_color_hex(0xff0000)); // Red
        lv_led_on(simulador_led); // Keep LED visually on but red
    }
}

static void update_simulador_button_styles() {
    for (int i = 0; i < 6; i++) {
        if (simulador_gate_buttons[i]) {
            if (strcmp(simulador_func_mode, simulador_gate_names[i]) == 0) {
                lv_obj_set_style_bg_color(simulador_gate_buttons[i], lv_color_hex(0x00ff00), 0); // Green highlight
            } else {
                 // Use default button color (might need to get theme default)
                 lv_obj_set_style_bg_color(simulador_gate_buttons[i], lv_theme_get_color_primary(simulador_gate_buttons[i]), 0);
            }
        }
    }
}

// Color definitions for Resistor bands
const char* color_names[] = {"Negro", "Marron", "Rojo", "Naranja", "Amarillo", "Verde", "Azul", "Morado", "Gris", "Blanco", "Dorado", "Plata"};
uint32_t color_hex_codes[] = {
    0x000000, 0x8B4513, 0xFF0000, 0xFFA500, 0xFFFF00, 0x008000,
    0x0000FF, 0x800080, 0x808080, 0xFFFFFF, 0xFFD700, 0xC0C0C0
};
int num_colors = sizeof(color_names) / sizeof(color_names[0]);

// Function to get color index by name
int get_color_index(const char* name) {
    for (int i = 0; i < num_colors; i++) {
        if (strcmp(color_names[i], name) == 0) {
            return i;
        }
    }
    return -1; // Not found
}


static void update_resistencias_display() {
    if (!resistencias_ta || !resistencias_color_label || !resistencias_smd_label) return;

    const char* number_str = lv_textarea_get_text(resistencias_ta);
    long value = atol(number_str); // Convert string to long integer
    char color_str_buffer[100] = "Colores: ";
    char smd_str_buffer[20] = "SMD: ";
    bool valid = false;

    // Clear previous bands
    for(int i=0; i<4; i++) {
         if(resistencias_color_bands[i]) lv_obj_set_style_bg_opa(resistencias_color_bands[i], LV_OPA_TRANSP, 0);
    }

    if (strlen(number_str) > 0 && value >= 0) {
        char s_val[15];
        sprintf(s_val, "%ld", value);
        int len = strlen(s_val);
        int band1_idx = -1, band2_idx = -1, band3_idx = -1, band4_idx = -1; // Tolerancia (Dorado por defecto)

        if (value == 0) { // Special case for 0 Ohm
             band1_idx = get_color_index("Negro");
             band2_idx = get_color_index("Negro");
             band3_idx = get_color_index("Negro");
             band4_idx = get_color_index("Dorado");
             sprintf(smd_str_buffer, "SMD: 0");
             valid = true;
        } else if (len >= 1) {
            band1_idx = s_val[0] - '0'; // First digit
            if (len == 1) { // e.g., 5 Ohms -> Verde, Negro, Dorado, Dorado
                band2_idx = 0; // Second digit is 0
                band3_idx = get_color_index("Dorado"); // Multiplier 0.1
                sprintf(smd_str_buffer, "SMD: %cR", s_val[0]);
            } else { // len >= 2
                band2_idx = s_val[1] - '0'; // Second digit
                int multiplier_power = len - 2;
                if (multiplier_power >= 0 && multiplier_power <= 9) {
                    band3_idx = multiplier_power; // Multiplier color index
                } else {
                    // Handle cases like 1.5 Ohm (not directly supported by simple logic)
                    // Or values needing Silver multiplier (0.01)
                    valid = false; // Mark as invalid for now
                }
                 // Generate SMD code (e.g., 220 -> 221, 4700 -> 472, 10 -> 100)
                sprintf(smd_str_buffer, "SMD: %c%c%d", s_val[0], s_val[1], multiplier_power);
            }
            band4_idx = get_color_index("Dorado"); // Default tolerance
            valid = (band1_idx >= 0 && band1_idx <= 9 &&
                     band2_idx >= 0 && band2_idx <= 9 &&
                     band3_idx >= 0 && band3_idx < num_colors && // Check multiplier index validity
                     band4_idx != -1);
        }


        if (valid) {
            sprintf(color_str_buffer, "Colores: %s, %s, %s, %s",
                    color_names[band1_idx], color_names[band2_idx], color_names[band3_idx], color_names[band4_idx]);

            int indices[] = {band1_idx, band2_idx, band3_idx, band4_idx};
            for(int i=0; i<4; i++) {
                if(resistencias_color_bands[i] && indices[i] != -1) {
                    lv_obj_set_style_bg_color(resistencias_color_bands[i], lv_color_hex(color_hex_codes[indices[i]]), 0);
                    lv_obj_set_style_bg_opa(resistencias_color_bands[i], LV_OPA_COVER, 0);
                     // Add black border for light colors like white/yellow/grey/silver
                    if (indices[i] >= 8) {
                         lv_obj_set_style_border_color(resistencias_color_bands[i], lv_color_black(), 0);
                         lv_obj_set_style_border_width(resistencias_color_bands[i], 1, 0);
                    } else {
                         lv_obj_set_style_border_opa(resistencias_color_bands[i], LV_OPA_TRANSP, 0);
                    }
                }
            }
        } else {
             sprintf(color_str_buffer, "Colores: Valor invalido");
             sprintf(smd_str_buffer, "SMD:");
        }

    } else {
        sprintf(color_str_buffer, "Colores:");
        sprintf(smd_str_buffer, "SMD:");
    }

    lv_label_set_text(resistencias_color_label, color_str_buffer);
    lv_label_set_text(resistencias_smd_label, smd_str_buffer);
}


static void save_editor_code() {
    if (!editor_code_ta) return;

    const char* code = lv_textarea_get_text(editor_code_ta);
    const char* filename = "/user_code.py"; // Use root for SPIFFS/LittleFS

    Serial.printf("Attempting to save code to %s...\n", filename);

    // --- SPIFFS/LittleFS Implementation Needed ---
    /*
    File file = SPIFFS.open(filename, "w"); // Open for writing (creates if doesn't exist)
    if (!file) {
        Serial.println("Error opening file for writing");
        // TODO: Show error message on screen?
        return;
    }
    if (file.print(code)) {
        Serial.println("Code saved successfully.");
    } else {
        Serial.println("Error writing to file.");
         // TODO: Show error message on screen?
    }
    file.close();
    */
    // --- End File System Section ---

     Serial.println("File system write operation needs implementation."); // Placeholder message
}
