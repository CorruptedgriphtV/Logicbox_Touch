try:
    import display_driver_utils
except ImportError:
    print("display_driver_utils module not found; ensure it is installed and your PYTHONPATH is set accordingly.")
    raise
try:
    import lvgl as lv
except ImportError:
    print("lvgl module not found; ensure the SDL lvgl version is installed and your PYTHONPATH is set accordingly.")
    raise
import time

# Configuración del driver de pantalla
drv = display_driver_utils.driver(width=320, height=240)

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
    ta.set_size(300, 140) # Adjusted size for 320x240 screen
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

# Función para el manejo de eventos de los switches en Practica 1
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

# Crea la pantalla para la Practica 1: Jueces
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

# Crea la pantalla para la Practica 2: sistema de seguridad
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
        switch_statess[idx] = True if state else False
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

# Crea la pantalla para las Practicas Precargadas
def create_practicas_screen():
    scr = lv.obj()
    lv.scr_load(scr)
    
    # Botón para la Practica 1: Jueces
    btn1 = lv.btn(scr)
    btn1.set_size(int(250 * 0.9), int(70 * 0.9))
    btn1.align(lv.ALIGN.CENTER, 0, -60)
    btn1.add_event_cb(lambda _: create_practica1_screen(), lv.EVENT.CLICKED, None)
    label1 = lv.label(btn1)
    label1.set_text("Practica 1: Jueces")
    label1.center()
    
    # Botón para la Practica 2: sistema de seguridad
    btn2 = lv.btn(scr)
    btn2.set_size(int(250 * 0.9), int(70 * 0.9))
    btn2.align(lv.ALIGN.CENTER, 0, 60)
    btn2.add_event_cb(lambda _: create_practica2_screen(), lv.EVENT.CLICKED, None)
    label2 = lv.label(btn2)
    label2.set_text("Practica 2: sistema de seguridad")
    label2.center()
    
    # Botón para regresar a la pantalla principal
    btn_back = lv.btn(scr)
    btn_back.set_size(int(150 * 0.9), int(40 * 0.9))
    btn_back.align(lv.ALIGN.BOTTOM_LEFT, 20, -20)
    btn_back.add_event_cb(lambda _: lv.scr_load(main_scr), lv.EVENT.CLICKED, None)
    label_back = lv.label(btn_back)
    label_back.set_text("Regresar")
    label_back.center()

# Crea la pantalla para Herramientas
def create_herramientas_screen():
    scr = lv.obj()
    lv.scr_load(scr)
    
    # Botón para Simulador de compuertas
    btn_simulador = lv.btn(scr)
    btn_simulador.set_size(int(300 * 0.9), int(70 * 0.9))
    btn_simulador.align(lv.ALIGN.CENTER, 0, -80)
    btn_simulador.add_event_cb(lambda _: create_simulador_screen(), lv.EVENT.CLICKED, None)
    label_simulador = lv.label(btn_simulador)
    label_simulador.set_text("Simulador de compuertas")
    label_simulador.center()
    
    # Botón para Resistencias a colores
    btn_resistencias = lv.btn(scr)
    btn_resistencias.set_size(int(300 * 0.9), int(70 * 0.9))
    btn_resistencias.align(lv.ALIGN.CENTER, 0, 40)
    btn_resistencias.add_event_cb(lambda _: create_resistencias_screen(), lv.EVENT.CLICKED, None)
    label_resistencias = lv.label(btn_resistencias)
    label_resistencias.set_text("Resistencias a colores")
    label_resistencias.center()

    # Botón para Editor de Codigo
    btn_editor = lv.btn(scr)
    btn_editor.set_size(int(300 * 0.9), int(70 * 0.9))
    btn_editor.align(lv.ALIGN.CENTER, 0, 160)
    btn_editor.add_event_cb(lambda _: create_editor_screen(), lv.EVENT.CLICKED, None)
    label_editor = lv.label(btn_editor)
    label_editor.set_text("Editor de Codigo")
    label_editor.center()
    
    # Botón para regresar a la pantalla principal
    btn_back = lv.btn(scr)
    btn_back.set_size(int(150 * 0.9), int(40 * 0.9))
    btn_back.align(lv.ALIGN.BOTTOM_LEFT, 20, -20)
    btn_back.add_event_cb(lambda _: lv.scr_load(main_scr), lv.EVENT.CLICKED, None)
    label_back = lv.label(btn_back)
    label_back.set_text("Regresar")
    label_back.center()

