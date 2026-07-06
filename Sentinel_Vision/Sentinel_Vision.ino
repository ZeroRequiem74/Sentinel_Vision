// Sentinel Vision Main Sketch
// Based on the ArduCAM ESP8266 OV2640 Video2SD demo

#include <SD.h>
#include <Wire.h>
#include <ArduCAM.h>
#include <SPI.h>
#include "memorysaver.h"

#if !(defined ESP8266)
#error Please select an ESP8266 board in Tools > Board
#endif

#if !(defined(OV2640_MINI_2MP) || (defined(ARDUCAM_SHIELD_V2) && defined(OV2640_CAM)))
#error Please enable OV2640_MINI_2MP in memorysaver.h
#endif

#define SD_CS 0
#define SPI_CS 16

#define pic_mun 200
#define rate 0x05
#define AVIOFFSET 240

unsigned long movi_size = 0;
unsigned long jpeg_size = 0;

const char zero_buf[4] = {0x00, 0x00, 0x00, 0x00};

const char avi_header[AVIOFFSET] PROGMEM = {
  0x52, 0x49, 0x46, 0x46, 0xD8, 0x01, 0x0E, 0x00, 0x41, 0x56, 0x49, 0x20, 0x4C, 0x49, 0x53, 0x54,
  0xD0, 0x00, 0x00, 0x00, 0x68, 0x64, 0x72, 0x6C, 0x61, 0x76, 0x69, 0x68, 0x38, 0x00, 0x00, 0x00,
  0xA0, 0x86, 0x01, 0x00, 0x80, 0x66, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00,
  0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x40, 0x01, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4C, 0x49, 0x53, 0x54, 0x84, 0x00, 0x00, 0x00,
  0x73, 0x74, 0x72, 0x6C, 0x73, 0x74, 0x72, 0x68, 0x30, 0x00, 0x00, 0x00, 0x76, 0x69, 0x64, 0x73,
  0x4D, 0x4A, 0x50, 0x47, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x00, 0x00, rate, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x73, 0x74, 0x72, 0x66,
  0x28, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x40, 0x01, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x18, 0x00, 0x4D, 0x4A, 0x50, 0x47, 0x00, 0x84, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4C, 0x49, 0x53, 0x54,
  0x10, 0x00, 0x00, 0x00, 0x6F, 0x64, 0x6D, 0x6C, 0x64, 0x6D, 0x6C, 0x68, 0x04, 0x00, 0x00, 0x00,
  0x64, 0x00, 0x00, 0x00, 0x4C, 0x49, 0x53, 0x54, 0x00, 0x01, 0x0E, 0x00, 0x6D, 0x6F, 0x76, 0x69,
};

ArduCAM myCAM(OV2640, SPI_CS);

void print_quartet(unsigned long i, File fd)
{
  fd.write(i % 0x100);
  i = i >> 8;

  fd.write(i % 0x100);
  i = i >> 8;

  fd.write(i % 0x100);
  i = i >> 8;

  fd.write(i % 0x100);
}

