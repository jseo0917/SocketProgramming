#ifndef __NET__
#define __NET__

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "graph.h"

typedef struct node_ node_t;
typedef struct interface_ interface_t;
typedef struct ip_address_
{
  char ip_addr[16];
} ip_address_t;

typedef struct mac_addres_
{
  unsigned char mac_addr[6];
} mac_address_t;

/*Defining L3 Properties*/
typedef struct node_nw_prop_
{
  bool is_lb_addr_config;
  ip_address_t lb_addr; // Loopback addr
} node_nw_prop_t;

typedef struct intf_nw_props_
{
  mac_address_t mac_addr;
  ip_address_t ip_addr;
  bool is_ipadd_config;
  char mask;
} intf_nw_props_t;

static inline void init_node_nw_prop(node_nw_prop_t *node_nw_prop)
{
  node_nw_prop->is_lb_addr_config = false;
  memset(node_nw_prop->lb_addr.ip_addr, 0, 16);
}

static inline void init_intf_nw_prop(intf_nw_props_t *intf_nw_props)
{
  memset(intf_nw_props->mac_addr.mac_addr, 0, 6);
  memset(intf_nw_props->ip_addr.ip_addr, 0, 16);
  intf_nw_props->is_ipadd_config = false;
}

#define IF_MAC(intf_ptr) ((intf_ptr)->inif_nw_props.mac_addr.mac_addr);
#define IF_IP(intf_ptr) ((intf_ptr)->intf_nw_props.ip_addr.ip_addr);
#define NODE_LO_ADDR(node_ptr) (node_ptr->node_nw_prop.lb_addr.ip_addr);

bool node_set_loopback_address(node_t *node, char *ip_addr);
bool node_set_intf_ip_address(node_t *node, char *local_if, char *ip_addr, char mask);
void interface_assign_mac_address(interface_t *interface);
interface_t *node_get_matching_subnet_interface(node_t *node, char *ip_addr);
unsigned int convert_ip_from_str_to_int(char *ip_addr);
void convert_ip_from_int_to_str(unsigned int ip_addr, char *output_buffer);
#endif
