/* Automatically generated nanopb header */
/* Generated by nanopb-0.2.9 at Thu Sep  4 11:30:21 2014. */

#ifndef _PB_AUTHEXCHPROTO_PB_H_
#define _PB_AUTHEXCHPROTO_PB_H_
#include <pb.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Enum definitions */
/* Struct definitions */
typedef struct {
    size_t size;
    uint8_t bytes[1];
} authExchProto_msgType_t;

typedef struct {
    size_t size;
    uint8_t bytes[64];
} authExchProto_data1_t;

typedef struct {
    size_t size;
    uint8_t bytes[64];
} authExchProto_data2_t;

typedef struct {
    size_t size;
    uint8_t bytes[64];
} authExchProto_data3_t;

typedef struct {
    size_t size;
    uint8_t bytes[4];
} authExchProto_data4_t;

typedef struct _authExchProto {
    authExchProto_msgType_t msgType;
    bool has_data1;
    authExchProto_data1_t data1;
    bool has_data2;
    authExchProto_data2_t data2;
    bool has_data3;
    authExchProto_data3_t data3;
    bool has_data4;
    authExchProto_data4_t data4;
} authExchProto;

/* Default values for struct fields */

/* Initializer values for message structs */
#define authExchProto_init_default               {{0, {0}}, false, {0, {0}}, false, {0, {0}}, false, {0, {0}}, false, {0, {0}}}
#define authExchProto_init_zero                  {{0, {0}}, false, {0, {0}}, false, {0, {0}}, false, {0, {0}}, false, {0, {0}}}

/* Field tags (for use in manual encoding/decoding) */
#define authExchProto_msgType_tag                1
#define authExchProto_data1_tag                  2
#define authExchProto_data2_tag                  3
#define authExchProto_data3_tag                  4
#define authExchProto_data4_tag                  5

/* Struct field encoding specification for nanopb */
extern const pb_field_t authExchProto_fields[6];

/* Maximum encoded size of messages (where known) */
#define authExchProto_size                       207

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