void Video2SD()
{
  char str[12];
  File outFile;
  byte buf[256];

  static int i = 0;
  static int k = 0;

  uint8_t temp = 0;
  uint8_t temp_last = 0;

  unsigned long position = 0;
  uint16_t frame_cnt = 0;
  uint8_t remnant = 0;

  movi_size = 0;

  k = k + 1;
  itoa(k, str, 10);
  strcat(str, ".avi");

  outFile = SD.open(str, O_WRITE | O_CREAT | O_TRUNC);

  if (!outFile)
  {
    Serial.println("open file failed");
    while (1);
    return;
  }

  for (i = 0; i < AVIOFFSET; i++)
  {
    char ch = pgm_read_byte(&avi_header[i]);
    buf[i] = ch;
  }

  outFile.write(buf, AVIOFFSET);

  Serial.println("Recording video, please wait...");

  for (frame_cnt = 0; frame_cnt < pic_mun; frame_cnt++)
  {
    yield();

    if (CrashDetected())
    {
      Serial.println("Crash Detected During Recording");
    }

    myCAM.flush_fifo();
    myCAM.clear_fifo_flag();
    myCAM.start_capture();

    while (!myCAM.get_bit(ARDUCHIP_TRIG, CAP_DONE_MASK));

    outFile.write("00dc");
    outFile.write(zero_buf, 4);

    i = 0;
    jpeg_size = 0;

    myCAM.CS_LOW();
    myCAM.set_fifo_burst();

#if !(defined(ARDUCAM_SHIELD_V2) && defined(OV2640_CAM))
    SPI.transfer(0xFF);
#endif

    while ((temp != 0xD9) | (temp_last != 0xFF))
    {
      temp_last = temp;
      temp = SPI.transfer(0x00);

      if (i < 256)
      {
        buf[i++] = temp;
      }
      else
      {
        myCAM.CS_HIGH();
        outFile.write(buf, 256);

        i = 0;
        buf[i++] = temp;

        myCAM.CS_LOW();
        myCAM.set_fifo_burst();

        jpeg_size += 256;
      }
    }

    if (i > 0)
    {
      myCAM.CS_HIGH();
      outFile.write(buf, i);
      jpeg_size += i;
    }

    temp_last = 0;
    temp = 0;

    remnant = (4 - (jpeg_size & 0x00000003)) & 0x00000003;
    jpeg_size = jpeg_size + remnant;
    movi_size = movi_size + jpeg_size;

    if (remnant > 0)
    {
      outFile.write(zero_buf, remnant);
    }

    position = outFile.position();
    outFile.seek(position - 4 - jpeg_size);
    print_quartet(jpeg_size, outFile);

    position = outFile.position();
    outFile.seek(position + 6);
    outFile.write("AVI1", 4);

    position = outFile.position();
    outFile.seek(position + jpeg_size - 10);
  }

  outFile.seek(4);
  print_quartet(movi_size + 0xd8, outFile);

  unsigned long us_per_frame = 1000000 / rate;

  outFile.seek(0x20);
  print_quartet(us_per_frame, outFile);

  unsigned long max_bytes_per_sec = movi_size * rate / frame_cnt;

  outFile.seek(0x24);
  print_quartet(max_bytes_per_sec, outFile);

  outFile.seek(0x30);
  print_quartet(max_bytes_per_sec, outFile);

  outFile.seek(0xe0);
  print_quartet(max_bytes_per_sec, outFile);

  outFile.seek(0xe8);
  print_quartet(movi_size, outFile);

  myCAM.CS_HIGH();

  outFile.close();

  Serial.println("Record video OK");
}

void setup()
{
  uint8_t vid;
  uint8_t pid;
  uint8_t temp;

  Wire.begin();

  Serial.begin(115200);
  Serial.println("ArduCAM Start!");

  pinMode(SPI_CS, OUTPUT);

  delay(1000);

  SPI.begin();

  myCAM.write_reg(ARDUCHIP_TEST1, 0x55);
  temp = myCAM.read_reg(ARDUCHIP_TEST1);

  if (temp != 0x55)
  {
    Serial.println("SPI interface Error!");
    while (1);
  }

  myCAM.wrSensorReg8_8(0xff, 0x01);
  myCAM.rdSensorReg8_8(OV2640_CHIPID_HIGH, &vid);
  myCAM.rdSensorReg8_8(OV2640_CHIPID_LOW, &pid);

  if ((vid != 0x26) && ((pid != 0x41) || (pid != 0x42)))
  {
    Serial.println("Can't find OV2640 module!");
  }
  else
  {
    Serial.println("OV2640 detected.");
  }

  myCAM.set_format(JPEG);
  myCAM.InitCAM();

  if (!SD.begin(SD_CS))
  {
    Serial.println("SD Card Error");
  }
  else
  {
    Serial.println("SD Card detected!");
  }

  InitAccelerometer();

  Serial.println("System Ready");
}

void loop()
{
  if (CrashDetected())
  {
    Serial.println("Crash Detected!");
    Video2SD();

    while (1)
    {
      delay(1000);
    }
  }

  delay(10);
}