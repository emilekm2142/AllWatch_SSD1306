#ifndef ID_READER_H
#define ID_READER_H

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


typedef const struct AllWatchSerialize_Id_table *AllWatchSerialize_Id_table_t;
typedef const flatbuffers_uoffset_t *AllWatchSerialize_Id_vec_t;
typedef flatbuffers_uoffset_t *AllWatchSerialize_Id_mutable_vec_t;
#ifndef AllWatchSerialize_Id_identifier
#define AllWatchSerialize_Id_identifier flatbuffers_identifier
#endif
#define AllWatchSerialize_Id_type_hash ((flatbuffers_thash_t)0xf8373b4a)
#define AllWatchSerialize_Id_type_identifier "\x4a\x3b\x37\xf8"



struct AllWatchSerialize_Id_table { uint8_t unused__; };

static inline size_t AllWatchSerialize_Id_vec_len(AllWatchSerialize_Id_vec_t vec)
__flatbuffers_vec_len(vec)
static inline AllWatchSerialize_Id_table_t AllWatchSerialize_Id_vec_at(AllWatchSerialize_Id_vec_t vec, size_t i)
__flatbuffers_offset_vec_at(AllWatchSerialize_Id_table_t, vec, i, 0)
__flatbuffers_table_as_root(AllWatchSerialize_Id)

__flatbuffers_define_string_field(0, AllWatchSerialize_Id, packageName, 0)
__flatbuffers_define_string_field(1, AllWatchSerialize_Id, friendlyName, 0)

#include "flatcc/flatcc_epilogue.h"
#endif /* ID_READER_H */
