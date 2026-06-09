#include <Arduino.h>
#include <SPI.h>

#define CAMERA_CS_PIN 4

void setup()
{
    Serial.begin(115200);

    SPI.begin();

    pinMode(
        CAMERA_CS_PIN,
        OUTPUT);

    digitalWrite(
        CAMERA_CS_PIN,
        HIGH);

    Serial.println(
        "Camera SPI Test");
}

void loop()
{
    digitalWrite(
        CAMERA_CS_PIN,
        LOW);

    SPI.transfer(0x00);

    digitalWrite(
        CAMERA_CS_PIN,
        HIGH);

    Serial.println(
        "SPI Transaction");

    delay(1000);
}