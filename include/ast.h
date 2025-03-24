#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TXT_MAGENTA "\x1b[35m"
#define  TXT_RED    "\x1b[31m"
#define  TXT_GREEN  "\x1b[32m"
#define  TXT_BLUE   "\x1b[34m"
#define  TXT_BOLD   "\x1b[1m"
#define  TXT_NULL   "\x1b[0m"

#define INIT_NOEUD(p)   if ((p = malloc(sizeof(ast))) == NULL)	\
    ErrorAst("echec allocation mémoire");		 \
  else {						                	        \
    p->type = 0;					    	               \
    p->type_str[0] = '\0';				            	\
    p->valeur = 0;	                             \
    p->ope = 0 ;                                  \
    p->opeC = OPC_SUP;                              \
    p->opB =  OP_OU;                                 \
    p->id[0]='\0';                                   \
    p->noeud[0]=NULL;					                        \
    p->noeud[1]=NULL;                                  \
    p->noeud[2]=NULL;                                   \
    p->codelen = 0 ;                                     \
    }								                                      \
\

enum {AST_NB = 256, TYPE_OP,TYPE_COMP,TYPE_ID,TYPE_OP_BOOL,TYPE_LISTinst,TYPE_AFFECT,
        TYPE_PRINT,TYPE_TQ,TYPE_SI,TYPE_FCT,TYPE_PARAM,
        TYPE_APPEL_FCT,TYPE_PROG,TYPE_MAIN};

typedef enum {OPC_SUP,OPC_INF,OPC_SUPEGAL,OPC_INFEGAL,OPC_DIFF,OPC_EGAL} OP_COMPARAISON;

typedef enum {OP_OU,OP_ET,OP_NON}OP_BOOL;

extern const char* OP_COMP_STR[];
extern const char* OP_BOOL_STR[];

typedef struct ast{
  int  type;      //type de Noeud
  char type_str[32];  //Nom de Noeud 
  int valeur;         //valeur de Nb 
  int ope;            //operateur arithmetique + - / % * 
  OP_COMPARAISON opeC ;  //operateur de Comparaison >,<,>=,<=,==,!=
  OP_BOOL opB         ;      // operateur Boolean ET OU NON 
  char id[32];          //id pour identificateur
  struct ast * noeud[3]; //les feuilles
  int codelen ;          //longeur de code
   

}ast;


ast * CreerFeuilleNB(int nb);
ast * CreerNoeudOP(int ope, ast * p1,ast * p2);
ast * CreeNoeudCOMP(OP_COMPARAISON opc, ast * p1 , ast * p2);
ast * CreeFeuilleID(char* id);

ast * CreerNoeudLIST_inst(ast * g, ast * d);
ast * CreerNoeudAffect(char * id,ast * A);
ast * CreeNoeudTQ(ast * exp, ast * LINST);
ast * CreeNoeudSiSinon(ast * exp,ast * L1 ,ast  * L2);
ast * CreeNoeudFCT(char * ID, ast * parametre, ast * corps);
ast * CreeNoeudList_PARAM(ast * p1 , ast * p2);
ast * CreeNoeudAPPEL_FCT(char * id,ast * parametre);
ast * CreeNoeudPROGRAMME(ast * fct,ast * main);
ast * CreeNoeudMAIN(ast *main);
ast * CreeNoeudPRINT(ast * print);
ast * CreeNoeudBOOL(OP_BOOL opb,ast * p1, ast * p2);

void FreeAst(ast * p);

void PrintAst(ast * p);
void ErrorAst(const char * errmsg);


#endif
