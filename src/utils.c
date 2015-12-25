/*
 * author: Darren Bounds <dbounds@intrusense.com>
 * copyright: Copyright (C) 2002 by Darren Bounds
 * license: This software is under GPL version 2 of license
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * packit official page at http://packit.sourceforge.net
 */

#include "../include/packit.h" 
#include "../include/inject.h"
#include "../include/capture.h"
#include "../include/error.h"


u_int32_t
retrieve_rand_int(u_int32_t r_size)
{
    static u_int32_t r_int;

#ifdef DEBUG
    fprintf(stdout, "\nDEBUG: retrieve_rand_int()\n");
#endif

    if(!r_int)
        r_int++;

    srand(time(0) ^ getpid() * r_int);
    r_int = rand() % r_size;

    return r_int;
}

u_int16_t
retrieve_datalink_hdr_len(u_int32_t d_link)
{
    u_int16_t len = 0;

#ifdef DEBUG
    fprintf(stdout, "\nDEBUG: retrieve_datalink_hdr_len()\n");
#endif

    switch(d_link)
    {   
        case DLT_NULL:
            len = 4;
            break;

        case DLT_EN10MB:
            len = ETH_H;
            break;

        default:
            len = ETH_H;
            break;
    }

#ifdef DEBUG
    fprintf(stdout, "DEBUG: hdr_len: %d\n", len);
#endif

    return len;
}

u_int8_t *
retrieve_rand_ipv4_addr()
{
    unsigned short oct, oct_cnt;
    u_int8_t *ip;

#ifdef DEBUG
    fprintf(stdout, "DEBUG: retrive_rand_ipv4_addr()\n");
#endif

    ip = malloc(sizeof(u_int8_t) * 16);

    for(oct_cnt = 1; oct_cnt < 5; oct_cnt++)
    {   

        while(1)
        {   
            oct = (unsigned short)retrieve_rand_int(0xFF);

            if(oct_cnt != 1 ||
              (oct_cnt == 1 && oct > 0 && oct < 239))
                break;
        }

        if(oct_cnt != 1)
            sprintf(ip, "%s.%d", ip, oct);
        else
            sprintf(ip, "%d", oct);
    }

    return ip;
}

u_int8_t *
retrieve_rand_ethernet_addr()
{
    u_int16_t oct, oct_cnt;
    static int seed;
    u_int8_t *eaddr;

#ifdef DEBUG
    fprintf(stdout, "DEBUG: retrieve_rand_ethernet_addr()\n");
#endif

    eaddr = malloc(sizeof(u_int8_t) * 18);

   for(oct_cnt = 1; oct_cnt < 7; oct_cnt++)
   {
       seed++;
       srand(time(0) ^ getpid() * seed);
       oct = rand() % 0xFF;

       if(oct_cnt != 1)
           sprintf(eaddr, "%s:%0x", eaddr, oct);
       else
           sprintf(eaddr, "%0x", oct);
   }

    return eaddr;
}

void
print_separator(int bnl, int anl, u_int8_t *msgp, ...)
{
    unsigned short i;
    unsigned short max_len = 76;
    unsigned short msg_len = 0;
    u_int8_t msg[255];

    va_list va;

#ifdef DEBUG
    fprintf(stdout, "\nDEBUG: print_separator()\n");
#endif

    va_start(va, msgp);
    vsnprintf(msg, 256, msgp, va);

    msg_len = strlen(msg);

    for(i = 0; i < bnl; i++)
        fprintf(stdout, "\n");

    fprintf(stdout, "-| %s |-", msg);

    for(i = 0; i < (max_len - msg_len - 6); i++)
        fprintf(stdout, "-");

    for(i = 0; i < anl; i++)
        fprintf(stdout, "\n");

    return;
}

