#include <pebble.h>
#include "UnitVersal.h"
#include "convert.h"
#include "pairs.h"
#include "menu.h"

static Window *s_window;
static GFont s_res_roboto_condensed_21, s_res_bitham_34_medium_numbers;
static TextLayer *s_highlight_a, *s_highlight_b;
static TextLayer *s_unitname_a, *s_unitname_b, *s_unit_a, *s_unit_b;
static ActionBarLayer *s_actionbar;
static GBitmap *s_action_up, *s_action_down, *s_action_select;

float a_quantity = 0.0, b_quantity = 0.0;
char a_quantity_buffer[7] = "012345", b_quantity_buffer[7] = "-67890";
char a_label_buffer[7] = "UNIT A", b_label_buffer[7] = "UNIT B";
bool selected_unit = false; // top unit is "True", bottom is "False"

static void initialise_ui(void) {
  // TODO (future) Don't hardcode the layout coordinates in this function
  s_window = window_create();
  #ifdef PBL_SDK_2
    window_set_fullscreen(s_window, true);
  #endif
  window_set_background_color(s_window, PBL_IF_COLOR_ELSE(GColorMintGreen, GColorWhite));
  s_res_roboto_condensed_21 = fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21);
  s_res_bitham_34_medium_numbers = fonts_get_system_font(FONT_KEY_BITHAM_34_MEDIUM_NUMBERS);

  // Ugly hack around Aplite ActionBarLayer not being full height in SDK 2.
  // This isn't "correct" but it works well enough, so...
  // TODO (future) Remove this once Aplite gets SDK 3 support
  #ifdef PBL_SDK_2
    #ifdef PBL_PLATFORM_APLITE
      static TextLayer *s_hackbar;
      s_hackbar = text_layer_create(GRect(124, 0, 20, 168));
      text_layer_set_background_color(s_hackbar, GColorBlack);
      layer_add_child(window_get_root_layer(s_window), (Layer *)s_hackbar);
    #endif
  #endif

  // s_highlight_a and s_highlight_b
  #if defined(PBL_RECT)
    s_highlight_a = text_layer_create(GRect(0, 20, 124, 61));
    s_highlight_b = text_layer_create(GRect(0, 92, 124, 61));
  #elif defined(PBL_ROUND)
    s_highlight_a = text_layer_create(GRect(0, 31, 180, 55));
    s_highlight_b = text_layer_create(GRect(0, 95, 180, 55));
  #endif
  text_layer_set_background_color(s_highlight_a, GColorClear);
  text_layer_set_background_color(s_highlight_b, GColorClear);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_highlight_a);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_highlight_b);


  // s_unitname_a
  #if defined(PBL_RECT)
    s_unitname_a = text_layer_create(GRect(0, 20, 114, 25));
  #elif defined(PBL_ROUND)
    s_unitname_a = text_layer_create(GRect(20, 31, 120, 25));
    text_layer_set_text_alignment(s_unitname_a, GTextAlignmentCenter);
  #endif
  text_layer_set_text(s_unitname_a, a_label_buffer);
  text_layer_set_font(s_unitname_a, s_res_roboto_condensed_21);
  text_layer_set_background_color(s_unitname_a, GColorClear);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_unitname_a);

  // s_unit_a
  #if defined(PBL_RECT)
    s_unit_a = text_layer_create(GRect(0, 45, 114, 34));
  #elif defined (PBL_ROUND)
    s_unit_a = text_layer_create(GRect(20, 52, 120, 34));
    text_layer_set_text_alignment(s_unit_a, GTextAlignmentCenter);
  #endif
  text_layer_set_text(s_unit_a, a_quantity_buffer);
  text_layer_set_font(s_unit_a, s_res_bitham_34_medium_numbers);
  text_layer_set_background_color(s_unit_a, GColorClear);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_unit_a);

  // s_unit_b
  #if defined(PBL_RECT)
    s_unit_b = text_layer_create(GRect(0, 92, 114, 34));
  #elif defined(PBL_ROUND)
    s_unit_b = text_layer_create(GRect(20, 95, 120, 34));
    text_layer_set_text_alignment(s_unit_b, GTextAlignmentCenter);
  #endif
  text_layer_set_text(s_unit_b, b_quantity_buffer);
  text_layer_set_font(s_unit_b, s_res_bitham_34_medium_numbers);
  text_layer_set_background_color(s_unit_b, GColorClear);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_unit_b);

  // s_unitname_b
  #if defined(PBL_RECT)
    s_unitname_b = text_layer_create(GRect(0, 128, 114, 25));
  #elif defined(PBL_ROUND)
    s_unitname_b = text_layer_create(GRect(20, 129, 120, 25));
    text_layer_set_text_alignment(s_unitname_b, GTextAlignmentCenter);
  #endif
  text_layer_set_text(s_unitname_b, b_label_buffer);
  text_layer_set_font(s_unitname_b, s_res_roboto_condensed_21);
  text_layer_set_background_color(s_unitname_b, GColorClear);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_unitname_b);
 
  // s_actionbar
  s_action_up = gbitmap_create_with_resource(RESOURCE_ID_ACTION_UP);
  s_action_select = gbitmap_create_with_resource(RESOURCE_ID_ACTION_SELECT);
  s_action_down = gbitmap_create_with_resource(RESOURCE_ID_ACTION_DOWN);
  s_actionbar = action_bar_layer_create();
  action_bar_layer_add_to_window(s_actionbar, s_window);
  action_bar_layer_set_background_color(s_actionbar, GColorBlack);
  action_bar_layer_set_icon(s_actionbar, BUTTON_ID_UP, s_action_up);
  action_bar_layer_set_icon(s_actionbar, BUTTON_ID_SELECT, s_action_select);
  action_bar_layer_set_icon(s_actionbar, BUTTON_ID_DOWN, s_action_down);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_actionbar);

}  // end initialize_ui


