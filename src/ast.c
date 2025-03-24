#include "../include/ast.h"
#include <stdio.h>
#include <string.h>

static void PrintNB(ast *p, char * indent);
static void PrintOP(ast *p,char * indent);
static void PrintID(ast *p,char * indent);
static void PrintLIST_inst(ast *p,char * indent);
static void PrintAFFECT(ast * p,char * indent);
static void PrintComp(ast *p, char * indent);

static void PrintTQ(ast *p, char * indent);
static void PrintSiSinon(ast * p , char * indent);

static void PrintFCT(ast * p , char * indent);
static void PrintLPARAM(ast * p , char * indent);
static void PrintAPPEL_FCT(ast * p , char * indent);

static void PrintProgramme(ast *p , char * indent);
static void PrintMAIN(ast * p , char * indent);

static void PrintPRINT(ast * p , char * indent);

static void PrintBOOL(ast * p,char * indent);


const char* OP_COMP_STR[]={
    ">",      // OPC_SUP
    "<",      // OPC_INF
    ">=",     // OPC_SUPEGAL
    "<=",     // OPC_INFEGAL
    "!=",     // OPC_DIFF
    "=="      // OPC_EGAL
};

const char * OP_BOOL_STR[]={
  "OU",  //OP_OU
  "ET",      //OP_ET
  "NON"     //OP_NON
};



ast * CreerFeuilleNB(int nb){
  ast * p;
  INIT_NOEUD(p);
  p->type = AST_NB;
  strcpy(p->type_str,"NB");
  p->valeur = nb;
  return p;
}
ast * CreerNoeudOP(int ope, ast * p1,ast * p2){
  ast * p;
  INIT_NOEUD(p);
  p->type=TYPE_OP;
  strcpy(p->type_str,"OP");
  p->ope=ope;
  p->noeud[0]=p1;
  p->noeud[1]=p2;
  return p;
}

ast * CreeNoeudCOMP(OP_COMPARAISON opc,ast * p1 , ast * p2){
  /*Cree le noeud pour les operations de Comparaison 
    >, < ,<=,>=, == , != */
  ast * p;
  INIT_NOEUD(p);
  p->type=TYPE_COMP;
  strcpy(p->type_str,"OP comparaison");
  p->opeC = opc;
  p->noeud[0]=p1;
  p->noeud[1]=p2;
  return p;
}
ast * CreeNoeudBOOL(OP_BOOL opb,ast * p1, ast * p2){
  ast * p ;
  INIT_NOEUD(p);
  p->type=TYPE_OP_BOOL;
  strcpy(p->type_str,"OP boolean");
  p->opB = opb;
  p->noeud[0] = p1;
  p->noeud[1] = p2;

  return p;
}


ast * CreeFeuilleID(char* id){
  ast*  p;
  INIT_NOEUD(p);
  p->type=TYPE_ID;
  strcpy(p->type_str,"ID");
  strcpy(p->id,id);

  return p;

}

ast * CreerNoeudLIST_inst(ast * g, ast * d){
  ast * p;
  INIT_NOEUD(p);
  p->type=TYPE_LISTinst;
  strcpy(p->type_str,"LIST INST");
  p->noeud[0]=g;
  p->noeud[1]=d;
  return p; 
}

ast * CreerNoeudAffect(char * id,ast * A){
  ast * p;
  INIT_NOEUD(p);
  p->type=TYPE_AFFECT;
  strcpy(p->type_str,"<-");
  strcpy(p->id,id);
  p->noeud[0]=A;

  return p;
}

ast * CreeNoeudTQ(ast * exp, ast * LINST){
  ast * p ;
  INIT_NOEUD(p);
  p->type=TYPE_TQ;
  strcpy(p->type_str,"STRUCT TQ");
  p->noeud[0] = exp;
  p->noeud[1] = LINST;
  
  return p;
}
ast * CreeNoeudSiSinon(ast * exp,ast * L1 ,ast  * L2){
  ast * p;
  INIT_NOEUD(p);
  p->type=TYPE_SI;
  strcpy(p->type_str,"SI SINON");
  p->noeud[0] = exp;
  p->noeud[1] = L1;
  p->noeud[2] = L2;

  return p;
}

