#include "SPI_SOFT.h"

void Soft_SPI_Init(void)
{
    //configura pines a usar como spi;
    spi_mosi_Direction=0;
    spi_clock_Direction=0;
    spi_CS_Direction=0;
    spi_miso_Direction=1;

    spi_clock=0;
    spi_CS=0;
    spi_mosi=0;
    spi_CS = 0;

    return;
}

unsigned char  Soft_SPI_Read()
{
  unsigned char cont_s= 0;
  unsigned char dato_ = 0;

  for(cont_s= 0; cont_s < 8; cont_s++)
  {
      dato_ <<= 1;
      spi_clock = 1;
      __delay_us(10);   //retardo de lectura

      if(spi_miso != 0)
      {
        dato_ |= 1;
      }
      spi_clock = 0;
      __delay_us(10);
  }
  
  return dato_;
}

void Soft_SPI_Write(char dato_)
{
  unsigned char cont_s= 0;

  for(cont_s = 0;cont_s < 8; cont_s++)
  {
    if((dato_ & 0x80) != 0) spi_mosi = 1;  //sino termina rotacion coloca un uno sino un cero
    else    spi_mosi = 0;
    dato_ <<= 1;  //vota
    //genera un pulso del reloj
    spi_clock = 1;
    __delay_us(10);
    spi_clock = 0;
    __delay_us(10);
  }
  
  return;
}

void SPI_array(const char *cadena)
{
    while(*cadena)
    {
        Soft_SPI_Write(*cadena);
        cadena++;
    }
    
    return;
}