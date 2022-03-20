// https://www.esp8266.com/viewtopic.php?t=17549
// https://microcontrollerslab.com/esp8266-interrupts-timers-arduino-ide-nodemcu/

#include "core.h"
#include "mqtt.h"

static void ICACHE_RAM_ATTR gpioIt ( void )
{
}

static void ICACHE_RAM_ATTR onTimerISR ( void )
{
	digitalWrite ( BUILTIN_LED, !digitalRead ( BUILTIN_LED ) );
}

void coreSetup ( void )
{
	// BUILDIN_LED = D4 = GPIO2
	pinMode ( BUILTIN_LED, OUTPUT );
	digitalWrite ( BUILTIN_LED, HIGH );

	// D2 = GPIO4
	pinMode ( D2, INPUT_PULLUP );
	attachInterrupt( digitalPinToInterrupt ( D2 ), gpioIt, FALLING );

	// timer used for bell duration
	timer1_isr_init ( );
	timer1_attachInterrupt ( onTimerISR );
	// clock 80 MHz

	timer1_write ( 5000000 / 2 / 1 ); // tick/s / frq => 1Hz for 1 sec

	timer1_enable ( TIM_DIV16, TIM_EDGE, TIM_LOOP ); // TIM_LOOP / TIM_SINGLE
}

void coreLoop ( void )
{
}

void coreMqttCallback ( const uint8_t topic, const char * const __restrict__ payload, const size_t len )
{
	switch ( topic )
	{
		case MQTT_TOPIC:
		{
			uint8_t qos = 0;
			bool retain = false;
			mqttClient.publish ( logsTopic, qos, retain, payload );
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
}
