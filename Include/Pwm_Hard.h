#include <xc.h>

#ifndef _XTAL_FREQ
#error Debe definir la frecuencia del oscilador
#endif

#ifndef PWM_HARD_H
#define PWM_HARD_H

unsigned int freq=0;
/*------------------------------------------------------------------------------
                            FUNCTION PROTOTYPES
 -----------------------------------------------------------------------------*/

void PWM_init_1(unsigned int frecuencia);
void PWM_1(char duty);
void PWM_Stop_1(void);

void PWM_init_2(unsigned int frecuencia);
void PWM_2(char duty);
void PWM_Stop_2(void);

#endif
