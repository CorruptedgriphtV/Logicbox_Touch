#ifndef UI_H
#define UI_H

// Include lv_conf.h before lvgl.h when LV_CONF_INCLUDE_SIMPLE is used
#include "lv_conf.h"
#include <lvgl.h>

// Declare screen objects globally so they can be accessed for navigation
extern lv_obj_t * main_scr; // Keep track of the main screen
extern lv_obj_t * shell_viewer_scr;
extern lv_obj_t * practicas_scr;
extern lv_obj_t * practica1_scr;
extern lv_obj_t * practica2_scr;
extern lv_obj_t * herramientas_scr;
extern lv_obj_t * simulador_scr;
extern lv_obj_t * resistencias_scr;
extern lv_obj_t * editor_scr;

// Function to create the main UI and all screens
void setup_ui();

// Functions to create individual screens (will be defined in ui.cpp)
void create_main_screen();
void create_shell_viewer_screen();
void create_practicas_screen();
void create_practica1_screen();
void create_practica2_screen();
void create_herramientas_screen();
void create_simulador_screen();
void create_resistencias_screen();
void create_editor_screen();

// Event handlers (will be defined in ui.cpp)
void main_button_event_handler(lv_event_t * e);
void back_to_main_event_handler(lv_event_t * e);
void back_to_practicas_event_handler(lv_event_t * e);
void back_to_herramientas_event_handler(lv_event_t * e);
// Add other specific event handlers as needed

#endif // UI_H
