try:
    import display_driver_utils
except ImportError:
    print("display_driver_utils module not found; ensure it is installed and your PYTHONPATH is set accordingly.")
    raise
except ImportError:
    print("lvgl module not found; ensure the SDL lvgl version is installed and your PYTHONPATH is set accordingly.")
    raise
import display_driver_utils
import time
import lvgl as lv


drv = display_driver_utils.driver(width=1024, height=768)

# Guarda la pantalla principal para poder volver a ella.
main_scr = lv.scr_act()

# Lee el contenido del archivo y actualiza el text area.
def update_shell_content(textarea):
    try:
        with open("shell_output.txt", "r") as f:
            content = f.read()
    except Exception as e:
        content = "Error al leer el archivo: {}".format(e)
    textarea.set_text(content)

# Crea una nueva pantalla que funciona como visor de shell.
def create_shell_viewer():
    scr = lv.obj()
    lv.scr_load(scr)
    
    # Text area para mostrar el contenido del archivo.
    ta = lv.textarea(scr)
    ta.set_size(1024 - 20, 768 - 100)
    ta.align(lv.ALIGN.TOP_MID, 0, 10)
    
    # Actualiza el contenido al iniciar.
    update_shell_content(ta)
    
    # Botón para refrescar el contenido (10% mas pequeño: 200x50 -> 180x45)
    btn = lv.btn(scr)
    btn.set_size(int(200 * 0.9), int(50 * 0.9))
    btn.align(lv.ALIGN.BOTTOM_MID, 0, -20)
    btn.add_event_cb(lambda _: update_shell_content(ta), lv.EVENT.CLICKED, None)
    label = lv.label(btn)
    label.set_text("Actualizar")
    label.center()

    # Botón para regresar a la pantalla principal (10% mas pequeño)
    btn_back = lv.btn(scr)
    btn_back.set_size(int(200 * 0.9), int(50 * 0.9))
    btn_back.align(lv.ALIGN.BOTTOM_LEFT, 20, -20)
    btn_back.add_event_cb(lambda _: lv.scr_load(main_scr), lv.EVENT.CLICKED, None)
    label_back = lv.label(btn_back)
    label_back.set_text("Regresar")
    label_back.center()

# Crea una nueva pantalla para "Practicas Precargadas"
def switch_event_handler(e, led, switches):
    # Example handler logic
    sw = e.get_target()
    if sw.get_state():
        led.set_color(lv.color_hex(0x00ff00))  # Green if any switch is on
    else:
        led.set_color(lv.color_hex(0xff0000))  # Red if all switches are off

    # Check if all switches are off
    if all(not s.get_state() for s in switches):
        led.set_color(lv.color_hex(0xff0000))  # Red if all switches are off
    else:
        led.set_color(lv.color_hex(0x00ff00))  # Green if any switch is on
def switch_event_handler(e, led, switches):
    # Cuenta cuántos switches están CHECKED
    count = 0
    for sw in switches:
        if sw.has_state(lv.STATE.CHECKED):
            count += 1
    if count >= 2:
        led.set_color(lv.color_hex(0x00ff00))  # Verde
    else:
        led.set_color(lv.color_hex(0xff0000))  # Rojo

