#ifndef __CORE_H__
#define __CORE_H__

#include <stdint.h>
#include <stdlib.h>

/// \brief output log topic
static const char logsTopic[] = "/domotic/logs";

/// \brief input topic id
enum {
	MQTT_DOM_BELL_LEN,
	MQTT_DOM_BELL_HELP,
	MQTT_DOM_BELL_STATUS,
	MQTT_DOM_BREAK,
	MQTT_DOM_FIRE,
	MQTT_UNKNOW
};

/// \biref input topic names
static const char * mqtt_topics[] = {
	[MQTT_DOM_BELL_LEN] = "/domotic/bell/duration",
	[MQTT_DOM_BELL_HELP] = "/domotic/bell/help",
	[MQTT_DOM_BELL_STATUS] = "/domotic/bell/status",
	[MQTT_DOM_BREAK] = "/domotic/break",
	[MQTT_DOM_FIRE] = "/domotic/fire",
	[MQTT_UNKNOW] = NULL
};

void coreSetup ( void );
void coreLoop ( void );
void coreMqttCallback ( const uint8_t topic, const char * const __restrict__ payload, const size_t len );
void coreMqttConnectCallback ( void );

#endif