u_int8_t *
retrieve_icmp_code(u_int16_t type, u_int16_t code)
{
    u_int8_t *icmp_c;

#ifdef DEBUG
    fprintf(stdout, "DEBUG: retrieve_icmp_code()\n");
#endif

    icmp_c = malloc(sizeof(unsigned char) * 32);

    if(type == ICMP_UNREACH)
    {
        switch(code)
        {
            case ICMP_UNREACH_NET:
                sprintf(icmp_c, "Network");
                break;

            case ICMP_UNREACH_HOST:
                sprintf(icmp_c, "Host");
                break;

            case ICMP_UNREACH_PROTOCOL:
                sprintf(icmp_c, "Protocol");
                break;

            case ICMP_UNREACH_PORT:
                sprintf(icmp_c, "Port");
                break;

            case ICMP_UNREACH_NEEDFRAG:
                sprintf(icmp_c, "Need Fragment");
                break;

            case ICMP_UNREACH_SRCFAIL: 
                sprintf(icmp_c, "Source Fail");
                break;

            case ICMP_UNREACH_NET_UNKNOWN:
                sprintf(icmp_c, "Network Unknown");
                break;

            case ICMP_UNREACH_HOST_UNKNOWN: 
                sprintf(icmp_c, "Host Unknown");
                break;

            case ICMP_UNREACH_ISOLATED: 
                sprintf(icmp_c, "Isolated");
                break;

            case ICMP_UNREACH_NET_PROHIB: 
                sprintf(icmp_c, "Network Prohibited");
                break;

            case ICMP_UNREACH_HOST_PROHIB:
                sprintf(icmp_c, "Host Prohibited");
                break;

            case ICMP_UNREACH_TOSNET: 
                sprintf(icmp_c, "ToS Network");
                break;

            case ICMP_UNREACH_TOSHOST: 
                sprintf(icmp_c, "ToS Host");
                break;

            case ICMP_UNREACH_FILTER_PROHIB: 
                sprintf(icmp_c, "Filter Prohibited");
                break;

            case ICMP_UNREACH_HOST_PRECEDENCE: 
                sprintf(icmp_c, "Host Precedence");
                break;

            case ICMP_UNREACH_PRECEDENCE_CUTOFF: 
                sprintf(icmp_c, "Precedence Cutoff");
                break;

            default:
                sprintf(icmp_c, "Unknown");
                break;
        }
    }
    else if(type == ICMP_REDIRECT)
    {
        switch(code)
        {
            case ICMP_REDIRECT_NET: 
                sprintf(icmp_c, "Network");
                break;

            case ICMP_REDIRECT_HOST: 
                sprintf(icmp_c, "Host");
                break;

            case ICMP_REDIRECT_TOSNET:
                sprintf(icmp_c, "ToS Net");
                break;

            case ICMP_REDIRECT_TOSHOST:
                sprintf(icmp_c, "ToS Host");
                break;

            default:
                sprintf(icmp_c, "Unknown");
                break;
        }
    }
    else if(type == ICMP_TIMXCEED)
    {
        switch(code)
        {
            case ICMP_TIMXCEED_INTRANS: 
                sprintf(icmp_c, "In Transit");
                break;

            case ICMP_TIMXCEED_REASS: 
                sprintf(icmp_c, "Reassembly");
                break;

            default:
                sprintf(icmp_c, "Unknown");
                break;
        }
    }
    else if(type == ICMP_PARAMPROB)
    {
        switch(code)
        {
            case ICMP_PARAMPROB_OPTABSENT:
                sprintf(icmp_c, "Missing Option");
                break;

            default:
                sprintf(icmp_c, "Unknown");
                break;
        }
    }
    else
    {
        sprintf(icmp_c, "Unknown");
    }

    return icmp_c;
}

u_int8_t *
retrieve_icmp_type(u_int16_t type)
{
    u_int8_t *icmp_t;

#ifdef DEBUG
    fprintf(stdout, "DEBUG: retrieve_icmp_type()\n");
#endif

    icmp_t = malloc(sizeof(unsigned char) * 32);
    memset(icmp_t, 0, sizeof(unsigned char) * 32);

    switch(type)
    {
        case ICMP_ECHOREPLY:
            sprintf(icmp_t, "Echo Reply");
            break;

        case ICMP_UNREACH:
            sprintf(icmp_t, "Unreachable");
            break;

        case ICMP_SOURCEQUENCH:
            sprintf(icmp_t, "Source Quench");
            break;

        case ICMP_REDIRECT:
            sprintf(icmp_t, "Redirect");
            break;

        case ICMP_ECHO:
            sprintf(icmp_t, "Echo Request");
            break;

        case ICMP_TIMXCEED:
            sprintf(icmp_t, "Time Exceeded");
            break;

        case ICMP_PARAMPROB:
            sprintf(icmp_t, "Param Problem");
            break;

        case ICMP_TSTAMP:
            sprintf(icmp_t, "Timestamp");
            break;

        case ICMP_TSTAMPREPLY:
            sprintf(icmp_t, "Timestamp Reply");
            break;

        case ICMP_IREQ:
            sprintf(icmp_t, "Info Request");
            break;

        case ICMP_IREQREPLY:
            sprintf(icmp_t, "Info Reply");
            break;

        case ICMP_ROUTERADVERT:
            sprintf(icmp_t, "Router Advertise");
            break;

        case ICMP_ROUTERSOLICIT:
            sprintf(icmp_t, "Router Selection");
            break;

        case ICMP_MASKREQ:
            sprintf(icmp_t, "Address Mask Request");
            break;

        case ICMP_MASKREPLY:
            sprintf(icmp_t, "Address Mask Reply");
            break;

        case ICMP_TRACEROUTE:
            sprintf(icmp_t, "Traceroute");
            break;

        default:
            sprintf(icmp_t, "Unknown");
            break;
    }

    return icmp_t;
}

