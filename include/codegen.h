#ifndef CODEGEN_H
#define CODEGEN_H

#include "ast.h"
#include "tabsymb.h"
#include <stdio.h>
#include <stdlib.h>


#define NB_INST_EMPILER 1 
#define NB_INST_DEPILER 1

#define __PREMIER_ADR__ 2
#define __REG_TMP__ 1  //REGISTRE TEMP POUR DEPILER


extern FILE * out;




#define EMPILER(){ \
    fprintf(out,"STORE %d ; empiler ; numero instruction %d \n",PILE++,NUM_INST);NUM_INST++;\
}
#define DEPILER(){\
    fprintf(out,"LOAD %d ; depiler ; numero instruction %d \n",--PILE,NUM_INST);NUM_INST++;\
}

void codegen(ast *p);

void codegenINIT(ts tabsymb);






#endif