/*
 * Copyright 2013 Jung-Sang Ahn <jungsang.ahn@gmail.com>.
 * All Rights Reserved.
 */

#ifndef _JSAHN_DOCIO_H
#define _JSAHN_DOCIO_H

#include "filemgr.h"
#include "common.h"

typedef uint16_t keylen_t;

struct docio_handle {
    struct filemgr *file;
    bid_t curblock;
    uint32_t curpos;
    // for buffer purpose
    bid_t lastbid;
    void *readbuffer;
};

#ifdef DOCIO_LEN_STRUCT_ALIGN
    // this structure will occupy 12 bytes
    struct docio_length {
        keylen_t keylen;
        uint16_t metalen;
        uint32_t bodylen;
        uint8_t flag;
    };
#else
    // this structure will occupy 9 bytes
    struct __attribute__ ((packed)) docio_length {
        keylen_t keylen;
        uint16_t metalen;
        uint32_t bodylen;
        uint8_t flag;
    };
#endif

struct docio_object {
    struct docio_length length;
    void *key;
    #ifdef __FDB_SEQTREE
        fdb_seqnum_t seqnum;
    #endif
    void *meta;
    void *body;
};

void docio_init(struct docio_handle *handle, struct filemgr *file);
void docio_free(struct docio_handle *handle);
INLINE bid_t docio_append_doc_raw(struct docio_handle *handle, uint64_t size, void *buf);

bid_t docio_append_doc_compact(struct docio_handle *handle, struct docio_object *doc);
bid_t docio_append_doc(struct docio_handle *handle, struct docio_object *doc);

struct docio_length docio_read_doc_length(struct docio_handle *handle, uint64_t offset);
void docio_read_doc_key(struct docio_handle *handle, uint64_t offset, keylen_t *keylen, void *keybuf);
uint64_t docio_read_doc_key_meta(struct docio_handle *handle, uint64_t offset, struct docio_object *doc);
uint64_t docio_read_doc(struct docio_handle *handle, uint64_t offset,
                        struct docio_object *doc);
int docio_check_buffer(struct docio_handle *dhandle, bid_t check_bid);
int docio_check_compact_doc(struct docio_handle *handle,
                            struct docio_object *doc);
#endif
