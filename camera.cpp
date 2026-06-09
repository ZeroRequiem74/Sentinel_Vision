#include "camera.h"

#include <Arduino.h>

void Camera_Init(void)
{
    Serial.println("Camera Init");
}

void Camera_CaptureJPEG(
    const char *filename)
{
    Serial.print("Capture: ");
    Serial.println(filename);
}