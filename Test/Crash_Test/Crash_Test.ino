#include <Arduino.h>
#include <Wire.h>
#include <math.h>

#define ADXL345_ADDRESS 0x53
#define CRASH_THRESHOLD 400.0

void setup()
{
    Serial.begin(115200);
    Wire.begin();

    Wire.beginTransmission(ADXL345_ADDRESS);
    Wire.write(0x2D);
    Wire.write(0x08);
    Wire.endTransmission();

    Serial.println("Crash Detection Test");
}

void loop()
{
    int16_t x;
    int16_t y;
    int16_t z;

    Wire.beginTransmission(ADXL345_ADDRESS);
    Wire.write(0x32);
    Wire.endTransmission(false);

    Wire.requestFrom(ADXL345_ADDRESS, 6);

    if(Wire.available() >= 6)
    {
        x = Wire.read() | (Wire.read() << 8);
        y = Wire.read() | (Wire.read() << 8);
        z = Wire.read() | (Wire.read() << 8);

        float magnitude =
            sqrt(
                (float)(x * x) +
                (float)(y * y) +
                (float)(z * z));

        Serial.println(magnitude);

        if(magnitude > CRASH_THRESHOLD)
        {
            Serial.println("ACCIDENT DETECTED");
        }
    }

    delay(100);
}