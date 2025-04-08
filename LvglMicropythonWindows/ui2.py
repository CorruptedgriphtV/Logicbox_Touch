import display_driver_utils
import time
import lvgl as lv

# Cambiamos el tamaño del driver a 320x240
drv = display_driver_utils.driver(width=320, height=240)

def create_simulador_screen():
    scr = lv.obj()
    # Fondo en naranja pastel
    scr.set_style_bg_color(lv.color_hex(0xFFDAB9), 0)
    lv.scr_load(scr)
    
    # Crear el LED (círculo virtual) más pequeño para 320x240
    led = lv.led(scr)
    led.set_size(30, 30)  # Reducido
    led.align(lv.ALIGN.TOP_MID, 0, 10)
    led.set_color(lv.color_hex(0xff0000))
    
    # Crear dos switches (pueden mantenerse igual, o ajustar tamaño si es necesario)
    sw1 = lv.switch(scr)
    sw1.align(lv.ALIGN.CENTER, -50, 0)
    sw1.clear_state(lv.STATE.CHECKED)
    
    sw2 = lv.switch(scr)
    sw2.align(lv.ALIGN.CENTER, 50, 0)
    sw2.clear_state(lv.STATE.CHECKED)
    
    # Variable para almacenar la función de la compuerta ("AND", "OR", etc.)
    func_mode = ["AND"]
    
    # Diccionario para guardar botones y lista de compuertas
    gate_names = ["AND", "OR", "XOR", "NAND", "NOR", "XNOR"]
    buttons = {}
    
    # Ajustamos tamaños y posiciones de los botones para la resolución 320x240
    # Primera fila: 3 botones centrados
    x_positions = [10, 110, 210]
    y_pos1 = 80
    for i, gate in enumerate(gate_names[:3]):
        btn = lv.btn(scr)
        btn.set_size(80, 30)
        btn.align(lv.ALIGN.BOTTOM_LEFT, x_positions[i], y_pos1)
        label = lv.label(btn)
        label.set_text(gate)
        label.center()
        buttons[gate] = btn
    
    # Segunda fila: otros 3 botones
    x_positions2 = [10, 110, 210]
    y_pos2 = 180
    for i, gate in enumerate(gate_names[3:]):
        btn = lv.btn(scr)
        btn.set_size(80, 30)
        btn.align(lv.ALIGN.BOTTOM_LEFT, x_positions2[i], y_pos2)
        label = lv.label(btn)
        label.set_text(gate)
        label.center()
        buttons[gate] = btn
    
    def update_button_styles():
        for gate in gate_names:
            if func_mode[0] == gate:
                buttons[gate].set_style_bg_color(lv.color_hex(0x00ff00), 0)
            else:
                buttons[gate].set_style_bg_color(lv.color_hex(0xffffff), 0)
    
    def update_led(ev=None):
        s1 = sw1.has_state(lv.STATE.CHECKED)
        s2 = sw2.has_state(lv.STATE.CHECKED)
        gate = func_mode[0]
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
            led.set_color(lv.color_hex(0x00ff00))
        else:
            led.set_color(lv.color_hex(0xff0000))
    
    def set_mode(gate, ev=None):
        func_mode[0] = gate
        update_button_styles()
        update_led(ev)
    
    for gate in gate_names:
        buttons[gate].add_event_cb(lambda ev, g=gate: set_mode(g, ev), lv.EVENT.CLICKED, None)
    
    sw1.add_event_cb(lambda ev: update_led(ev), lv.EVENT.VALUE_CHANGED, None)
    sw2.add_event_cb(lambda ev: update_led(ev), lv.EVENT.VALUE_CHANGED, None)
    
    try:
        update_led(None)
    except Exception as e:
        print(e)
        led.set_color(lv.color_hex(0xff0000))

create_simulador_screen()

while True:
    time.sleep_ms(1)