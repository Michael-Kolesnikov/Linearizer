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

typedef struct SymbolNode {
    Symbol *symbol;
    struct SymbolNode *next;
} SymbolNode;

unsigned int hash(const char *str) {
void symtab_init();
void symtab_free();
void symtab_add(const char *name, SymbolType type, const char *datatype);
void symtab_set_type(const char* name, const char* datatype);
Symbol *symtab_lookup(const char *name);
void symtab_print();

#endif
