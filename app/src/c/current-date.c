#include <pebble.h>

static Window *s_window;
static TextLayer *s_text_layer;
static char s_text_buffer[256];
static char s_glance_buffer[256];

static void prv_select_click_handler(ClickRecognizerRef recognizer, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "prv_select_click_handler");
}

static void prv_up_click_handler(ClickRecognizerRef recognizer, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "prv_up_click_handler");
}

static void prv_down_click_handler(ClickRecognizerRef recognizer, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "prv_down_click_handler");
}

static void prv_click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, prv_select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, prv_up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, prv_down_click_handler);
}

  // "%x", "%A", "%A %d %B %y", "%A %d %b %y"


#define DATE_FMT "%A %d %b %y"

static void prv_update_app_glance(AppGlanceReloadSession *session, 
                                  size_t limit,
                                  void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "prv_update_app_glance");
  if (limit < 1) return;
  
  time_t current_time = time(NULL);
  if (!strftime(s_glance_buffer, sizeof s_text_buffer, DATE_FMT, localtime(&current_time))) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "strftime error");
  }

  const AppGlanceSlice entry = (AppGlanceSlice) {
    .layout = {
      .icon = APP_GLANCE_SLICE_DEFAULT_ICON,
      .subtitle_template_string = s_glance_buffer
    },
    .expiration_time = time(NULL)+3600
  };
  const AppGlanceResult result = app_glance_add_slice(session, entry);
  if (result != APP_GLANCE_RESULT_SUCCESS) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "AppGlance Error: %d", result);
  }
}


static void prv_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  time_t current_time = time(NULL);
  
  s_text_layer = text_layer_create(GRect(0, 72, bounds.size.w, 20));
    
  if (strftime(s_text_buffer, sizeof s_text_buffer, DATE_FMT, localtime(&current_time))) {
    text_layer_set_text(s_text_layer, s_text_buffer);
  } else {
    text_layer_set_text(s_text_layer, "ERROR");
  }

  text_layer_set_text_alignment(s_text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(s_text_layer));
}

static void prv_window_unload(Window *window) {
  text_layer_destroy(s_text_layer);
}

static void prv_init(void) {
  s_window = window_create();
  window_set_click_config_provider(s_window, prv_click_config_provider);
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = prv_window_load,
    .unload = prv_window_unload,
  });
  const bool animated = true;
  window_stack_push(s_window, animated);

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", s_window);
}

static void prv_deinit(void) {
  window_destroy(s_window);

  app_glance_reload(prv_update_app_glance, NULL);
}

int main(void) {
  prv_init();
  app_event_loop();
  prv_deinit();
}
