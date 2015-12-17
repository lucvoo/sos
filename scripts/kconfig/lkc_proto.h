
/* confdata.c */
void conf_parse(const char *name);
int conf_read(const char *name);
int conf_read_simple(const char *name, int);
int conf_write(const char *name);
int conf_write_autoconf(void);
bool conf_get_changed(void);
void conf_set_changed_callback(void (*fn)(void));

/* menu.c */
struct menu rootmenu;

bool menu_is_visible(struct menu *menu);
const char * menu_get_prompt(struct menu *menu);
struct menu * menu_get_root_menu(struct menu *menu);
struct menu * menu_get_parent_menu(struct menu *menu);

/* symbol.c */
struct symbol * symbol_hash[SYMBOL_HASHSIZE];

struct symbol * sym_lookup(const char *name, int isconst);
struct symbol * sym_find(const char *name);
struct symbol ** sym_re_search(const char *pattern);
const char * sym_type_name(enum symbol_type type);
void sym_calc_value(struct symbol *sym);
enum symbol_type sym_get_type(struct symbol *sym);
bool sym_tristate_within_range(struct symbol *sym,tristate tri);
bool sym_set_tristate_value(struct symbol *sym,tristate tri);
tristate sym_toggle_tristate_value(struct symbol *sym);
bool sym_string_valid(struct symbol *sym, const char *newval);
bool sym_string_within_range(struct symbol *sym, const char *str);
bool sym_set_string_value(struct symbol *sym, const char *newval);
bool sym_is_changable(struct symbol *sym);
struct property * sym_get_choice_prop(struct symbol *sym);
struct property * sym_get_default_prop(struct symbol *sym);
const char * sym_get_string_value(struct symbol *sym);

const char * prop_get_type_name(enum prop_type type);

/* expr.c */
int expr_compare_type(enum expr_type t1, enum expr_type t2);
void expr_print(struct expr *e, void (*fn)(void *, struct symbol *, const char *), void *data, int prevtoken);
