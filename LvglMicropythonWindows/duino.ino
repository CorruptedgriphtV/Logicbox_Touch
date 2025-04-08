#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>  // Para sprintf

#define BAUD 9600
#define UBRR_VALUE ((F_CPU)/(16UL*BAUD)-1)

// Variable global para contar los pulsos del encoder (salida A)
volatile uint32_t pulse_count = 0;
// Bandera que indica que el sensor Hall se ha activado
volatile uint8_t hall_triggered = 0;

// Inicializa la UART
void uart_init(void) {
    // Establece la tasa de baudios
    UBRR0H = (unsigned char)(UBRR_VALUE >> 8);
    UBRR0L = (unsigned char)UBRR_VALUE;
    // Habilita transmisor y receptor
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    // Configura el frame: 8 bits de datos, 1 bit de parada, sin paridad
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

// Transmite un carácter por UART
void uart_transmit(unsigned char data) {
    // Espera a que el buffer esté vacío
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}

// Envía una cadena por UART
void uart_print(const char* str) {
    while (*str) {
        uart_transmit(*str++);
    }
}

// ISR para contar los pulsos del encoder (INT0 en PD2)
// Se configura para detectar flanco de subida
ISR(INT0_vect) {
    pulse_count++;
}

// ISR para el sensor Hall (Pin Change en PC5, que corresponde a PCINT13)
ISR(PCINT1_vect) {
    // Suponemos que el sensor devuelve nivel bajo al activar (activo en bajo)
    if (!(PINC & (1 << PC5))) {
        hall_triggered = 1;
    }
}

int main(void) {
    uart_init();
    
    // Configurar la entrada del encoder en PD2 (INT0)
    DDRD &= ~(1 << PD2);      // PD2 como entrada
    PORTD |= (1 << PD2);       // Activar resistencia pull-up
    
    // Configurar INT0 para detectar flanco de subida
    EICRA |= (1 << ISC01) | (1 << ISC00);
    EIMSK |= (1 << INT0);      // Habilitar INT0
    
    // Configurar el sensor Hall en PC5
    DDRC &= ~(1 << PC5);      // PC5 como entrada
    PORTC |= (1 << PC5);       // Activar la pull-up interna
    
    // Habilitar la interrupción por cambio de nivel para PC5
    // PC5 corresponde a PCINT13 (en el grupo PCIE1 para PORTC)
    PCMSK1 |= (1 << PCINT13);
    PCICR |= (1 << PCIE1);    // Habilitar interrupciones por cambio de nivel para el grupo 1
    
    sei();  // Habilitar interrupciones globales
    
    char buffer[32];
    
    while (1) {
        // Cuando se activa el sensor Hall, se asume que se ha completado una revolución o se marca el
        // inicio de una medición. Entonces se imprime el número de pulsos acumulados.
        if (hall_triggered) {
            sprintf(buffer, "Pulsos: %lu\n", pulse_count);
            uart_print(buffer);
            pulse_count = 0;   // Reinicia el contador para la siguiente medición
            hall_triggered = 0;
            // Retardo para evitar múltiples activaciones consecutivas (debounce)
            _delay_ms(200);
        }
    }
    
    return 0;
}