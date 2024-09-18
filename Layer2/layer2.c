// #include <stddef.h>
#include "layer2.h"
#include "../graph.h"
#include "../net.h"
#include "../communication.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>

void layer2_frame_recv(node_t *node, interface_t *interface, char *pkt, unsigned int pkt_size)
{
    printf("here");
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
/* PART FOR ARP HANDLER*/
void send_arp_broadcast_request(node_t *node, interface_t *oif, char *ip_addr)
{
    /*Initialize the Ethernet header*/
    ethernet_hdr_t *new_ethernet_hdr = (ethernet_hdr_t *)calloc(1, ETH_HDR_SIZE_EXCL_PAYLOAD + sizeof(arp_hdr_t));

    if (!oif)
    {
        oif = node_get_matching_subnet_interface(node, ip_addr);
        if (!oif)
        {
            printf("Error: %s : No eligible subnet for ARP Resolution regarding the IP-ADDR: %s", node->node_name, ip_addr);
            return;
        }
    }

    /*setting up the ethernet header*/
    layer2_fill_with_broadcast_mac(new_ethernet_hdr->dst_mac_addr.mac_addr);
    memcpy(new_ethernet_hdr->src_mac_addr.mac_addr, IF_MAC(oif), sizeof(mac_address_t));
    new_ethernet_hdr->type = ARP_MSG;

    /*setting up the arp broad cast msg in ethernet header*/
    arp_hdr_t *arp_hdr = (arp_hdr_t *)new_ethernet_hdr->payload;
    arp_hdr->hw_type = 1;
    arp_hdr->proto_type = 0x0800;
    arp_hdr->hw_addr_len = sizeof(mac_address_t);
    arp_hdr->proto_addr_len = 4;

    arp_hdr->op_code = ARP_BROAD_REQ;

    memcpy(arp_hdr->src_mac.mac_addr, IF_MAC(oif), sizeof(mac_address_t));

    inet_pton(AF_INET, IF_IP(oif), &arp_hdr->src_ip);
    arp_hdr->src_ip = htonl(arp_hdr->src_ip);

    memset(arp_hdr->dst_mac.mac_addr, 0, sizeof(mac_address_t));

    inet_pton(AF_INET, ip_addr, &arp_hdr->dst_ip);
    arp_hdr->dst_ip = htonl(arp_hdr->dst_ip);

    ETH_FCS(new_ethernet_hdr, sizeof(arp_hdr_t)) = 0;

    send_pkt_out((char *)new_ethernet_hdr, ETH_HDR_SIZE_EXCL_PAYLOAD + sizeof(arp_hdr_t), oif);

    free(new_ethernet_hdr);
}

static void send_arp_reply_msg(ethernet_hdr_t *ethernet_hdr_in, interface_t *oif)
{
    arp_entry_t *arp_entry = ethernet_hdr_in->payload;

    /*Generating a new ethernet header for replying*/
    ethernet_hdr_t *ethernet_hdr_out = (ethernet_hdr_t *)calloc(1, MAX_PACKET_BUFFER_SIZE);

    memcpy(ethernet_hdr_out->dst_mac_addr.mac_addr, arp_entry->mac_addr.mac_addr, sizeof(mac_address_t));
    memcpy(ethernet_hdr_out->src_mac_addr.mac_addr, IF_MAC(oif), sizeof(mac_address_t));
    ethernet_hdr_out->type = ARP_MSG;
}

static void process_arp_reply_msg(node_t *node, interface_t *iif, ethernet_hdr_t *ethernet_hdr)
{
}

static void process_arp_broadcast_request(node_t *node, interface_t *iif, ethernet_hdr_t *ethernet_hdr)
{
    printf("%s : ARP Broadcast msg revd on interface %s of node %s\n", iif->if_name, iif->att_node->node_name);

    /*First check whether we want to process the package or disgard it*/

    char ip_addr[16];

    char *base = ethernet_hdr;

    arp_entry_t *arp_entry = ethernet_hdr->payload;

    unsigned int arp_dst_ip = htonl(arp_entry);

    inet_ntop(AF_INET, &arp_dst_ip, ip_addr, 16);

    ip_addr[15] = '\0';

    if (strncmp(IF_IP(iif), ip_addr, 16))
    {
        printf("%s: DST IP address %s did not match with interface ip addr - ARP Broacast req dropped.%s", node->node_name, ip_addr, IF_IP(iif));

        return;
    }

    send_arp_reply_msg(ethernet_hdr, iif);
}