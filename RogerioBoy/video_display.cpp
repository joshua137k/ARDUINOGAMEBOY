#include "video_display.h"

ESP_8_BIT_composite videoOut(1);  // 1 para NTSC, 0 para PAL
uint8_t** frameBufferLines;
void initializeVideo() {
  videoOut.begin();
  clean();
}


void clean() {
  uint8_t** frameBufferLines = videoOut.getFrameBufferLines();
  for (int y = 0; y < 240; y++) {
    for (int x = 0; x < 256; x++) {
      frameBufferLines[y][x] = 0xFF; // Valor padrão (branco)
    }
  }
  videoOut.waitForFrame();
  frameBufferLines = videoOut.getFrameBufferLines();
}

void setPixel(int x, int y, uint8_t color) {
  if (x >= 0 && x < 256 && y >= 0 && y < 240) {
    frameBufferLines = videoOut.getFrameBufferLines();
    frameBufferLines[y][x] = color;
  }
}


void drawSquare(int x, int y, int size, uint8_t color) {
  for (int j = 0; j < size; j++) {
    for (int i = 0; i < size; i++) {
      setPixel(x + i, y + j, color);
    }
  }
  videoOut.waitForFrame();
  frameBufferLines = videoOut.getFrameBufferLines();
}

