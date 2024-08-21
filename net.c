#include "graph.h"
#include <memory.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void interface_assign_mac_address(interface_t *interface)
{

  for (int i = 0; i < 6; i++)
  {
    int r = rand() % 256;
    interface->intf_nw_props.mac_addr.mac_addr[i] = r;
  }
}

bool node_set_loopback_address(node_t *node, char *ip_addr)
{
  printf("[node_set_loopback_address] setting a loop back address for %s\n", node->node_name);

  assert(ip_addr);

  node->node_nw_prop.is_lb_addr_config = true;
  strncpy(node->node_nw_prop.lb_addr.ip_addr, ip_addr, 16);
  node->node_nw_prop.lb_addr.ip_addr[15] = '\0';

  printf("[node_set_loopback_address] Finished setting the loop back address.\n", node->node_name);
  return true;
}

bool node_set_intf_ip_address(node_t *node, char *local_if, char *ip_addr, char mask)
{
  printf("[node_set_intf_ip_address] Setting up the intf_ip_address, %s\n", local_if);
  interface_t *interface = get_node_if_by_name(node, local_if);
  if (!interface)
  {
    assert("Interface not found\n");
  }
  else
    printf("[node_set_intf_ip_address] Found the interface: %s\n", interface->if_name);

  strncpy(interface->intf_nw_props.ip_addr.ip_addr, ip_addr, 16);
  interface->intf_nw_props.ip_addr.ip_addr[15] = '\0';
  interface->intf_nw_props.mask = mask;
  interface->intf_nw_props.is_ipadd_config = true;
  printf("[node_set_intf_ip_address] Finished Setting up the intf_ip_address\n", local_if);
  return true;
}

interface_t *node_get_matching_subnet_interface(node_t *node, char *ip_addr)
{
  for (int i = 0; i < MAX_INTF_PER_NODE; i++)
  {
    interface_t *inf = node->intf[i];

    if (!inf)
    {
      break;
    }
    else
    {
      uint32_t node_ip_masked = 0;
      inet_pton(AF_INET, inf->intf_nw_props.ip_addr.ip_addr, &node_ip_masked);
      node_ip_masked = htonl(node_ip_masked);
      node_ip_masked = node_ip_masked >> (32 - inf->intf_nw_props.mask);

      uint32_t ip_masked = 0;
      inet_pton(AF_INET, ip_addr, &ip_masked);
      ip_masked = htonl(ip_masked);
      ip_masked = ip_masked >> (32 - inf->intf_nw_props.mask);

      if (node_ip_masked == ip_masked)
      {
        return inf;
      }
    }
  }
  return NULL;
}

unsigned int convert_ip_from_str_to_int(char *ip_addr)
{
  unsigned ip_addr_to_uint = 0;

  inet_pton(AF_INET, ip_addr, &ip_addr_to_uint);

  return htonl(ip_addr_to_uint);
}

void convert_ip_from_int_to_str(unsigned int ip_addr, char *output_buffer)
{
  ip_addr = htonl(ip_addr);
  inet_ntop(AF_INET, &ip_addr, output_buffer, 16);
  return;
}