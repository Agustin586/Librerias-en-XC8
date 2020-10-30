#include "Libreria pwm.h"

////////////////////////////////////////////////////////////////////////////////

//Funcion que inicializa las configuraciones principales
//Debe ir al comienzo del programa
//Ejemplo PWM_init_1(5000)  --> 5Khz
void PWM_init_1(unsigned int frecuencia)     //Frecuencia minima 246 HZ - Frecuencia maxima 62500 HZ todo esto con 4 MHz
{
    float per=0;
    
    //Transforma a periodo
    per = 1.0 / frecuencia;
    freq = frecuencia;
    
    // Tosc= periodo de frecuencia del pic = 1 / _XTAL_FREQ = 1 / 4000000
    //Valor del PR2
    PR2 = (int) (per / (4 * (1.0 / _XTAL_FREQ) * 16)) - 1;
    CCPR1L = 0;                         // Limpia el registro ccpr1l     LBS o LSB--> bit menos significativo
    CCP1X = 0;
    CCP1Y = 0;                          // Limpia el lbs del pwm 1
    
    
    //CCPR1H = CCPR1L Y DC1B(1-0) ----- 10 BITS = 8BIT + 2BITS 
    //ccp1h
    //Configuracion salida
    TRISCbits.TRISC2 = 0;               //Declara salida pwm
    
    //Configuracion timer2
    T2CKPS0 = 1;                        //Preescaler 1:16
    T2CKPS1 = 1;                        // -
    TMR2ON = 1;                         //Habilita Timer2
    
    //Configuracion PWM
    CCP1M3 = 1;                         //Modo PWM
    CCP1M2 = 1;                         // -
//    CCP1M1 = 0;                        
//    CCP1M0 = 0; 
    return;
}

////////////////////////////////////////////////////////////////////////////////

//Funcion que recive un porcentaje de la señal
//Ejemplo --> PWM_1(25) --> El duty es el 25% del periodo --> Si la frecuencia es 5Khz. Periodo es: 200us 
//Por tanto el duty = 50 us
void PWM_1(char duty) 
{
    float per=0,pulso_alto=0,aux=0;
    unsigned int pwm=0;
    
    //Calcula el preiodo de la frecuencia o señal
    per = 1.0 / freq; 
    //Hace que el duty no pase del periodo maximo real
    if(duty == 100) duty = 95;
    //Calcula el porcentaje del priodo del pulso en alto
    pulso_alto = (duty * per) / 100.0;    // per_total___100%
                                        // pulso_alto__duty
                                        
    //Carga el valor necesario en CCPR1H    
    aux = ((pulso_alto) / ((1.0/_XTAL_FREQ) * 16)); // Con 50 us --> aux = 12.5
    pwm = (int)aux;
    //CCP1CON &= 0b11001111; CCP1CON &= 0xCF;
    CCP1CON &= 0x0F;                    // Limpia los lbs del pwm 
    //  CCP1CON = 0b00001100;
    // &          0b00001111;
    //-------------------------
    //  CCP1CON = 0b00001100;
    
    
    //CCP1CON |= ((0011 1110 & 0000 0011) --> 0000 0010
    CCP1CON |= ((pwm & 0x03) << 4);     // Carga el valor a los lbs del pwm (1)
    //  pwm = 0b0000 1111 = 15 
    // &
    //        0b0000 0011
    //-------------------------
    //        0b0000 0011
    // << 4
    //        0b0011 0000
    //      CCP1CON = 0b0000 1100
    // OR
    //                0b0011 0000
    //_----------------------------
    //                0b0011 1100
    
    
    CCPR1L &= 0x00;                     // Limpiar registro ccp1l 
    // CCPR1L = 0b0000 0000
    
    CCPR1L |= (pwm & 0x3FC) >> 2;       // Carga el valor en ccp1l (2)
    // pwm = 323= 0b 01 0100 0011
    // &  0x3FC = 0b 11 1111 1100
    //----------------------------
    //            0b 01 0100 0000
    // >> 2
    //----------------------------
    // CCPR1L =   0b 00 0000 0000
    // OR
    //            0b 00 0101 0000
    //----------------------------
    //            0b 00 0101 0000
    
    
    // CCPR1H = CCPR1L : BIT0 Y BIT1
    // CCPR1H = 0b 01 0100 0011 --> con el ejemplo del pwm = 323
    
    return;
}

////////////////////////////////////////////////////////////////////////////////

//Funcion que detiene el PWM 1
void PWM_Stop_1(void)     //Funcion de apagado del PWM
{
    TMR2ON = 0;                         // Detiene la cuenta del tmr2
    CCPR1L = 0;                         // Limpia el registro ccpr1l
    CCP1X = 0;
    CCP1Y = 0;                          // Limpia el lbs del pwm 1
    //limpia todo el registro CCPR1H
    return;
}

////////////////////////////////////////////////////////////////////////////////

//Funcion que inicializa el PWM 2 
//Debe llamarse al comienzo del programa
void PWM_init_2(unsigned int frecuencia)     //Configura el PWM
{
    float per=0;
    
    //Transforma a periodo
    per = 1.0 / frecuencia;
    
    //Valor del PR2
    PR2 = (per / (4 * 1.0 / _XTAL_FREQ * 16)) - 1;                               
    CCPR2L = 0;
    CCP2X = 0;
    CCP2Y = 0; // Limpia el lbs del pwm 1
    
    //Configuracion salida
    TRISCbits.TRISC1 = 0;               //Declara salida pwm

    //Configuracion PWM
    CCP2M3 = 1;                         //Modo PWM
    CCP2M2 = 1;                         // -
    
    return;
}

////////////////////////////////////////////////////////////////////////////////

//Funcion que reliza otro pwm y se le envia el porcentaje del duty requerido
void PWM_2(char duty)
{
    float per=0,pulso_alto=0,aux=0;
    unsigned int pwm=0;
    
    //Calcula el preiodo de la frecuencia o señal
    per = 1.0 / freq; 
    //Hace que el duty no pase del periodo maximo real
    if(duty == 100) duty = 95;
    //Calcula el porcentaje del priodo del pulso en alto
    pulso_alto = (duty * per) / 100.0;    // per_total___100%
                                        // pulso_alto__duty
                                        
    //Carga el valor necesario en CCPR1H    
    aux = ((pulso_alto) / ((1.0/_XTAL_FREQ) * 16));
    pwm = (int)aux;
    CCP2CON &= 0x0F;
    CCP2CON |= ((pwm & 0x03) << 4);
    CCPR2L &= 0x00;
    CCPR2L |= (pwm & 0x3FC) >> 2;
    
    return;
}

////////////////////////////////////////////////////////////////////////////////

//Funcion que detiene el Pwm 2
void PWM_Stop_2(void)     //Funcion de apagado del PWM
{
    TMR2ON = 0;  
    CCPR2L = 0;
    CCP2X = 0;
    CCP2Y = 0;
    
    return;
}

