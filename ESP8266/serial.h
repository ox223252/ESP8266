#ifndef __SERIAL_H__
#define __SERIAL_H__

#include <SoftwareSerial.h>

/// \brief clean serial input buffer
inline void serialCleanInput ( void )
{
	while ( Serial.available ( ) > 0 )
	{
		Serial.read ( );
	}
}

/// \biref wait answer to yes no question
// \return true = yes / false = no
bool yesNo ( const unsigned long timeout, const bool r );

void serialGetLine ( char * const __restrict__ out, const size_t size );

#endif