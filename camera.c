#include "camera.h"

#include <Arduino.h>

void Camera_Init(void)
{
    Serial.println(
        "Camera Initialized");
}

void Camera_CaptureJPEG(
    const char *filename)
{
    Serial.print(
        "Capturing: ");

    Serial.println(filename);

    /*
     * TODO:
     * Arducam SPI Capture
     * Save JPEG to SD card
     */
}