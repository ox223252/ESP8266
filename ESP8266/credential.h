#ifndef __CREDENTIAL_H__
#define __CREDENTIAL_H__

#include <stdint.h>

static const struct{
	char* ssid;
	char* password;
	char* mqtt_server;
	uint16_t port;
}
network[] = 
{
	{ 
		.ssid = "SSID",
		.password = "PASS",
		.mqtt_server = "SERVER",
		.port = 1883
	}
};

#endif
