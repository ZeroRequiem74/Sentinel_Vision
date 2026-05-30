#include "adxl345.h"

#include <Arduino.h>

void ADXL345_Init(void)
{
    Serial.println(
        "ADXL345 Initialized");
}

void ADXL345_ReadXYZ(
    float *x,
    float *y,
    float *z)
{
    *x = 0.0f;
    *y = 0.0f;
    *z = 1.0f;

    /*
     * TODO:
     * Read actual I2C registers
     */
}