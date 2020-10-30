#include "EEPROM_EX.h"
#include "LCD20X4_I2C.h"
#include "OS_CONFIG.h"
#include <stdio.h>

////////////////////////////////////////////////////////////////////////////////

void EEPROM_init(void)
{
    unsigned char MD_1=0,MD_2=0;
    _Bool c_ok=0;
    
    //Pantalla de espera
    LCD_command(CLEAR);
    LCD_array(1,1,"  Espere mientras   ");
    LCD_array(2,1," borra la memoria...");
    __delay_ms(1000);
    
    GIEH = 0;
    GIEL = 0;
    
    ///////////////////--Para la cantidad de usuarios--/////////////////////////
    
    LCD_command(CLEAR);
    
    while(c_ok == 0)
    {
        i2c_start();              
        i2c_write(0b10100000);      //Dirección de EEPROM, modo escritura
        i2c_write(0);               //parte alta de direcccion
        i2c_write(0);               //Parte baja de direccdion

        i2c_write(48);
        i2c_write(48);

        i2c_stop();
        __delay_ms(20);


        //Verificacion de la primera y segunda posicion
        LCD_array(1,1,"Verificacion:");
        if(Read_User(0) == 0)   LCD_array(2,1,"...OK"),c_ok=1;
        else                    LCD_array(2,1,"ERROR"),c_ok=0;
        __delay_ms(100);
    }
    
    c_ok=0;

    ////////////////////////--Para borrar el resto--////////////////////////////
    while(c_ok == 0)
    {
        i2c_start();              
        i2c_write(0b10100000);    //Dirección de EEPROM, modo escritura
        i2c_write(0);       //parte alta de direcccion
        i2c_write(2);       //Parte baja de direccdion

        for(unsigned int j=0;j<10000;j++)
        {
            i2c_write(255);
        }

        i2c_stop();
        __delay_ms(20);


        //Verificacion de la ultima posicion
        if(M_Read(10000,1) == 207)  c_ok=1,LCD_array(3,1,"...OK");
        else                        c_ok=0,LCD_array(3,1,"ERROR");
        __delay_ms(100);
    }
    
    __delay_ms(1000);
    LCD_command(CLEAR);
    LCD_array(2,1,"     Terminado    ");
    __delay_ms(1500);
    LCD_command(CLEAR);
    
    GIEH = 1;
    GIEL = 1;
    
    return;
}

////////////////////////////////////////////////////////////////////////////////

void M_Write(unsigned int dir,long long mw_dato)
{
    char buffer[11];
    unsigned char MD_1=0,MD_2=0;
    long long dec1=0,dec2=0,dec3=0,dec4=0,dec5=0;
    
    //Decodificacion de datos:
    dec1 = mw_dato / 100000000;
    dec2 = (mw_dato / 1000000) - (dec1 * 100);
    dec3 = (mw_dato / 10000) - (dec1 * 10000 + dec2 * 100);
    dec4 = (mw_dato / 100) - (dec1 * 1000000 + dec2 * 10000 + dec3 * 100);
    dec5 = mw_dato - (dec1 * 100000000 + dec2 * 1000000 + dec3 * 10000 + dec4 * 100);
    
    
    //Envio de informacion:
    
    //Transforma la direccion a 2 bytes
    MD_1 = (dir & 0xff00) >> 8; // Parte alta
    MD_2 = dir & 0x00ff;        // Parte baja
    
    i2c_start();                // Comienza la comunicacion
    
    i2c_write(0b10100000);      // Modo escritura
    
    i2c_write(MD_1);            // Parte alta de la direccion de la memoria interna
    i2c_write(MD_2);            // Parte baja
    
    
    //Envio de decenas:
    
    //1ra decena
    sprintf(buffer,"%d",dec1);
    i2c_array(buffer);
    //2da decena
    sprintf(buffer,"%d",dec2);
    i2c_array(buffer);
    //3ra decena
    sprintf(buffer,"%d",dec3);
    i2c_array(buffer);
    //4ta decena
    sprintf(buffer,"%d",dec4);
    i2c_array(buffer);
    //5ta decena
    sprintf(buffer,"%d",dec5);
    i2c_array(buffer);
    
    //Fin de la comunicacion
    i2c_stop();
    
    return;
}

