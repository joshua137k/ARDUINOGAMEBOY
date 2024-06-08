#include "video_display.h"

#include "Interpreter.h"



void setup() {
  Serial.begin(115200);
  initializeVideo();
  
  execute_script("TOOS");
  

}






void loop() {

  // Nothing to do here
}