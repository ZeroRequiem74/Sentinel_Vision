#include "adxl345.h"

#include <Arduino.h>
#include <Wire.h>

void ADXL345_Init(void)
{
    Wire.begin();

    Serial.println("ADXL345 Init");
}

void ADXL345_ReadXYZ(
    float *x,
    float *y,
    float *z)
{
    *x = 0.0f;
    *y = 0.0f;
    *z = 1.0f;
}