u_int8_t *
retrieve_arp_type(u_int16_t op_type)
{
    u_int8_t *arp_t;

#ifdef DEBUG
    fprintf(stdout, "DEBUG: retrieve_arp_type() OPTYPE: %d\n", op_type);
#endif

    arp_t = malloc(sizeof(u_int8_t) * 32);

    switch(op_type)
    {
        case ARPOP_REQUEST:
            sprintf(arp_t, "Request");
            break;

        case ARPOP_REPLY:
            sprintf(arp_t, "Reply");
            break;

        case ARPOP_REVREQUEST:
            sprintf(arp_t, "Reverse Request");
            break;

        case ARPOP_REVREPLY:
            sprintf(arp_t, "Reverse Reply");
            break;

        case ARPOP_INVREQUEST:
            sprintf(arp_t, "Inverse Request");
            break;

        case ARPOP_INVREPLY:
            sprintf(arp_t, "Inverse Reply");
            break;

        default:
            sprintf(arp_t, "Unknown");
            break;
    }

#ifdef DEBUG
    fprintf(stdout, "DEBUG: ARP type: %s\n", arp_t);
#endif

    return arp_t;
}

u_int8_t *
retrieve_arp_hw_type(u_int16_t hw_type)
{
    u_int8_t *hw_t;

#ifdef DEBUG
    fprintf(stdout, "DEBUG: retrieve_arp_hw_type() HWTYPE: %d\n", hw_type);
#endif

    hw_t = malloc(sizeof(u_int8_t) * 32);

    switch(hw_type)
    {
        case ARPHRD_NETROM:
            sprintf(hw_t, "Net/ROM Pseudo");
            break;

        case ARPHRD_ETHER:
            sprintf(hw_t, "Ethernet 10/100");
            break;

        case ARPHRD_EETHER:
            sprintf(hw_t, "Experimental Ethernet");
            break;

        case ARPHRD_AX25:
            sprintf(hw_t, "AX.25 Level 2");
            break;

        case ARPHRD_PRONET:
            sprintf(hw_t, "ProNet Token Ring");
            break;

        case ARPHRD_IEEE802:
            sprintf(hw_t, "IEEE 802.2 Ethernet");
            break;

        case ARPHRD_ARCNET:
            sprintf(hw_t, "ARCNet");
            break;

        case ARPHRD_APPLETLK:
            sprintf(hw_t, "AppleTalk");
            break;

        case ARPHRD_LANSTAR:
            sprintf(hw_t, "Lanstar");
            break;

        case ARPHRD_DLCI:
            sprintf(hw_t, "Frame Relay DLCI");
            break;

        case ARPHRD_ATM:
            sprintf(hw_t, "ATM");
            break;

        case ARPHRD_METRICOM:
            sprintf(hw_t, "Metricom STRIP");
            break;

        case ARPHRD_IPSEC:
            sprintf(hw_t, "IPsec Tunnel");
            break;
    }

#ifdef DEBUG
    fprintf(stdout, "DEBUG: ARP hardware type: : %s\n", hw_t);
#endif

    return hw_t;
}

int
retrieve_tcp_flags()
{
    int flags = 0;

#ifdef DEBUG
    fprintf(stdout, "DEBUG: retrieve_tcp_flags()\n");
#endif

    if(thdr_o.urg)
        flags |= TH_URG;

    if(thdr_o.ack)
        flags |= TH_ACK;

    if(thdr_o.psh)
        flags |= TH_PUSH;

    if(thdr_o.rst)
        flags |= TH_RST;

    if(thdr_o.syn)
        flags |= TH_SYN;

    if(thdr_o.fin)
        flags |= TH_FIN;

    return flags;
}