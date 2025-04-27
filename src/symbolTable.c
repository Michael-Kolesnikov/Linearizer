#include "symbolTable.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 1000

static SymbolNode *table[HASH_SIZE];

unsigned int hash(const char *str) {
    unsigned int hash_value = 0;
    while (*str) {
        hash_value = (hash_value * 31) + (unsigned char)(*str++);
    }
    return hash_value % HASH_SIZE;
}

void symtab_init() {
    for (int i = 0; i < HASH_SIZE; i++) {
        table[i] = NULL;
    }
}

void symtab_free() {
    for (int i = 0; i < HASH_SIZE; i++) {
        SymbolNode *node = table[i];
        while (node) {
            SymbolNode *temp = node;
            free(node->symbol->name);
            free(node->symbol->datatype);
            free(node->symbol);
            node = node->next;
            free(temp);
        }
    }
}

void symtab_add(const char *name, SymbolType type, const char *datatype) {
    unsigned int index = hash(name);

    SymbolNode *node = table[index];
    while (node) {
        if (strcmp(node->symbol->name, name) == 0) {
            return;
        }
        node = node->next;
    }

    Symbol *s = malloc(sizeof(Symbol));
    s->name = strdup(name);
    s->type = type;
    s->datatype = datatype ? strdup(datatype) : NULL;

    SymbolNode *new_node = malloc(sizeof(SymbolNode));
    new_node->symbol = s;
    new_node->next = table[index];
    table[index] = new_node;
}

void symtab_set_type(const char *name, const char *datatype) {
    Symbol *s = symtab_lookup(name);
    if (s) s->datatype = strdup(datatype);
}

Symbol *symtab_lookup(const char *name) {
    unsigned int index = hash(name);

    SymbolNode *node = table[index];
    while (node) {
        if (strcmp(node->symbol->name, name) == 0) {
            return node->symbol;
        }
        node = node->next;
    }

    return NULL;
}

void symtab_print() {
    printf("Symbol Table:\n");
    for (int i = 0; i < HASH_SIZE; i++) {
        SymbolNode *node = table[i];
        while (node) {
            printf("  %s (%s) - type: %s\n", node->symbol->name,
                   node->symbol->type == SYM_TYPEDEF ? "typedef" : "identifier",
                   node->symbol->datatype ? node->symbol->datatype : "unknown");
            node = node->next;
        }
    }
}