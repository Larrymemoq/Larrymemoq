/*
 * File:   Contador_rfid_freq.c
 * Author: digarcia
 *
 * Created on 21 de agosto de 2023, 04:12 PM
 */

// Configuración del microcontrolador
#pragma config FOSC = INTOSC // Configurar el oscilador interno
#pragma config WDTE = OFF    // Desactivar el watchdog timer
#pragma config PWRTE = OFF   // Desactivar el power-up timer
#pragma config MCLRE = OFF   // Desactivar el reset por pin MCLR
#pragma config CP = OFF      // Desactivar la protección de código

#include <xc.h> // Incluir el archivo de cabecera específico del PIC

#define _XTAL_FREQ 4000000   // Frecuencia del oscilador en Hz

// Configuración de los pines
#define RESET_BUTTON PORTAbits.RA1
#define LED_PIN PORTAbits.RA2
#define COUNT_BUTTON PORTAbits.RA0

// Función de inicialización
void initialize() {
    // Configurar el pin RA2 como salida (LED)
    TRISA2 = 0;

    // Configurar el pin RA0 como entrada (botón de conteo)
    TRISA0 = 1;
    TRISA1 = 1;

    // Desactivar comparadores analógicos
    ANSELA = 0;

    // Habilitar resistencia de pull-up en el pin del botón
    WPUA0 = 1;
    WPUA1 = 1;
}

void main(void) {
    initialize();

    unsigned int contador = 0;
    LED_PIN = 0;

    while (1) {
        // Verificar si el botón de conteo está presionado
        if (COUNT_BUTTON == 0) {
            // Incrementar el contador
            contador++;

            // Reiniciar el contador y encender el LED si llega a 5
            if (contador >= 6) {
                contador = 0;
                LED_PIN = 1;  // Encender el LED
                __delay_ms(1000);
                LED_PIN = 0;
            }
            // Esperar hasta que se suelte el botón
            while (COUNT_BUTTON == 0);
        }
        if (RESET_BUTTON == 0){
            contador = 0;
            LED_PIN = 0;
        }
    }
}