////////////////////////////////////////////////////////////////////////////////

long long M_Read(unsigned int dir,unsigned char c_m_r)
{    
    unsigned char MD_1=0,MD_2=0;
    long long MR_Dato=0;
    char buffer[10];
    
    //GIEH = 0;
    GIEL = 0;
    
    Read_Eeprom = 1;
    
    //Envio de informacion para leer:
    
    //Transforma la direccion a 2 bytes
    MD_1 = (dir & 0xff00) >> 8; // Parte alta
    MD_2 = dir & 0x00ff;        // Parte baja
    
    
    i2c_start();                // Comienza la comunicacion
    i2c_write(0b10100000);      // Modo escritura
    
    i2c_write(MD_1);            // Parte alta de la direccion de la memoria interna
    i2c_write(MD_2);            // Parte baja
    
    i2c_restart();
    i2c_write(0b10100001);      // Cambia a modo lectura de datos
    
    
    //Lectura de datos: 
    
    //Guarda los datos obtenidos
    for(char i=0;i<c_m_r;i++)
    {
        if(i == (c_m_r - 1))     MR_Dato = MR_Dato + (i2c_read() - 48);
        else                     if(i != c_m_r)  MR_Dato = (MR_Dato + (i2c_read() - 48)) * 10,i2c_ack();
        
        __delay_us(10); // Deja un periodo entero en bajo
    }
    
    //Envia un No Ack para dejar de recivir 
    i2c_nack();
    
    //Fin de la comunicacion
    i2c_stop();                 

    Read_Eeprom = 0;
    
    //GIEH = 1;
    GIEL = 1;
    
    return MR_Dato;
}

////////////////////////////////////////////////////////////////////////////////

_Bool User_Scan(unsigned char num_user)
{
    _Bool user_exist=0;
    unsigned char pos=0;
    
    //Primero debe leer la cantidad de usuario
    cant_user = M_Read(0,2);
    
    //Segundo debe verificar si el numero de usuario ya existe
    for(char i=0;i<cant_user;i++)
    {
        if(i != 0)  pos = pos + 15;
        else        pos = 4;
        if(num_user == (M_Read(pos,2))) user_exist=1;

        if(user_exist)  break;
    }
    
    return user_exist;      //Devuelve un 1 si existe, sino un 0
}

////////////////////////////////////////////////////////////////////////////////

unsigned int New_User_Pos(void)
{
    unsigned char cant_user=0;
    unsigned int Posicion=0;
    
    cant_user = Read_User(0);
    
    if(cant_user != 0)  Posicion = (cant_user * 15) + 4;
    else                Posicion = 4;
    
    return Posicion;
}

////////////////////////////////////////////////////////////////////////////////

unsigned char Num_User(long long hash)
{
    unsigned char pos_h=0,num_user=0;
    
    if(Read_Hash_ex(hash) == 1)
    {
        for(char i=0;i<cant_user;i++)
        {
            if(i == 0)  pos_h=7;
            else        pos_h = pos_h + 15;
        
            if((hash) == (M_Read(pos_h,10)))    
            {
                num_user = (Read_User(pos_h-3));
                break;
            }
        }
        return num_user;       
    }
    else    return 0;

}

////////////////////////////////////////////////////////////////////////////////

unsigned long Read_Hash(unsigned char user)
{
    unsigned long M_Hash=0;
    unsigned char hash=0,pos_hash=0;
    
    if(User_Scan(user) == 0)    return 0;
    
    if(User_Scan(user) == 1)
    {
        //Con esto podemos obtener la posicion del hash
        pos_hash = Pos_Num_User(user) + 4;
        
        if(pos_hash != 0)
        {
            //Guarda el valor del hash
            M_Hash = M_Read(pos_hash,10);
        }
        else    M_Hash = 0;
    }
    
    return M_Hash;
}

