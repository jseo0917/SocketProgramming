#ifndef __GRAPH__
#define __GRAPH__

#include "glthread/glthread.h"
#include "net.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>

#define IF_NAME_SIZE 16
#define NODE_NAME_SIZE 16
#define MAX_INTF_PER_NODE 10

typedef struct node_ node_t;
typedef struct link_ link_t;

typedef struct interface_
{
  char if_name[IF_NAME_SIZE];
  struct node_ *att_node;
  struct link_ *link;
  intf_nw_props_t intf_nw_props;
} interface_t;

typedef struct link_
{
  interface_t intf1;
  interface_t intf2;
  unsigned int cost;
} link_t;

typedef struct node_
{
  char node_name[NODE_NAME_SIZE];
  interface_t *intf[MAX_INTF_PER_NODE];
  node_nw_prop_t node_nw_prop;
  unsigned int udp_port_number;
  int udp_sock_fd;
  glthread_t graph_glue;
} node_t;

typedef struct graph_
{
  char topology_name[32];
  glthread_t node_list;
} graph_t;

graph_t *create_new_graph(char *topology_name);

node_t *create_graph_node(graph_t *graph, char *node_name);

void insert_link_between_two_nodes(node_t *node1, node_t *node2, char *from_if_name, char *to_if_name, unsigned int cost);

void dump_graph(graph_t *topo);

void dump_node(node_t *node);

void dump_interface(interface_t *infc);

static inline interface_t *get_node_if_by_name(node_t *node, char *if_name)
{
  node_t *crr = node;

  while (crr)
  {
    for (int i = 0; i < MAX_INTF_PER_NODE; i++)
    {
      if (strcmp(crr->intf[i]->if_name, if_name) == 0)
      {
        return crr->intf[i];
      }
    }
  }

  return NULL;
}

static inline int get_node_intf_available_slot(node_t *node)
{
  for (int i = 0; i < MAX_INTF_PER_NODE; i++)
  {
    if (node->intf[i] != 0)
      continue;
    return i;
  }
  return -1;
}

static inline node_t *get_node_by_node_name(graph_t *topo, char *node_name)
{
  glthread_t *glNode = topo->node_list.right;

  while (glNode)
  {
    char *base = (char *)glNode;
    node_t *node = (node_t *)(base - (sizeof(node_t) - sizeof(glthread_t)));
    if (strcmp(node->node_name, node_name) == 0)
      return node;
    glNode = glNode->right;
  }

  return NULL;
}

static inline node_t *get_nbr_node(interface_t *interface)
{
  assert(interface->att_node);
  assert(interface->link);

  link_t *link = interface->link;
  if (&link->intf1 == interface)
    return link->intf2.att_node;
  else
    return link->intf1.att_node;
}

#endif
