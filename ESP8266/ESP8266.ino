#include "wifi.h"
#include "mqtt.h"
#include "serial.h"
#include "credential.h"

#include <EEPROM.h>


static uint8_t networkId = 0;
static char boardId[32] = { 0 };

#define EEPROM_BORAD_ID 0
#define EEPROM_NETWORK_ID ( EEPROM_BORAD_ID + sizeof ( boardId ) )

////////////////////////////////////////////////////////////////////////////////
/// Utils Part

#define min(a,b) ({__typeof__(a) _a=a;__typeof__(b) _b=b;(_a<_b)?_a:_b;})
#define max(a,b) ({__typeof__(a) _a=a;__typeof__(b) _b=b;(_a>_b)?_a:_b;})

static void eepromGet ( const size_t addr, void * const __restrict__ out, const size_t s )
{
	for ( uint8_t i = 0; i < s; i++ )
	{
		((uint8_t*)out)[ i ] = EEPROM.read ( addr + i );
	}
}

static void eepromSet ( const size_t addr, const void * const __restrict__ in, const size_t s )
{
	for ( uint8_t i = 0; i < s; i++ )
	{
		EEPROM.write ( addr + i, ((uint8_t*)in)[ i ] );
	}
	EEPROM.commit ( );
}

/// \brief read EEPROM to get boardId or set it to unknow
static void boardIdGet ( void )
{
	if ( EEPROM.read ( EEPROM_BORAD_ID ) == 0xff )
	{
		strcpy ( boardId, "unknow" );
		return;
	}
	eepromGet ( EEPROM_BORAD_ID, boardId, sizeof ( boardId ) );
	boardId[ sizeof(boardId) ] = 0;
}

/// \brief write board ID into the EEPROM
static void boardIdSet ( const char * const __restrict__ str )
{
	eepromSet ( EEPROM_BORAD_ID, (uint8_t*)boardId, min ( strlen ( boardId ) + 1, sizeof ( boardId ) ) );
}

////////////////////////////////////////////////////////////////////////////////
/// Main part
void setup ( void )
{
	
	pinMode ( BUILTIN_LED, OUTPUT );

	digitalWrite ( BUILTIN_LED, HIGH );
	delay ( 1000 );

	Serial.begin ( 115200 );
	Serial.println ( );

	// eeprom part, here we are setting the board ID
	// to know who talk on system
	EEPROM.begin ( sizeof ( boardId ) + sizeof ( networkId ) );

	eepromGet ( EEPROM_NETWORK_ID, &networkId, sizeof ( networkId ) );
	networkId %= sizeof ( network ) / sizeof ( *network );

	Serial.print ( "You will use " );
	Serial.print ( network[ networkId ].ssid );
	Serial.println ( ", are you OK ? (Y/n)" );

	if ( !yesNo ( 10000, true ) )
	{
		do
		{
			Serial.println ( "select new network " );
			for ( uint8_t i = 0; i < sizeof ( network ) / sizeof ( *network ); i++ )
			{
				Serial.printf ( " - %d : %s\r\n", i, network[ i ].ssid );
			}

			char r[5];
			serialGetLine ( r, sizeof ( r ) );
			networkId = atoi ( r ) % ( sizeof ( network ) / sizeof ( *network ) );

			Serial.print ( "You set " );
			Serial.print ( network[ networkId ].ssid );
			Serial.println ( ", are you OK ? (Y/n)" );
		}
		while ( !yesNo ( 10000, true ) );

		Serial.println ( "Save network id for next reboot" );
		eepromSet ( EEPROM_NETWORK_ID, &networkId, sizeof ( networkId ) );
	}

	mqttInit ( network[ networkId ].mqtt_server, network[ networkId ].port );
	wifiInit ( );
	wifiStart ( network[ networkId ].ssid, network[ networkId ].password );

	boardIdGet ( );


	Serial.print ( "Curren ID: " );
	Serial.println ( boardId );

	Serial.println ( "Do you want to set/change board ID ? (y/N)" );

	if ( !yesNo ( 10000, false ) )
	{ // user don't say yes
		return;
	}

	do
	{
		Serial.print ( "set board id: " );

		serialGetLine ( boardId, sizeof ( boardId ) );
	
		Serial.print ( "\nNew ID: " );
		Serial.println ( boardId );

		Serial.println ( "Is it Ok ? (y/N)" );
	}
	while ( !yesNo ( 10000, false ) );
	
	Serial.println ( "Save Board Id for next reboot" );

	boardIdSet ( boardId );

	serialCleanInput ( );
}

void loop ( void )
{
	delay ( 1000 );
}