////////////////////////////////////////////////////////////////////////////////

char Pos_Num_User(unsigned char num_user)
{
    unsigned char cant_user=0,pos=0,pos_usuario=0;
    
    //Primero escanea si el usuario existe
    if(User_Scan(num_user) == 1)
    {
        //Lee la cantidad de usuarios
        cant_user = M_Read(0,2);
        
        //Usa esa cantidad para buscar la posicion en la se encuentra el numero de usuario
        for(char i=0;i<cant_user;i++)
        {
            if(i == 0)  pos = 3;
            else        pos = pos + 15;
            if(num_user == M_Read(pos,2))
            {
                pos_usuario = pos;
                break;
            }
        }
    }
    
    if(User_Scan(num_user) == 0)    return 0;
    
    return  pos_usuario;
}

////////////////////////////////////////////////////////////////////////////////

_Bool Read_Hash_ex(long long cod_rfid)
{
    _Bool OK_HASH=0;
    unsigned char pos_hash=0;
    
    cant_user = Read_User(0);
    
    //Con esto podemos obtener la posicion del hash
    for(char i=0;i<cant_user;i++)
    {
        if(i == 0)  pos_hash=7;
        else        pos_hash = pos_hash + 15;

        if(cod_rfid == M_Read(pos_hash,10))
        {
            OK_HASH = 1;
            break;
        }
        else                                OK_HASH = 0;
    }
    
    return OK_HASH;
}

////////////////////////////////////////////////////////////////////////////////

void Sum_Cant_Usuarios(void)
{
    unsigned char cant_usuario=0;
    
    cant_usuario = Read_User(0);
    
    cant_usuario = cant_usuario + 1;
    
    Write_User(0,cant_usuario);
    
    return;
}

////////////////////////////////////////////////////////////////////////////////

void Write_User(unsigned int dir,unsigned char usuario)
{
    char buffer[5];
    unsigned char MD_1=0,MD_2=0;
    
    //Envio de informacion:
    
    //Transforma la direccion a 2 bytes
    MD_1 = (dir & 0xff00) >> 8; // Parte alta
    MD_2 = dir & 0x00ff;        // Parte baja
    
    i2c_start();                // Comienza la comunicacion
    
    i2c_write(0b10100000);      // Modo escritura
    
    if(dir > 0)
    {
        i2c_write(MD_1);            // Parte alta de la direccion de la memoria interna
        i2c_write(MD_2);            // Parte baja
    }
    else
    {
        i2c_write(0);
        i2c_write(0);
    }
    
    sprintf(buffer,"%02d",usuario);
    i2c_array(buffer);
    
    i2c_stop();
    
    return;
}

unsigned char Read_User(unsigned int dir)
{
    unsigned char MD_1=0,MD_2=0;
    unsigned char MR_Dato=0;
    
    //GIEH = 0;
    GIEL = 0;
    
    Read_Eeprom = 1;
    
    //Envio de informacion para leer:
    
    //Transforma la direccion a 2 bytes
    MD_1 = (dir & 0xff00) >> 8; // Parte alta
    MD_2 = dir & 0x00ff;        // Parte baja
    
    
    i2c_start();                // Comienza la comunicacion
    i2c_write(0b10100000);      // Modo escritura
    
    i2c_write(MD_1);            // Parte alta de la direccion de la memoria interna
    i2c_write(MD_2);            // Parte baja
    
    i2c_restart();
    i2c_write(0b10100001);      // Cambia a modo lectura de datos
    
    
    //Lectura de datos: 
    MR_Dato = (i2c_read() - 48) * 10;
    i2c_ack();
    MR_Dato = MR_Dato + (i2c_read() - 48);
    i2c_nack();
    
    
    //Fin de la comunicacion
    i2c_stop();                 
    
    Read_Eeprom = 0;
    
    //GIEH = 1;
    GIEL = 1;
    
    return MR_Dato;
}