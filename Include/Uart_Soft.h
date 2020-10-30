#ifndef _XTAL_FREQ
#error  Debe definir la frecuencia del oscilador
#endif

#define cristal 4       //Obtiene el reloj en mega
#define BAUDIOS 9600    //Configura la velocidad 
//Define pines de conexion del puerto serial tx y rx
#define TX_PIN      PORTBbits.RB2
#define RX_PIN      PORTBbits.RB1
#define TX_TRIS     TRISBbits.TRISB2
#define RX_TRIS     TRISBbits.TRISB1

void UART_Soft_init(void);                                                      // Inicializa el puerto serial por software
void UART_Soft_write(char Data);                                                // Escribe un caracter 
void UART_Soft_array(char *Text);                                               // Escribe un texto de caracteres constantes
void UART_Soft_arrayln(char *Text);                                             // Escribe un texto y da un salto de linea
char UART_Soft_read(void);                                                      // Lee un caracter
void UART_Soft_read_array(char* Buffer, char StopChar,unsigned int tama);       // Lee un texto 

//Constantes necesarias para el calculo de los baudios y retardos el programa
#define DELAY_FOR  (4*22/cristal)               // Retardo extra en la escritura 14us       // depende del compilador usado
#define DELAY_FOR_READ  (4*17/cristal)          // Retardo extra en la Lectura    10us      // depende del compilador usado
#define BITPERIOD ((1000000/BAUDIOS))           // Duración de  1 bit en microsegundos
#define BITPERIOD_FOR (BITPERIOD - DELAY_FOR)   // Retardo real a la Escritura
#define BITPERIOD_FOR_READ (BITPERIOD - DELAY_FOR_READ)// Retardo real a la lectura
#define HALFBITPERIOD (BITPERIOD/2)             // Duración medio bit
//Fin constastes del puerto serial

//Definiciones de funciones seriales
#define Serial_Soft_init        UART_Soft_init
#define Serial_Soft_escribir    UART_Soft_write
#define Serial_Soft_cadena      UART_Soft_array
#define Serial_Soft_cadenaln    UART_Soft_arrayln
#define Serial_Soft_leer        UART_Soft_read
#define Serial_Soft_leer_cadena UART_Soft_read_array

////////////////////////////////////////////////////////////////////////////////

void UART_Soft_init(void)
{
    TX_PIN=1;             // Pone en 1 el pin de tx
    RX_PIN=1;             // Pone en 1 el pin de rx
    TX_TRIS=0;   // Setea como salida tx
    RX_TRIS=1;   // Setea como entrada rx
    TX_PIN=1;
    
    __delay_us(BITPERIOD);
    
    return;
}

////////////////////////////////////////////////////////////////////////////////

void UART_Soft_write(char Data)
{
    unsigned char mask;
    TX_PIN = 0;
    __delay_us(BITPERIOD);

    //el for demora 14 ciclos de reloj
    for (mask=0x01;mask!=0;mask=mask<<1)
    {
        if(Data&mask)   TX_PIN = 1;
        else            TX_PIN = 0;
        __delay_us(BITPERIOD_FOR);  //para mayor precision acorde al cristal   porque son 14 cilcos de reloj
    }
    
    TX_PIN = 1;
    __delay_us(BITPERIOD);
    
    __delay_us(500);
    
    return;
}

////////////////////////////////////////////////////////////////////////////////

void UART_Soft_array(char *Text)
{
    while(*Text)    UART_Soft_write(*Text++);
    
    __delay_ms(1);
    
    return;
}

void UART_Soft_arrayln(char *Text)
{
    while(*Text)    UART_Soft_write(*Text++);
    
    UART_Soft_array("\n"),UART_Soft_write(13);
    
    __delay_ms(1);
    
    return;
}

////////////////////////////////////////////////////////////////////////////////

char UART_Soft_read(void)
{
    char mask;
    char Data=0;

    while(RX_PIN == 1); //sino llega el bit de star se queda a esperarlo
    
    __delay_us(HALFBITPERIOD);
    
    for(mask=0x01;mask!=0;mask=mask<<1)
    {
        __delay_us(BITPERIOD_FOR_READ);   //retardo teniendo en cuenta os del for y el if es de 4 ciclos de reloj
        if(RX_PIN)    Data=Data|mask;
    }
    
    __delay_us(BITPERIOD);
    
    return Data;
}

////////////////////////////////////////////////////////////////////////////////

void UART_Soft_read_array(char* Buffer, char StopChar,unsigned int tama)
{
    unsigned int con_t=0;

    while(*(Buffer-1)!=StopChar)        //mientras no llegue el caracter deseado
    {
        con_t++;   
        *Buffer++ = UART_Soft_read();   //sigue almacenando en el buffer
        if (con_t>=tama) break; 
    }
    *--Buffer = 0; //coloca un cero al final para que sea una cadena
}
