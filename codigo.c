#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "codigo.h"

static int temp_counter = 0;
static int label_counter = 0;

static int novo_label() {
    return label_counter++;
}

static int novo_temp() {
    return temp_counter++;
}

static int gerar(AST *no) {
    if (!no){
        return -1; 
    } 

    switch (no->tipo) {
        case AST_PROGRAMA: {
            gerar(no->filho1);
            break;
        }

        case AST_BLOCO: {
            gerar(no->filho1);
            gerar(no->filho2);
            break;
        }

        case AST_VARSECTION: {
            break; 
        }

        case AST_DECLVAR: {
            break; 
        }
            
    
            
        case AST_INTCONST: {
            int t = novo_temp();
            printf("li $t%d, %s\n", t, no->lexema);
            return t;
        }

        case AST_IDENT: {
            int t = novo_temp();
            printf("lw $t%d, %s\n", t, no->lexema);
            return t;
        }

        case AST_ATRIB: {
            int t = gerar(no->filho2);
            printf("sw $t%d, %s\n", t, no->filho1->lexema);
            break;
        }

        case AST_OP: {
            int t1 = gerar(no->filho1);
            int t2 = gerar(no->filho2);

            int t = novo_temp();

            if (strcmp(no->lexema, "+") == 0)
                printf("add $t%d, $t%d, $t%d\n", t, t1, t2);
            else if (strcmp(no->lexema, "-") == 0)
                printf("sub $t%d, $t%d, $t%d\n", t, t1, t2);
            else if (strcmp(no->lexema, "*") == 0)
                printf("mul $t%d, $t%d, $t%d\n", t, t1, t2);
            else if (strcmp(no->lexema, "/") == 0)
                printf("div $t%d, $t%d, $t%d\n", t, t1, t2);
            
            // Operadores relacionais
            else if (strcmp(no->lexema, "==") == 0)
                printf("seq $t%d, $t%d, $t%d\n", t, t1, t2);
            else if (strcmp(no->lexema, "!=") == 0)
                printf("sne $t%d, $t%d, $t%d\n", t, t1, t2);
            else if (strcmp(no->lexema, "<") == 0)
                printf("slt $t%d, $t%d, $t%d\n", t, t1, t2);
            else if (strcmp(no->lexema, ">") == 0)
                printf("sgt $t%d, $t%d, $t%d\n", t, t1, t2);
            else if (strcmp(no->lexema, "<=") == 0)
                printf("sle $t%d, $t%d, $t%d\n", t, t1, t2);
            else if (strcmp(no->lexema, ">=") == 0)
                printf("sge $t%d, $t%d, $t%d\n", t, t1, t2);

            return t;
        }
        
        case AST_ESCREVA: {
            int t = gerar(no->filho1);

            printf("move $a0, $t%d\n", t);
            printf("li $v0, 1\n");
            printf("syscall\n");

            break;
        }

        case AST_SE: {
            int cond = gerar(no->filho1);

            if (no->filho3) {
                // IF com ELSE
                int lbl_else = novo_label();
                int lbl_end  = novo_label();

                printf("beq $t%d, $zero, L%d\n", cond, lbl_else);

                // THEN
                gerar(no->filho2);
                printf("j L%d\n", lbl_end);

                // ELSE
                printf("L%d:\n", lbl_else);
                gerar(no->filho3);

                // FIM
                printf("L%d:\n", lbl_end);

            } else {
                // IF sem ELSE
                int lbl_end = novo_label();

                printf("beq $t%d, $zero, L%d\n", cond, lbl_end);

                gerar(no->filho2);

                printf("L%d:\n", lbl_end);
            }

            break;
        }

        case AST_ENQUANTO: {
            int lbl_inicio = novo_label();
            int lbl_fim    = novo_label();

            printf("L%d:\n", lbl_inicio);

            int cond = gerar(no->filho1);

            // Se condição == 0 → sai do loop
            printf("beq $t%d, $zero, L%d\n", cond, lbl_fim);

            // Corpo
            gerar(no->filho2);

            // Volta pro início
            printf("j L%d\n", lbl_inicio);

            printf("L%d:\n", lbl_fim);

            break;
        }
        
        return -1;
    }
    
    // Só percorre irmãos para certos tipos
    if (no->tipo != AST_SE && no->tipo != AST_ENQUANTO) {
        if (no->irmao) {
            gerar(no->irmao);
        }
    }
}

static void gerar_data(AST *no) {
    if (!no) return;

    if (no->tipo == AST_DECLVAR) {
        for (AST *id = no->filho1; id != NULL; id = id->irmao) {
            printf("%s: .word 0\n", id->lexema);
        }
    }

    gerar_data(no->filho1);
    gerar_data(no->filho2);
    gerar_data(no->filho3);
    gerar_data(no->irmao);
}


void gerar_codigo(AST *raiz) {
    printf(".data\n");
    gerar_data(raiz);

    printf(".text\n.globl main\nmain:\n");

    gerar(raiz);

    printf("li $v0, 10\nsyscall\n");
}