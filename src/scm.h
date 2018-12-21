#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <setjmp.h>
#include <gc/gc.h>
#include <gc/cord.h>

// type aliases
typedef struct scmval scmval;
typedef bool scm_bool_t;
typedef uint8_t scmbyte;
typedef int64_t scm_fixnum_t;
typedef scm_fixnum_t scmfix; // used a lot
typedef double scm_flonum_t;
typedef char scm_char_t;


enum {
    SCM_TYPE_UNDEF,
    SCM_TYPE_NULL,
    SCM_TYPE_EOF,
    SCM_TYPE_BOOL,
    SCM_TYPE_FIXNUM,
    SCM_TYPE_FLONUM,
    SCM_TYPE_CHAR,
    SCM_TYPE_STRING,
    SCM_TYPE_SYMBOL,
    SCM_TYPE_PAIR,
    SCM_TYPE_VECTOR,
    SCM_TYPE_BYTEVECTOR,
    SCM_TYPE_ENV,
    SCM_TYPE_SUBR,
    SCM_TYPE_CLOSURE,
    SCM_TYPE_ERROR,
    SCM_TYPE_PORT,
};

struct scmval {
    int type;
    union {
        scm_bool_t b;
        scm_fixnum_t i;
        scm_flonum_t d;
        scm_char_t c;
        void* o;
    };
};

#define scm_new(T) GC_MALLOC(sizeof(T))
#define scm_new_array(S, T) GC_MALLOC(S*sizeof(T))
#define scm_delete(P) GC_FREE(P)

static inline int type_of(scmval v) { return v.type; }
static inline scmval make_val(int type) { scmval v = { .type = type, .o = NULL }; return v; }
static inline scmval make_ptr(int type, void* o) { scmval v = { .type = type, .o = o }; return v; }

#include "scm/arity.h"
#include "scm/contract.h"
#include "scm/dict.h"
#include "scm/context.h"
#include "scm/bool.h"
#include "scm/number.h"
#include "scm/char.h"
#include "scm/string.h"
#include "scm/symbol.h"
#include "scm/error.h"
#include "scm/pair.h"
#include "scm/vector.h"
#include "scm/bytevector.h"
#include "scm/port.h"
#include "scm/proc.h"
#include "scm/env.h"
#include "scm/writer.h"
#include "scm/reader.h"

// utilities
void init_eval();
scmval eval(scmval, scmval);
