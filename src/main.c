#include <readline/readline.h>
#include <readline/history.h>
#include "scm.h"


static void repl();

////////////////////////////////////////////////////////////////////////////////
// I N I T I A L I Z A T I O N
////////////////////////////////////////////////////////////////////////////////
static void scm_init() {
    GC_INIT();

    init_context();
    init_errors();
    init_bool();
    init_char();
    init_symbol();
    init_pair();
    init_vector();
    init_bytevector();
    init_port();
    init_reader();
    init_eval();
}

////////////////////////////////////////////////////////////////////////////////
// M A I N
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]) {
    scm_init();
    printf("scm v0.1\n");
    load("./scheme/list.scm");
    repl();
    printf("Bye.\n");
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
// R E P L
////////////////////////////////////////////////////////////////////////////////
static char** proc_names;

static char* get_history_filename() {
    static const char* filename = ".scm_history";
    char *path;
    asprintf(&path, "%s/%s", getenv("HOME"), filename);
    return path;
}

static char* proc_name_generator(const char* text, int state) {
    static int list_index, len;
    char *name;
    if(!state) {
        list_index = 0;
        len = strlen(text);
    }
    while((name = proc_names[list_index++])) {
        if(!strncmp(name, text, len))
            return strdup(name);
    }
    return NULL;
}

static char** proc_name_completion_function(const char* text, int start, int end) {
    rl_attempted_completion_over = 1;
    return rl_completion_matches(text, proc_name_generator);
}

static void init_completion() {
    scmfix  size;
    scmval* procs;
    dict_keys(scm_context.globals, &size, &procs);
    proc_names = calloc(size+1, sizeof(char*));
    for(int i = 0; i < size; i++) {
        proc_names[i] = strdup(string_to_cstr(procs[i]));
    }
    proc_names[size] = NULL;
    rl_attempted_completion_function = proc_name_completion_function;
}

static void default_error_handler(scmval err) {
    write(scm_current_error_port(), err, scm_mode_display);
    fprintf(stderr, "\n");
}

static void repl() {
    char* path = get_history_filename();
    char* line = NULL;
    scmval in, v;
    read_history(path);
    init_completion();
    while(true) {
        line = readline("> ");
        if(!line)
            break;
        if(*line == ',' && *(line+1) == 'q')
            break;
        add_history(line);
        with_error_handler(default_error_handler) {
            in = open_input_string(line);
            v  = read(in);
            v  = eval(v, scm_context.toplevel);
            if(!is_undef(v)) {
                write(scm_current_output_port(), v, scm_mode_write | scm_mode_pp_quote);
                printf("\n");
            }
        }
        free(line);
    }
    write_history(path);
}

// MISC
void dstats() {
    printf("\n** [heap: %zd] [free: %zd] [bytes-since-gc: %zd] [total: %zd]\n",
            GC_get_heap_size(),
            GC_get_free_bytes(),
            GC_get_bytes_since_gc(),
            GC_get_total_bytes());
}
