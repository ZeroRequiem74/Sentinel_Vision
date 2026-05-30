#include "dashcam.h"
#include "camera.h"
#include "storage.h"
#include "adxl345.h"
#include "wifi.h"
#include "config.h"

#include <Arduino.h>
#include <stdio.h>
#include <math.h>

static RecordingState_t g_state;

static bool g_accidentDetected = false;

static uint32_t g_startTime;

static const char FIRST_FOLDER[] = "/FIRST";
static const char SECOND_FOLDER[] = "/SECOND";
static const char ACCIDENT_FOLDER[] = "/ACCIDENT";

static void CheckAccident(void);
static void RotateBuffers(void);
static void SaveAccident(void);
static void RecordFrame(void);

void Dashcam_Init(void)
{
    Storage_Init();

    Camera_Init();

    ADXL345_Init();

    Wifi_Init();

    Storage_CreateFolder(FIRST_FOLDER);
    Storage_CreateFolder(SECOND_FOLDER);
    Storage_CreateFolder(ACCIDENT_FOLDER);

    g_state = RECORD_FIRST;

    g_startTime = millis();
}

void Dashcam_Task(void)
{
    RecordFrame();

    CheckAccident();

    RotateBuffers();

    if(g_accidentDetected)
    {
        SaveAccident();

        g_accidentDetected = false;
    }
}

static void RecordFrame(void)
{
    static uint32_t frameCount = 0;

    char filename[64];

    if(g_state == RECORD_FIRST)
    {
        sprintf(filename,
                "/FIRST/IMG_%lu.JPG",
                frameCount);
    }
    else
    {
        sprintf(filename,
                "/SECOND/IMG_%lu.JPG",
                frameCount);
    }

    Camera_CaptureJPEG(filename);

    frameCount++;
}

static void CheckAccident(void)
{
    float ax;
    float ay;
    float az;

    float magnitude;

    ADXL345_ReadXYZ(&ax,
                    &ay,
                    &az);

    magnitude =
        sqrtf(ax * ax +
              ay * ay +
              az * az);

    if(magnitude > ACCIDENT_THRESHOLD)
    {
        g_accidentDetected = true;
    }
}

static void RotateBuffers(void)
{
    uint32_t elapsed;

    elapsed = millis() - g_startTime;

    if(elapsed < SIX_HOURS_MS)
    {
        return;
    }

    g_startTime = millis();

    if(g_state == RECORD_FIRST)
    {
        Storage_ClearFolder(SECOND_FOLDER);

        g_state = RECORD_SECOND;
    }
    else
    {
        Storage_DeleteFolder(FIRST_FOLDER);

        Storage_CopyFolder(
            SECOND_FOLDER,
            FIRST_FOLDER);

        Storage_ClearFolder(SECOND_FOLDER);

        g_state = RECORD_SECOND;
    }
}

static void SaveAccident(void)
{
    Storage_CopyFolder(
        FIRST_FOLDER,
        "/ACCIDENT/FIRST");

    Storage_CopyFolder(
        SECOND_FOLDER,
        "/ACCIDENT/SECOND");

    Wifi_SendAccidentNotification();
}