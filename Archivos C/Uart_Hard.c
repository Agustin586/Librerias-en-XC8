#include "C:\Users\aguat\Documents\MPLAB PROJECTS\Programas\P-Desarrollo-Final_18F.X\Configuracion_Bits.h"  //Configuracion de bits en archivo .h del proyecto
#include "C:\Users\aguat\Documents\MPLAB PROJECTS\Librerias XC8\Include\Uart_Hard.h"

void UART_init(unsigned int BAUD)
{
    //Configuracion de puertos
    TRISCbits.TRISC7 = 1;   //Pone el Rx como entrada para leer
    TRISCbits.TRISC6 = 0;   //Configura como salida el Tx
    
    
    //Configuracion de baudios
    TXSTAbits.BRGH = 0;     // Baja velocidad
    SPBRG = (unsigned char) (((_XTAL_FREQ / BAUD) / 64) - 1);            
    
    //Modo de comunicacion
    TXSTAbits.SYNC  = 0;     // Modo asincronico
    
    //Habilita los pines seriales Tx y Rx
    RCSTAbits.SPEN = 1;
    
    
    //Configuracion de registro de transmision
    TXSTAbits.TX9   = 0;     // Configura solo palabra de 8 bits
    TXSTAbits.TXEN  = 1;     // Habilita la transmicion
    
    //Configuracion de registro de recepcion
    RCSTAbits.CREN = 1;     // Habilita recepcion
    RCSTAbits.RC9  = 0;     // Recepción 8 bits
    
    return;
}

////////////////////////////////////////////////////////////////////////////////

void UART_int(unsigned char recepcion,unsigned char transmicion)
{
    if(recepcion == 1)
    {
        //Interrupcion por recepcion
        #ifdef _PIC18F4550_H_
        INTCONbits.GIE  = 1;    // Habilita interrupcion global
        PIE1bits.RCIE   = 1;    // Habilita la interrupcion por recepcion
        IPR1bits.RCIP   = 1;    // Pone prioridad alta en la interrupcion
        INTCONbits.PEIE = 1;    // Habilita la interrupcion por perifericos

        #else
        INTCONbits.GIE = 1;
        PIE1bits.RCIE  = 1;
        INTCONbits.PEIE= 1;
        #endif
    }
    
    if(transmicion == 1)
    {
        //Interrupcion por transmicion
        #ifdef _PIC18F4550_H_
        INTCONbits.GIE  = 1;    // Habilita interrupcion global
        PIE1bits.TXIE   = 1;    // Habilita la interrupcion por recepcion
        IPR1bits.RCIP   = 1;    // Pone prioridad alta en la interrupcion
        INTCONbits.PEIE = 1;    // Habilita la interrupcion por perifericos

        #else
        INTCONbits.GIE = 1;
        PIE1bits.TXIE  = 1;
        INTCONbits.PEIE= 1;
        #endif
    }
    
    return;
}

////////////////////////////////////////////////////////////////////////////////

void UART_write(char dato)
{
    TXREG = dato;               //Enviar el dato
    while(TXSTAbits.TRMT == 0); //Lee si todavia no se termino de enviar todo el dato
    return;
}

////////////////////////////////////////////////////////////////////////////////

void UART_array(const char* cadena)
{
    while(*cadena)
    {
        TXREG = *cadena;               //Enviar el dato
        while(TXSTAbits.TRMT == 0); //Lee si todavia no se termino de enviar todo el dato
        cadena++;
    }
    return;
}

////////////////////////////////////////////////////////////////////////////////

void UART_arrayln(const char *cadena)
{
    while(*cadena)
    {
        TXREG = *cadena;               //Enviar el dato
        while(TXSTAbits.TRMT == 0); //Lee si todavia no se termino de enviar todo el dato
        cadena++;
    }
    
    UART_array("\n");
    UART_write(13);
}

////////////////////////////////////////////////////////////////////////////////

char UART_read(void)
{
    if(PIR1bits.RCIF == 1)  return RCREG;   //Si hay algo para leer lo devuelve 
    else return 0;
}