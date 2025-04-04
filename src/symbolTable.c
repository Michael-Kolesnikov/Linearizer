#include "symbolTable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SYMBOLS 1000

static Symbol *table[MAX_SYMBOLS];
static int symbol_count = 0;

void symtab_init() {
    symbol_count = 0;
}

void symtab_free() {
    for (int i = 0; i < symbol_count; i++) {
        free(table[i]->name);
        free(table[i]->datatype);
        free(table[i]);
    }
}

void symtab_add(const char *name, SymbolType type, const char *datatype) {
    if (symtab_lookup(name)) return; // avoid duplicates
    if (symbol_count >= MAX_SYMBOLS) return;

    Symbol *s = malloc(sizeof(Symbol));
    s->name = strdup(name);
    s->type = type;
    s->datatype = datatype ? strdup(datatype) : NULL;
    table[symbol_count++] = s;
}

Symbol *symtab_lookup(const char *name) {
    for (int i = 0; i < symbol_count; i++) {
        if (strcmp(table[i]->name, name) == 0)
            return table[i];
    }
    return NULL;
}

void symtab_print() {
    printf("Symbol Table:\n");
    for (int i = 0; i < symbol_count; i++) {
        printf("  %s (%s) - type: %s\n",
               table[i]->name,
               table[i]->type == SYM_TYPEDEF ? "typedef" : "identifier",
               table[i]->datatype ? table[i]->datatype : "unknown");
    }
}
