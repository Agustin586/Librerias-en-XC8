#include <xc.h>
#define _XTAL_FREQ 4000000

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

