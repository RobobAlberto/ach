/* -*- mode: C; c-basic-offset: 4 -*- */
/* ex: set shiftwidth=4 tabstop=4 expandtab: */
/*
 * Copyright (c) 2011-2013, Georgia Tech Research Corporation
 * All rights reserved.
 *
 * Author(s): Neil T. Dantam <ntd@gatech.edu>
 * Georgia Tech Humanoid Robotics Lab
 * Under Direction of Prof. Mike Stilman <mstilman@cc.gatech.edu>
 *
 *
 * This file is provided under the following "BSD-style" License:
 *
 *
 *   Redistribution and use in source and binary forms, with or
 *   without modification, are permitted provided that the following
 *   conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 *   CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *   INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 *   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 *   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 *   USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *   AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *   ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *   POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifdef HAVE_CONFIG
#include "config.h"
#endif //HAVE_CONFIG

#include <ipcbench.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

#define PORT 8070

static int sock;
static struct sockaddr_in addr = {0};

static void s_sock(void) {
    sock = socket( AF_INET, SOCK_DGRAM, 0 );
    if( sock < 0 ) {
        perror( "Could not create socket");
        abort();
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
}

static void s_init_send(void) {
    s_sock();
    u_int yes = 1;
    if( setsockopt(sock, IPPROTO_IP, IP_MULTICAST_LOOP, &yes, sizeof(yes)) ) {
        perror("setsockopt for sender failed\n");
    }

    addr.sin_addr.s_addr = inet_addr("224.0.0.1");

}

static void s_init_recv(void) {
    s_sock();

    u_int yes = 1;
    if( setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) ) {
        perror("setsockopt for receiver failed\n");
    }

    addr.sin_addr.s_addr = INADDR_ANY;


    if (bind(sock, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        perror("Failed to bind the socket");
        abort();
    }
}

static void s_send( const struct timespec *ts ) {
    ssize_t r = sendto( sock, ts, sizeof(*ts), 0,
                        (struct sockaddr *) &addr, sizeof(addr) );
    if( sizeof(*ts) != r ) {
        perror( "could not send data on pipe" );
        abort();
    }
}

static void s_recv( struct timespec *ts ) {
    ssize_t r = recvfrom( sock, ts, sizeof(*ts), 0,
                          NULL, 0 );
    if( sizeof(*ts) != r ) {
        perror( "could not receive data on pipe" );
        abort();
    }
}

struct ipcbench_vtab ipc_bench_vtab_udp_multicast = {
    .init_send = s_init_send,
    .init_recv = s_init_recv,
    .send = s_send,
    .recv = s_recv
};
