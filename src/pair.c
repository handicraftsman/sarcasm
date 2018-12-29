#include "scm.h"

// globals
scmval scm_null;

// constructor
scmval make_pair(scmval car, scmval cdr) {
    scm_pair_t* l = scm_new(scm_pair_t);
    l->car = car;
    l->cdr = cdr;
    return make_ptr(SCM_TYPE_PAIR, l);
}

// standard library
static scmval scm_pair_p(scmval v) {
    if(is_pair(v) && !is_null(v))
        return scm_true;
    return scm_false;
}

static scmval scm_list_p(scmval v) {
    for( ; !is_null(v); v = cdr(v)) {
        if(!is_pair(v)) {
            return scm_false;
        }
    }
    return scm_true;
}

static scmval scm_null_p(scmval v) {
    return scm_bool(is_null(v));
}

static scmval scm_make_list(scmval s, scmval f) {
    check_arg("make-list", fixnum_c, s);
    opt_arg(f, scm_false);
    scmval r = scm_null;
    for(int i = 0; i < c_fix(s); i++) {
        r = cons(f, r);
    }
    return r;
}

static scmval scm_list(int argc, scmval* argv) {
    scmval r = scm_null;
    for(int i = argc - 1; i >= 0; i--) {
        r = cons(argv[i], r);
    }
    return r;
}

static scmval scm_cons(scmval h, scmval t) {
    return make_pair(h, t);
}

static scmval scm_car(scmval l) {
    check_arg("car", list_c, l);
    return car(l);
}

static scmval scm_cdr(scmval l) {
    check_arg("cdr", list_c, l);
    return cdr(l);
}

static scmval scm_setcar(scmval l, scmval v) {
    check_arg("set-car!", list_c, l);
    setcar(l, v);
    return scm_undef;
}

static scmval scm_setcdr(scmval l, scmval v) {
    check_arg("set-cdr!", list_c, l);
    setcdr(l, v);
    return scm_undef;
}

static scmval scm_length(scmval l) {
    check_arg("length", list_c, l);
    int i = 0;
    for(i = 0; !is_null(l); i++, l = cdr(l))
        ;
    return scm_fix(i);
}

// initialization
void init_pair(scmval env) {
    scm_null  = make_val(SCM_TYPE_NULL);

    define(env, "pair?", scm_pair_p, arity_exactly(1));
    define(env, "list?", scm_list_p, arity_exactly(1));
    define(env, "null?", scm_null_p, arity_exactly(1));
    define(env, "make-list", scm_make_list, arity_or(1, 2));
    define(env, "list", scm_list, arity_at_least(0));
    define(env, "cons", scm_cons, arity_exactly(2));
    define(env, "car", scm_car, arity_exactly(1));
    define(env, "cdr", scm_cdr, arity_exactly(1));
    define(env, "set-car!", scm_setcar, arity_exactly(2));
    define(env, "set-cdr!", scm_setcdr, arity_exactly(2));
    define(env, "length", scm_length, arity_exactly(1));
}

