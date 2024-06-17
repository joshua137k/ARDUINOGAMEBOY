#include "video_display.h"

ESP_8_BIT_composite videoOut(1);  // 1 para NTSC, 0 para PAL
uint8_t** matrizFrame;
void initializeVideo() {
  videoOut.begin();
  const int rows = 240;
  const int cols = 256;
  matrizFrame = new uint8_t*[rows];
  for (int i = 0; i < rows; ++i) {
      matrizFrame[i] = new uint8_t[cols];
  }
  clean();
  
}



void update(){
  videoOut.waitForFrame();
  uint8_t** frameBufferLines = videoOut.getFrameBufferLines();
  for (int y = 0; y < 240; y++) {
    for (int x = 0; x < 256; x++) {
      frameBufferLines[y][x] = matrizFrame[y][x]; 
    }
  }
  videoOut.waitForFrame();
}


void clean() {
  for (int y = 0; y < 240; y++) {
    for (int x = 0; x < 256; x++) {
      matrizFrame[y][x]=0xFF;
    }
  }
  update();
}

void setPixel(int x, int y, uint8_t color) {
  if (x >= 0 && x < 256 && y >= 0 && y < 240) {
    matrizFrame[y][x] = color;
  }
}


void drawSquare(int x, int y, int size, uint8_t color) {
  for (int j = 0; j < size; j++) {
    for (int i = 0; i < size; i++) {
      setPixel(x + i, y + j, color);
    }
  }
  update();
}

