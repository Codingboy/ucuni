#include "globals.h"
#include "led.h"
#include "button.h"
#include <stdlib.h>
#include "typedefs.h"
#include "servo.h"
#include "ez3.h"

Led* led1 = NULL;
Led* led2 = NULL;
Button* but1 = NULL;
Button* but2 = NULL;
Button* but3 = NULL;
Servo* servo = NULL;
EZ3* ez3 = NULL;
u8 Vcc = 5;
u8 Vref = 5;
