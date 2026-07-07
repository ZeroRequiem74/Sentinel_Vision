#include <Wire.h>
#include <math.h>

#define ADXL345_ADDRESS 0x53
#define CRASH_THRESHOLD 450.0

void InitAccelerometer()
{
    Wire.begin();

    // Put ADXL345 into Measurement Mode
    Wire.beginTransmission(ADXL345_ADDRESS);
    Wire.write(0x2D);
    Wire.write(0x08);
    Wire.endTransmission();

    Serial.println("ADXL345 Initialized");
}

bool CrashDetected()
{
    int16_t x;
    int16_t y;
    int16_t z;

    Wire.beginTransmission(ADXL345_ADDRESS);
    Wire.write(0x32);
    Wire.endTransmission(false);

    Wire.requestFrom(ADXL345_ADDRESS, 6);

    if (Wire.available() < 6)
    {
        return false;
    }

    x = Wire.read() | (Wire.read() << 8);
    y = Wire.read() | (Wire.read() << 8);
    z = Wire.read() | (Wire.read() << 8);

    float magnitude =
        sqrt(
            (float)x * x +
            (float)y * y +
            (float)z * z);

    if (magnitude > CRASH_THRESHOLD)
    {
        return true;
    }

    return false;
}