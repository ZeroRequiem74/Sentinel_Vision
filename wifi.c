#include "wifi.h"

#include <ESP8266WiFi.h>

void Wifi_Init(void)
{
    WiFi.mode(WIFI_AP);

    WiFi.softAP(
        "SentinelVision",
        "dashcam123");
}

void Wifi_SendAccidentNotification(
    void)
{
    /*
     * Future:
     * Send file link
     * Send push notification
     * Start web server
     */
}