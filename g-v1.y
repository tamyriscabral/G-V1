%{
#include <stdio.h>
#include <stdlib.h>

extern int yylex();
extern int yylineno;
extern char* yytext;
extern FILE* yyin;

void yyerror(const char *s);
%}

%token PRINCIPAL INT CAR LEIA ESCREVA NOVALINHA SE ENTAO SENAO FIMSE ENQUANTO
%token OU E IGUAL DIFERENTE MAIORIGUAL MENORIGUAL
%token IDENTIFICADOR INTCONST CARCONST CADEIACARACTERES

%%
/* Gramática G-V1 Limpa */

Programa: PRINCIPAL Bloco;

Bloco: '{' VarSection ListaComando '}';

VarSection: /* vazio */ 
          | ListaDeclVar;

ListaDeclVar: IDENTIFICADOR DeclVar ':' Tipo ';' ListaDeclVar
            | IDENTIFICADOR DeclVar ':' Tipo ';';

DeclVar: ',' IDENTIFICADOR DeclVar
       | /* vazio */;

Tipo: INT | CAR;

ListaComando: Comando
            | Comando ListaComando;

Comando: Expr ';'
       | LEIA IDENTIFICADOR ';'
       | ESCREVA Expr ';'
       | ESCREVA CADEIACARACTERES ';'
       | NOVALINHA ';'
       | SE '(' Expr ')' ENTAO Comando FIMSE
       | SE '(' Expr ')' ENTAO Comando SENAO Comando FIMSE
       | ENQUANTO '(' Expr ')' Comando
       | Bloco;

Expr: OrExpr
    | IDENTIFICADOR '=' Expr;

OrExpr: OrExpr OU AndExpr | AndExpr;
AndExpr: AndExpr E EqExpr | EqExpr;
EqExpr: EqExpr IGUAL DesigExpr | EqExpr DIFERENTE DesigExpr | DesigExpr;
DesigExpr: DesigExpr '<' AddExpr | DesigExpr '>' AddExpr | AddExpr;
AddExpr: AddExpr '+' MulExpr | MulExpr;
MulExpr: MulExpr '*' UnExpr | UnExpr;
UnExpr: '-' PrimExpr | '!' PrimExpr | PrimExpr;
PrimExpr: IDENTIFICADOR | CARCONST | INTCONST | '(' Expr ')';

%%

void yyerror(const char *s) {
    fprintf(stderr, "ERRO: %s na linha %d\n", s, yylineno);
}

int main(int argc, char** argv) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            perror(argv[1]);
            return 1;
        }
    }
    yyparse();
    return 0;
}