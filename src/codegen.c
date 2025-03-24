#include "../include/codegen.h"
#include "../include/ast.h"
#include "../include/tabsymb.h"


#include <stdio.h>
#include <string.h>




static void codegenNB(ast *p);
static void codegenOP(ast *p);
static void codegenLIST_INST(ast * p);
static void codegenCOMP(ast *p);
static void codegenID(ast *p);
static void codegenAFFECT(ast *p);
static void codegenTQ(ast * p);
static void codegenSisinon(ast * p);
static void codegenFCT(ast * p);
static void codegenAPPEL_FCT(ast * p );
static void codegenPARAM(ast * p);
static void codegenMAIN(ast * p);
static void codegenPROG(ast * p);
static void codegenPRINT(ast * p);
static void codegenBOOL(ast * p);

int PILE = __PREMIER_ADR__;


int NUM_INST = 0; // numero d instruction





void codegenINIT(ts tabsymb){
    //Fonction qui : compte le nombre de variable global 
    //dans la table de symble puis il rajoute a Pile 
    // POUR SAVOIR OU COMMENCE LA PILE 
    int nb_var_global = 0;
    for(int i=0;i<128;i++){
        if((strcmp(tabsymb[i].id,"")!=0)){ // Si l'ID n'est pas vide
            nb_var_global++;
        }
    }
    PILE = PILE + nb_var_global;
    printf("nombre de variable global: %d \n",nb_var_global);
    printf("Initialisation de la pile à l'adresse : %d\n", PILE);

}




void codegen(ast *p){
    
    if(p==NULL){return;}
    
    switch (p->type) {
        case TYPE_PROG:
            codegenPROG(p);
            break;
        case AST_NB:
            codegenNB(p);
            break;
        case TYPE_MAIN:
            codegenMAIN(p);
            break;
        case TYPE_LISTinst:
            codegenLIST_INST(p);
            break;
        case TYPE_OP:
            codegenOP(p);
            break;
        case TYPE_ID:
            codegenID(p);
            break;
        case TYPE_AFFECT:
            codegenAFFECT(p);
            break;
        case TYPE_TQ:
            codegenTQ(p);
            break;
        case TYPE_COMP:
            codegenCOMP(p);
            break;
        case TYPE_OP_BOOL:
            codegenBOOL(p);
            break;
        case TYPE_SI:
            codegenSisinon(p);
            break;
        case TYPE_FCT:
            codegenFCT(p);
            break;
        case TYPE_APPEL_FCT:
            codegenAPPEL_FCT(p);
            break;
        case TYPE_PARAM:
            codegenPARAM(p);
            break;
        case TYPE_PRINT:
            codegenPRINT(p);
            break;
        
    }
}

static void codegenPROG(ast *p){
    if(p->noeud[0]!=NULL){
        codegen(p->noeud[0]);
    }
    if(p->noeud[1]!=NULL){
        codegen(p->noeud[1]);
    }
}

static void codegenMAIN(ast * p){
    codegen(p->noeud[0]);
}

static void codegenAPPEL_FCT(ast * p){
    return;
}

static void codegenFCT(ast * p ){
    return ;
}
static void codegenPARAM(ast * p){
    return ; 
}



static void codegenLIST_INST(ast * p){
    
    if(p->noeud[0]!=NULL){
        codegen(p->noeud[0]);
    }
    
   if(p->noeud[1]!=NULL){
    codegen(p->noeud[1]);
   }
   
}



static void codegenNB(ast *p){
    /*code pour nb*/
    fprintf(out,"LOAD #%d  ; codegen NB\n",p->valeur);NUM_INST++;
    EMPILER();
}

static void codegenID(ast * p){
    //chercher l Id Dans table de symb chercher sont adresse 
    int adr_ID = chercher_id(TABSYMB,p->id,"GLOBAL");
    int adr_ID_RAM = adr_ID + __PREMIER_ADR__ ;
    fprintf(out,"LOAD %d  ; codegen ID %s\n",adr_ID_RAM,p->id);NUM_INST++;
    EMPILER(); 
    
}

static void codegenAFFECT(ast *p){
    /*code pour affectation*/
    codegen(p->noeud[0]);//code de l'exp a affecter
    int adr_ID = chercher_id(TABSYMB,p->id,"GLOBAL");
    int adr_ID_RAM = adr_ID + __PREMIER_ADR__ ;
    fprintf(out,"STORE %d ; codegen   affectation de %s \n",adr_ID_RAM,p->id);NUM_INST++;

}




