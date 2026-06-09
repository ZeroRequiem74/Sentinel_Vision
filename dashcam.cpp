#include "dashcam.h"
#include "camera.h"
#include "adxl345.h"
#include "storage.h"
#include "wifi.h"
#include "config.h"

#include <Arduino.h>
#include <math.h>

static RecordingState_t gState;
static bool gAccidentDetected;
static uint32_t gStartTime;

static void Dashcam_RecordFrame(void);
static void Dashcam_CheckAccident(void);
static void Dashcam_RotateBuffers(void);
static void Dashcam_SaveAccident(void);

void Dashcam_Init(void)
{
    Storage_Init();
    Camera_Init();
    ADXL345_Init();
    Wifi_Init();

    gState = RECORD_FIRST;
    gAccidentDetected = false;
    gStartTime = millis();
}

void Dashcam_Task(void)
{
    Dashcam_RecordFrame();

    Dashcam_CheckAccident();

    Dashcam_RotateBuffers();

    if(gAccidentDetected)
    {
        Dashcam_SaveAccident();
        gAccidentDetected = false;
    }
}