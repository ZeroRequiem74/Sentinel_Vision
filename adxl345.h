#ifndef ADXL345_H
#define ADXL345_H

void ADXL345_Init(void);

void ADXL345_ReadXYZ(
    float *x,
    float *y,
    float *z);

#endif