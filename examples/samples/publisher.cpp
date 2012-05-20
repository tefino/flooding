/*
 * Copyright (C) 2010-2011  George Parisis and Dirk Trossen
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 as published by the Free Software Foundation.
 *
 * Alternatively, this software may be distributed under the terms of
 * the BSD license.
 *
 * See LICENSE and COPYING for more details.
 */

#include <blackadder.hpp>
#include <signal.h>

Blackadder *ba;
int payload_size = 1400;
char *payload = (char *) malloc(payload_size);

void sigfun(int sig) {
    (void) signal(SIGINT, SIG_DFL);
    ba->disconnect();
    free(payload);
    delete ba;
    exit(0);
}

int main(int argc, char* argv[]) {
    (void) signal(SIGINT, sigfun);
    if (argc > 1) {
        int user_or_kernel = atoi(argv[1]);
        if (user_or_kernel == 0) {
            ba = Blackadder::Instance(true);
        } else {
            ba = Blackadder::Instance(false);
        }
    } else {
        /*By Default I assume blackadder is running in user space*/
        ba = Blackadder::Instance(true);
    }
    cout << "Process ID: " << getpid() << endl;
    string id = "0000000000000001";
    string prefix_id ;
    string bin_id = hex_to_chararray(id);
    string bin_prefix_id = hex_to_chararray(prefix_id);
    ba->publish_scope(bin_id, bin_prefix_id, DOMAIN_LOCAL, NULL, 0);

    prefix_id = "0000000000000001";
    bin_prefix_id = hex_to_chararray(prefix_id) ;
    id = "0000000000000001";
    bin_id = hex_to_chararray(id);
    ba->publish_info(bin_id, bin_prefix_id, DOMAIN_LOCAL, NULL, 0) ;

    id = "0000000000000002";
    bin_id = hex_to_chararray(id);
    ba->publish_info(bin_id, bin_prefix_id, DOMAIN_LOCAL, NULL, 0) ;

    id = "0000000000000004";
    bin_id = hex_to_chararray(id);
    ba->publish_info(bin_id, bin_prefix_id, DOMAIN_LOCAL, NULL, 0) ;

    id = "0000000000000008";
    bin_id = hex_to_chararray(id);
    ba->publish_info(bin_id, bin_prefix_id, DOMAIN_LOCAL, NULL, 0) ;

    id = "0000000000000010";
    bin_id = hex_to_chararray(id);
    ba->publish_info(bin_id, bin_prefix_id, DOMAIN_LOCAL, NULL, 0) ;

    while (true) {
        Event ev;
        ba->getEvent(ev);
        switch (ev.type) {
            case SCOPE_PUBLISHED:
                cout << "SCOPE_PUBLISHED: " << chararray_to_hex(ev.id) << endl;
                break;
            case SCOPE_UNPUBLISHED:
                cout << "SCOPE_UNPUBLISHED: " << chararray_to_hex(ev.id) << endl;
                break;
            case PLEASE_PUSH_DATA:
                cout << "PLEASE_PUSH_DATA: " << chararray_to_hex(ev.id) << endl;
                sprintf(payload, "this is published from node ? ID is %s", ev.id.c_str()) ;
                for (int i = 0; i < 1; i++) {
                    ba->publish_data(ev.id, IMPLICIT_RENDEZVOUS, (char*)ev.to_sub_FID._data, FID_LEN, payload, payload_size);
                }
                break;
            case STOP_PUBLISH:
                cout << "STOP_PUBLISH: " << chararray_to_hex(ev.id) << endl;
                break;
            case PUBLISHED_DATA:
                cout << "PUBLISHED_DATA: " << chararray_to_hex(ev.id) << endl;
                break;
        }
    }
    sleep(5);
    free(payload);
    ba->disconnect();
    delete ba;
    return 0;
}