def create_practica1_screen():
    scr = lv.obj()
    lv.scr_load(scr)
    
    # Crear el LED (círculo virtual)
    led = lv.led(scr)
    led.set_size(50, 50)
    led.align(lv.ALIGN.TOP_MID, 0, 30)
    led.set_color(lv.color_hex(0xff0000))  # Inicialmente rojo
    
    # Crear tres switches y guardarlos en una lista
    switches = []
    
    sw1 = lv.switch(scr)
    sw1.align(lv.ALIGN.CENTER, -100, 0)  # A la izquierda
    switches.append(sw1)
    
    sw2 = lv.switch(scr)
    sw2.align(lv.ALIGN.CENTER, 0, 0)  # Centro
    switches.append(sw2)
    
    sw3 = lv.switch(scr)
    sw3.align(lv.ALIGN.CENTER, 100, 0)  # A la derecha
    switches.append(sw3)
    
    # Agregar el mismo callback a cada switch.
    for sw in switches:
        sw.add_event_cb(lambda e, s=switches: switch_event_handler(e, led, s),
                        lv.EVENT.VALUE_CHANGED, None)
    # Aqui vas a programar el como mandar la señal del led verde a pin 27 del ESP32




    # Botón para regresar a la pantalla de prácticas precargadas
    btn_back = lv.btn(scr)
    btn_back.set_size(int(200 * 0.9), int(50 * 0.9))
    btn_back.align(lv.ALIGN.BOTTOM_LEFT, 20, -20)
    btn_back.add_event_cb(lambda _: create_practicas_screen(), lv.EVENT.CLICKED, None)
    label_back = lv.label(btn_back)
    label_back.set_text("Regresar")
    label_back.center()
def create_practica2_screen():
    # Crear la pantalla base y cargarla
    scr = lv.obj()
    lv.scr_load(scr)
    
    # Crear un contenedor con fondo azul turquesa y espaciado interior general de 30
    cont = lv.obj(scr)
    cont.set_size(1024, 768)
    cont.align(lv.ALIGN.CENTER, 0, 0)
    cont.set_style_bg_color(lv.color_hex(0x40E0D0), 0)  # Turquesa
    cont.set_style_pad_top(30, 0)
    # set_style_pad_inner no está disponible en SDL lvgl; usamos un padding interior general:
    cont.set_style_pad_all(30, 0)
    
    # LED widget para visual feedback, centrado en la parte superior del contenedor
    led = lv.led(cont)
    led.set_size(40, 40)
    led.align(lv.ALIGN.TOP_MID, 0, 30)  # 30 px desde el tope del contenedor
    led.set_color(lv.color_hex(0xaaaaaa))  # Inicialmente gris
    
    # Definir textos para los 5 switches
    switch_texts = ["Opcion 1", "Opcion 2", "Opcion 3", "Opcion 4", "Opcion 5"]
    sw_width = 100
    sw_height = 50
    screen_width = 1024  # Ancho de la pantalla fija
    spacing = (screen_width - (sw_width * 5)) // 6

    x_pos = spacing
    y_pos = 100  # Posición vertical fija para los switches

    # Estado de cada switch (False = off, True = on), inicialmente todos off.
    switch_states = [False] * 5

    # Callback para cada switch: actualiza el estado y el LED
    def switch_event_handler(e, idx, t):
        sw = e.get_target()
        # Usar el estado CHECKED para determinar si está encendido
        state = sw.has_state(lv.STATE.CHECKED)
        switch_states[idx] = True if state else False
        print("Switch {} toggled, state: {}".format(t, switch_states[idx]))
            
        # Si el switch 3 (índice 2) está encendido o dos o más switches están encendidos,
        # enciende el LED (verde); de lo contrario, lo pone en rojo.
        if switch_states[2] or sum(switch_states) >= 2:
            led.set_color(lv.color_hex(0x00ff00))  # LED verde
        else:
            led.set_color(lv.color_hex(0xff0000))  # LED rojo

    # Crear los 5 switches dispuestos horizontalmente dentro del contenedor
    for idx, text in enumerate(switch_texts):
        sw = lv.switch(cont)
        sw.set_size(sw_width, sw_height)
        sw.set_pos(x_pos, y_pos)
        sw.add_event_cb(lambda e, idx=idx, t=text: switch_event_handler(e, idx, t),
                        lv.EVENT.VALUE_CHANGED, None)
        lbl = lv.label(cont)
        lbl.set_text(text)
        lbl.align_to(sw, lv.ALIGN.OUT_BOTTOM_MID, 0, 5)
        x_pos += sw_width + spacing

    # Botón para regresar a la pantalla de prácticas precargadas
    btn_back = lv.btn(cont)
    btn_back.set_size(int(200 * 0.9 * 0.9), int(50 * 0.9 * 0.9))
    btn_back.align(lv.ALIGN.BOTTOM_LEFT, 20, -20)
    btn_back.add_event_cb(lambda _: create_practicas_screen(), lv.EVENT.CLICKED, None)
    label_back = lv.label(btn_back)
    label_back.set_text("Regresar")
    label_back.center()

    # Botón para regresar a la pantalla de prácticas precargadas
    btn_back = lv.btn(cont)
    btn_back.set_size(int(200 * 0.9 * 0.9), int(50 * 0.9 * 0.9))
    btn_back.align(lv.ALIGN.BOTTOM_LEFT, 20, -20)
    btn_back.add_event_cb(lambda _: create_practicas_screen(), lv.EVENT.CLICKED, None)
    label_back = lv.label(btn_back)
    label_back.set_text("Regresar")
    label_back.center()

