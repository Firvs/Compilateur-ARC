#ifndef TABSYMB_H
#define TABSYMB_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef enum {
    INT ,
    FUNC
}TypeSymbole;

struct tabsymb {
    char id[32];
    char type[32];
    int adresse;
    char CTXT[32];
    
};

typedef struct tabsymb ts[128];

extern ts TABSYMB;

int INIT_TABSYMB(ts tabsymb);


int chercher_id(ts tabsymb, char * id,char * contexte);
int ajouter_id(ts tabsymb,char * id,char * type,char * contexte); 
void Print_tabsymb(ts tabsymb);


#endif