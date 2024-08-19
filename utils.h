#ifndef __UTILS__
#define __UTILS__

#include <stdint.h>
#include <stdbool.h>

void apply_mask(char *prefix, char mask, char *str_prefix);

bool is_mac_broadcast_addr(unsigned char *mac_addr);

void layer2_fill_with_broadcast_mac(unsigned char *mac_array);

#endif