static void destroy_ui(Window *window) {
  text_layer_destroy(s_unitname_a);
  text_layer_destroy(s_unitname_b);
  text_layer_destroy(s_unit_a);
  text_layer_destroy(s_unit_b);
  action_bar_layer_destroy(s_actionbar);
}

void toggle_selected_unit() {
  if (selected_unit == true) {
    text_layer_set_background_color(s_highlight_a, GColorClear);
    text_layer_set_text_color(s_unitname_a, GColorBlack);
    text_layer_set_text_color(s_unit_a, GColorBlack);
    text_layer_set_background_color(s_highlight_b, PBL_IF_COLOR_ELSE(GColorBlueMoon, GColorBlack));
    text_layer_set_text_color(s_unitname_b, GColorWhite);
    text_layer_set_text_color(s_unit_b, GColorWhite);
    selected_unit = false;
  } else {
    text_layer_set_background_color(s_highlight_b, GColorClear);
    text_layer_set_text_color(s_unitname_b, GColorBlack);
    text_layer_set_text_color(s_unit_b, GColorBlack);
    text_layer_set_background_color(s_highlight_a, PBL_IF_COLOR_ELSE(GColorBlueMoon, GColorBlack));
    text_layer_set_text_color(s_unitname_a, GColorWhite);
    text_layer_set_text_color(s_unit_a, GColorWhite);
    selected_unit = true;
  }
}

void update_quantity_layers() {
  text_layer_set_text(s_unit_a, a_quantity_buffer);
  text_layer_set_text(s_unit_b, b_quantity_buffer);
}

void update_label_layers() {
  text_layer_set_text(s_unitname_a, a_label_buffer);
  text_layer_set_text(s_unitname_b, b_label_buffer);
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, (ClickHandler) toggle_selected_unit);
  window_multi_click_subscribe(BUTTON_ID_SELECT, 2, 0, 0, true, (ClickHandler) init_selected_unit_pair);
  window_long_click_subscribe(BUTTON_ID_SELECT, 500, (ClickHandler) menu_init, NULL);
  window_single_repeating_click_subscribe(BUTTON_ID_UP, 100, (ClickHandler) increment_unit);
  window_single_repeating_click_subscribe(BUTTON_ID_DOWN, 100, (ClickHandler) decrement_unit);
}

static void init(void) {
  initialise_ui();
  action_bar_layer_set_click_config_provider(s_actionbar, click_config_provider);
  toggle_selected_unit();
  init_unit_pairs();
  window_stack_push(s_window, true);
}

static void deinit(void) {
  destroy_ui(s_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
