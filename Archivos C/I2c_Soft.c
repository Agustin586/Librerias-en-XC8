#include "C:\Users\aguat\Documents\MPLAB PROJECTS\Programas\P-Desarrollo-Final_18F.X\Configuracion_Bits.h"
#include "C:\Users\aguat\Documents\MPLAB PROJECTS\Librerias XC8\Include\I2c_Soft.h"

/*==============================================================================
                    INICIALIZACION DE LA COMUNICACION
==============================================================================*/
void i2c_init(void)
{
    SDA_TRIS = 1;           // Suelta la linea de datos
    SCL_TRIS = 1;           // Suelta la linea de reloj
    
    return;
}

/*==============================================================================
                    CONDICION DE INICIO DE COMUNICACION
==============================================================================*/
void i2c_start(void)
{
    //Condicion de inicio
    SDA_TRIS = 1;
    SCL_TRIS = 1;
    __delay_us(5);
    
    //Condicion de start
    SDA_PIN  = 0;           // Pone un cero en el bus
    SDA_TRIS = 0;           // Libera el bus para poner un cero
    __delay_us(2);          // Tiempo de lectura de condicion de inicio
    
    //Tiempo en bajo del bus
    SCL_PIN  = 0;           // Pone un cero en el reloj
    SCL_TRIS = 0;           // Libera el bus de reloj
    __delay_us(3);
    
    return;
}

/*==============================================================================
                    CONDICION DE PARADA DE LA COMUNICACION
==============================================================================*/
void i2c_stop(void)
{
    SDA_PIN  = 0;
    SDA_TRIS = 0;
    SCL_PIN  = 0;
    SCL_TRIS = 0;
    __delay_us(1);
    
    SCL_TRIS = 1;           // Levanta el bus en scl
    __delay_us(2);          
    SDA_TRIS = 1;           // Levanta el bus de sda
    __delay_us(2);
    
    __delay_us(5);
    
    return;
}

/*==============================================================================
            FUNCION DE ESCRITURA DE DATOS EN EL OTRO DISPOSITIVO
==============================================================================*/
void i2c_write(char date)
{
    //Reinicia el valor del estado del ack
    ACK_EST = 1;
    
    if(SCL_PIN == 1)
    {
        SCL_PIN  = 0;
        SCL_TRIS = 0;
    }
    __delay_us(5);
    
    for(char i=0;i<8;i++)
    {        
        if((date & 0x80) == 0)
        {
            SDA_PIN  = 0;           // Pone un cero en el bus de datos
            SDA_TRIS = 0;           // Librera el bus
        }
        else    SDA_TRIS = 1;       // Pone un '1' en sda
        
        date <<= 1;                 // Mueve el dato
        
        //Reloj
        __delay_us(2);
        SCL_TRIS = 1;               // Pone un '1' en scl
        __delay_us(3);

        SCL_TRIS = 0;               // Pone un '0' en scl
        SCL_PIN  = 0;
        __delay_us(5);              // Velocidad de frecuencia
    }
    
    //Noveno reloj para el ack
    SDA_TRIS = 1;
    __delay_us(2);
    SCL_TRIS = 1;
    ACK_EST = SDA_PIN;
    __delay_us(3);
    
    SCL_TRIS = 0;
    SCL_PIN  = 0;
    __delay_us(5);
    
    return;
}

void i2c_array(const char *cadena)
{
    while(*cadena)
    {
        i2c_write(*cadena);
        cadena++;
    }
    return;
}

/*==============================================================================
                    FUNCION DE LECTURA DEL DISPOSITIVO 
==============================================================================*/
char i2c_read(void)
{
    char dato=0;
    
    //Libera el bus al esclavo
    SDA_TRIS = 1;
    
    //Pone el clock en 0
    if(SCL_PIN == 1)
    {
        SCL_PIN  = 0;
        SCL_TRIS = 0;
    }
        
    for(char i=0;i<8;i++)
    {
        __delay_us(5);
        dato <<= 1;             // Realiza un corrimiento de la variable
        SCL_TRIS = 1;
        __delay_us(2);
        if(SDA_PIN == 1)    dato |= 1;
        __delay_us(3);

        if(SCL_PIN == 1)
        {
            SCL_TRIS = 0;           // Pone un '0' en sda
            SCL_PIN  = 0;  
        }   
    }
    
    return dato;
}

void i2c_nack(void)
{
    __delay_us(5);
    SDA_TRIS = 1;
    __delay_us(1);
    SCL_TRIS = 1;
    __delay_us(4);
    SCL_PIN  = 0;
    SCL_TRIS = 0;
    __delay_us(5);
    SDA_PIN  = 0;
    SDA_TRIS = 0;
    
    return;
}

void i2c_ack(void)
{
    if(SCL_PIN == 1)
    {
        SCL_PIN  = 0;
        SCL_TRIS = 0;   
    }
    __delay_us(5);
    SDA_PIN  = 0;
    SDA_TRIS = 0;
    __delay_us(1);
    SCL_TRIS = 1;
    __delay_us(3);
    SCL_PIN  = 0;
    SCL_TRIS = 0;
    __delay_us(1);
    SDA_TRIS = 1;
    
    return;
}

void i2c_restart(void)
{
    i2c_stop();
    i2c_start();
    
    return;
}