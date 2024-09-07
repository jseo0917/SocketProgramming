#include "graph.h"
#include "utils.h"
#include <unistd.h>
#include "communication.h"
#include "CommandParser/libcli.h"
/*
 * PROJECT DESCRIPTION
 * This project demonstrates the
 */

extern graph_t *build_first_topo();
extern graph_t *build_linear_topo();

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

extern graph_t *build_first_topo();
extern graph_t *build_linear_topo();
extern void nw_init_cli();
graph_t *topo = NULL;

int main(int arg, char **argv)
{

  nw_init_cli();
  topo = build_linear_topo();
  start_shell();

  return 0;
  // show_help_handler(0, 0, MODE_UNKNOWN);
  // topo = build_first_topo();
  // start_shell();

  // graph_t *topo = build_first_topo();

  // dump_graph(topo);

  // // utils_test();

  // sleep(3);

  // node_t *snode = get_node_by_node_name(topo, "H3");

  // interface_t *oif = get_node_if_by_name(snode, "eth0/4");

  // char msg[] = "hello, how are you\0";

  // send_pkt_out(msg, strlen(msg), oif);
  // send_pkt_flood(snode, NULL, msg, strlen(msg));

  sleep(5);
  return 0;
}
