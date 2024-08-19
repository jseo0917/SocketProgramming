#include "graph.h"
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>

void insert_link_between_two_nodes(node_t *node1, node_t *node2, char *from_if_name, char *to_if_name, unsigned int cost)
{
  link_t *link = calloc(1, sizeof(link_t));

  strncpy(link->intf1.if_name, from_if_name, IF_NAME_SIZE);
  link->intf1.if_name[IF_NAME_SIZE - 1] = '\0';

  strncpy(link->intf2.if_name, to_if_name, IF_NAME_SIZE);
  link->intf2.if_name[IF_NAME_SIZE - 1] = '\0';

  link->intf1.link = link;
  link->intf2.link = link;

  link->intf1.att_node = node1;
  link->intf2.att_node = node2;
  link->cost = cost;

  int empty_intf_slot;

  empty_intf_slot = get_node_intf_available_slot(node1);

  printf("Available slot for node %s: %d\n", node1->node_name, empty_intf_slot);

  node1->intf[empty_intf_slot] = &link->intf1;

  empty_intf_slot = get_node_intf_available_slot(node2);

  printf("Available slot for node %s: %d\n", node2->node_name, empty_intf_slot);
  node2->intf[empty_intf_slot] = &link->intf2;

  init_intf_nw_prop(&link->intf1.intf_nw_props);
  init_intf_nw_prop(&link->intf2.intf_nw_props);

  interface_assign_mac_address(&link->intf1);
  interface_assign_mac_address(&link->intf2);
}

graph_t *create_new_graph(char *topology_name)
{

  graph_t *graph = calloc(1, sizeof(graph_t));
  strncpy(graph->topology_name, topology_name, 32);
  graph->topology_name[31] = '\0';

  init_glthread(&graph->node_list);
  return graph;
}

node_t *create_graph_node(graph_t *graph, char *node_name)
{
  printf("[create_graph_node] generating node for %s\n", node_name);

  node_t *node = calloc(1, sizeof(node_t));
  strncpy(node->node_name, node_name, NODE_NAME_SIZE);
  node->node_name[NODE_NAME_SIZE - 1] = '\0';

  init_node_nw_prop(&node->node_nw_prop);
  init_glthread(&node->graph_glue);
  glthread_add_right(&graph->node_list, &node->graph_glue);

  printf("[create_graph_node] successfully generated the node,\n");
  return node;
}

node_t *getNode(glthread_t *glNode)
{
  char *base = (char *)glNode;
  return (node_t *)(base - (sizeof(node_t) - sizeof(glthread_t)));
}

void dump_graph(graph_t *topo)
{
  printf("Topology NAME = %s\n", topo->topology_name);

  glthread_t *base_glthread = topo->node_list.right;

  while (base_glthread)
  {
    char *base = (char *)base_glthread;

    node_t *node = (node_t *)(base - (sizeof(node_t) - sizeof(glthread_t)));

    dump_node(node);

    base_glthread = base_glthread->right;
  }
}

void dump_node(node_t *node)
{
  printf("Node Name = %s\n", node->node_name);
  printf("\tlo addr : %s\n", node->node_nw_prop.lb_addr.ip_addr);
  for (int i = 0; i < MAX_INTF_PER_NODE; i++)
  {
    if (!node->intf[i])
      break;
    else
      dump_interface(node->intf[i]);
  }
}

void dump_interface(interface_t *infc)
{
  printf("Interface Name = %s\n", infc->if_name);

  node_t *nbrNode = get_nbr_node(infc);
  printf("\tNbr Node: %s Local Node : %s, cost = %d\n", infc->att_node, nbrNode->node_name, infc->link->cost);
  printf("\tIP ADDr = %s/%d ", infc->intf_nw_props.ip_addr.ip_addr, infc->intf_nw_props.mask);
  printf("\tMAC = %u:%u:%u:%u:%u:%u ", infc->intf_nw_props.mac_addr.mac_addr[0],
         infc->intf_nw_props.mac_addr.mac_addr[1],
         infc->intf_nw_props.mac_addr.mac_addr[2],
         infc->intf_nw_props.mac_addr.mac_addr[3],
         infc->intf_nw_props.mac_addr.mac_addr[4],
         infc->intf_nw_props.mac_addr.mac_addr[5]);
}
