#include "graph.h"
#include "utils.h"
/*
 * PROJECT DESCRIPTION
 * This project demonstrates the
 */

extern graph_t *build_first_topo();

void utils_test()
{
  char str_prefix[16];

  apply_mask("122.1.1.1", (char)24, str_prefix);

  if (strncmp("122.1.1.0", str_prefix, 16) != 0)
  {
    printf("[utils_test] TEST FAILED - 1");
    return;
  }

  unsigned char mac_addr[6] = {0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF};
  if (!is_mac_broadcast_addr(mac_addr))
  {
    printf("[utils_test] TEST FAILED - 2");
    return;
  }
}

int main(int arg, char **argv)
{
  graph_t *topo = build_first_topo();

  dump_graph(topo);

  utils_test();

  return 0;
}
