#include <pebble.h>
#include "UnitVersal.h"
#include "pairs.h"

static Window *s_menu_window;
static MenuLayer *s_menu_layer;


static uint16_t get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *context) {
  return (sizeof(unit_pair_array) / sizeof(UnitPair));
}

static void draw_row_callback(GContext *ctx, Layer *cell_layer, MenuIndex *cell_index, void *context) {
  menu_cell_basic_draw(ctx, cell_layer, unit_pair_array[cell_index->row].pair_name, NULL, NULL);
}

static void select_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *context) {
  selected_unit_pair = &unit_pair_array[cell_index->row];
  init_selected_unit_pair();
  window_stack_pop(true);
}

static void menu_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_menu_layer = menu_layer_create(bounds);
  menu_layer_set_click_config_onto_window(s_menu_layer, window);
  #ifdef PBL_SDK_3
    menu_layer_set_normal_colors(s_menu_layer, PBL_IF_COLOR_ELSE(GColorMintGreen, GColorWhite), GColorBlack);
    menu_layer_set_highlight_colors(s_menu_layer, PBL_IF_COLOR_ELSE(GColorBlueMoon, GColorBlack), GColorWhite);
  #endif
  menu_layer_set_callbacks(s_menu_layer, NULL, (MenuLayerCallbacks) {
      .get_num_rows = (MenuLayerGetNumberOfRowsInSectionsCallback)get_num_rows_callback,
      .draw_row = (MenuLayerDrawRowCallback)draw_row_callback,
      .select_click = (MenuLayerSelectCallback)select_callback,
  });
  layer_add_child(window_layer, menu_layer_get_layer(s_menu_layer));
}

void menu_window_unload(Window *window) {
  menu_layer_destroy(s_menu_layer);
}

void menu_init() {
  s_menu_window = window_create();
  window_set_window_handlers(s_menu_window, (WindowHandlers) {
    .load = menu_window_load,
    .unload = menu_window_unload,
  });
  window_stack_push(s_menu_window, true);
}

