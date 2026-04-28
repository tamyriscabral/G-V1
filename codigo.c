#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "codigo.h"

static int temp_counter = 0;

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
            gerar(no->filho2); //comandos
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
            break;
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

            return t;
        }
        
        case AST_ESCREVA: {
            int t = gerar(no->filho1);

            printf("move $a0, $t%d\n", t);
            printf("li $v0, 1\n");
            printf("syscall\n");

            break;
        }
    
    }
    
    if (no->irmao) {
        gerar(no->irmao); 
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