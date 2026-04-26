#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "semantico.h"

static void erro(const char *msg, int linha) {
    printf("ERRO: %s %d\n", msg, linha);
    exit(1);
}

static TipoSimbolo analisar(AST *no, ScopeStack *pilha) {
    if (!no) return TIPO_INT;

    switch (no->tipo) {

    case AST_PROGRAMA:
        return analisar(no->filho1, pilha);

    case AST_BLOCO:
        empilhar_escopo(pilha);
        analisar(no->filho1, pilha); // VARSECTION
        analisar(no->filho2, pilha); // comandos
        desempilhar_escopo(pilha);
        break;

    case AST_VARSECTION:
        analisar(no->filho1, pilha);
        break;

    case AST_DECLVAR: {
        TipoSimbolo tipo = analisar(no->filho2, pilha);

        for (AST *id = no->filho1; id != NULL; id = id->irmao) {
            if (buscar_no_escopo_atual(pilha, id->lexema)) {
                erro("Variavel ja declarada", id->linha);
            }
            inserir_simbolo(pilha, id->lexema, tipo, id->linha);
        }
        break;
    }

    case AST_TIPO:
        if (strcmp(no->lexema, "int") == 0)
            return TIPO_INT;
        else
            return TIPO_CAR;

    case AST_LISTACOMANDO:
        analisar(no->filho1, pilha);
        break;

    case AST_COMANDO_VAZIO:
        break;

    case AST_LEIA: {
        AST *id = no->filho1;
        if (!buscar_na_pilha(pilha, id->lexema)) {
            erro("Variavel nao declarada", id->linha);
        }
        break;
    }

    case AST_ESCREVA:
        analisar(no->filho1, pilha);
        break;

    case AST_NOVALINHA:
        break;

    case AST_SE: {
        TipoSimbolo t = analisar(no->filho1, pilha);
        if (t != TIPO_INT) {
            erro("Condicao deve ser INT", no->linha);
        }
        analisar(no->filho2, pilha);
        analisar(no->filho3, pilha);
        break;
    }

    case AST_ENQUANTO: {
        TipoSimbolo t = analisar(no->filho1, pilha);
        if (t != TIPO_INT) {
            erro("Condicao deve ser INT", no->linha);
        }
        analisar(no->filho2, pilha);
        break;
    }

    case AST_ATRIB: {
        AST *id = no->filho1;
        Symbol *s = buscar_na_pilha(pilha, id->lexema);

        if (!s) {
            erro("Variavel nao declarada", id->linha);
        }

        TipoSimbolo t = analisar(no->filho2, pilha);

        if (s->tipo != t) {
            erro("Tipos incompativeis", no->linha);
        }

        return s->tipo;
    }

    case AST_IDENT: {
        Symbol *s = buscar_na_pilha(pilha, no->lexema);
        if (!s) {
            erro("Variavel nao declarada", no->linha);
        }
        return s->tipo;
    }

    case AST_INTCONST:
        return TIPO_INT;

    case AST_CARCONST:
        return TIPO_CAR;

    case AST_STRING:
        return TIPO_CAR;

    case AST_OP: {
        TipoSimbolo t1 = analisar(no->filho1, pilha);
        TipoSimbolo t2 = no->filho2 ? analisar(no->filho2, pilha) : TIPO_INT;

        // Aritméticos
        if (!strcmp(no->lexema, "+") ||
            !strcmp(no->lexema, "-") ||
            !strcmp(no->lexema, "*") ||
            !strcmp(no->lexema, "/")) {

            if (t1 != TIPO_INT || t2 != TIPO_INT)
                erro("Operacao aritmetica requer INT", no->linha);

            return TIPO_INT;
        }

        // Relacionais
        if (!strcmp(no->lexema, "==") ||
            !strcmp(no->lexema, "!=") ||
            !strcmp(no->lexema, "<") ||
            !strcmp(no->lexema, ">") ||
            !strcmp(no->lexema, "<=") ||
            !strcmp(no->lexema, ">=")) {

            if (t1 != t2)
                erro("Comparacao com tipos diferentes", no->linha);

            return TIPO_INT;
        }

        // Lógicos
        if (!strcmp(no->lexema, "||") ||
            !strcmp(no->lexema, "&")) {

            if (t1 != TIPO_INT || t2 != TIPO_INT)
                erro("Operacao logica requer INT", no->linha);

            return TIPO_INT;
        }

        // Unários
        if (!strcmp(no->lexema, "!") ||
            !strcmp(no->lexema, "unary-")) {

            if (t1 != TIPO_INT)
                erro("Operador unario requer INT", no->linha);

            return TIPO_INT;
        }

        break;
    }
    }

    if (no->irmao)
        analisar(no->irmao, pilha);

    return TIPO_INT;
}

void analisar_semantico(AST *raiz) {
    ScopeStack pilha;
    iniciar_pilha(&pilha);

    analisar(raiz, &pilha);

    liberar_pilha(&pilha);
}