ast * CreeNoeudFCT(char * ID, ast * parametre, ast * corp){
  ast * p ;
  INIT_NOEUD(p);
  p->type=TYPE_FCT;
  strcpy(p->type_str,"FONCTION");
  strcpy(p->id,ID);
  p->noeud[0] = parametre;
  p->noeud[1] = corp ; 
  return p;
}
ast * CreeNoeudList_PARAM(ast * p1 , ast * p2){
  ast * p ;
  INIT_NOEUD(p);
  p->type=TYPE_PARAM;
  strcpy(p->type_str,"List Parametre");
  p->noeud[0] = p1;
  p->noeud[1] = p2;

  return p;
}

ast * CreeNoeudAPPEL_FCT(char * id,ast * parametres){
  ast * p ; 
  INIT_NOEUD(p);
  p->type = TYPE_APPEL_FCT;
  strcpy(p->type_str,"APPEL FONCTION");
  strcpy(p->id,id);
  p->noeud[0]=parametres;
  return p ;
}

ast * CreeNoeudPROGRAMME(ast * fct,ast * main){
  ast * p ;
  INIT_NOEUD(p);
  p->type = TYPE_PROG;
  strcpy(p->type_str,"PROGRAMME PRINCIPALE");
  p->noeud[0]=fct;
  p->noeud[1]=main;

  return p;

}

ast * CreeNoeudMAIN(ast * main){
  ast * p ;
  INIT_NOEUD(p);
  p->type = TYPE_MAIN;
  strcpy(p->type_str,"MAIN");
  p->noeud[0]=main;

  return p ;
}

ast * CreeNoeudPRINT(ast * print){
  ast *p ;
  INIT_NOEUD(p);
  p->type = TYPE_PRINT;
  strcpy(p->type_str,"PRINT");
  p->noeud[0]=print;
  return p;
}


void FreeAst(ast * p){
  if (p == NULL) return;
  free(p->noeud[0]);
  if(p->noeud[0]!=NULL){free(p->noeud[1]);}
  if(p->noeud[2]!=NULL){free(p->noeud[2]);}
  free(p);
}

int profondeur = 0 ; // profondeur de larbre 

void PrintAst(ast * p){
  if (p == NULL) return;
  char indent[32] ="";
  int i=0;
  for(i=0;i<profondeur;i++){
    indent[i]='\t';
  }
  indent[i]='\0';
  switch(p->type){
  case AST_NB:
    PrintNB(p,indent);
    break;
  case TYPE_OP:
    PrintOP(p,indent);
    break;
  case TYPE_COMP:
    PrintComp(p,indent);
    break;
  case TYPE_ID:
    PrintID(p,indent);
    break;
  case TYPE_LISTinst:
    PrintLIST_inst(p,indent);
    break;
  case TYPE_AFFECT:
    PrintAFFECT(p,indent);
    break;
  case TYPE_TQ:
    PrintTQ(p,indent);
    break;
  case TYPE_SI:
    PrintSiSinon(p,indent);
    break;
  case TYPE_FCT:
    PrintFCT(p,indent);
    break;
  case TYPE_APPEL_FCT:
    PrintAPPEL_FCT(p,indent);
    break;
  case TYPE_PROG:
    PrintProgramme(p,indent);
    break;
  case TYPE_MAIN:
    PrintMAIN(p,indent);
    break;
  case TYPE_PRINT:
    PrintPRINT(p,indent);
    break;
  case TYPE_OP_BOOL:
    PrintBOOL(p,indent);
    break;
  case TYPE_PARAM:
    PrintLPARAM(p,indent);
    break;
  default:
    fprintf(stderr,"[Erreur] type <%d>: %s non reconnu\n",p->type,p->type_str);
    break;
  }
}

