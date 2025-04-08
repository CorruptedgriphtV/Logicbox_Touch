#include "ui.h"
#include <Arduino.h> // For Serial prints, pinMode, digitalWrite etc.

// Define global screen objects
lv_obj_t * main_scr;
lv_obj_t * shell_viewer_scr = NULL; // Initialize to NULL
lv_obj_t * practicas_scr = NULL;
lv_obj_t * practica1_scr = NULL;
lv_obj_t * practica2_scr = NULL;
lv_obj_t * herramientas_scr = NULL;
lv_obj_t * simulador_scr = NULL;
lv_obj_t * resistencias_scr = NULL;
lv_obj_t * editor_scr = NULL;

// --- Event Handlers ---

// Generic handler to load a screen stored in event user data
void load_screen_event_handler(lv_event_t * e) {
    lv_obj_t * screen_to_load = (lv_obj_t *)lv_event_get_user_data(e);
    if (screen_to_load) {
        lv_scr_load(screen_to_load);
    }
}

// Handler for main screen buttons
void main_button_event_handler(lv_event_t * e) {
    lv_obj_t * btn = lv_event_get_target(e);
    lv_obj_t * label = lv_obj_get_child(btn, 0);
    const char * text = lv_label_get_text(label);

    if (strcmp(text, "Visualizer") == 0) {
        if (!shell_viewer_scr) create_shell_viewer_screen(); // Create if not exists
        lv_scr_load(shell_viewer_scr);
    } else if (strcmp(text, "Practicas Precargadas") == 0) {
        if (!practicas_scr) create_practicas_screen();
        lv_scr_load(practicas_scr);
    } else if (strcmp(text, "Editor de Codigo") == 0) {
         if (!editor_scr) create_editor_screen();
         lv_scr_load(editor_scr);
    } else if (strcmp(text, "Herramientas") == 0) {
        if (!herramientas_scr) create_herramientas_screen();
        lv_scr_load(herramientas_scr);
    } else {
        Serial.printf("Button %s clicked\n", text);
    }
}

// Generic back button handler to load the screen passed in user_data
void back_button_event_handler(lv_event_t * e) {
    lv_obj_t * target_screen = (lv_obj_t *)lv_event_get_user_data(e);
    if (target_screen) {
        lv_scr_load(target_screen);
    }
}

// --- Screen Creation Functions ---

void create_main_screen() {
    // Get the current active screen (which is the base screen created by lv_init)
    main_scr = lv_scr_act();
    lv_obj_clean(main_scr); // Clean if re-creating
    lv_obj_set_style_bg_color(main_scr, lv_color_hex(0xE0E0E0), LV_PART_MAIN); // Light grey background

    lv_obj_t * cont = lv_obj_create(main_scr);
    lv_obj_set_size(cont, LV_PCT(100), LV_PCT(100));
    lv_obj_center(cont);
    lv_obj_set_style_bg_opa(cont, LV_OPA_TRANSP, 0); // Make container transparent
    lv_obj_set_style_border_width(cont, 0, 0);

    const char* button_texts[] = {
        "Visualizer",
        "Practicas Precargadas",
        "Editor de Codigo",
        "Herramientas"
    };

    // Create buttons in a 2x2 grid
    for (int i = 0; i < 4; i++) {
        lv_obj_t * btn = lv_btn_create(cont);
        // Adjust size for 320x240 screen
        lv_obj_set_size(btn, lv_pct(45), lv_pct(40));

        // Align in a 2x2 grid
        if (i == 0) lv_obj_align(btn, LV_ALIGN_TOP_LEFT, 10, 10);
        else if (i == 1) lv_obj_align(btn, LV_ALIGN_TOP_RIGHT, -10, 10);
        else if (i == 2) lv_obj_align(btn, LV_ALIGN_BOTTOM_LEFT, 10, -10);
        else if (i == 3) lv_obj_align(btn, LV_ALIGN_BOTTOM_RIGHT, -10, -10);

        lv_obj_add_event_cb(btn, main_button_event_handler, LV_EVENT_CLICKED, NULL);

        lv_obj_t * label = lv_label_create(btn);
        lv_label_set_text(label, button_texts[i]);
        lv_obj_center(label);
    }
}

