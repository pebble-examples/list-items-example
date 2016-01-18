#include "main_window.h"

static Window *s_window;
static TextLayer *s_text_layer;

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_text_layer = text_layer_create(grect_inset(bounds, GEdgeInsets(20, 5)));
  text_layer_set_text(s_text_layer, "Waiting...");
  text_layer_set_text_alignment(s_text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(s_text_layer));
}

static void window_unload(Window *window) {
  text_layer_destroy(s_text_layer);

  window_destroy(s_window);
}

void main_window_push() {
  s_window = window_create();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  window_stack_push(s_window, true);
}

void main_window_set_text(char *text) {
  text_layer_set_text(s_text_layer, text);
}
