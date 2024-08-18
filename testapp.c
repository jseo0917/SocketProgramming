#include "graph.h"

extern graph_t *build_first_topo();

int main(int arg, char **argv){
  graph_t *topo = build_first_topo();
  dump_graph(topo);

  // printf("TOPO NAME: %s\n", topo->topology_name);
  // glthread_t* glNode = topo->node_list.right;
  // char* base = (char*) glNode;
  // node_t* node = (node_t*) (base - (sizeof(node_t) - sizeof(glthread_t)));
  // printf("Node Name: %s\n", node->node_name);
  //
  // get_node_by_node_name(topo, "R0_re");

  return 0;
}
