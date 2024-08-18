#include "graph.h"
#include <memory.h>
#include <stdio.h>
#include <stdbool.h>
static unsigned int hash_code(void *ptr, unsigned int size) {
  unsigned int value=0, i = 0;
  char* str = (char*)ptr;

  while(i < size) {
    value += *str;
    value *=97;
    str++;
    i++;
  }
  return value;
}

void interface_assign_mac_address(interface_t *interface) {

  node_t *node = interface->att_node;

  if(!node)
    return;

  unsigned int hash_code_val = 0;
  hash_code_val = hash_code(node->node_name, NODE_NAME_SIZE);
  hash_code_val *= hash_code(interface->if_name, IF_NAME_SIZE);
  memset(interface->intf_nw_props.mac_addr.mac_addr, 0, sizeof(interface->intf_nw_props.mac_addr.mac_addr));
  memcpy(interface->intf_nw_props.mac_addr.mac_addr, (char *)&hash_code_val, sizeof(unsigned int));
}

bool node_set_loopback_address(node_t *node, char *ip_addr) {
  printf("[node_set_loopback_address] setting a loop back address for %s\n", node->node_name);

  assert(ip_addr);

  node->node_nw_prop.is_lb_addr_config = true;
  strncpy(node->node_nw_prop.lb_addr.ip_addr, ip_addr, 16);
  node->node_nw_prop.lb_addr.ip_addr[15] = '\0';

  printf("[node_set_loopback_address] Finished setting the loop back address.\n", node->node_name);
  return true;
}

bool node_set_intf_ip_address(node_t *node, char *local_if, char *ip_addr, char mask) {
  printf("[node_set_intf_ip_address] Setting up the intf_ip_address, %s\n", local_if );
  interface_t *interface = get_node_if_by_name(node, local_if);
  if(!interface) {
    assert("Interface not found\n");
  }
  else
    printf("[node_set_intf_ip_address] Found the interface: %s\n", interface->if_name);

  strncpy(interface->intf_nw_props.ip_addr.ip_addr, ip_addr, 16);
  interface->intf_nw_props.ip_addr.ip_addr[15] = '\0';
  interface->intf_nw_props.mask = mask;
  interface->intf_nw_props.is_ipadd_config =true;
  printf("[node_set_intf_ip_address] Finished Setting up the intf_ip_address\n", local_if );
  return true;
}

bool node_unset_intf_ip_address(node_t *node, char *local_if) {
  return true;
}