def create_practicas_screen():
    scr = lv.obj()
    lv.scr_load(scr)
    
    # Botón para la Practica 1: Jueces
    btn1 = lv.btn(scr)
    btn1.set_size(int(400 * 0.9 * 0.9), int(100 * 0.9 * 0.9))
    btn1.align(lv.ALIGN.CENTER, 0, -60)
    btn1.add_event_cb(lambda _: create_practica1_screen(), lv.EVENT.CLICKED, None)
    label1 = lv.label(btn1)
    label1.set_text("Practica 1: Jueces")
    label1.center()
    
    # Botón para la Practica 2: sistema de seguridad
    btn2 = lv.btn(scr)
    btn2.set_size(int(400 * 0.9 * 0.9), int(100 * 0.9 * 0.9))
    btn2.align(lv.ALIGN.CENTER, 0, 60)
    btn2.add_event_cb(lambda _: create_practica2_screen(), lv.EVENT.CLICKED, None)
    label2 = lv.label(btn2)
    label2.set_text("Practica 2: sistema de seguridad")
    label2.center()
    
    # Botón para regresar a la pantalla principal
    btn_back = lv.btn(scr)
    btn_back.set_size(int(200 * 0.9 * 0.9), int(50 * 0.9 * 0.9))
    btn_back.align(lv.ALIGN.BOTTOM_LEFT, 20, -20)
    btn_back.add_event_cb(lambda _: lv.scr_load(main_scr), lv.EVENT.CLICKED, None)
    label_back = lv.label(btn_back)
    label_back.set_text("Regresar")
    label_back.center()

# -------------------
# Evento de los botones en la pantalla principal
def button_event_handler(e):
    btn = e.get_target()
    label = btn.get_child(0)
    text = label.get_text()
    if text == "Visualizer":
        create_shell_viewer()
    elif text == "Practicas Precargadas":
        create_practicas_screen()
    elif text == "Herramientas":
        create_herramientas_screen()
    else:
        print("Button {} clicked".format(text))

# Crea una nueva pantalla para "Herramientas"
def create_herramientas_screen():
    scr = lv.obj()
    lv.scr_load(scr)
    
    # Botón para Simulador de compuertas
    btn_simulador = lv.btn(scr)
    btn_simulador.set_size(int(400 * 0.9), int(100 * 0.9))
    btn_simulador.align(lv.ALIGN.CENTER, 0, -60)
    btn_simulador.add_event_cb(lambda _: create_simulador_screen(), lv.EVENT.CLICKED, None)
    label_simulador = lv.label(btn_simulador)
    label_simulador.set_text("Simulador de compuertas")
    label_simulador.center()
    
    # Botón para Resistencias a colores
    btn_resistencias = lv.btn(scr)
    btn_resistencias.set_size(int(400 * 0.9), int(100 * 0.9))
    btn_resistencias.align(lv.ALIGN.CENTER, 0, 60)
    btn_resistencias.add_event_cb(lambda _: print("Resistencias a colores"), lv.EVENT.CLICKED, None)
    label_resistencias = lv.label(btn_resistencias)
    label_resistencias.set_text("Resistencias a colores")
    label_resistencias.center()
    
    # Botón para regresar a la pantalla principal
    btn_back = lv.btn(scr)
    btn_back.set_size(int(200 * 0.9), int(50 * 0.9))
    btn_back.align(lv.ALIGN.BOTTOM_LEFT, 20, -20)
    btn_back.add_event_cb(lambda _: lv.scr_load(main_scr), lv.EVENT.CLICKED, None)
    label_back = lv.label(btn_back)
    label_back.set_text("Regresar")
    label_back.center()

