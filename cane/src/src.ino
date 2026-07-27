#include "../include/main.hpp"
#include "../include/speaker.hpp"
#include "../include/websocket.hpp"
void setup() {
  main_setup();
  ws_setup();
  //speaker_setup();
}

void loop() {
  main_loop();
  ws_loop();
  //speaker_loop();
}
