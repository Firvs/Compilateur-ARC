#include "../include/semantic.h"
#include "../include/ast.h"



void semantic(ast *p){
    switch (p->type) {
        case TYPE_PROG:
            if(p->noeud[0]!=NULL){semantic(p->noeud[0]);p->codelen += p->noeud[0]->codelen;}
            if(p->noeud[1]!=NULL){semantic(p->noeud[1]);p->codelen += p->noeud[1]->codelen;}
            break;
        case TYPE_MAIN:
            if(p->noeud[0]!=NULL){semantic(p->noeud[0]);p->codelen = p->noeud[0]->codelen;}
            break;
        case AST_NB:
            if(((p->valeur >= 16384)&& (p->valeur <= -16383)) )
            {fprintf(stderr, "Erreur : VALEUR de %d  depasse la limite geré par la machine RAM\n",p->valeur);exit(EXIT_FAILURE);}
             
            p->codelen= 1 + NB_INST_EMPILER;
        break;
        case TYPE_ID:
            p->codelen= 1 + NB_INST_EMPILER;
        break;
        case TYPE_LISTinst:
            semantic(p->noeud[0]);
            if(p->noeud[1]!=NULL){
                semantic(p->noeud[1]);
                p->codelen = p->noeud[0]->codelen + p->noeud[1]->codelen;
            }
            else{
                p->codelen = p->noeud[0]->codelen;
            }
        break;
        case TYPE_OP:
            semantic(p->noeud[0]);
            semantic(p->noeud[1]);
            p->codelen=(p->noeud[0]->codelen )+ (p->noeud[1]->codelen)+ 
            ( NB_INST_EMPILER) + (2 * NB_INST_DEPILER) +  2 ;
        break;
        case TYPE_TQ:
            semantic(p->noeud[0]);
            semantic(p->noeud[1]);
            p->codelen=p->noeud[0]->codelen + p->noeud[1]->codelen + 
            NB_INST_DEPILER + 2  + 1 /*NOP*/ ;
        break;
        case TYPE_SI:
            semantic(p->noeud[0]);
            semantic(p->noeud[1]);

            if(p->noeud[2]!=NULL){
            semantic(p->noeud[2]);
            p->codelen=p->noeud[0]->codelen + p->noeud[1]->codelen +
            p->noeud[2]->codelen + NB_INST_DEPILER +2;}
            else {
                p->codelen=p->noeud[0]->codelen + p->noeud[1]->codelen + NB_INST_DEPILER + 1 ;   ;         }
        break;
        case TYPE_PRINT:
            semantic(p->noeud[0]);
            p->codelen = p->noeud[0]->codelen + 2 + NB_INST_DEPILER ;
            break;
        case TYPE_AFFECT:
            semantic(p->noeud[0]);
            p->codelen += p->noeud[0]->codelen  + 1  ;
            break;
        case TYPE_COMP:
            semantic(p->noeud[0]);
            semantic(p->noeud[1]);
            switch(p->opeC){
                case OPC_SUP:
                    p->codelen = (p->noeud[0]->codelen) + (p->noeud[1]->codelen)+
                    ( NB_INST_EMPILER) + (2 * NB_INST_DEPILER) + 5 + 1;
                break;
                case OPC_INF:
                    p->codelen = (p->noeud[0]->codelen) + (p->noeud[1]->codelen)+
                    ( NB_INST_EMPILER) + (2 * NB_INST_DEPILER) + 5 + 1;
                break;
                case OPC_SUPEGAL:
                    p->codelen = (p->noeud[0]->codelen) + (p->noeud[1]->codelen)+
                    ( NB_INST_EMPILER) + (2 * NB_INST_DEPILER) + 6 + 1;
                break;
                case OPC_INFEGAL:
                    p->codelen = (p->noeud[0]->codelen) + (p->noeud[1]->codelen)+
                    ( NB_INST_EMPILER) + (2 * NB_INST_DEPILER) + 6 + 1;
                break;
                case OPC_EGAL:
                    p->codelen = (p->noeud[0]->codelen) + (p->noeud[1]->codelen)+
                    ( NB_INST_EMPILER) + (2 * NB_INST_DEPILER) + 5 + 1;
                break;
                case OPC_DIFF:
                    p->codelen = (p->noeud[0]->codelen) + (p->noeud[1]->codelen)+
                    ( NB_INST_EMPILER) + (2 * NB_INST_DEPILER) + 5 + 1;
                break;
        }
        break;

        case TYPE_OP_BOOL:
            switch(p->opB){
                case OP_ET:
                semantic(p->noeud[0]);p->codelen +=p->noeud[0]->codelen;
                semantic(p->noeud[1]);p->codelen +=p->noeud[1]->codelen;
                p->codelen +=  7 + ( 2 * NB_INST_DEPILER) + NB_INST_EMPILER ;
                break;
                case OP_OU:
                semantic(p->noeud[0]);p->codelen +=p->noeud[0]->codelen;
                semantic(p->noeud[1]);p->codelen +=p->noeud[1]->codelen;
                p->codelen += 6 + ( 2 * NB_INST_DEPILER) + NB_INST_EMPILER ;
                break;
                case OP_NON:
                semantic(p->noeud[0]);p->codelen +=p->noeud[0]->codelen;
                p->codelen += 5 + NB_INST_DEPILER + NB_INST_EMPILER;
                break;
            }
        break;


    }
}


