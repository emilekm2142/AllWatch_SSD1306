#ifndef MESSAGE_READER_H
#define MESSAGE_READER_H

/* Generated by flatcc 0.5.2 FlatBuffers schema compiler for C by dvide.com */

#ifndef FLATBUFFERS_COMMON_READER_H
#include "flatbuffers_common_reader.h"
#endif
#include "flatcc/flatcc_flatbuffers.h"
#ifndef __alignas_is_defined
#include <stdalign.h>
#endif
#include "flatcc/flatcc_prologue.h"
#ifndef flatbuffers_identifier
#define flatbuffers_identifier 0
#endif
#ifndef flatbuffers_extension
#define flatbuffers_extension ".bin"
#endif


typedef const struct AllWatchSerialize_Message_table *AllWatchSerialize_Message_table_t;
typedef const flatbuffers_uoffset_t *AllWatchSerialize_Message_vec_t;
typedef flatbuffers_uoffset_t *AllWatchSerialize_Message_mutable_vec_t;
#ifndef AllWatchSerialize_Message_identifier
#define AllWatchSerialize_Message_identifier flatbuffers_identifier
#endif
#define AllWatchSerialize_Message_type_hash ((flatbuffers_thash_t)0x438ea666)
#define AllWatchSerialize_Message_type_identifier "\x66\xa6\x8e\x43"



struct AllWatchSerialize_Message_table { uint8_t unused__; };

static inline size_t AllWatchSerialize_Message_vec_len(AllWatchSerialize_Message_vec_t vec)
__flatbuffers_vec_len(vec)
static inline AllWatchSerialize_Message_table_t AllWatchSerialize_Message_vec_at(AllWatchSerialize_Message_vec_t vec, size_t i)
__flatbuffers_offset_vec_at(AllWatchSerialize_Message_table_t, vec, i, 0)
__flatbuffers_table_as_root(AllWatchSerialize_Message)

__flatbuffers_define_string_field(0, AllWatchSerialize_Message, senderName, 0)
__flatbuffers_define_scalar_field(1, AllWatchSerialize_Message, id, flatbuffers_int32, int32_t, INT32_C(0))
__flatbuffers_define_string_field(2, AllWatchSerialize_Message, content, 0)
__flatbuffers_define_vector_field(3, AllWatchSerialize_Message, image, flatbuffers_uint8_vec_t, 0)

#include "flatcc/flatcc_epilogue.h"
#endif /* MESSAGE_READER_H */