// Placeholder function for shell viewer content update
void update_shell_content_cpp(lv_event_t * e) {
    lv_obj_t * ta = (lv_obj_t *)lv_event_get_user_data(e);
    // --- Placeholder for reading "shell_output.txt" ---
    // This requires filesystem (SPIFFS/SD) setup.
    // Example:
    // String content = readFile(SPIFFS, "/shell_output.txt");
    // if (content == "") {
    //     content = "Error reading file or file empty.";
    // }
    // lv_textarea_set_text(ta, content.c_str());
    // --- End Placeholder ---
    lv_textarea_set_text(ta, "File reading not implemented.\nPress Refresh.");
    Serial.println("Refresh button clicked - File reading needs implementation.");
}

void create_shell_viewer_screen() {
    shell_viewer_scr = lv_obj_create(NULL); // Create a new screen object

    // Text area
    lv_obj_t * ta = lv_textarea_create(shell_viewer_scr);
    lv_obj_set_size(ta, lv_pct(90), lv_pct(60)); // Use percentages
    lv_obj_align(ta, LV_ALIGN_TOP_MID, 0, 10);
    lv_textarea_set_placeholder_text(ta, "Loading shell output...");

    // Initial content load (placeholder)
    update_shell_content_cpp(NULL); // Pass NULL initially, or create dummy event if needed
    lv_event_t dummy_event; // Create a dummy event to pass textarea
    dummy_event.user_data = ta;
    update_shell_content_cpp(&dummy_event);


    // Refresh Button
    lv_obj_t * btn_refresh = lv_btn_create(shell_viewer_scr);
    lv_obj_set_size(btn_refresh, lv_pct(40), 50); // Adjust size
    lv_obj_align(btn_refresh, LV_ALIGN_BOTTOM_RIGHT, -10, -10);
    // Pass the text area as user data to the event handler
    lv_obj_add_event_cb(btn_refresh, update_shell_content_cpp, LV_EVENT_CLICKED, ta);
    lv_obj_t * label_refresh = lv_label_create(btn_refresh);
    lv_label_set_text(label_refresh, "Actualizar");
    lv_obj_center(label_refresh);

    // Back Button
    lv_obj_t * btn_back = lv_btn_create(shell_viewer_scr);
    lv_obj_set_size(btn_back, lv_pct(40), 50); // Adjust size
    lv_obj_align(btn_back, LV_ALIGN_BOTTOM_LEFT, 10, -10);
    // Use the generic back button handler, passing the main screen as user data
    lv_obj_add_event_cb(btn_back, back_button_event_handler, LV_EVENT_CLICKED, main_scr);
    lv_obj_t * label_back = lv_label_create(btn_back);
    lv_label_set_text(label_back, "Regresar");
    lv_obj_center(label_back);
}

// --- Implementations for other screens ---

void create_practicas_screen() {
    practicas_scr = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(practicas_scr, lv_color_hex(0xD0E0F0), 0); // Light blue background

    // Button Practica 1
    lv_obj_t * btn1 = lv_btn_create(practicas_scr);
    lv_obj_set_size(btn1, 225, 63); // 250*0.9, 70*0.9
    lv_obj_align(btn1, LV_ALIGN_CENTER, 0, -40); // Adjusted position
    lv_obj_add_event_cb(btn1, load_screen_event_handler, LV_EVENT_CLICKED, practica1_scr); // Load practica1 screen
    lv_obj_t * label1 = lv_label_create(btn1);
    lv_label_set_text(label1, "Practica 1: Jueces");
    lv_obj_center(label1);

    // Button Practica 2
    lv_obj_t * btn2 = lv_btn_create(practicas_scr);
    lv_obj_set_size(btn2, 225, 63); // 250*0.9, 70*0.9
    lv_obj_align(btn2, LV_ALIGN_CENTER, 0, 40); // Adjusted position
    lv_obj_add_event_cb(btn2, load_screen_event_handler, LV_EVENT_CLICKED, practica2_scr); // Load practica2 screen
    lv_obj_t * label2 = lv_label_create(btn2);
    lv_label_set_text(label2, "Practica 2: Seguridad");
    lv_obj_center(label2);

    // Back Button
    lv_obj_t * btn_back = lv_btn_create(practicas_scr);
    lv_obj_set_size(btn_back, 135, 36); // 150*0.9, 40*0.9
    lv_obj_align(btn_back, LV_ALIGN_BOTTOM_LEFT, 10, -10);
    lv_obj_add_event_cb(btn_back, back_button_event_handler, LV_EVENT_CLICKED, main_scr); // Go back to Main
    lv_obj_t * label_back = lv_label_create(btn_back);
    lv_label_set_text(label_back, "Regresar");
    lv_obj_center(label_back);

    // Pre-create screens if not already done
    if (!practica1_scr) create_practica1_screen();
    if (!practica2_scr) create_practica2_screen();
}

