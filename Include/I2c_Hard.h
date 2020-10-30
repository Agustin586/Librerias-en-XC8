//*****************************************************************************
//*Pagina:http://microcontroladores-mrelberni.com/i2c-pic-comunicacion-serial/*
//*Youtube:https://www.youtube.com/watch?v=ENNSAeUCJGA&t=44s                  *
//*****************************************************************************

#ifndef _XTAL_FREQ
#error  Debe definir la frecuencia del oscilador
#endif

//Definiciones de pines para utilizar
#define SDATRIS TRISCbits.TRISC4
#define SCLTRIS TRISCbits.TRISC3

void i2c_init(void);
void i2c_start(void);
void i2c_restart(void);
void i2c_stop(void);
char i2c_read(void);
void i2c_ack(void);
void i2c_nack(void);
char i2c_write(char I2C_data);