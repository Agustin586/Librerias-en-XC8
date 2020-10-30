#include "C:\Users\aguat\Documents\MPLAB PROJECTS\Programas\P-Desarrollo-Final_18F.X\Configuracion_Bits.h"        //Configuracion de bits en archivo .h del proyecto
#include "C:\Users\aguat\Documents\MPLAB PROJECTS\Librerias XC8\Include\Lcd_i2c.h"

/*==============================================================================
                            INICIALIZACION DEL LCD
==============================================================================*/
void LCD_init(void)
{
    //Tiempo de inicilizacion
    __delay_ms(25);
    
    for(char i=0;i<3;i++)
    {
        LCD_command(0x03);                  //Recomendado para inicializar el controlador
        __delay_ms(7);
    }
    
    //Configuraciones del LCD 16x2
	LCD_command(0x33);                      //Inicializa el controlador
    __delay_us(100);                        
    LCD_command(0x32);                      //Modo de 4 bits
    __delay_ms(5);  
	LCD_command(0x28);                      //Modo 4 bits,2 lineas y fuente de 5x7
    __delay_us(100);
	LCD_command(0x0C);                  
    __delay_us(100);
	LCD_command(0x06);
    __delay_us(100);
    LCD_command(CLEAR);                     //Limpia la pantalla
    __delay_ms(7);
    
    return;
}

/*==============================================================================
                        ENVIA LOS COMANDOS DEL LCD
==============================================================================*/
void LCD_command(unsigned char cmd)
{
    //Inicia la comunicacion con el esclavo
    i2c_start();                            // Inicia la comunicacion
    __delay_us(100);
    
    i2c_write(0x4E);                        // Id del esclavo
    __delay_us(100);
    
    
    
    //Envia la parte alta del comando
    i2c_write((cmd & 0xf0)| 0x0c);          // Envia parte alta de comando y pone E = 1
    __delay_us(100);
    
    i2c_write((cmd & 0xf0)| 0x08);          // E = 0
    __delay_us(100);
    
    
    //Envia la parte baja del comando
    i2c_write((cmd<<4 & 0xf0)| 0x0c);       // Parte baja y E = 1
    __delay_us(100);
    
    i2c_write((cmd<<4 & 0xf0)| 0x08);       // E = 0
    __delay_us(100);
    
    
    
    i2c_stop();                             // Termina la comunicacion
    __delay_ms(2);
    
    return;
}


/*==============================================================================
                        ENVIA UN CARACTER AL LCD
==============================================================================*/
void LCD_date(char date)
{
    //Inicia la comunicacion con el esclavo
    i2c_start();                            // Inicia la comunicacion
    __delay_us(100);
    
    i2c_write(0x4E);                        // Id esclavo
    __delay_us(100);


    
    //Escribe la parte alta del dato
    i2c_write((date & 0xf0)| 0x0d);         // Parte alta, RS = 1, E = 1
    __delay_us(100);
    
    i2c_write((date & 0xf0)| 0x09);         // RS = 1, E = 1
    __delay_us(100);

    
    //Escribe la parte baja del dato
    i2c_write((date<<4 & 0xf0)| 0x0d);      // Parte baja, RS = 1, E = 1
    __delay_us(100);
    
    i2c_write((date<<4 & 0xf0)| 0x09);      // RS = 1, E = 1
    __delay_us(100);
    
    
    
    i2c_stop();                             // Termina la comunicacion
    __delay_ms(2);
    
    return;
}

/*==============================================================================
                    UBICA Y ENVIA UNA PALABRA EN EL LCD
==============================================================================*/
void LCD_array(int x,int y,const char *date)
{
    //Ubica el lcd
    switch(x)
    {
        case 1: LCD_command(0x80 | (0x00 + (y - 1)));
        break;
        case 2: LCD_command(0x80 | (0x40 + (y - 1)));
        break;
        case 3: LCD_command(0x80 | (0x14 + (y - 1)));
        break;
        case 4: LCD_command(0x80 | (0x54 + (y - 1)));
        break;
    }
    
    __delay_ms(1);
    
    //Envia la cadena
    while(*date)
    {
        LCD_date(*date);
        date++;
    }
    
    return;
}

/*==============================================================================
                UBICA EL LCD EN UNA POSICION ESPECÍFICA
==============================================================================*/
void LCD_xy(int x,int y)
{
    switch(x)
    {
        case 1: LCD_command(0x80 | (0x00 + (y - 1)));
        break;
        case 2: LCD_command(0x80 | (0x40 + (y - 1)));
        break;
        case 3: LCD_command(0x80 | (0x14 + (y - 1)));
        break;
        case 4: LCD_command(0x80 | (0x54 + (y - 1)));
        break;
    }
    
    return;
}

/*==============================================================================
                    UTILIZA EL CORRIMIENTO DE PANTALLA
==============================================================================*/
void LCD_shift(unsigned char dir,unsigned char cant)
{
    char i=1;
    //Realiza el corrimiento de pantalla
    while(i <= cant)
    {
        LCD_command(dir);
        __delay_ms(250);
        i++;
    }
    LCD_command(HOME);                      //Vuelve a la posicion inicial
    i = 1;
    
    return;
}

/*==============================================================================
            GENERA UN CARACTER ESPECIAL QUE SE GUARDA EN EL LCD
==============================================================================*/
void LCD_character(unsigned char adress,char caracter[])
{
    LCD_command(0x40 + (adress * 8));
    for(char i=0;i<8;i++)
    {
        LCD_date(caracter[i]);
    }
    
    return;
}
