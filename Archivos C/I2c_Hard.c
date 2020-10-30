#include "i2c_com.h"

void i2c_init(void){
    /*Libera bus*/
    SDATRIS = 1;
    SCLTRIS = 1;
    
    /*Configuraciones de Registros*/
    SSPSTAT = 0b10000000;   //bit 7 : "1" standar-speed, "0" high-speed; bit 6: "0" sin SMBus 
    SSPCON = 0b00101000;    //bit 5: "1" habilita com.i2c, "0" desabilita; bit 3-0: Modo de trabajo "1000" Fosc/(4*(SSPADD +1))
    SSPCON2 = 0x00;
    SSPADD = 9; //Para 4MHz
}

void i2c_start(void){   
    SSPCON2bits.SEN = 1;    //Habilita la comunicacion i2c
    while(SSPCON2bits.SEN == 1);    //Espera a que termine
}

void i2c_restart(void){
    SSPCON2bits.RSEN = 1;   //Realiza un reseteo 
    while(SSPCON2bits.RSEN == 1);   //Espera a que termine
}

void i2c_stop(void){
    SSPCON2bits.PEN = 1;    //Realiza un stop
    while(SSPCON2bits.PEN == 1);    //Espera a que termine
}

char i2c_read(void){
    PIR1bits.SSPIF = 0;         	//clear SSP interrupt bit
    SSPCON2bits.RCEN = 1;       	//set the receive enable bit to initiate a read of 8 bits from the serial eeprom
    while(PIR1bits.SSPIF == 0); 	//Espera que la transmisionse complete  
    return SSPBUF;                  //Retorna el dato leido
}

void i2c_ack(void){
    PIR1bits.SSPIF = 0;            //clear SSP interrupt bit
    SSPCON2bits.ACKDT = 0;         //clear the Acknowledge Data Bit - this means we are sending an Acknowledge or 'ACK'
    SSPCON2bits.ACKEN = 1;         //set the ACK enable bit to initiate transmission of the ACK bit to the serial eeprom
    while(PIR1bits.SSPIF == 0);    //Wait for interrupt flag to go high indicating transmission is complete
}

void i2c_nack(void){
    PIR1bits.SSPIF = 0;            //clear SSP interrupt bit
    SSPCON2bits.ACKDT = 1;         //set the Acknowledge Data Bit- this means we are sending a No-Ack or 'NAK'
    SSPCON2bits.ACKEN = 1;         //set the ACK enable bit to initiate transmission of the ACK bit to the serial eeprom
    while(PIR1bits.SSPIF == 0);    // Wait for interrupt flag to go high indicating transmission is complete
}

char  i2c_write(char I2C_data){    
    PIR1bits.SSPIF = 0;             
    SSPBUF = I2C_data;              //Envia data por I2C
    while(PIR1bits.SSPIF == 0);     //Wait for interrupt flag to go high indicating transmission is complete
    if(ACKSTAT)
        return 1;
    else
        return 2;
}
        