#include "scm.h"

// constructor
scmval make_subr(const char* name, subr_f fn, arity_t arity) {
    scm_subr_t* subr = scm_new(scm_subr_t);
    subr->name  = make_string(name);
    subr->f     = fn;
    subr->arity = arity;
    return make_ptr(SCM_TYPE_SUBR, subr);
}

scmval make_closure(scmval name, scmfix argc, scmval* argv, scmval env, scmval body) {
    scm_closure_t* closure = scm_new(scm_closure_t);
    closure->name = name;
    closure->argc = argc;
    closure->argv = argv;
    closure->env  = env;
    closure->body = body;
    return make_ptr(SCM_TYPE_CLOSURE, closure);
}

// arity
static void arity_error(scmval v, int argc) {
    arity_t arity = subr_arity(v);
    char* n = string_to_cstr(subr_name(v));
    char* m;
    switch(arity.type) {
        case ARITY_EXACTLY:
            asprintf(&m, "exactly %d argument%s", arity.min, arity.min <= 1 ? "" : "s");
            break;
        case ARITY_OR:
            asprintf(&m, "%d or %d arguments", arity.min, arity.max);
            break;
        case ARITY_BETWEEN:
            asprintf(&m, "%d to %d arguments", arity.min, arity.max);
            break;
        case ARITY_AT_LEAST:
            asprintf(&m, "at least %d argument%s", arity.min, arity.min <= 1 ? "" : "s");
            break;
    }
    error(arity_error_type, "%s expects %s but received %d", n, m, argc);
}

void check_arity(scmval v, scmfix argc) {
    bool match = false;
    arity_t arity = subr_arity(v);
    switch(arity.type) {
        case ARITY_EXACTLY:  match = (argc == arity.min); break;
        case ARITY_OR:       match = (argc == arity.min || argc == arity.max); break;
        case ARITY_BETWEEN:  match = (arity.min <= argc && argc <= arity.max); break;
        case ARITY_AT_LEAST: match = (argc >= arity.min); break;
    }
    if(!match)
        arity_error(v, argc);
}

scmfix argc_from_arity(scmval subr, scmfix len) {
    scmfix argc;
    arity_t arity = subr_arity(subr);
    switch(arity.type) {
        case ARITY_EXACTLY: // already checked
        case ARITY_AT_LEAST:
            argc = len;
            break;
        case ARITY_OR:
        case ARITY_BETWEEN:
            argc = arity.max; // need to pad up to arity.max
            break;
    }
    return argc;
}

scmval apply_funcall(scmval s, scmfix argc, scmval* argv) {
    scmval r = scm_undef;
    if(subr_arity(s).type == ARITY_AT_LEAST) {
        r = funcall(s, argc, argv);
    } else {
        switch(argc) {
            case  0: r = funcall0(s); break;
            case  1: r = funcall(s, argv[0]); break;
            case  2: r = funcall(s, argv[0], argv[1]); break;
            case  3: r = funcall(s, argv[0], argv[1], argv[2]); break;
            case  4: r = funcall(s, argv[0], argv[1], argv[2], argv[3]); break;
            case  5: r = funcall(s, argv[0], argv[1], argv[2], argv[3], argv[4]); break;
            case  6: r = funcall(s, argv[0], argv[1], argv[2], argv[3], argv[4], argv[5]); break;
            case  7: r = funcall(s, argv[0], argv[1], argv[2], argv[3], argv[4], argv[5], argv[6]); break;
            case  8: r = funcall(s, argv[0], argv[1], argv[2], argv[3], argv[4], argv[5], argv[6], argv[7]); break;
            case  9: r = funcall(s, argv[0], argv[1], argv[2], argv[3], argv[4], argv[5], argv[6], argv[7], argv[8]); break;
            case 10: r = funcall(s, argv[0], argv[1], argv[2], argv[3], argv[4], argv[5], argv[6], argv[7], argv[8], argv[9]); break;
        }
    }
    return r;
}

