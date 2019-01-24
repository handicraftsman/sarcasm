#include <ffi.h>

typedef struct scm_foreign_lib  scm_foreign_lib_t;
typedef struct scm_foreign_obj  scm_foreign_obj_t;
typedef struct scm_foreign_type scm_foreign_type_t;

struct scm_foreign_lib {
    char* name;
    void* handle;
};

struct scm_foreign_obj {
    char*   name;
    scmval  ret;
    scmval  args;
    void*   handle;
    ffi_cif cif;
};

struct scm_foreign_type {
    char*       name;
    short       code;
    ffi_type*   type;
};

// constructors
scmval make_foreign_lib(const char*, void*);
scmval make_foreign_object(const char*, scmval, scmval, ffi_cif);
scmval make_foreign_type(const char*, short, ffi_type*);

// predicates
static inline bool is_foreign_lib(scmval obj) { return type_of(obj) == SCM_TYPE_FOREIGN_LIB; }
static inline bool is_foreign_obj(scmval obj) { return type_of(obj) == SCM_TYPE_FOREIGN_OBJ; }
static inline bool is_foreign_ptr(scmval obj) { return type_of(obj) == SCM_TYPE_FOREIGN_PTR; }
static inline bool is_foreign_type(scmval obj) { return type_of(obj) == SCM_TYPE_FOREIGN_TYPE; }

// contracts
define_contract(foreign_lib_c, "foreign library", is_foreign_lib);
define_contract(foreign_obj_c, "foreign object", is_foreign_obj);
define_contract(foreign_ptr_c, "foreign pointer", is_foreign_ptr);
define_contract(foreign_type_c, "foreign type", is_foreign_type);

// accessors
static inline scm_foreign_lib_t* get_foreign_lib(scmval v) { return (scm_foreign_lib_t*)v.o; }
static inline char*              foreign_lib_name(scmval v) { return get_foreign_lib(v)->name; }
static inline void*              foreign_lib_handle(scmval v) { return get_foreign_lib(v)->handle; }

static inline scm_foreign_obj_t* get_foreign_obj(scmval v) { return (scm_foreign_obj_t*)v.o; }
static inline char*              foreign_obj_name(scmval v) { return get_foreign_obj(v)->name; }
static inline scmval             foreign_obj_ret(scmval v) { return get_foreign_obj(v)->ret; }
static inline scmval             foreign_obj_args(scmval v) { return get_foreign_obj(v)->args; }
static inline void*              foreign_obj_handle(scmval v) { return get_foreign_obj(v)->handle; }
static inline ffi_cif            foreign_obj_cif(scmval v) { return get_foreign_obj(v)->cif; }

static inline scm_foreign_type_t* get_foreign_type(scmval v) { return (scm_foreign_type_t*)v.o; }
static inline char*     foreign_type_name(scmval v) { return get_foreign_type(v)->name; }
static inline short     foreign_type_code(scmval v) { return get_foreign_type(v)->code; }
static inline ffi_type* foreign_type_type(scmval v) { return get_foreign_type(v)->type; }

// foreign pointer
static inline scmval s_ptr(void* o) { return make_ptr(SCM_TYPE_FOREIGN_PTR, o); }
static inline void*  c_ptr(scmval v) { return v.o; }

// standard lib
void init_foreign(scmval);

scmval foreign_call(scmval, int, scmval*);


