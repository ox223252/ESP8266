#include "wifi.h"
#include "mqtt.h"

#include "core.h"

bool mqttConnected = false; ///< mqtt status

AsyncMqttClient mqttClient;
Ticker mqttReconnectTimer;

/// \brief convert topic string into an enum value
/// \param[ in ] topic: string we search to find in mqtt_topics
/// \return id of the topic
static uint8_t mqttTopicConvert ( const char * const __restrict__ topic )
{
	for ( uint8_t i = 0; i < MQTT_UNKNOW; i++ )
	{
		if ( !strcmp ( mqtt_topics[ i ], topic ) )
		{
			return i;
		}
	}

	return MQTT_UNKNOW;
}

/// \note redefine this function to defined MQTT comportement
/// \brief callback on receive msg
/// \param [ in ] topic: topic where data is transmited
/// \param [ in ] payload: data trasmited
/// \param [ in ] properties: qos / dup / retain
/// \param [ in ] len:data length
/// \param [ in ] index: id of paket
/// \param [ in ] total: ?
static void mqttOnMessage ( const char * const __restrict__ topic, const char * const __restrict__ payload, 
	const AsyncMqttClientMessageProperties properties, const size_t len, const size_t index, const size_t total )
{
	coreMqttCallback ( mqttTopicConvert ( topic ), payload, len );
}

/// \breif callback on disconnect event
/// \param [ in ] reason: ?
static void mqttOnDisconnect ( const AsyncMqttClientDisconnectReason reason )
{
	Serial.println ( "[MQTT] Disconnected from MQTT." );

	if ( WiFi.isConnected ( ) )
	{
		mqttReconnectTimer.once ( 2, mqttConnect );
	}
	mqttConnected = false;
}

/// \brief callback on connect event
/// \param[ in ] sessionPresent: ?
static void mqttOnConnect ( const bool sessionPresent )
{
	Serial.println ( "[MQTT] Connected to MQTT." );
	Serial.print ( "[MQTT] Session present: " );
	Serial.println ( sessionPresent );

	for ( uint8_t i = 0; mqtt_topics[ i ] ; i++ )
	{
		mqttClient.subscribe ( mqtt_topics[ i ], 1 );
	}
	mqttConnected = true;

	coreMqttConnectCallback ( );
}

void mqttConnect ( void )
{
	Serial.println ( "[MQTT] try to connect" );
	mqttClient.connect();
}

void mqttInit ( const char * const __restrict__ mqtt_server, const uint16_t port )
{
	mqttClient.setServer ( mqtt_server, port );
	mqttClient.onMessage ( mqttOnMessage );
	mqttClient.onConnect ( mqttOnConnect );
	mqttClient.onDisconnect ( mqttOnDisconnect );
}