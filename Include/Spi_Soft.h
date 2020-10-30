#ifndef XC_SOFT_SPI_H
#define XC_SOFT_SPI_H
#endif

#ifndef XC_CONFIGURACION_H
    #include "CONFIG_PIC.h"
#endif

#define MFRC522_CS              LATB0   // SS1
#define MFRC522_CS2             LATB1   // SS2
#define MFRC522_Rst             LATB3   // RST
#define SoftSPI_SDO             LATA4   //MOSI
#define SoftSPI_CLK             LATB2   //SCK
#define SoftSPI_SDI             RA5   //MISO

#define MFRC522_CS_Direction    TRISB0
#define MFRC522_CS_Direction2   TRISB1
#define MFRC522_Rst_Direction   TRISB3
#define SoftSPI_SDO_Direction   TRISA4
#define SoftSPI_CLK_Direction   TRISB2
#define SoftSPI_SDI_Direction   TRISA5

#define spi_miso            SoftSPI_SDI
#define spi_clock           SoftSPI_CLK
#define spi_mosi            SoftSPI_SDO
#define spi_CS              MFRC522_CS 

#define spi_miso_Direction  SoftSPI_SDI_Direction
#define spi_clock_Direction SoftSPI_CLK_Direction
#define spi_mosi_Direction  SoftSPI_SDO_Direction
#define spi_CS_Direction    SoftSPI_SDI_Direction

unsigned char Dato;

void Soft_SPI_Init(void);           // Inicializa el spi
void Soft_SPI_Write(char dato_);    // Envia un caracter por spi
void SPI_array(const char *cadena); // Envia una cadena por spi
unsigned char  Soft_SPI_Read();     // Lee un caracter por spi