# Crea la pantalla para el Simulador de compuertas (versión mejorada de ui2.py)
def create_simulador_screen():
    scr = lv.obj()
    # Fondo en naranja pastel
    scr.set_style_bg_color(lv.color_hex(0xFFDAB9), 0)
    lv.scr_load(scr)
    
    # Crear el LED (círculo virtual)
    led = lv.led(scr)
    led.set_size(50, 50)
    led.align(lv.ALIGN.TOP_MID, 0, 30)
    led.set_color(lv.color_hex(0xff0000))  # Inicialmente rojo
    
    # Crear dos switches
    sw1 = lv.switch(scr)
    sw1.align(lv.ALIGN.CENTER, -100, 0)  # A la izquierda
    sw1.clear_state(lv.STATE.CHECKED)
    
    sw2 = lv.switch(scr)
    sw2.align(lv.ALIGN.CENTER, 100, 0)  # A la derecha
    sw2.clear_state(lv.STATE.CHECKED)
    
    # Variable para almacenar la función de la compuerta ("AND", "OR", etc.)
    func_mode = ["AND"]
    
    # Diccionario para guardar botones y lista de compuertas
    gate_names = ["AND", "OR", "XOR", "NAND", "NOR", "XNOR"]
    buttons = {}
    
    # Ajustamos tamaños y posiciones de los botones para la resolución 320x240
    # Primera fila: 3 botones centrados
    x_positions = [20, 120, 220]
    y_pos1 = 80
    for i, gate in enumerate(gate_names[:3]):
        btn = lv.btn(scr)
        btn.set_size(80, 40)
        btn.set_pos(x_positions[i], y_pos1)
        label = lv.label(btn)
        label.set_text(gate)
        label.center()
        buttons[gate] = btn
    
    # Segunda fila: otros 3 botones
    x_positions2 = [20, 120, 220]
    y_pos2 = 140
    for i, gate in enumerate(gate_names[3:]):
        btn = lv.btn(scr)
        btn.set_size(80, 40)
        btn.set_pos(x_positions2[i], y_pos2)
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
    
    # Inicializar estilos de botones
    update_button_styles()
    
    # Actualizar LED según estado inicial
    update_led()
    
    # Botón para regresar a la pantalla de herramientas
    btn_back = lv.btn(scr)
    btn_back.set_size(int(150 * 0.9), int(40 * 0.9))
    btn_back.align(lv.ALIGN.BOTTOM_LEFT, 20, -20)
    btn_back.add_event_cb(lambda _: create_herramientas_screen(), lv.EVENT.CLICKED, None)
    label_back = lv.label(btn_back)
    label_back.set_text("Regresar")
    label_back.center()

# Crea la pantalla para Resistencias a colores (implementación de RESISTE.py)
def create_resistencias_screen():
    scr = lv.obj()
    scr.set_style_bg_color(lv.color_hex(0xFFFFFF), 0)  # Fondo blanco
    lv.scr_load(scr)
    
    # Diccionario de colores para dígitos y exponentes (multiplicador)
    color_codes = {
        '0': 'Negro',
        '1': 'Marron',
        '2': 'Rojo',
        '3': 'Naranja',
        '4': 'Amarillo',
        '5': 'Verde',
        '6': 'Azul',
        '7': 'Morado',
        '8': 'Gris',  
        '9': 'Blanco'
    }
            
    # Diccionario de colores en formato lv.color_hex
    color_hex_codes = {
        'Negro': lv.color_hex(0x000000),
        'Marron': lv.color_hex(0x8B4513),
        'Rojo': lv.color_hex(0xFF0000),
        'Naranja': lv.color_hex(0xFFA500),
        'Amarillo': lv.color_hex(0xFFFF00),
        'Verde': lv.color_hex(0x008000),
        'Azul': lv.color_hex(0x0000FF),
        'Morado': lv.color_hex(0xEE82EE),
        'Gris': lv.color_hex(0x808080),
        'Blanco': lv.color_hex(0xFFFFFF),
        'Dorado': lv.color_hex(0xFFD700)
    }
    
    # Área de entrada para el número
    ta = lv.textarea(scr)
    ta.set_one_line(True)
    ta.set_width(400)
    ta.align(lv.ALIGN.TOP_MID, 0, 30)
    ta.set_placeholder_text("Ingrese valor (ej., 220)")
    
    # Label para mostrar el código de colores
    color_label = lv.label(scr)
    color_label.set_text("Colores: ")
    color_label.align(lv.ALIGN.TOP_MID, 0, 80)
    
    # Label para mostrar el código SMD debajo de los colores
    smd_label = lv.label(scr)
    smd_label.set_text("SMD: ")
    smd_label.align(lv.ALIGN.TOP_MID, 0, 130)
    
    # Función que convierte un valor numérico a un código de 4 bandas
    def number_to_colors(number):
        try:
            value = int(number)
            # Solo se admiten valores de dos dígitos significativos
            if value < 10 or value > 999:
                raise ValueError("Número fuera de rango")
            s = str(value)
            if len(s) < 2:
                s = s.zfill(2)
            # Primeras dos bandas: dígitos significativos
            first_digit = s[0]
            second_digit = s[1]
            # La tercera banda es el exponente (multiplicador = 10^(n-2))
            multiplier_exponent = str(len(s) - 2)
            colors = [color_codes[first_digit],
                      color_codes[second_digit],
                      color_codes[multiplier_exponent],
                      "Dorado"]
            return colors
        except ValueError:
            return ["Entrada inválida"]
    
    # Función para actualizar la pantalla (colores y código SMD)
    def update_colors(ev=None):
        number_str = ta.get_text()
        colors = number_to_colors(number_str)
        if colors[0] == "Entrada inválida":
            color_label.set_text("Colores: Entrada inválida")
            smd_label.set_text("SMD: ")
        else:
            color_label.set_text("Colores: " + ", ".join(colors))
            try:
                value = int(number_str)
                s = str(value)
                if len(s) < 2:
                    s = s.zfill(2)
                # El código SMD consta de los dos primeros dígitos y el exponente
                smd_code = s[0:2] + str(len(s)-2)
                smd_label.set_text("SMD: " + smd_code)
            except:
                smd_label.set_text("SMD: ")
            
            # Dimensiones para las bandas de colores
            band_width = 40
            band_height = 100
            spacing = 10
            count = len(colors)
            
            # Crear rectángulos para cada banda, centrados en la pantalla
            for i, color in enumerate(colors):
                rect = lv.obj(scr)
                rect.set_size(band_width, band_height)
                # Calcular el offset horizontal para centrar los rectángulos
                offset_x = int((i - (count - 1) / 2) * (band_width + spacing))
                rect.align(lv.ALIGN.TOP_MID, offset_x, 180)
                rect.set_style_bg_color(color_hex_codes[color], 0)
    
    # Asignar callback para actualizar al cambiar el valor
    ta.add_event_cb(lambda ev: update_colors(ev), lv.EVENT.VALUE_CHANGED, None)
    
    # Botón para regresar a la pantalla de herramientas
    btn_back = lv.btn(scr)
    btn_back.set_size(int(200 * 0.9), int(50 * 0.9))
    btn_back.align(lv.ALIGN.BOTTOM_LEFT, 20, -20)
    btn_back.add_event_cb(lambda _: create_herramientas_screen(), lv.EVENT.CLICKED, None)
    label_back = lv.label(btn_back)
    label_back.set_text("Regresar")
    label_back.center()

