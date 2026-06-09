#include "storage.h"

#include <Arduino.h>
#include <SD.h>
#define SD_CS_PIN 15

void Storage_Init(void)
{
    if(!SD.begin(SD_CS_PIN))
    {
        Serial.println("SD Failed");

        while(1);
    }

    Serial.println("SD Ready");
}

void Storage_CreateFolder(
    const char *path)
{
    SD.mkdir(path);
}

void Storage_DeleteFolder(
    const char *path)
{
    SD.rmdir(path);
}

void Storage_ClearFolder(
    const char *path)
{
}

void Storage_CopyFolder(
    const char *src,
    const char *dst)
{
}