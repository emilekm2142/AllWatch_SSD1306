#ifndef WIDGET_READER_H
#define WIDGET_READER_H

/* Generated by flatcc 0.5.2 FlatBuffers schema compiler for C by dvide.com */

#ifndef FLATBUFFERS_COMMON_READER_H
#include "flatbuffers_common_reader.h"
#endif
#ifndef TEMPLATEROOT_READER_H
#include "TemplateRoot_reader.h"
#endif
#ifndef ID_READER_H
#include "Id_reader.h"
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


typedef const struct AllWatchSerialize_Widget_table *AllWatchSerialize_Widget_table_t;
typedef const flatbuffers_uoffset_t *AllWatchSerialize_Widget_vec_t;
typedef flatbuffers_uoffset_t *AllWatchSerialize_Widget_mutable_vec_t;
#ifndef AllWatchSerialize_Widget_identifier
#define AllWatchSerialize_Widget_identifier flatbuffers_identifier
#endif
#define AllWatchSerialize_Widget_type_hash ((flatbuffers_thash_t)0xb5c9588f)
#define AllWatchSerialize_Widget_type_identifier "\x8f\x58\xc9\xb5"



struct AllWatchSerialize_Widget_table { uint8_t unused__; };

static inline size_t AllWatchSerialize_Widget_vec_len(AllWatchSerialize_Widget_vec_t vec)
__flatbuffers_vec_len(vec)
static inline AllWatchSerialize_Widget_table_t AllWatchSerialize_Widget_vec_at(AllWatchSerialize_Widget_vec_t vec, size_t i)
__flatbuffers_offset_vec_at(AllWatchSerialize_Widget_table_t, vec, i, 0)
__flatbuffers_table_as_root(AllWatchSerialize_Widget)

__flatbuffers_define_scalar_field(0, AllWatchSerialize_Widget, refresh, flatbuffers_int64, int64_t, INT64_C(0))
__flatbuffers_define_table_field(1, AllWatchSerialize_Widget, template, AllWatchSerialize_TemplateRoot_table_t, 0)
__flatbuffers_define_table_field(2, AllWatchSerialize_Widget, id, AllWatchSerialize_Id_table_t, 0)

#include "flatcc/flatcc_epilogue.h"
#endif /* WIDGET_READER_H */