// --- Practica 1 (Jueces) ---
// We need to store the switches to check their state in the handler
static lv_obj_t * p1_switches[3];
static lv_obj_t * p1_led;

// Event handler for Practica 1 switches
void practica1_switch_event_handler(lv_event_t * e) {
    int checked_count = 0;
    for (int i = 0; i < 3; i++) {
       if (lv_obj_has_state(p1_switches[i], LV_STATE_CHECKED)) {
           checked_count++;
       }
    }

    if (checked_count >= 2) {
        lv_led_set_color(p1_led, lv_color_hex(0x00ff00)); // Green
        lv_led_on(p1_led);
        // Placeholder for GPIO control
        // digitalWrite(27, HIGH); // Turn on external LED
        Serial.println("Practica 1: LED Green (GPIO 27 HIGH - Placeholder)");
    } else {
        lv_led_set_color(p1_led, lv_color_hex(0xff0000)); // Red
        lv_led_on(p1_led); // Keep LED visible but red
        // Placeholder for GPIO control
        // digitalWrite(27, LOW); // Turn off external LED
        Serial.println("Practica 1: LED Red (GPIO 27 LOW - Placeholder)");
    }
}

void create_practica1_screen() {
    practica1_scr = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(practica1_scr, lv_color_hex(0xF0F0F0), 0); // Light background

    // LED
    p1_led = lv_led_create(practica1_scr);
    lv_obj_set_size(p1_led, 50, 50);
    lv_obj_align(p1_led, LV_ALIGN_TOP_MID, 0, 20); // Adjusted position
    lv_led_set_color(p1_led, lv_color_hex(0xff0000)); // Start Red

    // Switches
    p1_switches[0] = lv_switch_create(practica1_scr);
    lv_obj_align(p1_switches[0], LV_ALIGN_CENTER, -80, 20); // Adjusted position

    p1_switches[1] = lv_switch_create(practica1_scr);
    lv_obj_align(p1_switches[1], LV_ALIGN_CENTER, 0, 20); // Adjusted position

    p1_switches[2] = lv_switch_create(practica1_scr);
    lv_obj_align(p1_switches[2], LV_ALIGN_CENTER, 80, 20); // Adjusted position

    // Add event handlers
    for (int i = 0; i < 3; i++) {
        lv_obj_add_event_cb(p1_switches[i], practica1_switch_event_handler, LV_EVENT_VALUE_CHANGED, NULL); // Pass NULL, handler uses global p1_led
    }

    // Initialize LED state
    practica1_switch_event_handler(NULL); // Call handler to set initial state (pass dummy event)

    // Back Button
    lv_obj_t * btn_back = lv_btn_create(practica1_scr);
    lv_obj_set_size(btn_back, 120, 45); // Adjusted size
    lv_obj_align(btn_back, LV_ALIGN_BOTTOM_LEFT, 10, -10);
    // Ensure practicas_scr is created before adding callback to it
    if (!practicas_scr) create_practicas_screen();
    lv_obj_add_event_cb(btn_back, back_button_event_handler, LV_EVENT_CLICKED, practicas_scr); // Go back to Practicas screen
    lv_obj_t * label_back = lv_label_create(btn_back);
    lv_label_set_text(label_back, "Regresar");
    lv_obj_center(label_back);

     // Placeholder for GPIO setup (should be in main setup)
    // pinMode(27, OUTPUT);
    // digitalWrite(27, LOW); // Ensure LED is off initially
}

// --- Practica 2 (Seguridad) ---
static lv_obj_t * p2_switches[5];
static lv_obj_t * p2_led;
static bool p2_switch_states[5] = {false}; // Track state

// Event handler for Practica 2 switches
void practica2_switch_event_handler(lv_event_t * e) {
    lv_obj_t * sw = lv_event_get_target(e);
    int idx = (int)(lv_uintptr_t)lv_event_get_user_data(e); // Get index from user data

    p2_switch_states[idx] = lv_obj_has_state(sw, LV_STATE_CHECKED);

    int checked_count = 0;
    for (int i = 0; i < 5; i++) {
        if (p2_switch_states[i]) {
            checked_count++;
        }
    }

    // Logic: LED green if switch 3 (index 2) is ON OR at least 2 switches are ON
    if (p2_switch_states[2] || checked_count >= 2) {
        lv_led_set_color(p2_led, lv_color_hex(0x00ff00)); // Green
        lv_led_on(p2_led);
        Serial.println("Practica 2: LED Green");
    } else {
        lv_led_set_color(p2_led, lv_color_hex(0xff0000)); // Red
        lv_led_on(p2_led);
        Serial.println("Practica 2: LED Red");
    }
}


