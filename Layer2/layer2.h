#ifndef __LAYER2__
#define __LAYER2__

// #include "../glthread/glthread.h"

#include "../net.h"
#include "../tcpconst.h"
#include <stdlib.h>
#include "../graph.h"
// typedef struct interface_ interface_t;

#pragma pack(push, 1)

typedef struct arp_hdr_
{
    short hw_type;
    short proto_type;      /*0X0800 FOR IPV4*/
    char hw_addr_len;      /*6 FOR MAC*/
    char proto_addr_len;   /*4 FOR IPV4*/
    short op_code;         /*req or reply*/
    mac_address_t src_mac; /*MAC OF interface*/
    unsigned int src_ip;   /*IP OF interface*/
    mac_address_t dst_mac; /*?*/
    unsigned int dst_ip;   /* IP FOR WHICH ARP is being resolved*/
} arp_hdr_t;

typedef struct ethernet_hdr_
{
    mac_address_t dst_mac_addr;
    mac_address_t src_mac_addr;
    short type;
    char payload[248];
    unsigned int FCS;
} ethernet_hdr_t;

#pragma pack(pop)

typedef struct arp_table_
{
    glthread_t arp_entries;
} arp_table_t;

typedef struct arp_entry_
{
    ip_address_t ip_addr;
    mac_address_t mac_addr;
    char oif_name[IF_NAME_SIZE];
    glthread_t arp_glue;
} arp_entry_t;

/*RETURNING THE SIZE OF the struct ethernet header excluding the payload*/
#define ETH_HDR_SIZE_EXCL_PAYLOAD \
    (sizeof(ethernet_hdr_t) - sizeof(((ethernet_hdr_t *)0)->payload))

/*RETURNING THE VALUE OF FCS*/
#define ETH_FCS(eth_hdr_ptr, payload_size) \
    (*(unsigned int *)((char *)(((ethernet_hdr_t *)eth_hdr_ptr)->payload) + payload_size))

static inline ethernet_hdr_t *ALLOC_ETH_HDR_WITH_PAYLOAD(char *pkt, unsigned int pkt_size)
{
    // PKT Structure
    // |    18 Bytes     |      Actual Pkt      |

    char *temp = (char *)calloc(1, pkt_size);

    ethernet_hdr_t *eth_header = (ethernet_hdr_t *)(pkt_size - ETH_HDR_SIZE_EXCL_PAYLOAD);
    memset((char *)eth_header, 0, ETH_HDR_SIZE_EXCL_PAYLOAD);
    memcpy(eth_header->payload, temp, pkt_size);
    free(temp);

    ETH_FCS(eth_header, pkt_size) = 0;

    return eth_header;
}

static inline bool l2_frame_recv_qualify_on_interface(interface_t *interface, ethernet_hdr_t *ethernet_hdr)
{
    // Check whether in L3 mode

    if (IS_INTF_L3_MODE(interface))
    {
        return false;
    }

    // Check if L3 mode and dst mac in ethernet header
    if (memcmp(IF_MAC(interface), ethernet_hdr->dst_mac_addr.mac_addr, sizeof(mac_address_t)) == 0)
    {
        return true;
    }

    // check if L3 mode and dst mac in ethernet header == broadcast mac addr
    if (IS_MAC_BROADCAST_ADDR(ethernet_hdr->dst_mac_addr.mac_addr))
    {
        return true;
    }
    // else => false
    return false;
}

void layer2_frame_recv(node_t *node, interface_t *interface, char *pkt, unsigned int pkt_size);

void init_arp_table(arp_table_t **arp_table);

arp_entry_t *arp_table_lookup(arp_table_t *arp_table, char *ip_addr);

void delete_arp_table_entry(arp_table_t *arp_table, char *ip_addr);

bool arp_table_entry_add(arp_table_t *arp_table, arp_entry_t *arp_entry);

void arp_table_update_from_arp_reply(arp_table_t *arp_table, arp_hdr_t *arp_hdr, interface_t *iif);

void send_arp_broadcast_request(node_t *node, interface_t *oif, char *ip_addr);

#endif