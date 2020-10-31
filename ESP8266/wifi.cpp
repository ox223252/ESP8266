#include "wifi.h"
#include "mqtt.h"

// WIFI client event: https://github.com/techiesms/ESP8266-Series/blob/master/WiFiClientEvents/WiFiClientEvents.ino

bool wifiConnected = false; ///< wifi status

/// \brief convert status id into a string
/// \param [ in ] status: id of the string to be retruned
/// \return the string of status
static String connectionStatus( const int status )
{
	switch ( status )
	{
		case WL_CONNECTED:
		{
			return "Connected";
		}
		case WL_NO_SSID_AVAIL:
		{
			return "Network not availible";
		}
		case WL_CONNECT_FAILED:
		{
			return "Wrong password";
		}
		case WL_IDLE_STATUS:
		{
			return "Idle status";
		}
		case WL_DISCONNECTED:
		{
			return "Disconnected";
		}
		default:
		{
			return "Unknown";
		}
	}
}

/// \brief wifi callback for all events
/// \param [ in ] event: event id
static void wifiOnEvent ( const WiFiEvent_t event  )
{
	switch ( event )
	{
		case WIFI_EVENT_STAMODE_CONNECTED:
		{
			Serial.printf("[WiFi] %d, Connected\r\n", event);
			break;
		}
		case WIFI_EVENT_STAMODE_DISCONNECTED:
		{
			Serial.printf("[WiFi] %d, Disconnected - Status %d, %s\r\n", event, WiFi.status(), connectionStatus( WiFi.status() ).c_str() );
			wifiConnected = false;
			break;
		}
		case WIFI_EVENT_STAMODE_AUTHMODE_CHANGE:
		{
			Serial.printf("[WiFi] %d, AuthMode Change\r\n", event);
			break;
		}
		case WIFI_EVENT_STAMODE_GOT_IP:
		{
			Serial.printf("[WiFi] %d, Got IP\r\n", event);
			Serial.println ( WiFi.localIP ( ) );
			mqttConnect ( );
			wifiConnected = true;
			break;
		}
		case WIFI_EVENT_STAMODE_DHCP_TIMEOUT:
		{
			Serial.printf("[WiFi] %d, DHCP Timeout\r\n", event);
			break;
		}
		case WIFI_EVENT_SOFTAPMODE_STACONNECTED:
		{
			Serial.printf("[AP] %d, Client Connected\r\n", event);
			break;
		}
		case WIFI_EVENT_SOFTAPMODE_STADISCONNECTED:
		{
			Serial.printf("[AP] %d, Client Disconnected\r\n", event);
			break;
		}
		case WIFI_EVENT_SOFTAPMODE_PROBEREQRECVED:
		{
			Serial.printf("[AP] %d, Probe Request Recieved\r\n", event);
			break;
		}
		default:
		{
			Serial.printf("[AP] %d, Default\r\n", event);
			break;
		}
	}
}

void wifiInit ( void )
{
	Serial.println ( "[WiFi] init" );
	WiFi.onEvent ( wifiOnEvent );
	WiFi.mode ( WIFI_STA );
}

void wifiStart ( const char * const __restrict__ ssid, const char * const __restrict__ password )
{
	// if wifi already connected
	WiFi.disconnect();
	
	// wifi settings part
	WiFi.begin ( ssid, password );
}