void create_practica2_screen() {
    practica2_scr = lv_obj_create(NULL);
    // Use screen directly instead of container for simplicity
    lv_obj_set_style_bg_color(practica2_scr, lv_color_hex(0x40E0D0), 0); // Turquoise background
    lv_obj_set_style_pad_all(practica2_scr, 10, 0); // Add some padding

    // LED
    p2_led = lv_led_create(practica2_scr);
    lv_obj_set_size(p2_led, 40, 40);
    lv_obj_align(p2_led, LV_ALIGN_TOP_MID, 0, 10); // Adjusted position
    lv_led_set_color(p2_led, lv_color_hex(0xAAAAAA)); // Start Grey

    // Switches and Labels
    const char* switch_texts[] = {"Opcion 1", "Opcion 2", "Opcion 3", "Opcion 4", "Opcion 5"};
    int num_switches = 5;
    int sw_width = 55; // Smaller switches for 320 width
    int sw_height = 25;
    int total_sw_width = num_switches * sw_width;
    // Calculate spacing dynamically based on screen width and switch width
    int screen_w = lv_disp_get_hor_res(NULL); // Get actual screen width
    int spacing = (screen_w - total_sw_width - lv_obj_get_style_pad_left(practica2_scr, 0) - lv_obj_get_style_pad_right(practica2_scr, 0)) / (num_switches + 1);
    int start_x = lv_obj_get_style_pad_left(practica2_scr, 0) + spacing;
    int y_pos = 70; // Vertical position for switches

    for (int i = 0; i < num_switches; i++) {
        p2_switches[i] = lv_switch_create(practica2_scr);
        lv_obj_set_size(p2_switches[i], sw_width, sw_height);
        lv_obj_set_pos(p2_switches[i], start_x + i * (sw_width + spacing), y_pos);
        // Pass the index 'i' as user data. Need to cast carefully.
        lv_obj_add_event_cb(p2_switches[i], practica2_switch_event_handler, LV_EVENT_VALUE_CHANGED, (void*)(lv_uintptr_t)i);

        lv_obj_t * lbl = lv_label_create(practica2_scr);
        lv_label_set_text(lbl, switch_texts[i]);
        lv_obj_align_to(lbl, p2_switches[i], LV_ALIGN_OUT_BOTTOM_MID, 0, 5); // Align label below switch
    }

     // Initialize LED state
    practica2_switch_event_handler(NULL); // Call handler to set initial state (pass dummy event)

    // Back Button
    lv_obj_t * btn_back = lv_btn_create(practica2_scr);
    lv_obj_set_size(btn_back, 120, 45); // Adjusted size
    lv_obj_align(btn_back, LV_ALIGN_BOTTOM_LEFT, 10, -10);
    // Ensure practicas_scr is created before adding callback to it
    if (!practicas_scr) create_practicas_screen();
    lv_obj_add_event_cb(btn_back, back_button_event_handler, LV_EVENT_CLICKED, practicas_scr); // Go back to Practicas
    lv_obj_t * label_back = lv_label_create(btn_back);
    lv_label_set_text(label_back, "Regresar");
    lv_obj_center(label_back);
}

