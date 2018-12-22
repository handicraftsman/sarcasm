#include "scm.h"

static void write_char(scmval, scmval, scmfix);
static void write_pair(scmval, scmval, scmfix);
static void write_vector(scmval, scmval, scmfix);
static void write_bytevector(scmval, scmval, scmfix);

void write(scmval p, scmval v, scmfix flags) {
    switch(type_of(v)) {
        case SCM_TYPE_UNDEF:
            scm_puts(p, "#<undefined>");
            break;
        case SCM_TYPE_VOID:
            scm_puts(p, "#<void>");
            break;
        case SCM_TYPE_NULL:
            scm_puts(p, "()");
            break;
        case SCM_TYPE_EOF:
            scm_puts(p, "#<eof>");
            break;
        case SCM_TYPE_BOOL:
            scm_puts(p, is_true(v) ? "#t" : "#f");
            break;
        case SCM_TYPE_FIXNUM:
            scm_printf(p, "%lld", fixnum_value(v));
            break;
        case SCM_TYPE_FLONUM:
            scm_printf(p, "%lf", flonum_value(v));
            break;
        case SCM_TYPE_CHAR:
            if(flags & scm_mode_write) {
                write_char(p, v, flags);
            } else {
                scm_putc(p, char_value(v));
            }
            break;
        case SCM_TYPE_STRING:
            if(flags & scm_mode_write) {
                scm_putc(p, '"');
                scm_puts(p, string_value(v));
                scm_putc(p, '"');
            } else {
                scm_puts(p, string_value(v));
            }
            break;
        case SCM_TYPE_SYMBOL:
            scm_puts(p, string_value(v));
            break;
        case SCM_TYPE_PAIR:
            write_pair(p, v, flags);
            break;
        case SCM_TYPE_VECTOR:
            write_vector(p, v, flags);
            break;
        case SCM_TYPE_BYTEVECTOR:
            write_bytevector(p, v, flags);
            break;
        case SCM_TYPE_ENV:
            scm_puts(p, "#<environment>");
            break;
        case SCM_TYPE_SUBR:
            scm_printf(p, "#<primitive:%s>", string_value(subr_name(v)));
            break;
        case SCM_TYPE_CLOSURE:
            if(is_undef(closure_name(v)))
                scm_printf(p, "#<closure:%p>", get_closure(v));
            else
                scm_printf(p, "#<closure:%s>", string_value(closure_name(v)));
            break;
        case SCM_TYPE_ERROR:
            scm_putc(p, '[');
            write(p, error_type(v), flags);
            scm_puts(p, "] ");
            write(p, error_message(v), flags);
            break;
        case SCM_TYPE_PORT:
            scm_printf(p, "#<%sput-port:%s>", is_input_port(v) ? "in" : "out", port_name(v));
            break;
    }
}

static void write_char(scmval p, scmval v, scmfix flags) {
    scm_char_t c = char_value(v);
    scm_puts(p, "#\\");
    switch(c) {
        case '\a':  scm_puts(p, "alarm");       break;
        case '\b':  scm_puts(p, "backspace");   break;
        case '\n':  scm_puts(p, "newline");     break;
        case '\0':  scm_puts(p, "null");        break;
        case '\r':  scm_puts(p, "return");      break;
        case ' ':   scm_puts(p, "space");       break;
        case '\t':  scm_puts(p, "tab");         break;
        case 0xb:   scm_puts(p, "vtab");        break;
        default:    scm_putc(p, c);             break;
    }
}

static void write_pair(scmval p, scmval v, scmfix flags) {
    if(flags & scm_mode_pp_quote) {
        if(!is_null(v) && is_eq(car(v), scm_quote)) {
            scm_putc(p, '\'');
            write_pair(p, cdr(v), flags);
            return;
        }
    }
    scm_putc(p, '(');
    for(scmval l = v; !is_null(l); l = cdr(l)) {
        write(p, car(l), flags);
        if(!is_pair(cdr(l))) {
            scm_puts(p, " . ");
            write(p, cdr(l), flags);
            break;
        }
        if(!is_null(cdr(l))) {
            scm_putc(p, ' ');
        }
    }
    scm_putc(p, ')');
}

static void write_vector(scmval p, scmval v, scmfix flags) {
    scm_puts(p, "#(");
    for(int i = 0; i < vector_size(v); i++) {
        if(i > 0) {
            scm_putc(p, ' ');
        }
        write(p, vector_ref(v, i), flags);
    }
    scm_putc(p, ')');
}

static void write_bytevector(scmval p, scmval v, scmfix flags) {
    scm_puts(p, "#u8(");
    for(int i = 0; i < bytevector_size(v); i++) {
        if(i > 0) scm_putc(p, ' ');
        write(p, bytevector_ref(v, i), flags);
    }
    scm_putc(p, ')');
}

