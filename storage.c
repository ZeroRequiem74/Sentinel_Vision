#include "storage.h"

#include <Arduino.h>
#include <SD.h>

void Storage_Init(void)
{
    if(!SD.begin())
    {
        Serial.println(
            "SD Failed");

        while(1);
    }

    Serial.println(
        "SD Ready");
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
    Serial.print(
        "Clearing ");

    Serial.println(path);

    /*
     * TODO:
     * Delete contents only
     */
}

void Storage_CopyFolder(
    const char *src,
    const char *dst)
{
    Serial.print(
        "Copy ");

    Serial.print(src);

    Serial.print(" -> ");

    Serial.println(dst);

    /*
     * TODO:
     * Recursive file copy
     */
}