// --- Herramientas Screen ---
void create_herramientas_screen() {
    herramientas_scr = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(herramientas_scr, lv_color_hex(0xF5F5DC), 0); // Beige background

    // Button Simulador
    lv_obj_t * btn_sim = lv_btn_create(herramientas_scr);
    lv_obj_set_size(btn_sim, 270, 63); // 300*0.9, 70*0.9
    lv_obj_align(btn_sim, LV_ALIGN_TOP_MID, 0, 15); // Adjusted position
    // Pre-create or ensure screen exists before setting callback data
    if (!simulador_scr) create_simulador_screen();
    lv_obj_add_event_cb(btn_sim, load_screen_event_handler, LV_EVENT_CLICKED, simulador_scr);
    lv_obj_t * label_sim = lv_label_create(btn_sim);
    lv_label_set_text(label_sim, "Simulador Compuertas");
    lv_obj_center(label_sim);

    // Button Resistencias
    lv_obj_t * btn_res = lv_btn_create(herramientas_scr);
    lv_obj_set_size(btn_res, 270, 63); // 300*0.9, 70*0.9
    lv_obj_align(btn_res, LV_ALIGN_CENTER, 0, 0); // Center vertically
    if (!resistencias_scr) create_resistencias_screen();
    lv_obj_add_event_cb(btn_res, load_screen_event_handler, LV_EVENT_CLICKED, resistencias_scr);
    lv_obj_t * label_res = lv_label_create(btn_res);
    lv_label_set_text(label_res, "Resistencias Colores");
    lv_obj_center(label_res);

    // Button Editor
    lv_obj_t * btn_edit = lv_btn_create(herramientas_scr);
    lv_obj_set_size(btn_edit, 270, 63); // 300*0.9, 70*0.9
    lv_obj_align(btn_edit, LV_ALIGN_BOTTOM_MID, 0, -15); // Adjusted position
    if (!editor_scr) create_editor_screen();
    lv_obj_add_event_cb(btn_edit, load_screen_event_handler, LV_EVENT_CLICKED, editor_scr);
    lv_obj_t * label_edit = lv_label_create(btn_edit);
    lv_label_set_text(label_edit, "Editor de Codigo");
    lv_obj_center(label_edit);

    // Back Button
    lv_obj_t * btn_back = lv_btn_create(herramientas_scr);
    lv_obj_set_size(btn_back, 135, 36); // 150*0.9, 40*0.9
    lv_obj_align(btn_back, LV_ALIGN_BOTTOM_LEFT, 10, -10);
    lv_obj_add_event_cb(btn_back, back_button_event_handler, LV_EVENT_CLICKED, main_scr); // Go back to Main
    lv_obj_t * label_back = lv_label_create(btn_back);
    lv_label_set_text(label_back, "Regresar");
    lv_obj_center(label_back);
}

// --- Simulador Logic ---
static const char* gate_names[] = {"AND", "OR", "XOR", "NAND", "NOR", "XNOR"};
static const int num_gates = 6;
static lv_obj_t* gate_buttons[num_gates];
static lv_obj_t* sim_sw1;
static lv_obj_t* sim_sw2;
static lv_obj_t* sim_led;
static int current_gate_index = 0; // 0 = AND

void update_sim_button_styles() {
    for (int i = 0; i < num_gates; i++) {
        if (i == current_gate_index) {
             lv_obj_set_style_bg_color(gate_buttons[i], lv_color_hex(0x00ff00), 0); // Green for selected
         } else {
              // Use default button color (fetch theme default)
              lv_theme_t * theme = lv_disp_get_theme(lv_disp_get_default()); // Get theme from default display
              lv_color_t default_color = theme ? theme->color_primary : lv_color_hex(0xCCCCCC); // Approximation, with fallback
              if (gate_buttons[i]) { // Check if button exists
                  default_color = lv_obj_get_style_bg_color(gate_buttons[i], LV_PART_MAIN); // Try getting current color
             }
             // Fallback if theme color isn't easily accessible or button doesn't exist yet
             // Replace lv_color_eq with direct comparison for 16-bit color
             if (default_color.full == lv_color_hex(0x00ff00).full) {
                 default_color = lv_color_hex(0xCCCCCC); // Fallback grey if current is green
             }
             lv_obj_set_style_bg_color(gate_buttons[i], default_color, 0);
        }
    }
}

void update_sim_led() {
    // Ensure switches and LED exist before accessing
    if (!sim_sw1 || !sim_sw2 || !sim_led) return;

    bool s1 = lv_obj_has_state(sim_sw1, LV_STATE_CHECKED);
    bool s2 = lv_obj_has_state(sim_sw2, LV_STATE_CHECKED);
    bool result = false;

    switch (current_gate_index) {
        case 0: result = s1 && s2; break; // AND
        case 1: result = s1 || s2; break; // OR
        case 2: result = s1 ^ s2; break;  // XOR
        case 3: result = !(s1 && s2); break; // NAND
        case 4: result = !(s1 || s2); break; // NOR
        case 5: result = (s1 == s2); break; // XNOR
    }

    if (result) {
        lv_led_set_color(sim_led, lv_color_hex(0x00ff00)); // Green
        lv_led_on(sim_led);
    } else {
        lv_led_set_color(sim_led, lv_color_hex(0xff0000)); // Red
        lv_led_on(sim_led); // Keep LED visible but red
    }
}

void sim_gate_button_event_handler(lv_event_t * e) {
    lv_obj_t * btn = lv_event_get_target(e);
    // Find which button was clicked by comparing pointers
    for(int i=0; i<num_gates; ++i) {
        if (btn == gate_buttons[i]) {
            current_gate_index = i;
            break;
        }
    }
    update_sim_button_styles();
    update_sim_led();
}

