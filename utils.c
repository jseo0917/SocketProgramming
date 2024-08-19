#include "utils.h"
#include <sys/socket.h>
#include <arpa/inet.h> /*for inet_ntop & inet_pton*/
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

void apply_mask(char *prefix, char mask, char *str_prefix)
{
    uint32_t ip_addr = 0;
    uint32_t subnetMask = 0XFFFFFFFF;

    if (mask == 32)
    {
        strncpy(str_prefix, prefix, 16);
        str_prefix[15] = '\0';
        return;
    }

    inet_pton(AF_INET, prefix, &ip_addr);
    ip_addr = htonl(ip_addr);

    subnetMask = subnetMask << (32 - mask);
    ip_addr = ip_addr & subnetMask;

    ip_addr = htonl(ip_addr);
    inet_ntop(AF_INET, &ip_addr, str_prefix, 16);
    str_prefix[15] = '\0';
}

void layer2_fill_with_broadcast_mac(unsigned char *mac_array)
{
    mac_array[0] = 0XFF;
    mac_array[1] = 0XFF;
    mac_array[2] = 0XFF;
    mac_array[3] = 0XFF;
    mac_array[4] = 0XFF;
    mac_array[5] = 0XFF;
}

bool is_mac_broadcast_addr(unsigned char *mac_addr)
{
    unsigned char *broadcast_mac;
    memset(broadcast_mac, 0, 6);
    layer2_fill_with_broadcast_mac(broadcast_mac);

    if (strncmp(mac_addr, broadcast_mac, 6) == 0)
    {
        return true;
    }

    return false;
}