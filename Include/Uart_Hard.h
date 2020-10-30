#ifndef XC_UART_HARD_H
#define XC_UART_HARD_H
#endif

#ifndef _XTAL_FREQ
#error  Debe definir la frecuencia del oscilador
#endif

//Definiciones de funciones
#define Serial_Hard_init    UART_init 
#define Serial_interrupt    UART_int
#define Serial_write        UART_write
#define Serial_print        UART_array
#define Serial_println      UART_arrayln
#define Serial_read         UART_read

//Activar o desactivar parametros
#define RECEP_ON    1
#define RECEP_OFF   0
#define TRANS_ON    1
#define TRANS_OFF   0

//Funciones
void UART_init(unsigned int BAUD);                                  // Inicializa las configuraciones
void UART_int(unsigned char recepcion,unsigned char transmicion);   // Configura las interrupciones del serial
void UART_write(char dato);                                         // Escribe un caracter
void UART_array(const char* cadena);                                // Envia una cadena de caracteres
void UART_arrayln(const char *cadena);                              // Envia una cadena con un salto de linea
char UART_read(void);                                               // Lee un caracter
