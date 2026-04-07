%{
#include <stdio.h>
#include "g-v1.tab.h"
extern int yylineno;
%}

%option noyywrap
%option yylineno

%%
"principal"     { return PRINCIPAL; }
"int"           { return INT; }
"car"           { return CAR; }
"leia"          { return LEIA; }
"escreva"       { return ESCREVA; }
"novalinha"     { return NOVALINHA; }
"se"            { return SE; }
"entao"         { return ENTAO; }
"senao"         { return SENAO; }
"fimse"         { return FIMSE; }
"enquanto"      { return ENQUANTO; }

"OU"            { return OU; }
"E"             { return E; }
"=="            { return IGUAL; }
"!="            { return DIFERENTE; }
">="            { return MAIORIGUAL; }
"<="            { return MENORIGUAL; }

[0-9]+          { return INTCONST; }
"'"."'"         { return CARCONST; }
\"[^\"\n]*\"    { return CADEIACARACTERES; }
[a-zA-Z][a-zA-Z0-9_]* { return IDENTIFICADOR; }

":"             { return ':'; }
";"             { return ';'; }
","             { return ','; }
"="             { return '='; }
"{"             { return '{'; }
"}"             { return '}'; }
"("             { return '('; }
")"             { return ')'; }

"/*"            { 
                  int c;
                  while ((c = input()) != 0) {
                      if (c == '*') {
                          if ((c = input()) == '/') break;
                          else unput(c);
                      }
                  }
                  if (c == 0) printf("ERRO: COMENTÁRIO NAO TERMINA %d\n", yylineno);
                }

[ \t\r]+        { /* ignora espaços */ }
\n              { /* yylineno conta automaticamente */ }

.               { printf("ERRO: CARACTERE INVÁLIDO %d\n", yylineno); }
%%