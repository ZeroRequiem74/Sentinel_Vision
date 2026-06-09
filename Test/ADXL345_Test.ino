#include <Wire.h>

#define ADXL345_ADDR 0x53

void setup()
{
    Serial.begin(115200);
    Wire.begin();

    Wire.beginTransmission(ADXL345_ADDR);
    Wire.write(0x2D);
    Wire.write(0x08);
    Wire.endTransmission();

    Serial.println("ADXL345 Ready");
}

void loop()
{
    int16_t x, y, z;

    Wire.beginTransmission(ADXL345_ADDR);
    Wire.write(0x32);
    Wire.endTransmission(false);

    Wire.requestFrom(ADXL345_ADDR, 6);

    x = Wire.read() | (Wire.read() << 8);
    y = Wire.read() | (Wire.read() << 8);
    z = Wire.read() | (Wire.read() << 8);

    Serial.print("X: ");
    Serial.print(x);

    Serial.print(" Y: ");
    Serial.print(y);

    Serial.print(" Z: ");
    Serial.println(z);

    delay(500);
}