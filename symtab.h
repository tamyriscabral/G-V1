#ifndef SYMTAB_H
#define SYMTAB_H

typedef enum {
    TIPO_INT,
    TIPO_CAR
} TipoSimbolo;

typedef struct Symbol {
    char *nome;
    TipoSimbolo tipo;
    int linha;
    struct Symbol *prox;
} Symbol;

typedef struct Scope {
    Symbol *simbolos;
    struct Scope *prox;
} Scope;

typedef struct {
    Scope *topo;
} ScopeStack;

/* Pilha de escopos */
void iniciar_pilha(ScopeStack *pilha);
void empilhar_escopo(ScopeStack *pilha);
void desempilhar_escopo(ScopeStack *pilha);

/* Símbolos */
Symbol *buscar_no_escopo_atual(ScopeStack *pilha, const char *nome);
Symbol *buscar_na_pilha(ScopeStack *pilha, const char *nome);
int inserir_simbolo(ScopeStack *pilha, const char *nome, TipoSimbolo tipo, int linha);

/* Liberação total */
void liberar_pilha(ScopeStack *pilha);

#endif