#ifndef AST_H
#define AST_H

typedef struct {
    char *str;
} Atributo;

typedef struct {
    char *lexema;
    int linha;
} TokenInfo;

typedef enum {
    AST_PROGRAMA,
    AST_BLOCO,
    AST_VARSECTION,
    AST_DECLVAR,
    AST_TIPO,
    AST_LISTACOMANDO,
    AST_COMANDO_VAZIO,
    AST_LEIA,
    AST_ESCREVA,
    AST_NOVALINHA,
    AST_SE,
    AST_ENQUANTO,
    AST_ATRIB,
    AST_OP,
    AST_IDENT,
    AST_INTCONST,
    AST_CARCONST,
    AST_STRING
} AstKind;

typedef struct ast {
    AstKind tipo;
    char *lexema;
    int linha;
    struct ast *filho1;
    struct ast *filho2;
    struct ast *filho3;
    struct ast *irmao;
} AST;

AST *criar_no(AstKind tipo, const char *lexema, int linha,
              AST *filho1, AST *filho2, AST *filho3);

AST *adicionar_irmao(AST *no, AST *irmao);
void imprimir_ast(AST *no, int nivel);
void liberar_ast(AST *no);

#endif