static void codegenOP(ast *p){
    /*code pour les operation Arithmetiques */
    codegen(p->noeud[0]);
    codegen(p->noeud[1]);
    DEPILER();
    fprintf(out,"STORE %d\n",__REG_TMP__);NUM_INST++;
    DEPILER();
    switch (p->ope) {
        case '+':
        fprintf(out,"ADD %d\n",__REG_TMP__);NUM_INST++;
        break;
        case '-':
        fprintf(out,"SUB %d\n",__REG_TMP__);NUM_INST++;
        break;
        case '*':
        fprintf(out,"MUL %d\n",__REG_TMP__);NUM_INST++;
        break;
        case '/':
        fprintf(out,"DIV %d\n",__REG_TMP__);NUM_INST++;
        break;
        case '%':
        fprintf(out,"MOD %d\n",__REG_TMP__);NUM_INST++;
        break;       
    }
    EMPILER(); 
}

static void codegenCOMP(ast * p){
    /*Generer le code pour les operation de comparaison*/
    codegen(p->noeud[0]);
    codegen(p->noeud[1]);
    DEPILER();
    fprintf(out,"STORE %d; dans TMP \n",__REG_TMP__);NUM_INST++;
    DEPILER();
    switch(p->opeC){
        case OPC_SUP:
        fprintf(out,"SUB %d ; >   ; numero inst == %d\n",__REG_TMP__,NUM_INST);NUM_INST++;
        fprintf(out,"JUMG %d ;      numero inst  ===== %d\n",NUM_INST + 3 ,NUM_INST );NUM_INST++;
        fprintf(out,"LOAD #0 ; > numero inst == %d\n",NUM_INST);NUM_INST++;
        fprintf(out,"JUMP %d ; > numero inst == %d\n",NUM_INST + 2,NUM_INST );NUM_INST++;
        fprintf(out,"LOAD #1 ; > nuemro inst == %d\n",NUM_INST);NUM_INST++;
        break;
        case OPC_INF:
        fprintf(out,"SUB %d  ; < ; numero inst === %d \n", __REG_TMP__ , NUM_INST);NUM_INST++;
        fprintf(out,"JUML %d ; codegen < ;numero inst ==== %d\n",NUM_INST+3,NUM_INST);NUM_INST++;
        fprintf(out,"LOAD #0 ; < ; numero inst ==== %d\n", NUM_INST);NUM_INST++;
        fprintf(out,"JUMP %d ; < ; numero inst ==== %d\n",NUM_INST + 2,NUM_INST);NUM_INST++;
        fprintf(out,"LOAD #1 ; < ; numero inst ==== %d\n", NUM_INST);NUM_INST++;
        break;
        case OPC_SUPEGAL:
        fprintf(out,"SUB %d   ; >=\n",__REG_TMP__);NUM_INST++;
        fprintf(out,"JUMZ %d  ; codegen >=\n",NUM_INST+4 );NUM_INST++;
        fprintf(out,"JUMG %d  ; >=\n",NUM_INST+3 );NUM_INST++;
        fprintf(out,"LOAD #0  ; >=\n");NUM_INST++;
        fprintf(out,"JUMP %d  ; >=\n",NUM_INST+2);NUM_INST++;
        fprintf(out,"LOAD #1  ; >=\n");NUM_INST++;
        break;
        case OPC_INFEGAL:
        fprintf(out,"SUB %d  ; <=\n",__REG_TMP__);NUM_INST++;
        fprintf(out,"JUMZ %d ; codegen <=\n",NUM_INST+4  );NUM_INST++;
        fprintf(out,"JUML %d ; <=\n",NUM_INST+3  );NUM_INST++;
        fprintf(out,"LOAD #0 ; <=\n");NUM_INST++;
        fprintf(out,"JUMP %d ; <=\n",NUM_INST+2 );NUM_INST++;
        fprintf(out,"LOAD #1 ; <=\n");NUM_INST++;
        break;
        case OPC_EGAL:
        fprintf(out,"SUB %d  ; ==\n",__REG_TMP__);NUM_INST++;
        fprintf(out,"JUMZ %d ; codegen ==\n",NUM_INST+3);NUM_INST++;
        fprintf(out,"LOAD #0 ; ==\n");NUM_INST++;
        fprintf(out,"JUMP %d ; ==\n",NUM_INST+2);NUM_INST++;
        fprintf(out,"LOAD #1 ; ==\n");NUM_INST++;
        break;
        case OPC_DIFF:
        fprintf(out,"SUB %d  ; !=\n",__REG_TMP__);NUM_INST++;
        fprintf(out,"JUMZ %d ; codegen !=\n",NUM_INST+3);NUM_INST++;
        fprintf(out,"LOAD #1 ; !=\n");NUM_INST++;
        fprintf(out,"JUMP %d ; !=\n",NUM_INST+2);NUM_INST++;
        fprintf(out,"LOAD #0 ; !=\n");NUM_INST++;
        break;

    }
    EMPILER();
}