void ErrorAst(const char * errmsg){
  fprintf(stderr,"[AST error] %s\n",errmsg);
  exit(1);
}

static void PrintNB(ast *p, char *indent){
  printf("%s" TXT_BOLD TXT_MAGENTA "Noeud:  " TXT_NULL "%p\n",indent, p);
  printf("%s" TXT_BOLD "Type:   " TXT_NULL "%s\n",indent, p->type_str);
  printf("%s" TXT_BOLD "Valeur: " TXT_NULL "%d\n",indent, p->valeur);
}

static void PrintOP(ast *p,char *indent){
  printf("%s"TXT_BOLD TXT_BLUE "Noeud:  " TXT_NULL "%p\n",indent,p);
  printf("%s" TXT_BOLD "Type   "TXT_NULL "%s\n",indent, p->type_str);
  printf("%s"TXT_BOLD "operateur  "TXT_NULL "%c\n",indent,p->ope);
  profondeur++;
  PrintAst(p->noeud[0]);
  PrintAst(p->noeud[1]);
  profondeur--;
}

static void PrintComp(ast * p , char * indent){
  printf("%s" TXT_BOLD TXT_BLUE "Noeud:  "TXT_NULL "%p\n",indent,p);
  printf("%s" TXT_BOLD "Type:   "TXT_NULL "%s\n",indent, p->type_str);
  printf("%s"TXT_BOLD "operateur COMP  "TXT_NULL "%s\n",indent,OP_COMP_STR[p->opeC]);
  profondeur++;
  PrintAst(p->noeud[0]);
  PrintAst(p->noeud[1]);
  profondeur--;
}


static void PrintID(ast *p,char * indent){
  printf("%s" TXT_BOLD TXT_BLUE "Noeud:  "TXT_NULL "%p\n",indent,p);
  printf("%s" TXT_BOLD "Type:   "TXT_NULL "%s\n",indent, p->type_str);
  printf("%s"TXT_BOLD "identificateur:  " TXT_NULL "%s\n",indent,p->id);
}

static void PrintLIST_inst(ast * p, char * indent){
  printf("%s" TXT_BOLD TXT_BLUE "Noeud:  "TXT_NULL "%p\n",indent,p);
  printf("%s" TXT_BOLD "Type:   "TXT_NULL "%s\n",indent, p->type_str);
  printf("%s" TXT_BOLD TXT_GREEN "Noeud Gauche:  "TXT_NULL "%p\n",indent,p->noeud[0]);
  profondeur++;
  PrintAst(p->noeud[0]);
  printf("%s" TXT_BOLD TXT_GREEN "Noeud Droite:  "TXT_NULL "%p\n",indent,p->noeud[1]);
  if(p->noeud[1]!=NULL){PrintAst(p->noeud[1]);}
  profondeur--;
}





static void PrintAFFECT(ast *p, char * indent){
  printf("%s" TXT_BOLD TXT_BLUE "Noeud:  "TXT_NULL "%p\n",indent,p);
  printf("%s" TXT_BOLD "Type:   "TXT_NULL "%s\n",indent, p->type_str);
  printf("%s" TXT_BOLD TXT_GREEN " ID affecté :  "TXT_NULL "%s\n",indent,p->id);
  profondeur++;
 
  printf("%s" TXT_BOLD TXT_GREEN "expression affecter:  "TXT_NULL"%p\n",indent,p->noeud[0]);
  PrintAst(p->noeud[0]);
  profondeur--;
}


static void PrintTQ(ast * p , char * indent){
  printf("%s" TXT_BOLD TXT_BLUE "Noeud:  "TXT_NULL "%p\n",indent,p);
  printf("%s" TXT_BOLD "Type:   "TXT_NULL "%s\n",indent, p->type_str);
  profondeur++;
  PrintAst(p->noeud[0]);
  PrintAst(p->noeud[1]);
  profondeur--;
}


