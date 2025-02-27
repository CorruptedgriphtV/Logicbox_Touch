<div align="center">

# Manual LogicBox Touch 2025


</div>


Colaboradores autorizados a hacer cambios:<br>
    • EMILIO ESTEFAN ARREDONDO HINOSTROSA<br>
    • MELVI ESTRELLA LOPEZ CASTRO<br>
    • JOSAFAT CRUZ OJEDA<br>


<hr>

# Introducción
LogicBox Touch es un sistema que permite a los usuarios aprender lógica digital y programación básica en mecatrónica de manera interactiva. Es un dispositivo digital educativo portátil con una pantalla táctil (CYD - Cheap Yellow Display) que permite la interacción mediante un stylus, eliminando la necesidad de botones físicos.

El LogicBox Touch Pro integra puertos GPIO para la conexión de sensores y actuadores, permitiendo realizar prácticas de circuitos electrónicos y proyectos basados en hardware real. Se propondrán dos prácticas para que el alumno pueda aprender a usar la herramienta. Además, se desarrollará una carcasa impresa en 3D para proteger y personalizar el dispositivo, optimizando su portabilidad y robustez.

El dispositivo también incluirá herramientas virtuales educativas, simuladores de lógica digital y acceso a guías interactivas, ofreciendo una solución completa para la práctica y aprendizaje de la electrónica aplicada, con un enfoque modular y práctico que emula la experiencia de dispositivos como el Flipper Zero, adaptado para el entorno académico.

# • Componentes del LogicBox Touch
Para poder construir tu LogicBox Touch puedes comprar un kit preensamblado de nuestra empresa o conseguir los siguientes materiales que normalmente utilizamos para nuestros productos: 
- Pantalla táctil TFT 
- Microcontrolador compatible con Arduino (O COMPRAR CYD)
- Módulo de alimentación TP-4056 
- Bateria de litio de cualquier valor
- Placa de expansión con puertos GPIO MCP23017
- Sensores y actuadores (opcional)
- Carcasa impresa en 3D (opcional)
- Stylus para pantalla táctil
- Cable USB para programación y carga
- Manual de usuario y guías interactivas
- Boton de prendido y apagado

# • Especificaciones Técnicas


### Pantalla Táctil CYD (Cheap Yellow Display)
- **Tipo:** TFT
- **Tamaño:** 2.4 pulgadas
- **Resolución:** 320x240 píxeles
- **Interfaz:** SPI
- **Voltaje de operación:** 3.3V/5V
- **Consumo de corriente:** 50mA (típico)
- **Características adicionales:** Soporte para entrada táctil resistiva

### Módulo de Alimentación TP 4056
- **Tipo:** Cargador de batería de litio
- **Voltaje de entrada:** 4.5V-5.5V
- **Voltaje de salida:** 4.2V (carga completa)
- **Corriente de carga:** 1A (ajustable)
- **Protección:** Sobrecarga, sobrecorriente, cortocircuito

### Batería de Litio
- **Capacidad:** 500mAh
- **Voltaje nominal:** 3.7V
- **Tiempo de carga:** Aproximadamente 1 hora (con corriente de carga de 1A)
- **Tiempo de uso** Aproximadamente 5 horas (brillo al 50%, standby 2 horas, sin perifericos conectados )
### Encendido y Apagado del LogicBox Touch

Para encender el LogicBox Touch, sigue estos pasos:

1. Asegúrate de que la batería esté cargada o conecta el dispositivo a una fuente de alimentación mediante el cable USB.
2. Presiona el botón de encendido ubicado en el lateral del dispositivo.
3. Espera unos segundos hasta que la pantalla táctil se ilumine y muestre la pantalla de inicio.

Para apagar el LogicBox Touch:

1. Presiona el boton de apagado.
2. Asegurate de cargar tu equipo al menos cada 2 semanas

### Configuración Inicial

1. Al encender el dispositivo por primera vez, entra en ajustes y edita las configuraciones.
2. Utiliza el stylus o el dedo para interactuar con la pantalla táctil y completar la configuración inicial.

### Uso del Stylus

