#include <pebble.h>
#include "UnitVersal.h"
#include "convert.h"
#include "pairs.h"

// TODO (future) Allow users to configure (on phone) just the unit pairs they
// care about, plus custom units. We only need to read that config during
// init, so we can count the unit pairs during init_unit_pairs() and
// create an appropriately sized array.

UnitPair *selected_unit_pair = NULL;

// TODO Don't define array length in two places. In the future, we won't
// know its size at compile-time.
UnitPair unit_pair_array[8];

void init_selected_unit_pair() {
  a_quantity = selected_unit_pair->unit_a_default_value;
  convert_a_to_b();
  strcpy(a_label_buffer, selected_unit_pair->unit_a_label);
  strcpy(b_label_buffer, selected_unit_pair->unit_b_label);
  update_quantity_layers();
  update_label_layers();
}


void init_unit_pairs() {
  // TODO (future) Read the UnitPair configs into the array from Pebble's
  // persistant storage? (UnitPair configs are hard-coded in this version.)

  strncpy(unit_pair_array[0].unit_a_label, "°C", 5);
  strncpy(unit_pair_array[0].unit_b_label, "°F", 5);
  strncpy(unit_pair_array[0].pair_name, "°C <-> °F", 15);
  unit_pair_array[0].unit_a_default_value = 20.0;
  unit_pair_array[0].unit_a_in_b_factor = 0.0; // 0 is sentinel for special case
  unit_pair_array[0].a_step = 1;
  unit_pair_array[0].b_step = 1;

  strncpy(unit_pair_array[1].unit_a_label, "km", 5);
  strncpy(unit_pair_array[1].unit_b_label, "mi", 5);
  strncpy(unit_pair_array[1].pair_name, "km <-> mi", 15);
  unit_pair_array[1].unit_a_default_value = 25.0;
  unit_pair_array[1].unit_a_in_b_factor = 0.621371;
  unit_pair_array[1].a_step = 1;
  unit_pair_array[1].b_step = 1;

  strncpy(unit_pair_array[2].unit_a_label, "m", 5);
  strncpy(unit_pair_array[2].unit_b_label, "ft", 5);
  strncpy(unit_pair_array[2].pair_name, "m <-> ft", 15);
  unit_pair_array[2].unit_a_default_value = 25.0;
  unit_pair_array[2].unit_a_in_b_factor = 3.28084;
  unit_pair_array[2].a_step = 1;
  unit_pair_array[2].b_step = 1;

  strncpy(unit_pair_array[3].unit_a_label, "cm", 5);
  strncpy(unit_pair_array[3].unit_b_label, "in", 5);
  strncpy(unit_pair_array[3].pair_name, "c <-> in", 15);
  unit_pair_array[3].unit_a_default_value = 30.0;
  unit_pair_array[3].unit_a_in_b_factor = 0.393701;
  unit_pair_array[3].a_step = 1;
  unit_pair_array[3].b_step = 1;


  strncpy(unit_pair_array[4].unit_a_label, "kg", 5);
  strncpy(unit_pair_array[4].unit_b_label, "lb", 5);
  strncpy(unit_pair_array[4].pair_name, "kg <-> lb", 15);
  unit_pair_array[4].unit_a_default_value = 45.0;
  unit_pair_array[4].unit_a_in_b_factor = 2.20462;
  unit_pair_array[4].a_step = 1;
  unit_pair_array[4].b_step = 1;

  strncpy(unit_pair_array[5].unit_a_label, "g", 5);
  strncpy(unit_pair_array[5].unit_b_label, "oz", 5);
  strncpy(unit_pair_array[5].pair_name, "g <-> oz", 15);
  unit_pair_array[5].unit_a_default_value = 90.0;
  unit_pair_array[5].unit_a_in_b_factor = 0.035274;
  unit_pair_array[5].a_step = 5;
  unit_pair_array[5].b_step = 1;

  strncpy(unit_pair_array[6].unit_a_label, "L", 5);
  strncpy(unit_pair_array[6].unit_b_label, "gal", 5);
  strncpy(unit_pair_array[6].pair_name, "L <-> gal", 15);
  unit_pair_array[6].unit_a_default_value = 4.0;
  unit_pair_array[6].unit_a_in_b_factor = 0.264172;
  unit_pair_array[6].a_step = 1;
  unit_pair_array[6].b_step = 1;

  strncpy(unit_pair_array[7].unit_a_label, "mL", 5);
  strncpy(unit_pair_array[7].unit_b_label, "fl oz", 5);
  strncpy(unit_pair_array[7].pair_name, "mL <-> fl oz", 15);
  unit_pair_array[7].unit_a_default_value = 30.0;
  unit_pair_array[7].unit_a_in_b_factor = 0.033814;
  unit_pair_array[7].a_step = 5;
  unit_pair_array[7].b_step = 1;


  // TODO (future) set default Unit Pair according to user preference
  selected_unit_pair = &unit_pair_array[0];
  init_selected_unit_pair();

}

