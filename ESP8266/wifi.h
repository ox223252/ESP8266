#ifndef __WIFI_H__
#define __WIFI_H__

#include <ESP8266WiFi.h>
	// https://github.com/techiesms/ESP8266-Series
	// https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi
	// https://github.com/me-no-dev/ESPAsyncTCP

extern bool wifiConnected; ///< wifi status

void wifiInit ( void );
void wifiStart ( const char * const __restrict__ ssid, const char * const __restrict__ password );

#endif