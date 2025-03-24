%{
  #include <string.h>
  #include "parser.h"

  extern void yyerror(char *);

  char errmsg[256]="";
  const char charerr[] = "\x1b[1m\x1b[31m[erreur lexicale]\x1b[0m caractère \x1b[41m%c\x1b[0m inattendu";
  
  /* MACRO défini 
   * Action executee avant chaque action semantique (meme vide)  
   * et rempli la variable Bison `yylloc` avec la position du token
   */
#define YY_USER_ACTION                                             \
  yylloc.first_line = yylloc.last_line;                            \
  yylloc.first_column = yylloc.last_column;                        \
  if (yylloc.last_line == yylineno)                                \
    yylloc.last_column += yyleng;                                  \
  else {                                                           \
    yylloc.last_line = yylineno;                                   \
    yylloc.last_column = 1;					   \
  }


  

%}
  
%option nounput
%option noinput
%option yylineno

NOMBRE         ([1-9][0-9]*|0)
LETTRE          [a-zA-Z]
ID {LETTRE}({LETTRE}|{NOMBRE})*

COMMENTAIRE_LIGNE   \/\/[^\n]*



%%


{COMMENTAIRE_LIGNE} {/*gestion de commentaire ligne*/}



"MAIN"      {return MAIN;}
"DEBUT"     {return DEBUT;}
";"         {return SEP;}
"FIN"       {return FIN;}
"VAR"       {return VAR;}
"TQ"        {return TQ;}
"FTQ"       {return FTQ;}
"FAIRE"     {return FAIRE;}
"SI"        {return SI;}
"SINON"     {return SINON;}
"ALORS"     {return ALORS;}
"FSI"       {return FSI;}
"FCT"       {return FCT;}
"PRINT"     {return PRINT;}
"<-"        {return FLECHE;}
">="        {return SUPEGAL;}
"<="        {return INFEGAL;}
">"         {return SUP;}
"<"         {return INF;}
"=="        {return EGAL;}
"!="        {return DIFF;}
"NON"       {return NON;}
"ET"        {return ET;}
"OU"        {return OU;}
"VRAI"      {return VRAI;}
"FAUX"      {return FAUX;}


[-+*/%(),]  {return yytext[0];}
{NOMBRE}        { yylval.nb = atoi(yytext); return NB; }
{ID}          {strcpy(yylval.id,yytext); return ID;}
[ \t\n] {}

.               {           
                  sprintf(errmsg,charerr, yytext[0]);
		  yyerror(errmsg);
		  return 1;
                }

%%
 