def create_simulador_screen():
    scr = lv.obj()
    scr.set_style_bg_color(lv.color_hex(0xFFDAB9), 0)  # Naranja pastel
    lv.scr_load(scr)
    
    # Crear el LED (círculo virtual)
    led = lv.led(scr)
    led.set_size(50, 50)
    led.align(lv.ALIGN.TOP_MID, 0, 30)
    led.set_color(lv.color_hex(0xff0000))  # Inicialmente rojo
    
    # Crear dos switches
    sw1 = lv.switch(scr)
    sw1.align(lv.ALIGN.CENTER, -100, 0)  # A la izquierda
    
    sw2 = lv.switch(scr)
    sw2.align(lv.ALIGN.CENTER, 100, 0)  # A la derecha
    
    # Crear un selector de funciones (dropdown list)
    dd = lv.dropdown(scr)
    dd.set_options("\n".join(["AND", "OR", "XOR", "NAND", "NOR", "XNOR"]))
    dd.align(lv.ALIGN.CENTER, 0, 100)
    
    # Callback para actualizar el LED según la función seleccionada y los switches
    def update_led(e):
        state1 = sw1.has_state(lv.STATE.CHECKED)
        state2 = sw2.has_state(lv.STATE.CHECKED)
        func = dd.get_selected_str()
        
        if func == "AND":
            result = state1 and state2
        elif func == "OR":
            result = state1 or state2
        elif func == "XOR":
            result = state1 != state2
        elif func == "NAND":
            result = not (state1 and state2)
        elif func == "NOR":
            result = not (state1 or state2)
        elif func == "XNOR":
            result = state1 == state2
        else:
            result = False
        
        led.set_color(lv.color_hex(0x00ff00) if result else lv.color_hex(0xff0000))
    
    # Agregar callbacks a los switches y el dropdown
    sw1.add_event_cb(lambda e: update_led(e), lv.EVENT.VALUE_CHANGED, None)
    sw2.add_event_cb(lambda e: update_led(e), lv.EVENT.VALUE_CHANGED, None)
    dd.add_event_cb(lambda e: update_led(e), lv.EVENT.VALUE_CHANGED, None)
    
    # Botón para regresar a la pantalla de herramientas
    btn_back = lv.btn(scr)
    btn_back.set_size(int(200 * 0.9), int(50 * 0.9))
    btn_back.align(lv.ALIGN.BOTTOM_LEFT, 20, -20)
    btn_back.add_event_cb(lambda _: create_herramientas_screen(), lv.EVENT.CLICKED, None)
    label_back = lv.label(btn_back)
    label_back.set_text("Regresar")
    label_back.center()

def new_button_event_handler(_):
    print("hola mundo")

# Pantalla principal con botones
cont = lv.obj(main_scr)
cont.set_size(1024, 768)
cont.center()

# Etiquetas personalizadas para cada botón
button_texts = [
    "Visualizer",
    "Practicas Precargadas",
    "Editor de Codigo",
    "Herramientas"
]

# Crear botones en la pantalla principal
for i in range(4):
    btn = lv.btn(cont)
    btn.set_size(int((1024 // 2) * 0.9), int((768 // 2) * 0.9))
    # Alineación simple según posición en una cuadrícula 2x2.
    btn.align_to(cont, lv.ALIGN.CENTER, (i % 2) * 512 - 256, (i // 2) * 384 - 192)
    btn.add_event_cb(button_event_handler, lv.EVENT.CLICKED, None)
    
    label = lv.label(btn)
    label.set_text(button_texts[i])
    label.center()
    

# Bucle principal
while True:
    time.sleep_ms(1)
