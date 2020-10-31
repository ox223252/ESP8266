#ifndef __MQTT_H__
#define __MQTT_H__

#include <stdint.h>

#include <Ticker.h>
#include <AsyncMqttClient.h> 
	// https://github.com/marvinroger/async-mqtt-client
	// https://github.com/me-no-dev/ESPAsyncTCP

extern bool mqttConnected; ///< mqtt status
extern AsyncMqttClient mqttClient;

/// \brief used to init mqtt
void mqttInit ( const char * const __restrict__ mqtt_server, const uint16_t port );

/// \brief used to connect mqtt
void mqttConnect ( void );

#endif