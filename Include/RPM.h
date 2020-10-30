#include <xc.h>
#include <stdint.h>

#ifndef _XTAL_FREQ
#error Debe definir la frecuencia del oscilador
#endif

#ifndef RPM_H
#define RPM_H

void RPM_init(void);
float RPM_read(void);
void __interrupt() ventana(void);

_Bool on=0;
char pwm=0;

#endif