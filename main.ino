#include "dashcam.h"

void setup()
{
    Serial.begin(115200);

    Dashcam_Init();
}

void loop()
{
    Dashcam_Task();
}