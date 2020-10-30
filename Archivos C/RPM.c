#include "RPM.h"

/*------------------------------------------------------------------------------
                        VARIABLE DECLARATIONS GLOBAL
------------------------------------------------------------------------------*/
char flanco=1;    //Detecta cuando se produce el primer y segundo pulso por RB0
unsigned int T=0;

/*==============================================================================
  FUNCTION:     RPM_init()
  DESCRIPTION:  Inicializa los registros y configuraciones necesarios
  PARAMETERS:   Ninguno
  RETURNS:      Nada
  REQUIREMENTS: Ingreso de pulso por RB0
==============================================================================*/
void RPM_init(void)
{
    //Configuracion de Puertos
    TRISBbits.TRISB0 = 1;
    
    //Configuracion de timer 1 como contador de oscilador interno
    T1CONbits.T1CKPS1 = 1;              //Preescaler en 1:8
    T1CONbits.T1CKPS0 = 1;              //Preescaler en 1:8
    T1CONbits.TMR1CS = 0;               //Reloj interno/4
    T1CONbits.TMR1ON = 1;               //Hablita timer1
    
    //Configuracion de interrupcion por RB0
    INTCONbits.GIE = 1;
    INTCONbits.INTE = 0;                //Habilita interrupcion por RB0
    INTEDG = 1;                         //Por flanco ascendente
    INTF = 0;                           //Bandera en 0
    
    return;
}


/*==============================================================================
  FUNCTION:     RPM_read()
  DESCRIPTION:  Genera el valor de frecuencia y rpm 
  PARAMETERS:   Ninguno
  RETURNS:      Flotante --> variable rpm
  REQUIREMENTS: Ninguno
==============================================================================*/
float RPM_read(void)
{
    /*-----------------------------------------------------------------------
                            VARIABLE DECLARATIONS
    -----------------------------------------------------------------------*/
    float freq=0,rpm=0;

    flanco = 1;
    TMR1IF = 1;
    INTCONbits.INTE = 1;
    while(flanco <= 2);
    
    //Calcula la frecuencia y la transforma en rpm
    freq = 1 / (T * 0.000008);      //Calcula la frecuencia de los pulsos
    rpm = freq * 60.0;                    //Calcula los rpm
    
    //Devuelve el valor de rpm
    return rpm;
}


/*==============================================================================
  FUNCTION:     ventana()
  DESCRIPTION:  Genera una ventana que depende de la frecuencia que ingresa
  PARAMETERS:   Ninguno
  RETURNS:      Nada
  REQUIREMENTS: Ingreso de pulsos por RB0
==============================================================================*/
void __interrupt() ventana(void)
{
    if(INTCONbits.INTF == 1)
    {
        //Detecta el pulso y comienza a contar
        if(flanco == 1)
        {
            if(PIR1bits.TMR1IF == 1)
            {
                TMR1 = 0;                       //Inicia el timer1 en 0
                PIR1bits.TMR1IF = 0;            //Inicia el timer1
            }
        }

        //Cuando se genere el segundo flanco guarda el valor
        if(flanco == 2)
        {
            //Guarda el valor del segundo flanco ascendente
            T1CONbits.TMR1ON = 0;
            T = TMR1;                       //Guarda el valor obtenido
            T1CONbits.TMR1ON = 1;
            INTCONbits.INTE = 0;
        }

        INTCONbits.INTF = 0;
        flanco++;
    }
    
    return;
}


/*******************************************************************************
|  LIBRERIE:      RPM.c
|  AUTHOR:        -
|  DATE:          27 APRIL 2020
|  DESCRIPTION:   Lee las revoluciones por minutos de una determ. frecuencia,
|                 mediante las interrupciones por RB0 con flanco ascendente.
|  REQUIREMENTS:  Ingreso de pulsos por entra RB0
|
|  REVISION HISTORY:
|  27 APRIL 2020    V1.0 Creado
|
|  10 MAY 2020      V1.16 Muestra las rev por min de forma flotante  
|
|  10 MAY 2020      V1.7 Convierte la variable flotante en entera, por lo tanto
|  3:37             devuelve solo valores enteros
|
*******************************************************************************/



