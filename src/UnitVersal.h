#pragma once
#include "pairs.h"

void update_quantity_layers();
void update_label_layers();
void toggle_selected_unit();

extern float a_quantity;
extern float b_quantity;
extern char a_quantity_buffer[];
extern char b_quantity_buffer[];
extern char a_label_buffer[];
extern char b_label_buffer[];
extern bool selected_unit;

