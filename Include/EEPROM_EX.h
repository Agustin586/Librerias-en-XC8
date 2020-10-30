#ifndef XC_EEPROM_EX_H
#define XC_EEPROM_EX_H
#endif

#ifndef _XTAL_FREQ
    #define _XTAL_FREQ 48000000
#endif


#ifndef XC_SOFT_I2C_H
    #include "SOFT_I2C.h"
#endif

#include <xc.h>

unsigned char cant_user=0;

void EEPROM_init(void);                                                         // Inicializa la cantidad de usuarios
void M_Write(unsigned int dir,long long mw_dato);                               // Escribe el hash
long long M_Read(unsigned int dir,unsigned char c_m_r);                         // Lee solo una parte de la memoria
_Bool User_Scan(unsigned char num_user);                                        // Detecta si el usuario existe
unsigned int New_User_Pos(void);                                                // Detecta si hay un espacio libre      
unsigned char Num_User(long long hash);                                         // Busca el numero del usuario con el hash
unsigned long Read_Hash(unsigned char user);                                    // Busca el hash del usuario
_Bool Read_Hash_ex(long long cod_rfid);                                         // Busca si existe ese hash 
char Pos_Num_User(unsigned char num_user);                                      // Busca la posicion de un numero de usuario
void Sum_Cant_Usuarios(void);                                                   // Suma la cantidad total de usuarios
void Write_User(unsigned int dir,unsigned char usuario);                        // Escribe un usuario en la posicion especificado
unsigned char Read_User(unsigned int dir);                                      // Lee la cantidad de usuarios,los usuarios.
