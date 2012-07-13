/*!
\file pinoperations.h
\brief Hardware near operations on ports and pins
*/

#ifndef PINOPERATIONS_H
#define PINOPERATIONS_H

#include "typedefs.h"
#include <stdbool.h>

/*!
\brief Sets a pin as input.

\param[in] port port
\param[in] pin pin

\pre port >= 1
\pre port < 6
\pre pin >= 0
\pre pin < 8

\modifies DDR<port>
\modifies PORT<port>
*/
void setAsInput(u8 port, u8 pin);

/*!
\brief Sets a pin as output.

\param[in] port port
\param[in] pin pin

\pre port >= 1
\pre port < 6
\pre pin >= 0
\pre pin < 8

\modifies DDR<port>
\modifies PORT<port>
*/
void setAsOutput(u8 port, u8 pin);

/*!
\brief Sets a pin.

\param[in] port port
\param[in] pin pin

\pre port >= 1
\pre port < 6
\pre pin >= 0
\pre pin < 8
\pre port/pin shall be in outputmode otherwise the pullupresistor is modified

\modifies DDR<port>
\modifies PORT<port>
*/
void setOutput(u8 port, u8 pin);

/*!
\brief Clears a pin.

\param[in] port port
\param[in] pin pin

\pre port >= 1
\pre port < 6
\pre pin >= 0
\pre pin < 8
\pre port/pin shall be in outputmode otherwise the pullupresistor is modified

\modifies DDR<port>
\modifies PORT<port>
*/
void clearOutput(u8 port, u8 pin);

/*!
\brief Toggles a pin.

\param[in] port port
\param[in] pin pin

\pre port >= 1
\pre port < 6
\pre pin >= 0
\pre pin < 8
\pre port/pin shall be in outputmode otherwise the pullupresistor is modified

\modifies DDR<port>
\modifies PORT<port>
*/
void toggleOutput(u8 port, u8 pin);

/*!
\brief Gets the inputstate of a pin.

\param[in] port port
\param[in] pin pin

\pre port >= 1
\pre port < 6
\pre pin >= 0
\pre pin < 8
\pre port/pin shall be in inputmode

\return true if high
\return false if low
*/
bool getInput(u8 port, u8 pin);

/*!
\brief Gets the outputstate of a pin.

\param[in] port port
\param[in] pin pin

\pre port >= 1
\pre port < 6
\pre pin >= 0
\pre pin < 8
\pre port/pin shall be in outputmode

\return true if high
\return false if low
*/
bool getOutput(u8 port, u8 pin);

#endif
