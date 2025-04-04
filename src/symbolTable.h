#ifndef SYMTAB_H
#define SYMTAB_H

typedef enum {
    SYM_TYPEDEF,
    SYM_IDENTIFIER
} SymbolType;

typedef struct {
    char *name;
    SymbolType type;
    char *datatype;
} Symbol;

void symtab_init();
void symtab_free();
void symtab_add(const char *name, SymbolType type, const char *datatype);
Symbol *symtab_lookup(const char *name);
void symtab_print();

#endif
