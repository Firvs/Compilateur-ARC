%{
  #include <stdio.h>
  #include <ctype.h>
  #include <string.h>
  #include "codegen.h"
  #include "ast.h"
  #include "tabsymb.h"
  #include "semantic.h"
    
  extern int yylex();
  static void print_file_error(char * s, char *errmsg);

  struct ast * ARBRE_ABSTRAIT = NULL;

  FILE * out;
  ts TABSYMB;
  char CTXT[32]="GLOBAL";
  void yyerror(const char * s);

  char srcname[64];
  char exename[64] = "a.out";
  FILE * exefile;

 

%}

%union{
  int nb;
  struct ast* arbre;
  char id[32];
 };

%define parse.error detailed


%locations


%right FLECHE 

%left '+' '-'
%left '*' '/' '%'
%nonassoc SUP INF 
%right SUPEGAL INFEGAL
%right EGAL DIFF
%left OU 
%left ET 
%left NON 


%type <arbre> EXP LIST_INST INST LIST_INST_NONEMPTY
%type <arbre> AFFECT STRUCT_TQ STRUCT_SI INST_PRINT
%type <arbre> FONCTION APPEL_FCT L_PARAMETRES DECLA_FCT

%token <nb> NB VRAI FAUX
%token <id> ID
%token MAIN DEBUT FIN SEP VAR TQ FAIRE FTQ
%token SI FSI SINON ALORS
%token FCT RETOURNER 
%token FLECHE "<-"
%token PRINT
%token <arbre> SUP '>'
%token <arbre> INF '<'
%token <arbre> SUPEGAL ">="
%token <arbre> INFEGAL "<="
%token <arbre> DIFF "!="
%token <arbre> EGAL "=="




%start PROGRAMME

%%




/* PROGRAMME PRINCIPALE  */

PROGRAMME: 
FONCTION 
MAIN'('')' 
DECLA 
DEBUT 
  LIST_INST 
FIN {ARBRE_ABSTRAIT = CreeNoeudPROGRAMME($1,CreeNoeudMAIN($7));PrintAst(ARBRE_ABSTRAIT);}
; 


/* FONCTION */

FONCTION:%empty {$$=NULL;}
|FCT ID {ajouter_id(TABSYMB,$2,"FCT",CTXT);strcpy(CTXT,$2);} '('L_PARAMETRES ')' 
  DECLA_FCT 
  DEBUT 
    LIST_INST
  FIN  {$$=CreeNoeudFCT($2,$5,$9);strcpy(CTXT,"GLOBAL");}
;

L_PARAMETRES:%empty {$$=NULL;}
|ID ',' L_PARAMETRES {ajouter_id(TABSYMB,$1,"INT",CTXT);$$=CreeNoeudList_PARAM(CreeFeuilleID($1),$3);}
;

DECLA_FCT:%empty  {$$=NULL;}
|VAR ID SEP DECLA_FCT {ajouter_id(TABSYMB,$2,"INT",CTXT);}
;

APPEL_FCT: 
  ID '('L_PARAMETRES')' {$$=CreeNoeudAPPEL_FCT($1,$3);}
;


/* INSTRUCTION */

INST: EXP SEP   {$$=$1;}
|AFFECT         {$$=$1;}
|STRUCT_TQ      {$$=$1;}
|STRUCT_SI      {$$=$1;}
|APPEL_FCT      {$$=$1;}
|INST_PRINT SEP {$$=$1;}
;

INST_PRINT:PRINT EXP {$$=CreeNoeudPRINT($2);}
;
STRUCT_TQ:TQ EXP FAIRE
            LIST_INST
          FTQ {$$=CreeNoeudTQ($2,$4);}
;

STRUCT_SI:
SI EXP ALORS
  LIST_INST
SINON 
  LIST_INST
FSI  {$$=CreeNoeudSiSinon($2,$4,$6);}
|
SI EXP ALORS
    LIST_INST
FSI {$$=CreeNoeudSiSinon($2,$4,NULL);}
;

LIST_INST:
    LIST_INST_NONEMPTY
    | %empty { $$ = NULL; }
;

LIST_INST_NONEMPTY:
    INST { $$ = CreerNoeudLIST_inst($1, NULL); }
    | INST LIST_INST_NONEMPTY { $$ = CreerNoeudLIST_inst($1, $2); }