# Crea la pantalla para el Editor de Codigo
def create_editor_screen():
    scr = lv.obj()
    lv.scr_load(scr)

    # Text area para ingresar el código
    code_ta = lv.textarea(scr)
    code_ta.set_size(300, 80)
    code_ta.align(lv.ALIGN.TOP_MID, 0, 10)
    code_ta.set_placeholder_text("Escribe tu código aquí...")

    # Text area para mostrar el código
    code_display_ta = lv.textarea(scr)
    code_display_ta.set_size(300, 40)
    code_display_ta.align(lv.ALIGN.TOP_MID, 0, 100)
    code_display_ta.set_placeholder_text("Visualización del código...")

    # Keyboard
    kb = lv.keyboard(scr)
    kb.set_textarea(code_ta)
    kb.set_size(320, 70)
    kb.align(lv.ALIGN.BOTTOM_MID, 0, 0)

    # Save buttons
    save_btn = lv.btn(scr)
    save_btn.set_size(100, 50)
    save_btn.align(lv.ALIGN.TOP_MID, 0, 300) # Corrected alignment
    save_label = lv.label(save_btn)
    save_label.set_text("Save")
    save_label.center()

    def save_code(e):
        code = code_ta.get_text()
        try:
            with open("user_code.py", "w") as f:
                f.write(code)
            print("Code saved to user_code.py")
        except Exception as e:
            print("Error saving code: {}".format(e))

    save_btn.add_event_cb(save_code, lv.EVENT.CLICKED, None)

    # Botón para regresar a la pantalla de herramientas
    btn_back = lv.btn(scr)
    btn_back.set_size(int(150 * 0.9), int(40 * 0.9))
    btn_back.align(lv.ALIGN.BOTTOM_LEFT, 20, 200)
    btn_back.add_event_cb(lambda _: create_herramientas_screen(), lv.EVENT.CLICKED, None)
    label_back = lv.label(btn_back)
    label_back.set_text("Regresar")
    label_back.center()

# Evento de los botones en la pantalla principal
def button_event_handler(e):
    btn = e.get_target()
    label = btn.get_child(0)
    text = label.get_text()
    if text == "Visualizer":
        create_shell_viewer()
    elif text == "Practicas Precargadas":
        create_practicas_screen()
    elif text == "Editor de Codigo": # Added condition for Editor button
        create_editor_screen()
    elif text == "Herramientas":
        create_herramientas_screen()
    else:
        print("Button {} clicked".format(text))

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
    btn.set_size(int((320 // 2) * 0.9), int((240 // 2) * 0.9))
    # Alineación simple según posición en una cuadrícula 2x2.
    btn.align_to(cont, lv.ALIGN.CENTER, (i % 2) * 160 - 80, (i // 2) * 120 - 60)
    btn.add_event_cb(button_event_handler, lv.EVENT.CLICKED, None)
    
    label = lv.label(btn)
    label.set_text(button_texts[i])
    label.center()

# Bucle principal
while True:
    time.sleep_ms(1)
