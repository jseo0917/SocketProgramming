// #include <stddef.h>
#include "layer2.h"
#include "../graph.h"
#include "../net.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>

void layer2_frame_recv(node_t *node, interface_t *interface, char *pkt, unsigned int pkt_size)
{
}

// Should be called when a node is created during topology creation
// init_node_nw_prop()
void init_arp_table(arp_table_t **arp_table)
{
    *arp_table = calloc(1, sizeof(arp_table_t));
    init_glthread(&((*arp_table)->arp_entries));
}

arp_entry_t *arp_table_lookup(arp_table_t *arp_table, char *ip_addr)
{
    glthread_t *crr;
    arp_entry_t *arp_entry;

    crr = &arp_table->arp_entries;

    while (crr)
    {
        char *base = (char *)crr;

        arp_entry = (arp_entry_t *)(base - (sizeof(arp_entry_t) - sizeof(glthread_t)));

        if (arp_entry == NULL)
            break;

        if (strncmp(arp_entry->ip_addr.ip_addr, ip_addr, 16) == 0)
        {
            return arp_entry;
        }

        crr = crr->right;
    }

    return NULL;
}

void delete_arp_table_entry(arp_table_t *arp_table, char *ip_addr)
{
}

bool arp_table_entry_add(arp_table_t *arp_table, arp_entry_t *arp_entry)
{
    arp_entry_t *get_entry_table = arp_table_lookup(arp_table, arp_entry->ip_addr.ip_addr);

    if (get_entry_table && memcmp(get_entry_table, arp_entry, sizeof(arp_entry_t)) == 0)
        return false;

    if (get_entry_table)
    {
        delete_arp_table_entry(arp_table, arp_entry->ip_addr.ip_addr);
    }

    init_glthread(&arp_entry->arp_glue);

    glthread_add_right(&arp_table->arp_entries, &arp_entry->arp_glue);

    return true;
}

void arp_table_update_from_arp_reply(arp_table_t *arp_table, arp_hdr_t *arp_hdr, interface_t *iif)
{
    unsigned int src_ip = 0;
    assert(arp_hdr->op_code == ARP_REPLY);
    arp_entry_t *arp_entry = calloc(1, sizeof(arp_entry_t));
    src_ip = htonl(arp_hdr->src_ip);
    inet_ntop(AF_INET, &src_ip, &arp_entry->ip_addr.ip_addr, 16);
    arp_entry->ip_addr.ip_addr[15] = '\0';
    memcpy(arp_entry->mac_addr.mac_addr, arp_hdr->src_mac.mac_addr, sizeof(mac_address_t));
    strncpy(arp_entry->oif_name, iif->if_name, IF_NAME_SIZE);

    bool rc = arp_table_entry_add(arp_table, arp_entry);

    if (rc == false)
    {
        free(arp_entry);
    }
}

void dump_arp_table(arp_table_t *arp_table)
{
    glthread_t *crr;
    arp_entry_t *arp_entry;

    crr = &arp_table->arp_entries;

    while (crr)
    {
        char *base = (char *)crr;

        arp_entry = (arp_entry_t *)(base - (sizeof(arp_entry_t) - sizeof(glthread_t)));

        if (arp_entry == NULL)
            break;

        printf("IP: %s, MAC : %u:%u:%u:%u:%u:%u, OIF = %s\n",
               arp_entry->ip_addr.ip_addr,
               arp_entry->mac_addr.mac_addr[0],
               arp_entry->mac_addr.mac_addr[1],
               arp_entry->mac_addr.mac_addr[2],
               arp_entry->mac_addr.mac_addr[3],
               arp_entry->mac_addr.mac_addr[4],
               arp_entry->mac_addr.mac_addr[5],
               arp_entry->oif_name);

        crr = crr->right;
    }

    return;
}

void send_arp_broadcast_request(node_t *node, interface_t *oif, char *ip_addr)
{
}