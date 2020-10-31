#include "wifi.h"
#include "mqtt.h"

bool mqttConnected = false; ///< mqtt status

AsyncMqttClient mqttClient;
Ticker mqttReconnectTimer;

enum {
	MQTT_UNKNOW
};

const char * mqtt_topics[] =  {
	[MQTT_UNKNOW] = NULL
};

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
	switch ( mqttTopicConvert ( topic ) )
	{
		default:
		{
			Serial.println("[MQTT] Publish received.");
			Serial.print("[MQTT]   topic: ");
			Serial.println(topic);
			Serial.print("[MQTT]   qos: ");
			Serial.println(properties.qos);
			Serial.print("[MQTT]   dup: ");
			Serial.println(properties.dup);
			Serial.print("[MQTT]   retain: ");
			Serial.println(properties.retain);
			Serial.print("[MQTT]   len: ");
			Serial.println(len);
			Serial.print("[MQTT]   index: ");
			Serial.println(index);
			Serial.print("[MQTT]   total: ");
			Serial.println(total);
			break;
		}
	}
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