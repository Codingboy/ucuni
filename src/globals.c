#include "globals.h"

#include "led.h"
#include "button.h"
#include <stdlib.h>
#include "typedefs.h"
#include "servo.h"

Led* led1 = NULL;
Led* led2 = NULL;
Button* but1 = NULL;
Servo* servo = NULL;
u8 Vcc = 5;
u8 Vref = 5;