static void PrintSiSinon(ast * p , char * indent){
  printf("%s" TXT_BOLD TXT_BLUE "Noeud:  "TXT_NULL "%p\n",indent,p);
  printf("%s" TXT_BOLD "Type:   "TXT_NULL "%s\n",indent, p->type_str);
  profondeur++;
  PrintAst(p->noeud[0]);
  PrintAst(p->noeud[1]);
  PrintAst(p->noeud[2]);
  profondeur--;
}


static void PrintFCT(ast * p , char * indent){
  printf("%s" TXT_BOLD TXT_BLUE "Noeud:  "TXT_NULL "%p\n",indent,p);
  printf("%s" TXT_BOLD "Type:   "TXT_NULL "%s\n",indent, p->type_str);
  printf("%s" TXT_BOLD "NOM:   "TXT_NULL "%s\n",indent, p->id);
  profondeur++;
  PrintAst(p->noeud[0]);
  PrintAst(p->noeud[1]);
  profondeur--;
}
static void PrintAPPEL_FCT(ast * p , char * indent){
  printf("%s" TXT_BOLD TXT_BLUE "Noeud:  "TXT_NULL "%p\n",indent,p);
  printf("%s" TXT_BOLD "Type:   "TXT_NULL "%s\n",indent, p->type_str);
  printf("%s" TXT_BOLD "ID FONCTION : "TXT_NULL "%s\n",indent,p->id);
  profondeur++;
  PrintAst(p->noeud[0]);
  profondeur--;

}


static void PrintProgramme(ast *p , char * indent){
  printf("%s" TXT_BOLD TXT_BLUE "Noeud:  "TXT_NULL "%p\n",indent,p);
  printf("%s" TXT_BOLD "Type:   "TXT_NULL "%s\n",indent, p->type_str);
  profondeur++;
  PrintAst(p->noeud[0]);
  PrintAst(p->noeud[1]);
  profondeur--;
}

static void PrintMAIN(ast * p , char * indent){
  printf("%s" TXT_BOLD TXT_BLUE "Noeud:  "TXT_NULL "%p\n",indent,p);
  printf("%s" TXT_BOLD "Type:   "TXT_NULL "%s\n",indent, p->type_str);
  profondeur++;
  PrintAst(p->noeud[0]);
  profondeur--;
}

static void PrintPRINT(ast * p, char * indent){
  printf("%s" TXT_BOLD TXT_BLUE "Noeud:  "TXT_NULL "%p\n",indent,p);
  printf("%s" TXT_BOLD "Type:   "TXT_NULL "%s\n",indent, p->type_str);
  printf("%s" TXT_BOLD "contenu de PRINT :   "TXT_NULL "\n",indent);
  profondeur++;
  PrintAst(p->noeud[0]);
  profondeur--;
  
}

static void PrintBOOL(ast * p , char * indent){
    printf("%s" TXT_BOLD TXT_BLUE "Noeud:  "TXT_NULL "%p\n",indent,p);
    printf("%s" TXT_BOLD "Type:   "TXT_NULL "%s\n",indent, p->type_str);
    printf("%s"TXT_BOLD "operateur BOOL  "TXT_NULL "%s\n",indent,OP_BOOL_STR[p->opB]);
    profondeur++;
    PrintAst(p->noeud[0]);
    if(p->noeud[1]!=NULL){PrintAst(p->noeud[1]);}
    profondeur--;

}


static void PrintLPARAM(ast * p , char * indent){
    printf("%s" TXT_BOLD TXT_BLUE "Noeud:  "TXT_NULL "%p\n",indent,p);
    printf("%s" TXT_BOLD "Type:   "TXT_NULL "%s\n",indent, p->type_str);
    profondeur++;
    PrintAst(p->noeud[0]);
    PrintAst(p->noeud[1]);
    profondeur--;


}