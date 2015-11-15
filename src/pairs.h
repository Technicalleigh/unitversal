#pragma once

typedef struct {
  char unit_a_label[6];
  char unit_b_label[6];
  char pair_name[15];
  float unit_a_default_value;
  float unit_a_in_b_factor;
  short a_step;
  short b_step;
} UnitPair;

// TODO Don't define array length in two places. In the future, we won't
// know its size at compile-time.
extern UnitPair unit_pair_array[8];
extern UnitPair *selected_unit_pair;

void init_selected_unit_pair();
void init_unit_pairs();

