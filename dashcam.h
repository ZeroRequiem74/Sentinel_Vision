#ifndef DASHCAM_H
#define DASHCAM_H

#include <stdint.h>
#include <stdbool.h>

typedef enum
{
    RECORD_FIRST = 0,
    RECORD_SECOND
} RecordingState_t;

void Dashcam_Init(void);
void Dashcam_Task(void);

#endif