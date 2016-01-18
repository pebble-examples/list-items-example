#include "comm.h"

#define NUM_ITEMS 6 // Max items expected from JS

static int s_data_items[] = { 2, 4, 8, 16, 32, 64 };
static int s_current_index = 0;

static void send_list() {
  // Send the first item
  DictionaryIterator *iter;
  if(app_message_outbox_begin(&iter) == APP_MSG_OK) {
    dict_write_int(iter, s_current_index, &s_data_items[s_current_index], 
                                                          sizeof(int), true);
    app_message_outbox_send();
  }
}

static void inbox_received_handler(DictionaryIterator *iter, void *context) {
  // An item was received; check which one
  for(int key = 0; key < NUM_ITEMS; key++) {
    Tuple *item_t = dict_find(iter, key);
    if(item_t) {
      // Update UI
      static char s_buff[32];
      snprintf(s_buff, sizeof(s_buff), "JS sent value: %d", (int)item_t->value->int32);
      APP_LOG(APP_LOG_LEVEL_INFO, "%s", s_buff);
      main_window_set_text(&s_buff[0]);

      // Once all items have been received from JS, send a list in reply
      if(key == NUM_ITEMS - 1) {
        send_list();
      }
    }
  }
}

static void outbox_sent_handler(DictionaryIterator *iter, void *context) {
  // Last message was successful; update UI
  static char s_buff[32];
  snprintf(s_buff, sizeof(s_buff), "Sent value: %d", s_data_items[s_current_index]);
  main_window_set_text(&s_buff[0]);

  // Increment the index
  s_current_index++;

  if(s_current_index < NUM_ITEMS) {
    // Send the next item
    DictionaryIterator *iter;
    if(app_message_outbox_begin(&iter) == APP_MSG_OK) {
      dict_write_int(iter, s_current_index, &s_data_items[s_current_index], 
                                                            sizeof(int), true);
      app_message_outbox_send();
    }
  } else {
    // We have reached the end of the sequence
    APP_LOG(APP_LOG_LEVEL_INFO, "All transmission complete!");
  }
}

void comm_init() {
  app_message_register_inbox_received(inbox_received_handler);
  app_message_register_outbox_sent(outbox_sent_handler);

  const int inbox_size = 64;
  const int outbox_size = 64;
  app_message_open(inbox_size, outbox_size);
}