void sim_switch_event_handler(lv_event_t * e) {
    update_sim_led();
}


void create_simulador_screen() {
    simulador_scr = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(simulador_scr, lv_color_hex(0xFFDAB9), 0); // Peach Puff

    // LED
    sim_led = lv_led_create(simulador_scr);
    lv_obj_set_size(sim_led, 40, 40); // Smaller LED
    lv_obj_align(sim_led, LV_ALIGN_TOP_MID, 0, 10);
    lv_led_set_color(sim_led, lv_color_hex(0xff0000)); // Start Red

    // Switches
    sim_sw1 = lv_switch_create(simulador_scr);
    lv_obj_align(sim_sw1, LV_ALIGN_TOP_LEFT, 20, 60);
    lv_obj_add_event_cb(sim_sw1, sim_switch_event_handler, LV_EVENT_VALUE_CHANGED, NULL);

    sim_sw2 = lv_switch_create(simulador_scr);
    lv_obj_align(sim_sw2, LV_ALIGN_TOP_RIGHT, -20, 60);
    lv_obj_add_event_cb(sim_sw2, sim_switch_event_handler, LV_EVENT_VALUE_CHANGED, NULL);

    // Gate Buttons (2 rows of 3)
    int btn_width = 80;
    int btn_height = 35;
    int btn_y1 = 110;
    int btn_y2 = 160;
    int screen_w = lv_disp_get_hor_res(NULL);
    int btn_x_start = (screen_w - (3 * btn_width + 2 * 10)) / 2; // Center the row

    for (int i = 0; i < num_gates; i++) {
        gate_buttons[i] = lv_btn_create(simulador_scr);
        lv_obj_set_size(gate_buttons[i], btn_width, btn_height);
        int row = i / 3;
        int col = i % 3;
        int y_pos = (row == 0) ? btn_y1 : btn_y2;
        int x_pos = btn_x_start + col * (btn_width + 10);
        lv_obj_set_pos(gate_buttons[i], x_pos, y_pos);

        lv_obj_t * label = lv_label_create(gate_buttons[i]);
        lv_label_set_text(label, gate_names[i]);
        lv_obj_center(label);
        lv_obj_add_event_cb(gate_buttons[i], sim_gate_button_event_handler, LV_EVENT_CLICKED, NULL);
    }

    // Back Button
    lv_obj_t * btn_back = lv_btn_create(simulador_scr);
    lv_obj_set_size(btn_back, 120, 40); // Adjusted size
    lv_obj_align(btn_back, LV_ALIGN_BOTTOM_LEFT, 10, -10);
    // Ensure herramientas_scr exists
    if (!herramientas_scr) create_herramientas_screen();
    lv_obj_add_event_cb(btn_back, back_button_event_handler, LV_EVENT_CLICKED, herramientas_scr); // Go back to Herramientas
    lv_obj_t * label_back = lv_label_create(btn_back);
    lv_label_set_text(label_back, "Regresar");
    lv_obj_center(label_back);

    // Initial setup
    update_sim_button_styles();
    update_sim_led();
}


// --- Resistor Calculator Logic ---
static lv_obj_t * res_textarea;
static lv_obj_t * res_color_label;
static lv_obj_t * res_smd_label;
static lv_obj_t * res_band_objs[4]; // Objects to show color bands

// Color definitions (match Python)
const char* res_color_names[] = {"Negro", "Marron", "Rojo", "Naranja", "Amarillo", "Verde", "Azul", "Morado", "Gris", "Blanco"};
lv_color_t res_color_hex[] = {
    lv_color_hex(0x000000), lv_color_hex(0x8B4513), lv_color_hex(0xFF0000), lv_color_hex(0xFFA500),
    lv_color_hex(0xFFFF00), lv_color_hex(0x008000), lv_color_hex(0x0000FF), lv_color_hex(0xEE82EE), // Violet instead of Purple
    lv_color_hex(0x808080), lv_color_hex(0xFFFFFF)
};
lv_color_t gold_color = lv_color_hex(0xFFD700);
// Note: Silver multiplier (-2) is not handled in the Python code, so omitted here too.

