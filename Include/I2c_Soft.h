#ifndef _XTAL_FREQ
#error  Debe definir la frecuencia del oscilador
#endif

#ifndef SOFT_I2C_H
#define SOFT_I2C_H

//Definiciones de pines para utilizar
#define SDA_TRIS    TRISBbits.TRISB0
#define SCL_TRIS    TRISBbits.TRISB1
#define SDA_PIN     PORTBbits.RB0
#define SCL_PIN     PORTBbits.RB1

//Definiciones de funciones
#define I2c_init    i2c_init
#define I2c_start   i2c_start
#define I2c_stop    i2c_stop
#define I2c_write   i2c_write
#define I2c_read    i2c_read
#define I2c_ack     i2c_ack
#define I2c_nack    i2c_nack
#define I2c_print   i2c_array

_Bool ACK_EST=0;

void i2c_init(void);            // Inicializa el i2c
void i2c_start(void);           // Condicion de start de la comunicacion
void i2c_stop(void);            // Condicion de stopt de la comunicacion
void i2c_restart(void);         // Condicion de restart
void i2c_write(char date);      // Envia un caracter por i2c
char i2c_read(void);            // Lee un caracter por i2c
void i2c_ack(void);             // Condicion de ack
void i2c_nack(void);            // Condicion de nack 
void i2c_clock(char speed);     // Clock del i2c
void i2c_array(const char *);   // Envia una cadena de caracteres

#endif
