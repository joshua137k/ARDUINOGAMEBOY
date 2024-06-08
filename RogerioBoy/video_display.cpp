#include "video_display.h"

ESP_8_BIT_composite videoOut(true /* = NTSC */);

void initializeVideo() {
  videoOut.begin();
  uint8_t** frameBufferLines = videoOut.getFrameBufferLines();

  // Inicializa a tela com um quadro padrão, se necessário
  for (int y = 0; y < 240; y++) {
    for (int x = 0; x < 256; x++) {
      frameBufferLines[y][x] = 0x00; // Valor padrão (preto) ou qualquer outro valor inicial
    }
  }

  videoOut.waitForFrame(); // Aguarda a renderização do quadro inicial
}
void setPixel(int x, int y, uint8_t color) {
  uint8_t** frameBufferLines = videoOut.getFrameBufferLines();
  if (x >= 0 && x < 256 && y >= 0 && y < 240) {
    frameBufferLines[y][x] = color;
  }
  
  
}

