#ifndef VIDEO_DISPLAY_H
#define VIDEO_DISPLAY_H

#include <ESP_8_BIT_composite.h>

extern ESP_8_BIT_composite videoOut;

void initializeVideo();
void setPixel(int x, int y, uint8_t color);



#endif // VIDEO_DISPLAY_H