void update_resistor_display(lv_event_t * e) {
    // Ensure UI elements exist
    if (!res_textarea || !res_color_label || !res_smd_label) return;
    for(int i=0; i<4; ++i) { if (!res_band_objs[i]) return; }

    const char * text = lv_textarea_get_text(res_textarea);
    long value = atol(text); // Convert text to long integer
    char color_str[100] = "Colores: ";
    char smd_str[20] = "SMD: ";
    bool valid = false;

    // Handle only positive integers as per Python logic
    if (strlen(text) > 0 && value >= 0) {
        char val_str[20];
        sprintf(val_str, "%ld", value);
        int len = strlen(val_str);

        if (value == 0) { // Special case for 0 Ohm
             strcat(color_str, "Negro, Negro, Negro, Dorado");
             strcpy(smd_str, "SMD: 0");
             lv_obj_set_style_bg_color(res_band_objs[0], res_color_hex[0], 0);
             lv_obj_set_style_bg_color(res_band_objs[1], res_color_hex[0], 0);
             lv_obj_set_style_bg_color(res_band_objs[2], res_color_hex[0], 0);
             lv_obj_set_style_bg_color(res_band_objs[3], gold_color, 0);
             valid = true;
        } else if (len >= 1) {
            int d1 = 0, d2 = 0, exp = 0;
            // Logic based on Python: assumes 2 significant digits + multiplier
            if (len == 1) { // e.g., 5 treated as 5 Ohm (50 * 10^0 is not standard) -> Verde, Negro, Negro
                d1 = val_str[0] - '0';
                d2 = 0;
                exp = 0; // Multiplier 10^0 = 1
            } else { // len >= 2, e.g., 220 -> d1=2, d2=2, exp=1 (10^1)
                d1 = val_str[0] - '0';
                d2 = val_str[1] - '0';
                exp = len - 2; // Multiplier 10^(len-2)
            }

            // Check if digits and exponent are within the standard color code range (0-9)
            if (d1 >= 0 && d1 <= 9 && d2 >= 0 && d2 <= 9 && exp >= 0 && exp <= 9) {
                strcat(color_str, res_color_names[d1]);
                strcat(color_str, ", ");
                strcat(color_str, res_color_names[d2]);
                strcat(color_str, ", ");
                strcat(color_str, res_color_names[exp]);
                strcat(color_str, ", Dorado"); // Assuming 4th band is always Gold (5% tolerance)

                // SMD code: first two digits + exponent
                sprintf(smd_str, "SMD: %d%d%d", d1, d2, exp);

                // Update band colors
                lv_obj_set_style_bg_color(res_band_objs[0], res_color_hex[d1], 0);
                lv_obj_set_style_bg_color(res_band_objs[1], res_color_hex[d2], 0);
                lv_obj_set_style_bg_color(res_band_objs[2], res_color_hex[exp], 0);
                lv_obj_set_style_bg_color(res_band_objs[3], gold_color, 0);
                valid = true;
            }
        }
    }

    if (!valid) {
        strcat(color_str, "Entrada invalida");
        strcpy(smd_str, "SMD: ");
        // Clear bands or set to grey
        for(int i=0; i<4; ++i) {
             lv_obj_set_style_bg_color(res_band_objs[i], lv_color_hex(0xCCCCCC), 0); // Light Grey
        }
    }

    lv_label_set_text(res_color_label, color_str);
    lv_label_set_text(res_smd_label, smd_str);
}


