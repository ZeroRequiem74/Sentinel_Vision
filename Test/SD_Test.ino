#include <SPI.h>
#include <SD.h>

#define SD_CS 15

void setup()
{
    Serial.begin(115200);

    if(!SD.begin(SD_CS))
    {
        Serial.println("SD FAILED");
        return;
    }

    Serial.println("SD OK");

    File file = SD.open("/test.txt", FILE_WRITE);

    if(file)
    {
        file.println("Sentinel Vision Test");
        file.close();

        Serial.println("Write Success");
    }

    file = SD.open("/test.txt");

    while(file.available())
    {
        Serial.write(file.read());
    }

    file.close();
}

void loop()
{
}