1. El stylus se utiliza para navegar por los menús y seleccionar opciones en la pantalla táctil.
2. Toca suavemente la pantalla con el stylus para hacer selecciones.
3. Para desplazarte por menús largos, desliza el stylus hacia arriba o hacia abajo en la pantalla.

Con estos pasos, estarás listo para comenzar a usar tu LogicBox Touch y explorar todas sus funcionalidades.

### Navegación por Menús

1. La pantalla de inicio muestra varios iconos que representan las diferentes funciones del LogicBox Touch.
2. Utiliza el stylus para tocar el icono de la función que deseas utilizar.
3. Para volver a la pantalla anterior, toca el botón de retroceso en la esquina superior izquierda de la pantalla.

Con estos pasos, podrás navegar fácilmente por los menús del LogicBox Touch y acceder a todas sus funciones.
### Descripción de Iconos y Funciones

1. **Icono de Inicio**: Representa la pantalla principal del dispositivo donde se encuentran los accesos directos a las funciones principales.
2. **Icono de Configuración**: Permite acceder a los ajustes del dispositivo, como brillo de pantalla, volumen, y configuración de red.
3. **Icono de Simuladores**: Acceso a los simuladores de lógica digital y otras herramientas educativas virtuales.
4. **Icono de Prácticas**: Acceso a las prácticas propuestas para aprender lógica digital y programación básica.
5. **Icono de Sensores**: Permite gestionar y configurar los sensores y actuadores conectados al dispositivo.
6. **Icono de Ayuda**: Acceso a guías interactivas y manuales de usuario para resolver dudas y problemas comunes.

### Práctica 1: Introducción a la Lógica Digital

1. **Objetivo**: Aprender los conceptos básicos de la lógica digital utilizando el LogicBox Touch.
2. **Materiales Necesarios**: LogicBox Touch, stylus.
3. **Instrucciones**:
    - Enciende el LogicBox Touch y selecciona el icono de prácticas.
    - Selecciona "Práctica 1: Introducción a la Lógica Digital".
    - Sigue las instrucciones en pantalla para completar los ejercicios de lógica digital.

### Práctica 2: Programación Básica en Mecatrónica

1. **Objetivo**: Introducir al usuario a la programación básica en mecatrónica utilizando el LogicBox Touch.
2. **Materiales Necesarios**: LogicBox Touch, stylus, sensores y actuadores (opcional).
3. **Instrucciones**:
    - Enciende el LogicBox Touch y selecciona el icono de prácticas.
    - Selecciona "Práctica 2: Programación Básica en Mecatrónica".
    - Sigue las instrucciones en pantalla para escribir y ejecutar programas básicos que interactúan con sensores y actuadores.

### Uso de los Puertos GPIO

1. **Descripción**: Los puertos GPIO permiten la conexión de sensores y actuadores al LogicBox Touch.
2. **Ejemplos de Conexión**:
    - **Sensor de Temperatura**: Conecta el sensor de temperatura al puerto GPIO 1 y sigue las instrucciones en pantalla para leer la temperatura.
    - **LED**: Conecta un LED al puerto GPIO 2 y sigue las instrucciones en pantalla para encender y apagar el LED.

### Simuladores de Lógica Digital

1. **Descripción**: Los simuladores de lógica digital permiten practicar y experimentar con circuitos lógicos sin necesidad de hardware adicional.
2. **Acceso**: Selecciona el icono de simuladores en la pantalla de inicio y elige el simulador de lógica digital.
3. **Instrucciones**:
    - Sigue las instrucciones en pantalla para crear y simular circuitos lógicos.
    - Utiliza el stylus para colocar y conectar componentes en el simulador.

### Guías Interactivas

1. **Descripción**: Las guías interactivas proporcionan instrucciones paso a paso para realizar diversas tareas y prácticas con el LogicBox Touch.
2. **Acceso**: Selecciona el icono de ayuda en la pantalla de inicio y elige la guía interactiva que deseas seguir.
3. **Instrucciones**:
    - Sigue las instrucciones en pantalla para completar la guía interactiva.
    - Utiliza el stylus para interactuar con la pantalla y seguir los pasos de la guía.

