#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

#define SD_CS_PIN 15

void setup()
{
    Serial.begin(115200);

    if(!SD.begin(SD_CS_PIN))
    {
        Serial.println("SD FAILED");
        return;
    }

    Serial.println("SD INITIALIZED");

    File testFile =
        SD.open("/test.txt", FILE_WRITE);

    if(testFile)
    {
        testFile.println("SD CARD TEST");

        testFile.close();

        Serial.println("WRITE SUCCESS");
    }

    testFile = SD.open("/test.txt");

    if(testFile)
    {
        while(testFile.available())
        {
            Serial.write(
                testFile.read());
        }

        testFile.close();
    }
}

void loop()
{
}