static void codegenBOOL(ast * p){
    
    /*Genere code pour les operations BOOLEEN*/
    switch (p->opB){
        case OP_OU:
        codegen(p->noeud[0]);
        DEPILER();
        fprintf(out,"JUMG %d ; numero instructions === %d\n",NUM_INST+8,NUM_INST);NUM_INST++;
        fprintf(out,"JUMZ %d ; numero instructions === %d\n",NUM_INST+5,NUM_INST);NUM_INST++;
        codegen(p->noeud[1]);
        DEPILER();
        fprintf(out,"JUMG %d ; numero instructions === %d\n",NUM_INST+3,NUM_INST);NUM_INST++;
        fprintf(out,"LOAD #0 ; numero instructions === %d\n",NUM_INST);NUM_INST++;
        fprintf(out,"JUMP %d ; numero instructions === %d\n",NUM_INST+2,NUM_INST);NUM_INST++;
        fprintf(out,"LOAD #1 ; numero instructions === %d\n",NUM_INST);NUM_INST++;
        break;

        case OP_ET:
        codegen(p->noeud[0]);
        DEPILER();
        fprintf(out,"JUML %d ; numero instructions === %d\n",NUM_INST+7,NUM_INST);NUM_INST++;
        fprintf(out,"JUMZ %d ; numero instructions === %d\n",NUM_INST+6,NUM_INST);NUM_INST++;
        codegen(p->noeud[1]);
        DEPILER();
        fprintf(out,"JUML %d ; numero instructions === %d\n",NUM_INST+3,NUM_INST);NUM_INST++;
        fprintf(out,"JUMZ %d ; numero instructions === %d\n",NUM_INST+2,NUM_INST);NUM_INST++;
        fprintf(out,"LOAD #1 ; numero instructions === %d\n",NUM_INST);NUM_INST++;
        fprintf(out,"JUMP %d ; numero instructions === %d\n",NUM_INST+2,NUM_INST);NUM_INST++;
        fprintf(out,"LOAD #0 ; numero instructions === %d\n",NUM_INST);NUM_INST++;

        break;
        
        case OP_NON:
        codegen(p->noeud[0]);
        DEPILER();
        fprintf(out,"JUML %d ; numero instructions === %d \n",NUM_INST+4,NUM_INST);NUM_INST++;
        fprintf(out,"JUMZ %d ; numero instructions === %d\n",NUM_INST+3,NUM_INST);NUM_INST++;
        fprintf(out,"LOAD #0 ; numero instructions === %d \n",NUM_INST);NUM_INST++;
        fprintf(out,"JUMP %d ; numero instructions === %d \n",NUM_INST+2,NUM_INST);NUM_INST++;
        fprintf(out,"LOAD #1 ; numero instructions === %d \n",NUM_INST);NUM_INST++;
        break;
    }
    EMPILER();

}



static void codegenTQ(ast *p){
    /*GENERE CODE  TQ*/
    int LONGEUR_CODE_Avant_TQ = NUM_INST ;
    codegen(p->noeud[0]);
    DEPILER();
    fprintf(out,"JUMZ %d ; BLOC TQ ; CODELEN BLOC INST  == %d\n",
                NUM_INST + p->noeud[1]->codelen + 2,  p->noeud[1]->codelen );NUM_INST++;
    codegen(p->noeud[1]);

    fprintf(out,"JUMP %d ; BLOC TQ ; CODELEN DE %s ==== %d\n ",
             LONGEUR_CODE_Avant_TQ ,p->type_str,p->codelen  );NUM_INST++; 

    fprintf(out,"NOP ; fin de boucle TQ \n");NUM_INST++;
}

static void codegenSisinon(ast * p){
    /*GENERE CODE SI SINON*/
    codegen(p->noeud[0]);
    DEPILER();
    fprintf(out,"JUMZ %d; STRUCT SI\n",NUM_INST + 7);NUM_INST++;
    codegen(p->noeud[1]);
    if(p->noeud[2]!=NULL){
        fprintf(out,"JUMP %d;STRUCT SINON\n",NUM_INST + 5 );NUM_INST++;
        codegen(p->noeud[2]);
    }
}

static void codegenPRINT(ast * p){
    /*genere code pour PRINT */
    codegen(p->noeud[0]);//l exp a PRINTER 
    DEPILER();
    fprintf(out,"WRITE ; PRINT \n");NUM_INST++;
    fprintf(out,"NOP ;FIN PRINT\n");NUM_INST++;
}