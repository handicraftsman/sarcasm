// globals
extern scmval scm_true;
extern scmval scm_false;

// constructors
scmval make_bool(scm_bool_t);

// predicates
static inline bool is_bool(scmval v) { return type_of(v) == SCM_TYPE_BOOL; }
static inline bool is_true(scmval v) { return is_bool(v) && v.b; }
static inline bool is_false(scmval v) { return is_bool(v) && !v.b; }

// contracts
define_contract(bool_c, "boolean", is_bool);

// accessors
static inline scm_bool_t bool_value(scmval v) { return v.b; }

// standard library
void init_bool(scm_ctx_t*);
static inline bool is_eq(scmval x, scmval y) { return x.o == y.o; }
bool is_eqv(scmval, scmval);
bool is_equal(scmval, scmval);

