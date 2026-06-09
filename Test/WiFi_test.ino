#include <ESP8266WiFi.h>

void setup()
{
    Serial.begin(115200);

    WiFi.mode(WIFI_AP);

    WiFi.softAP(
        "SentinelVision",
        "dashcam123");

    Serial.print("IP: ");
    Serial.println(
        WiFi.softAPIP());
}

void loop()
{
}