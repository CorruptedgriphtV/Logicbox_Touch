import display_driver_utils
import time
import lvgl as lv

drv = display_driver_utils.driver(width=320, height=240)

def create_simulador_screen():
    scr = lv.obj()
    scr.set_style_bg_color(lv.color_hex(0xFFDAB9), 0)  # Fondo en naranja pastel
    lv.scr_load(scr)
    
    # Crear el LED (círculo virtual) para mostrar el resultado
    led = lv.led(scr)
    led.set_size(50, 50)
    led.align(lv.ALIGN.TOP_MID, 0, 30)
    led.set_color(lv.color_hex(0xff0000))  # Inicialmente rojo (apagado)
    
    # Crear dos switches para representar las entradas
    sw1 = lv.switch(scr)
    sw1.align(lv.ALIGN.CENTER, -100, 0)
    sw1.clear_state(lv.STATE.CHECKED)
    
    sw2 = lv.switch(scr)
    sw2.align(lv.ALIGN.CENTER, 100, 0)
    sw2.clear_state(lv.STATE.CHECKED)
    
    # Variable para almacenar la función de la compuerta
    func_mode = ["AND"]  # Por defecto AND
    
    # Diccionario para guardar los botones de compuerta
    gate_names = ["AND", "OR", "XOR", "NAND", "NOR", "XNOR"]
    buttons = {}
    
    # Crear botones para seleccionar la compuerta y ubicarlos en dos filas
    # Primera fila: AND, OR, XOR
    x_positions = [20, 120, 220]
    y_pos1 = -50
    for i, gate in enumerate(gate_names[:3]):
        btn = lv.btn(scr)
        btn.set_size(80, 40)
        btn.align(lv.ALIGN.BOTTOM_LEFT, x_positions[i], y_pos1)
        label = lv.label(btn)
        label.set_text(gate)
        label.center()
        buttons[gate] = btn
    
    # Segunda fila: NAND, NOR, XNOR
    x_positions2 = [20, 120, 220]
    y_pos2 = -100
    for i, gate in enumerate(gate_names[3:]):
        btn = lv.btn(scr)
        btn.set_size(80, 40)
        btn.align(lv.ALIGN.BOTTOM_LEFT, x_positions2[i], y_pos2)
        label = lv.label(btn)
        label.set_text(gate)
        label.center()
        buttons[gate] = btn
    
    # Función para actualizar el estilo de los botones según la compuerta seleccionada
    def update_button_styles():
        for gate in gate_names:
            if func_mode[0] == gate:
                buttons[gate].set_style_bg_color(lv.color_hex(0x00ff00), 0)
            else:
                buttons[gate].set_style_bg_color(lv.color_hex(0xffffff), 0)
    
    # Función para actualizar el LED según la función y el estado de los switches
    def update_led(ev=None):
        s1 = sw1.has_state(lv.STATE.CHECKED)
        s2 = sw2.has_state(lv.STATE.CHECKED)
        gate = func_mode[0]
        # Para la compuerta AND se lanza excepción si ambos switches están apagados en la configuración inicial
        if gate == "AND" and not s1 and not s2:
            raise Exception("Error: Configuración inicial inválida: ambos switches apagados con AND seleccionado.")
        result = False
        if gate == "AND":
            result = s1 and s2
        elif gate == "OR":
            result = s1 or s2
        elif gate == "XOR":
            result = (s1 and not s2) or (not s1 and s2)
        elif gate == "NAND":
            result = not (s1 and s2)
        elif gate == "NOR":
            result = not (s1 or s2)
        elif gate == "XNOR":
            result = (s1 == s2)
            
        if result:
            led.set_color(lv.color_hex(0x00ff00))  # Verde
        else:
            led.set_color(lv.color_hex(0xff0000))  # Rojo
    
    # Función para cambiar el modo de compuerta
    def set_mode(gate, ev=None):
        func_mode[0] = gate
        update_button_styles()
        update_led(ev)
    
    # Asignar callbacks a cada botón de compuerta
    for gate in gate_names:
        buttons[gate].add_event_cb(lambda ev, g=gate: set_mode(g, ev), lv.EVENT.CLICKED, None)
    
    # Asignar el callback a ambos switches para actualizar el LED al cambiar su estado
    sw1.add_event_cb(lambda ev: update_led(ev), lv.EVENT.VALUE_CHANGED, None)
    sw2.add_event_cb(lambda ev: update_led(ev), lv.EVENT.VALUE_CHANGED, None)
    
    # Ejecutar la actualización inicial controlando la excepción
    try:
        update_led(None)
    except Exception as e:
        print(e)
        led.set_color(lv.color_hex(0xff0000))

create_simulador_screen()

while True:
    time.sleep_ms(1)