;

DECLA:%empty     
|VAR ID SEP DECLA {ajouter_id(TABSYMB,$2,"INT",CTXT);}
;

AFFECT:ID "<-" EXP SEP  {$$=CreerNoeudAffect($1,$3);}
|ID "<-" AFFECT         {$$=CreerNoeudAffect($1,$3);}
;

EXP: NB                         {$$=CreerFeuilleNB($1);}
| '-' NB                        {$$=CreerFeuilleNB(-$2);}
| NON EXP                       {$$=CreeNoeudBOOL(OP_NON,$2,NULL);}
|EXP '+' EXP                    {$$=CreerNoeudOP('+',$1,$3);}
|EXP '*' EXP                    {$$=CreerNoeudOP('*',$1,$3);}
|EXP '-' EXP                    {$$=CreerNoeudOP('-',$1,$3);}
|EXP '/' EXP                    {$$=CreerNoeudOP('/',$1,$3);}
|EXP '%' EXP                    {$$=CreerNoeudOP('%',$1,$3);}
|'('EXP')'                      {$$=$2;}
|ID                             {$$=CreeFeuilleID($1);}
|EXP SUP EXP                    {$$=CreeNoeudCOMP(OPC_SUP,$1,$3);}
|EXP INF EXP                    {$$=CreeNoeudCOMP(OPC_INF,$1,$3);}
|EXP SUPEGAL EXP                {$$=CreeNoeudCOMP(OPC_SUPEGAL,$1,$3);}
|EXP INFEGAL EXP                {$$=CreeNoeudCOMP(OPC_INFEGAL,$1,$3);}
|EXP EGAL EXP                   {$$=CreeNoeudCOMP(OPC_EGAL,$1,$3);}
|EXP DIFF EXP                   {$$=CreeNoeudCOMP(OPC_DIFF,$1,$3);}
|EXP OU EXP                     {$$=CreeNoeudBOOL(OP_OU,$1,$3);}
|EXP ET EXP                     {$$=CreeNoeudBOOL(OP_ET,$1,$3);}
|VRAI                           {$$=CreerFeuilleNB(1);}
|FAUX                           {$$=CreerFeuilleNB(0);}
;

%%

int main( int argc, char * argv[] ) {
  
  extern FILE *yyin;
  
  if (argc > 1){
    strcpy(srcname, argv[1]);
    if ( (yyin = fopen(srcname,"r"))==NULL ){
      char errmsg[256];
      sprintf(errmsg,"fichier \x1b[1m\x1b[33m' %s '\x1b[0m introuvable",srcname);
      print_file_error(argv[0],errmsg);
      exit(1);
    }
  }  else {
    print_file_error(argv[0],"aucun fichier en entree");
    exit(1);
  }
  if (argc == 3){
    strcpy(exename, argv[2]);
  }
  
  out = fopen("out.ram", "w");
    if (out == NULL) {
        fprintf(stderr, "Erreur : impossible d'ouvrir le fichier out.ram pour écrire.\n");
        exit(1);
    }
  

   /*INIT TABLE SYMBOLE*/

  INIT_TABSYMB(TABSYMB);

  
  
  yyparse();

  codegenINIT(TABSYMB);

  /*analyse semantique */
  semantic(ARBRE_ABSTRAIT);


  printf("############### TABLE DE SYMBOLE #################\n");
  Print_tabsymb(TABSYMB);

  printf("codelen PROGRAMME === %d \n",ARBRE_ABSTRAIT->codelen);

  printf("###########  CODE GENERER #######\n");


  /*generation de code */
  codegen(ARBRE_ABSTRAIT);


  FreeAst(ARBRE_ABSTRAIT);




  fclose(yyin);
}



static void print_file_error(char * prog, char *errmsg){
  fprintf(stderr,
	  "\x1b[1m%s:\x1b[0m \x1b[31m\x1b[1merreur fatale:\x1b[0m %s\nechec de la compilation\n",
	  prog, errmsg);
}

void yyerror(const char * s)
{
  fprintf(stderr, "\x1b[1m%s:%d:%d:\x1b[0m %s\n", srcname, yylloc.first_line, yylloc.first_column, s);
  exit(0);
}
