#include <SPI.h>

#define CAMERA_CS 4

void setup()
{
    Serial.begin(115200);

    SPI.begin();

    pinMode(CAMERA_CS, OUTPUT);
    digitalWrite(CAMERA_CS, HIGH);

    Serial.println("Camera Test");
}

void loop()
{
    digitalWrite(CAMERA_CS, LOW);

    SPI.transfer(0x00);

    digitalWrite(CAMERA_CS, HIGH);

    Serial.println("SPI Transaction Sent");

    delay(1000);
}