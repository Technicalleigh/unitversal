#include <pebble.h>
#include "UnitVersal.h"
#include "convert.h"

const float ABS_ZERO_C = -273.15;
const float ABS_ZERO_F = -459.67;

void update_quantity_buffers() {
  int a_decimal = get_one_decimal(a_quantity);
  int b_decimal = get_one_decimal(b_quantity);

  if ((int)a_quantity == 0 && a_quantity < 0 && a_decimal != 0)  // corner case: -0.x needs the negative sign added manually
    snprintf(a_quantity_buffer, 7, "-%d.%d", (int)a_quantity, a_decimal);
  else 
    snprintf(a_quantity_buffer, 7, "%d.%d", (int)a_quantity, a_decimal);
  
  if ((int)b_quantity == 0 && b_quantity < 0 && b_decimal != 0)  // corner case: -0.x needs the negative sign added manually
    snprintf(b_quantity_buffer, 7, "-%d.%d", (int)b_quantity, b_decimal);
  else
    snprintf(b_quantity_buffer, 7, "%d.%d", (int)b_quantity, b_decimal);

  update_quantity_layers();
}

void set_abs_zero() {
  a_quantity = ABS_ZERO_C;
  b_quantity = ABS_ZERO_F;
}

void convert_a_to_b() {
  if (selected_unit_pair->unit_a_in_b_factor != 0.0)
    b_quantity = a_quantity * selected_unit_pair->unit_a_in_b_factor;
  else {
    // sentinel found, we're dealing with a temperature scale
    // TODO (future) allow for C-K and K-F conversions, too
    if (a_quantity > -273.15)
      b_quantity = (a_quantity * 1.8) + 32.0;
    else
      set_abs_zero();
  }
  update_quantity_buffers();
}

void convert_b_to_a() {
  if (selected_unit_pair->unit_a_in_b_factor != 0.0) 
    a_quantity = b_quantity / selected_unit_pair->unit_a_in_b_factor;
  else {
    // sentinel found, we're dealing with a temperature scale
    // TODO (future) allow for C-K and K-F conversions, too
    if (b_quantity > ABS_ZERO_F)
      a_quantity = (b_quantity - 32.0) * (5.0/9.0);
    else
      set_abs_zero();     
  }
  update_quantity_buffers();
}

void increment_unit() {
  if (selected_unit == true) {
    a_quantity = (float)((int)a_quantity + (int)selected_unit_pair->a_step);
    convert_a_to_b();
  } else {
      b_quantity = (float)((int)b_quantity + (int)selected_unit_pair->b_step);
      convert_b_to_a();
  }
  update_quantity_layers();
}

void decrement_unit() {
  if (selected_unit == true) {
    // do we need to drop a decimal? e.g. 2.41 decrement to 2.0, not 1.0?
    // but be careful not to drop e.g. 2.04 to 2.0 -- that should go to 1.0
    if ((int)(10.0 * a_quantity) > (int)(10 * (int)(a_quantity)))
      a_quantity = (float)((int)a_quantity);
    else
      a_quantity = (float)((int)a_quantity - (int)selected_unit_pair->a_step);
    convert_a_to_b();
  } else {  // selected_unit == false
      if ((int)(10.0 * b_quantity) > (int)(10 * (int)(b_quantity)))
        b_quantity = (float)((int)b_quantity);
      else
        b_quantity = (float)((int)b_quantity - (int)selected_unit_pair->b_step);
      convert_b_to_a();
  }
  update_quantity_layers();
}

int get_one_decimal(float tmp_quantity) {
  // truncate
  int decimal;
  tmp_quantity = tmp_quantity * 10.0;
  decimal = (int)tmp_quantity % 10; // get first sig digit
  if (decimal < 0)
    decimal = decimal * (-1);
  
  // round up if necessary
  tmp_quantity = tmp_quantity * 10.0;  // second time *10'ing, second sig digit is now in the whole part
  if ((int)tmp_quantity % 10 >= 5)
    decimal++;
  
  return decimal;
}

