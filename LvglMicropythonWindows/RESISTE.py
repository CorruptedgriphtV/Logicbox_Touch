import display_driver_utils
import time
import lvgl as lv

drv = display_driver_utils.driver(width=320, height=240)

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

def crear_pantalla_codigo_colores_resistor():
    scr = lv.obj()
    scr.set_style_bg_color(lv.color_hex(0xFFFFFF), 0)  # Fondo blanco
    lv.scr_load(scr)
    
    # Área de entrada para el número
    ta = lv.textarea(scr)
    ta.set_one_line(True)
    ta.set_width(200)
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
            
            # Dimensiones reducidas (20% menos)
            small_width = 24   # 30 * 0.8
            small_height = 48  # 60 * 0.8
            spacing = 10
            count = len(colors)
            
            # Crear rectángulos para cada banda, centrados en la pantalla
            for i, color in enumerate(colors):
                rect = lv.obj(scr)
                rect.set_size(small_width, small_height)
                # Calcular el offset horizontal para centrar los rectángulos
                offset_x = int((i - (count - 1) / 2) * (small_width + spacing))
                rect.align(lv.ALIGN.TOP_MID, offset_x, 180)
                rect.set_style_bg_color(color_hex_codes[color], 0)
    
    # Asignar callback para actualizar al cambiar el valor
    ta.add_event_cb(lambda ev: update_colors(ev), lv.EVENT.VALUE_CHANGED, None)

crear_pantalla_codigo_colores_resistor()

while True:
    time.sleep_ms(1)