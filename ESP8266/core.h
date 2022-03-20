#ifndef __CORE_H__
#define __CORE_H__

#include <stdint.h>
#include <stdlib.h>

/// \brief output log topic
static const char logsTopic[] = "/mqtt/logs";

/// \brief input topic id
enum {
	MQTT_TOPIC,
	MQTT_UNKNOW
};

/// \biref input topic names
static const char * mqtt_topics[] = {
	[MQTT_TOPIC] = "/mqtt/topic",
	[MQTT_UNKNOW] = NULL
};

void coreSetup ( void );
void coreLoop ( void );
void coreMqttCallback ( const uint8_t topic, const char * const __restrict__ payload, const size_t len );
void coreMqttConnectCallback ( void );

#endif