### Cuidado del Dispositivo

1. **Limpieza**: Limpia la pantalla táctil con un paño suave y seco. Evita el uso de productos químicos.
2. **Almacenamiento**: Guarda el LogicBox Touch en un lugar seco y seguro cuando no esté en uso.
3. **Batería**: Carga la batería regularmente para mantener el dispositivo en buen estado de funcionamiento.

### Resolución de Problemas Comunes

1. **El dispositivo no enciende**:
    - Asegúrate de que la batería esté cargada.
    - Conecta el dispositivo a una fuente de alimentación mediante el cable USB.
    - Presiona el botón de encendido durante unos segundos.

2. **La pantalla táctil no responde**:
    - Limpia la pantalla con un paño suave y seco.
    - Reinicia el dispositivo presionando el botón de encendido durante 10 segundos.

3. **Problemas de conexión con sensores y actuadores**:
    - Verifica que los sensores y actuadores estén correctamente conectados a los puertos GPIO.
    - Consulta la guía interactiva correspondiente para obtener instrucciones detalladas.

### Carcasa Impresa en 3D

1. **Descripción**: La carcasa impresa en 3D protege y personaliza el LogicBox Touch.
2. **Instrucciones**:
    - Descarga el archivo de diseño de la carcasa desde nuestro sitio web.
    - Imprime la carcasa utilizando una impresora 3D.
    - Ensambla la carcasa siguiendo las instrucciones incluidas en el archivo de diseño.

### Accesorios Adicionales

1. **Descripción**: Accesorios adicionales para mejorar la funcionalidad y personalización del LogicBox Touch.
2. **Ejemplos**:
    - **Soporte para Stylus**: Un soporte para mantener el stylus siempre a mano.
    - **Protector de Pantalla**: Un protector de pantalla para evitar arañazos y daños.
    - **Funda de Transporte**: Una funda para transportar el LogicBox Touch de manera segura.

### Enlaces a Documentación y Soporte

1. **Documentación**: Accede a la documentación completa del LogicBox Touch en nuestro sitio web.
2. **Soporte**: Contacta con nuestro equipo de soporte para resolver cualquier duda o problema.
3. **Comunidad y Foros de Usuarios**: Únete a nuestra comunidad en línea para compartir experiencias y obtener ayuda de otros usuarios.

### Comunidad y Foros de Usuarios

1. **Descripción**: Únete a nuestra comunidad en línea para compartir experiencias y obtener ayuda de otros usuarios.
2. **Acceso**: Visita nuestro sitio web y regístrate en los foros de usuarios.
3. **Beneficios**:
    - Comparte tus proyectos y prácticas con otros usuarios.
    - Obtén ayuda y consejos de la comunidad.
    - Participa en eventos y concursos organizados por nuestra empresa.
<!--

# Contenidos del Manual

A continuación, se detallan los subtítulos que debería llevar el manual para guiar al usuario en el uso del LogicBox Touch 2025:

## 1. Descripción del Dispositivo
- Componentes del LogicBox Touch
- Especificaciones Técnicas

## 2. Primeros Pasos
- Encendido y Apagado
- Configuración Inicial
- Uso del Stylus

## 3. Interfaz de Usuario
- Navegación por Menús
- Descripción de Iconos y Funciones

## 4. Prácticas Propuestas
- Práctica 1: Introducción a la Lógica Digital
- Práctica 2: Programación Básica en Mecatrónica

## 5. Conexión de Sensores y Actuadores
- Uso de los Puertos GPIO
- Ejemplos de Conexión

## 6. Uso de Simuladores y Herramientas Virtuales
- Simuladores de Lógica Digital
- Guías Interactivas

## 7. Mantenimiento y Solución de Problemas
- Cuidado del Dispositivo
- Resolución de Problemas Comunes

## 8. Personalización y Accesorios
- Carcasa Impresa en 3D
- Accesorios Adicionales

## 9. Recursos Adicionales
- Enlaces a Documentación y Soporte
- Comunidad y Foros de Usuarios

<hr>

-->
