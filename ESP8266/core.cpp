#include "core.h"
#include "mqtt.h"

#ifdef ESP8266
	#define GPIO_IT D2
#else
	#warning
#endif

#define CLOCK_DIV TIM_DIV16

static const uint32_t mClock = F_CPU / 2 / ((CLOCK_DIV==TIM_DIV1) ? 1 : (CLOCK_DIV==TIM_DIV16) ? 16 : 256);

typedef struct _bell {
	uint8_t quiet:1, ///< is the bell quiet ?
		active:1,    ///< need to set the bell active
		status:1,    ///< current status of the bell
		alarm:1;     ///< while alarm request set, bell always honk
} bell_t;

bell_t bell = {
	.quiet = false,
	.active = false,
	.status = false,
	.alarm = false
};

static void led ( const bool on )
{
	digitalWrite ( BUILTIN_LED, on ? LOW : HIGH );
}

static void noize ( const bool on )
{
	digitalWrite ( D3, on ? LOW : HIGH );
}


static void ICACHE_RAM_ATTR bellIt ( void )
{
	bell.active = true;
}

static void ICACHE_RAM_ATTR onTimerISR ( void )
{
    bell.active = false;
}

double periode = 1.0;

void coreSetup ( void )
{
	// BUILDIN_LED = D4 = GPIO2
	pinMode ( BUILTIN_LED, OUTPUT );
	digitalWrite ( BUILTIN_LED, HIGH );

	pinMode ( GPIO_IT, INPUT_PULLUP ); // le GPIO est à 1 par defaut, quand la boule passe ça active l'optocoupleur et fait passer le GPUO à zero
	attachInterrupt( digitalPinToInterrupt ( GPIO_IT ), bellIt, FALLING );

	// D3 = GPIO0
	pinMode ( D3, OUTPUT );
	digitalWrite ( D3, HIGH );

	// timer used for bell duration
	timer1_isr_init ( );
	timer1_attachInterrupt ( onTimerISR );
}

void coreLoop ( void )
{
	if ( bell.alarm )
	{
		noize ( true );
		return;
	}

	if ( bell.active
		&& !bell.status )
	{ // bell of need to set on
		mqttClient.publish ( "/domotic/bell", 0, 1, "on" );

		led ( true );
		noize ( !bell.quiet );

		bell.status = true;

		timer1_write ( periode * mClock ); // periode / nbCycleByHz => 1Hz for 1 sec
		timer1_enable ( CLOCK_DIV, TIM_EDGE, TIM_SINGLE );
	}

	if ( !bell.active
		&& bell.status )
	{
		mqttClient.publish ( "/domotic/bell", 0, 1, "off" );

		led ( false );
		noize ( false );

		bell.status = false;

		timer1_disable ( );
	}
}

void coreMqttCallback ( const uint8_t topic, const char * const __restrict__ payload, const size_t len )
{
	switch ( topic )
	{
		case MQTT_DOM_BELL_LEN:
		{
			double duration = atof ( payload );
			if ( duration <= 0.0
				|| duration > 25.0 )
			{
				return;
			}
			periode = duration;
			break;
		}
		case MQTT_DOM_BELL_HELP:
		{
			mqttClient.publish ( logsTopic, 0, 0, "cmds : /domotic/bell/status [quiet|noizy|force]" );
			mqttClient.publish ( logsTopic, 0, 0, "cmds : /domotic/bell/duration 1.0" );
			break;
		}
		case MQTT_DOM_BELL_STATUS:
		{
			if ( strncmp ( payload, "quiet", len ) == 0 )
			{
				bell.quiet = true;
			}
			else if ( strncmp ( payload, "noizy", len ) == 0 )
			{
				bell.quiet = false;
			}
			else if ( strncmp ( payload, "force", len ) == 0 )
			{
				bell.active = true;
			}
			break;
		}
		case MQTT_DOM_BREAK:
		{
			bell.alarm = !strncmp ( payload, "on", len );
			break;
		}
		case MQTT_DOM_FIRE:
		{
			bell.alarm = !strncmp ( payload, "on", len );
			break;
		}
		default:
		{
			break;
		}
	}
}

void coreMqttConnectCallback ( void )
{
	mqttClient.publish ( "/domotic/bell", 0, 1, bell.status ? "on" : "off" );
}
