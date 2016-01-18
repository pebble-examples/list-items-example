/**
 * Simple app demonstrating list transmission.
 * When JS is ready, it will send a list of items to the C side.
 * When the transfer is complete, the C side will send a list back to JS.
 */

#include <pebble.h>

#include "modules/comm.h"
#include "windows/main_window.h"

static void init() {
  comm_init();
  
  main_window_push();
}

static void deinit() { }

int main() {
  init();
  app_event_loop();
  deinit();
}
