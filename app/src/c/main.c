#include <pebble.h>

// ---------- REUSABLE STUFF ---------

static int font_get_height(GFont font, const char* text, int width, GTextAttributes* attrs) {
  GSize size = graphics_text_layout_get_content_size_with_attributes(
    text, font, GRect(0, 0, width, 1000), 
    GTextOverflowModeWordWrap, GTextAlignmentCenter, attrs);
  return size.h;
}

static void tuple_find_cstring(DictionaryIterator *iter, 
                               uint32_t key,
                               char* buffer, 
                               size_t buffer_size) {
  Tuple *t = dict_find(iter, key);
  if (!t) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Missing key: %lu", (unsigned long) key);
    return;
  }  
  if (t->type != TUPLE_CSTRING) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Wrong %lu type: %d", (unsigned long) key, t->type);
    return;
  }
  strncpy(buffer, (const char*) t->value, buffer_size);
}

static void storage_read(uint32_t key, char* buffer, size_t buffer_size, const char* def) {
  if (!persist_exists(key)) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Key %lu doesn't exist.", (unsigned long) key);
    strncpy(buffer, def, buffer_size);
    return;
  }
  
  int read = persist_read_string(key, buffer, buffer_size);
  if (read == E_DOES_NOT_EXIST) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "E_DOES_NOT_EXIST for %lu.", (unsigned long) key);
    strncpy(buffer, def, buffer_size);
    return;
  }
}

static void storage_write(uint32_t key, const char* buffer) {
  int written = persist_write_string(key, buffer);
  if (written < 0) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "Writer error %d for %lu.", written, (unsigned long) key);
  }
  if ((size_t) written != strlen(buffer) + 1) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "Write length mismatch %d vs %lu for %lu.", written, (unsigned long) strlen(buffer), (unsigned long) key);
  }
}

// ---------- STATIC VARS ---------

static Window *s_window;
static Layer *s_canvas_layer;

static char s_app_fmt[256];
static char s_app_buffer[256];

static char s_app_glance_fmt[256];
static char s_app_glance_buffer[256];

static GTextAttributes *s_app_text_attributes;

// ---------- APP PREFS ---------

static void log_prefs() {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "  s_glance_fmt: %s", s_app_glance_fmt);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "  s_app_fmt: %s", s_app_fmt);
}

static void read_prefs() {
  storage_read(MESSAGE_KEY_APP_FMT, s_app_fmt, sizeof s_app_fmt, "%x");
  storage_read(MESSAGE_KEY_APP_GLANCE_FMT, s_app_glance_fmt, sizeof s_app_glance_fmt, "%x");
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Read prefs:");
  log_prefs();
}

static void save_prefs() {
  storage_write(MESSAGE_KEY_APP_FMT, s_app_fmt);
  storage_write(MESSAGE_KEY_APP_GLANCE_FMT, s_app_glance_fmt);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Saved prefs:");
  log_prefs();
  read_prefs();
}

// ---------- APP CODE ---------

static void prv_update_app_glance(AppGlanceReloadSession *session, 
                                  size_t limit,
                                  void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "prv_update_app_glance");

  time_t t = time_start_of_today();
  for (size_t i = 0; i < limit; ++i) {
    struct tm* t_tm = gmtime(&t);
    t_tm->tm_mday++;
    time_t expiration_time = mktime(t_tm);
      
    if (!strftime(s_app_glance_buffer, sizeof s_app_glance_buffer, s_app_glance_fmt, localtime(&t))) {
      APP_LOG(APP_LOG_LEVEL_ERROR, "strftime error");
    }
  
    const AppGlanceSlice entry = (AppGlanceSlice) {
      .layout = {
        .icon = PUBLISHED_ID_TODAY_25,
        .subtitle_template_string = s_app_glance_buffer
      },
      .expiration_time = expiration_time
    };
    const AppGlanceResult result = app_glance_add_slice(session, entry);
  
    if (result != APP_GLANCE_RESULT_SUCCESS) {
      APP_LOG(APP_LOG_LEVEL_ERROR, "AppGlance Error: %d", result);
    }
    t = expiration_time;
  }
}

static void refresh_ui() {
  time_t current_time = time(NULL);
  strftime(s_app_buffer, sizeof s_app_buffer, s_app_fmt, localtime(&current_time));
  layer_mark_dirty(s_canvas_layer);
}

static void canvas_update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);
  graphics_context_set_text_color(ctx, GColorBlack);
  int center = bounds.origin.y + (bounds.size.h / 2);
  int width = bounds.size.w;
  // graphics_draw_line(ctx, GPoint(bounds.origin.x, center), GPoint(width, center));

  const char *labelText = "Today Is";
  GFont labelFont = fonts_get_system_font(FONT_KEY_GOTHIC_24);
  int labelHeight = font_get_height(labelFont, labelText, width, NULL);

  const char *dateText = s_app_buffer;
  GFont dateFont = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
  int dateHeight = font_get_height(dateFont, dateText, width, s_app_text_attributes);

  int totalHeight = labelHeight + dateHeight;
  int top = center - totalHeight / 2;

  {
    GRect rect = GRect(bounds.origin.x, top, width, labelHeight);
    // graphics_draw_rect(ctx, rect);
    graphics_draw_text(
      ctx, labelText, labelFont, rect, GTextOverflowModeWordWrap,  GTextAlignmentCenter, NULL);
  }
                                            
  {
    GRect rect = GRect(bounds.origin.x, top + labelHeight, width, dateHeight);
    // graphics_draw_rect(ctx, rect);
    graphics_draw_text(
      ctx, dateText, dateFont, rect, GTextOverflowModeWordWrap,  GTextAlignmentCenter, s_app_text_attributes);
  }
}

static void prv_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_canvas_layer = layer_create(bounds);
  layer_set_update_proc(s_canvas_layer, canvas_update_proc);
  layer_add_child(window_get_root_layer(window), s_canvas_layer);

  s_app_text_attributes = graphics_text_attributes_create();
  graphics_text_attributes_enable_screen_text_flow(s_app_text_attributes, 5);

  refresh_ui();
}

static void prv_window_unload(Window *window) {
  layer_destroy(s_canvas_layer);
  graphics_text_attributes_destroy(s_app_text_attributes);
}

static void prv_inbox_received_handler(DictionaryIterator *iter, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "prv_inbox_received_handler");

  tuple_find_cstring(iter, MESSAGE_KEY_APP_GLANCE_FMT, s_app_glance_fmt, sizeof s_app_glance_fmt);
  tuple_find_cstring(iter, MESSAGE_KEY_APP_FMT, s_app_fmt, sizeof s_app_fmt);

  save_prefs();
  refresh_ui();
}
  
static void prv_init(void) {
  read_prefs();
  
  s_window = window_create();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = prv_window_load,
    .unload = prv_window_unload,
  });
  window_stack_push(s_window, true /* animated */);

  app_message_set_context(s_window);
  app_message_register_inbox_received(prv_inbox_received_handler);
  app_message_open(128, 128);

  APP_LOG(APP_LOG_LEVEL_DEBUG, "init done.");
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
