#include "RTC_3231.h"
#include "SOFT_I2C.h"

void RTC_send_hora(void)
{
    char uni=0,dec=0;
    
    //Comunicacion i2c
    i2c_start();
    
    
    i2c_write(0xD0);            // Direccion de esclavo
    i2c_write(0x00);            // Escribe en los segundos
    
    //Envia el valor de los segundos
    dec = (int)(seg / 10);      // Convierte a codigo bcd la decena
    uni = seg - (dec*10);       // Convierte a codigo bcd la unidad
    i2c_write((dec<<4)|uni);    // Envia los segundos
    
    //Limpia el valor
    dec = 0;
    uni = 0;
    //Envia el valor de los minutos
    dec = (int)(min / 10);      // Convierte a codigo bdc la decena
    uni = min - (dec*10);       // Convierte a codigo bcd la unidad
    i2c_write((dec<<4)|uni);    // Envia los mitutos
    
    //Limpia el valor
    dec = 0;
    uni = 0;
    //Envia el valor de la hora
    dec = (int)(hora / 10);
    uni = hora - (dec*10);
    i2c_write((dec<<4)|uni);    // Envia las horas
    
    
    i2c_stop();                 // Finaliza la comunicacion

    return;
}
void RTC_send_fecha(void)
{
    char uni=0,dec=0;
    
    //Comunicacion i2c
    i2c_start();
    
    
    i2c_write(0xD0);            // Direccion de esclavo
    i2c_write(0x04);            // Escribe en los dias
    
    //Envia el valor del dia
    dec = (int)(dia / 10);      // Convierte a codigo bcd la decena
    uni = dia - (dec*10);       // Convierte a codigo bcd la unidad
    i2c_write((dec<<4)|uni);    // Envia los dias
    
    //Limpia el valor
    dec = 0;
    uni = 0;
    //Envia el valor del mes
    dec = (int)(mes / 10);
    uni = mes - (dec*10);
    i2c_write((dec<<4)|uni);    // Envia los segundos
    
    //Limpia el valor
    dec = 0;
    uni = 0;
    //Envia el valor de los añios
    dec = (int)(year / 10);
    uni = year - (dec*10);
    i2c_write((dec<<4)|uni);    // Envia los años
    
    
    i2c_stop();                 // Finaliza la comunicacion

    return;
}

void RTC_read_time(void)
{
    char aux1,aux2;
    

    //Comunicacion i2c
    i2c_start();
    i2c_write(0xD0);
    i2c_write(0x00);
    
    //Recive segundos
    i2c_restart();
    i2c_write(0xD1);
    seg = i2c_read();
    
    //Finaliza
    i2c_nack();
    i2c_stop();
    
    
    
    //Vuelve a iniciar
    i2c_start();
    i2c_write(0xD0);
    i2c_write(0x01);
    
    //Recive minutos
    i2c_restart();
    i2c_write(0xD1);
    min = i2c_read();
    
    //Finaliza
    i2c_nack();
    i2c_stop();
    
    
    
    //Vuelve a iniciar 
    i2c_start();
    i2c_write(0xD0);
    i2c_write(0x02);
    
    //Recive hora
    i2c_restart();
    i2c_write(0xD1);
    hora = i2c_read();
    
    //Finaliza
    i2c_nack();
    i2c_stop();
    
    
    
    //Formato Decimal
    //Segundos
    aux1 = (seg >> 4) * 10;    // Ej: 0x59 --> 0x05 * 10 = 50
    aux2 = seg & 0x0F;         // Ej: 0x59 --> 0x09 = 9
    seg = aux1 + aux2;         // Ej: 50 + 9 = 59
    
    //Minutos
    aux1 = (min >> 4) * 10;    // Ej: 0x59 --> 0x05 * 10 = 50
    aux2 = min & 0x0F;         // Ej: 0x59 --> 0x09 = 9
    min = aux1 + aux2;         // Ej: 50 + 9 = 59
    
    //Horas
    aux1 = (hora >> 4) * 10;    
    aux2 = hora & 0x0F;         
    hora = aux1 + aux2;         
    
    return;
}
void RTC_read_date(void)
{
    char aux1,aux2;
    
    //Comunicacion i2c
    i2c_start();
    i2c_write(0xD0);
    i2c_write(0x04);
    
    i2c_restart();
    i2c_write(0xD1);
    dia = i2c_read();
    
    i2c_nack();
    i2c_stop();
    
    
    
    i2c_start();
    i2c_write(0xD0);
    i2c_write(0x05);
    
    i2c_restart();
    i2c_write(0xD1);
    mes = i2c_read();
    
    i2c_nack();
    i2c_stop();
    
    
    
    i2c_start();
    i2c_write(0xD0);
    i2c_write(0x06);
    
    i2c_restart();
    i2c_write(0xD1);
    year = i2c_read();
    
    i2c_nack();
    i2c_stop();
    
    
    
    //Formato Decimal
    //Dia
    aux1 = (dia >> 4) * 10;    // Ej: 0x31 --> 0x03 * 10 = 30
    aux2 = dia & 0x0F;         // Ej: 0x31 --> 0x01 = 1
    dia = aux1 + aux2;         // Ej: 30 + 1 = 31
    
    //Mes
    aux1 = (mes >> 4) * 10;    
    aux2 = mes & 0x0F;         
    mes = aux1 + aux2;         
    
    //Year
    aux1 = (year >> 4) * 10;    
    aux2 = year & 0x0F;         
    year = aux1 + aux2;         
    
    return;
}