void create_resistencias_screen() {
    resistencias_scr = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(resistencias_scr, lv_color_hex(0xFFFFFF), 0); // White background

    // Text Area for input
    res_textarea = lv_textarea_create(resistencias_scr);
    lv_obj_set_size(res_textarea, 280, 40); // Adjusted size
    lv_obj_align(res_textarea, LV_ALIGN_TOP_MID, 0, 10);
    lv_textarea_set_one_line(res_textarea, true);
    lv_textarea_set_placeholder_text(res_textarea, "Ingrese valor (ej. 220, 4k7, 1M)"); // Updated placeholder
    // Allow k, M, R? For now, just digits based on Python code.
    lv_textarea_set_accepted_chars(res_textarea, "0123456789");
    lv_obj_add_event_cb(res_textarea, update_resistor_display, LV_EVENT_VALUE_CHANGED, NULL);

    // Labels for output
    res_color_label = lv_label_create(resistencias_scr);
    lv_label_set_long_mode(res_color_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(res_color_label, 300);
    lv_obj_align(res_color_label, LV_ALIGN_TOP_MID, 0, 60);
    lv_label_set_text(res_color_label, "Colores: ");

    res_smd_label = lv_label_create(resistencias_scr);
    lv_obj_align_to(res_smd_label, res_color_label, LV_ALIGN_OUT_BOTTOM_MID, 0, 10); // Align smd_label below color_label
    lv_label_set_text(res_smd_label, "SMD: ");

    // Color Bands display
    int band_width = 30;
    int band_height = 60;
    int band_spacing = 10;
    int band_y = 140;
    int screen_w = lv_disp_get_hor_res(NULL);
    int total_width = 4 * band_width + 3 * band_spacing;
    int band_x_start = (screen_w - total_width) / 2;

    for (int i = 0; i < 4; i++) {
        res_band_objs[i] = lv_obj_create(resistencias_scr);
        lv_obj_set_size(res_band_objs[i], band_width, band_height);
        lv_obj_set_pos(res_band_objs[i], band_x_start + i * (band_width + band_spacing), band_y);
        lv_obj_set_style_radius(res_band_objs[i], 3, 0); // Slightly rounded corners
        lv_obj_set_style_border_width(res_band_objs[i], 1, 0); // Add border
        lv_obj_set_style_border_color(res_band_objs[i], lv_color_black(), 0);
        lv_obj_set_style_bg_color(res_band_objs[i], lv_color_hex(0xCCCCCC), 0); // Start light grey
    }

    // Back Button
    lv_obj_t * btn_back = lv_btn_create(resistencias_scr);
    lv_obj_set_size(btn_back, 120, 40); // Adjusted size
    lv_obj_align(btn_back, LV_ALIGN_BOTTOM_LEFT, 10, -10);
    // Ensure herramientas_scr exists
    if (!herramientas_scr) create_herramientas_screen();
    lv_obj_add_event_cb(btn_back, back_button_event_handler, LV_EVENT_CLICKED, herramientas_scr); // Go back to Herramientas
    lv_obj_t * label_back = lv_label_create(btn_back);
    lv_label_set_text(label_back, "Regresar");
    lv_obj_center(label_back);

    // Initial update
    update_resistor_display(NULL);
}

// Placeholder for saving code
void save_code_cpp(lv_event_t * e) {
     lv_obj_t * ta = (lv_obj_t *)lv_event_get_user_data(e);
     const char * code = lv_textarea_get_text(ta);
     // --- Placeholder for saving to "user_code.py" ---
     // Requires filesystem (SPIFFS/SD)
     // Example:
     // writeFile(SPIFFS, "/user_code.py", code);
     // --- End Placeholder ---
     Serial.printf("Save button clicked. Code:\n%s\n(Saving needs implementation)\n", code);
}


void create_editor_screen() {
    editor_scr = lv_obj_create(NULL);

    // Code Input Text Area
    lv_obj_t * code_ta = lv_textarea_create(editor_scr);
    lv_obj_set_size(code_ta, lv_pct(90), lv_pct(40));
    lv_obj_align(code_ta, LV_ALIGN_TOP_MID, 0, 5);
    lv_textarea_set_placeholder_text(code_ta, "Escribe tu codigo aqui...");

    // Keyboard
    lv_obj_t * kb = lv_keyboard_create(editor_scr);
    // lv_obj_set_size(kb, lv_pct(100), lv_pct(45)); // Adjust size if needed
    lv_keyboard_set_textarea(kb, code_ta); // Link keyboard to text area

    // Save Button
    lv_obj_t * save_btn = lv_btn_create(editor_scr);
    lv_obj_set_size(save_btn, 100, 40);
    lv_obj_align_to(save_btn, code_ta, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 10); // Position save_btn below text area
    lv_obj_add_event_cb(save_btn, save_code_cpp, LV_EVENT_CLICKED, code_ta); // Pass text area
    lv_obj_t * save_label = lv_label_create(save_btn);
    lv_label_set_text(save_label, "Save");
    lv_obj_center(save_label);

    // Back Button
    lv_obj_t * btn_back = lv_btn_create(editor_scr);
    lv_obj_set_size(btn_back, 100, 40);
    lv_obj_align_to(btn_back, save_btn, LV_ALIGN_OUT_RIGHT_MID, 10, 0); // Position btn_back next to save button
    lv_obj_add_event_cb(btn_back, back_button_event_handler, LV_EVENT_CLICKED, herramientas_scr); // Go back to Herramientas
    lv_obj_t * label_back = lv_label_create(btn_back);
    lv_label_set_text(label_back, "Regresar");
    lv_obj_center(label_back);

    // Note: The code_display_ta from Python is omitted for simplicity in C++
    // You could add another text area if needed, but it complicates keyboard handling.
}


// --- Main UI Setup Function ---

void setup_ui() {
    // Create the main screen first
    create_main_screen();

    // Optionally pre-create other screens here if needed,
    // or create them on demand when their button is clicked.
    // create_shell_viewer_screen(); // Example: pre-create
}
