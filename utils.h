#ifndef __UTILS__
#define __UTILS__

#include <stdint.h>
#include <stdbool.h>

#define IS_MAC_BROADCAST_ADDR(mac) \
    (mac[0] == 0XFF && mac[1] == 0XFF && mac[2] == 0XFF &\     
     mac[3] == 0XFF &&             \
     mac[4] == 0XFF && mac[5] == 0XFF)

void apply_mask(char *prefix, char mask, char *str_prefix);

bool is_mac_broadcast_addr(unsigned char *mac_addr);

void layer2_fill_with_broadcast_mac(unsigned char *mac_array);

#endif