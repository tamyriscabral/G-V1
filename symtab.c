#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

void iniciar_pilha(ScopeStack *pilha) {
    pilha->topo = NULL;
}

void empilhar_escopo(ScopeStack *pilha) {
    Scope *novo = (Scope *) malloc(sizeof(Scope));
    if (!novo) {
        fprintf(stderr, "Erro de alocacao de memoria\n");
        exit(1);
    }

    novo->simbolos = NULL;
    novo->prox = pilha->topo;
    pilha->topo = novo;
}

void desempilhar_escopo(ScopeStack *pilha) {
    if (!pilha->topo) return;

    Scope *escopo = pilha->topo;
    Symbol *s = escopo->simbolos;

    while (s) {
        Symbol *temp = s;
        s = s->prox;
        free(temp->nome);
        free(temp);
    }

    pilha->topo = escopo->prox;
    free(escopo);
}

Symbol *buscar_no_escopo_atual(ScopeStack *pilha, const char *nome) {
    if (!pilha->topo) return NULL;

    Symbol *s = pilha->topo->simbolos;
    while (s) {
        if (strcmp(s->nome, nome) == 0) {
            return s;
        }
        s = s->prox;
    }
    return NULL;
}

Symbol *buscar_na_pilha(ScopeStack *pilha, const char *nome) {
    Scope *escopo = pilha->topo;

    while (escopo) {
        Symbol *s = escopo->simbolos;
        while (s) {
            if (strcmp(s->nome, nome) == 0) {
                return s;
            }
            s = s->prox;
        }
        escopo = escopo->prox;
    }

    return NULL;
}

int inserir_simbolo(ScopeStack *pilha, const char *nome, TipoSimbolo tipo, int linha) {
    if (!pilha->topo) return 0;

    if (buscar_no_escopo_atual(pilha, nome) != NULL) {
        return 0;
    }

    Symbol *novo = (Symbol *) malloc(sizeof(Symbol));
    if (!novo) {
        fprintf(stderr, "Erro de alocacao de memoria\n");
        exit(1);
    }

    novo->nome = strdup(nome);
    novo->tipo = tipo;
    novo->linha = linha;
    novo->prox = pilha->topo->simbolos;
    pilha->topo->simbolos = novo;

    return 1;
}

void liberar_pilha(ScopeStack *pilha) {
    while (pilha->topo) {
        desempilhar_escopo(pilha);
    }
}