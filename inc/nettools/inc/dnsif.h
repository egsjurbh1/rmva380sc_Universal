/*
 *  Copyright 2007 by Texas Instruments Incorporated.
 *  All rights reserved. Property of Texas Instruments Incorporated.
 *  Restricted rights to use, duplicate or disclose this code are
 *  granted through contract.
 *
 *  @(#) TCP/IP_Network_Developers_Kit 1.93.00.09 08-16-2007 (ndk-c09)
 */
/*
 *  ======== dnsif.h ========
 *
 *
 */

//--------------------------------------------------------------------------
// Network Tooks Library
//--------------------------------------------------------------------------
// DNSIF.H
//
// Basic DNS Resolution Routine Prototypes
//
// Author: Michael A. Denio
// Copyright 1999, 2000 by Texas Instruments Inc.
//-------------------------------------------------------------------------
#ifndef _DNSIF_H_
#define _DNSIF_H_

//
// Response codes
// (Codes under 16 are from a DNS reply packet)
//
#define NOERROR         0               // (DNS) no error
#define FORMERR         1               // (DNS) format error
#define SERVFAIL        2               // (DNS) server failure
#define NXDOMAIN        3               // (DNS) non existent domain
#define NOTIMP          4               // (DNS) not implemented
#define REFUSED         5               // (DNS) query refused
#define OVERFLOW        16              // buffer overflow
#define MEMERROR        17              // memory allocation error
#define SOCKETERROR     18              // internal socket error
#define NODNSREPLY      19              // DNS Server did not respond

#define MAXIPADDR        8

//
// Host Entry Structure
//
struct _hostent {
        char   *h_name;                         // Official CNAME of host
        int     h_addrtype;                     // Address Type (AF_INET)
        int     h_length;                       // Address Length (4)
        int     h_addrcnt;                      // Number of IP addresses found
        IPN     h_addr[MAXIPADDR];      // List of up to MAXIPADDR IPaddr (network fmt)
        };
typedef struct _hostent HOSTENT;

extern char *DNSErrorStrings[20];

#define DNSErrorStr(err) DNSErrorStrings[err]

//--------------------------------------------------------------------
// DNSGetHostname
//
// Requests the hostname which matches the IPHost supplied to SetConfig,
// or uses the first IP address found in the system if SetConfig was
// not supplied with a host address. The hostname is copied into the
// buffer pointed to by 'pNameBuf' with a max size of 'size'.
// NULL terminates the name when space allows.
//
// Returns 0 if OK, or error code
//--------------------------------------------------------------------
_extern int DNSGetHostname( char *pNameBuf, int size );

//--------------------------------------------------------------------
// DNSGetHostByAddr
//
// Looks up the supplied IP address. On a successful return, pScrapBuf
// can be treated as a HOSTENT structure. The size of the scrap buffer
// (size) must be greater than the size of the structure as the
// structure will contain pointers into the scrap buffer, and the
// scrap buffer is also used for temporary name storage. 512 bytes
// should be sufficient for most requests.
//
// Returns 0 if OK, or error code
//--------------------------------------------------------------------
_extern int DNSGetHostByAddr( IPN IPAddr, void *pScrapBuf, int size );

//--------------------------------------------------------------------
// DNSGetHostByName
//
// Looks up the supplied hostname. On a successful return, pScrapBuf
// can be treated as a HOSTENT structure. The size of the scrap buffer
// (size) must be greater than the size of the structure as the
// structure will contain pointers into the scrap buffer, and the
// scrap buffer is also used for temporary name storage. 512 bytes
// should be sufficient for most requests.
//
// If the host name 'Name' is terminated with a dot ('.'), the dot is
// removed. If the name contains a dot anywhere, it is used unmodified
// for an initial lookup. If the lookup fails - or if the name did not
// contain a dot, the default domain suffix is applied.
//
// Returns 0 if OK, or error code
//--------------------------------------------------------------------
_extern int DNSGetHostByName( char *Name, void *pScrapBuf, int size );

//
// DNSServerOpen
//
// Create an instance of the DNS Server
//
_extern HANDLE DNSServerOpen( NTARGS *pNTA );

//
// DNSServerClose
//
// Destroy an instance of the DNS Server
//
_extern void DNSServerClose( HANDLE h );

#endif


