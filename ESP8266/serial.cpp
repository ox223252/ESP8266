#include "serial.h"

bool yesNo ( const unsigned long timeout, const bool r )
{
	serialCleanInput ( );
	
	unsigned long start = millis ( );
	while ( ( millis ( ) - start ) < timeout )
	{
		switch ( Serial.read ( ) )
		{
			case 'y':
			{
				serialCleanInput ( );
				return ( true );
			}
			case 'n':
			{
				serialCleanInput ( );
				return ( false );
			}
		}
		delay ( 100 );
	}

	serialCleanInput ( );
	return ( r );
}

void serialGetLine ( char * const __restrict__ out, const size_t size )
{
	for ( size_t i = 0; i < size ; i++ )
	{
		while ( Serial.available ( ) <= 0 )
		{
			delay ( 100 );
		}

		out[ i ] = Serial.read ( );

		switch ( out[ i ] )
		{
			case '\n':
			case '\r':
			{
				out[ i ] = 0;
				break;
			}
		}

		if ( !out[ i ] )
		{
			break;
		}
	}
	
	out[ size ] = 0;
}