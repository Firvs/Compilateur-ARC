#include "../include/tabsymb.h"
#include <string.h>
#include <stdio.h>







int INIT_TABSYMB(ts tabsymb){     

    for(int i=0;i<128;i++){    
            strcpy(tabsymb[i].id,""); 
            strcpy(tabsymb[i].type,"");
            tabsymb[i].adresse=0;
            tabsymb[i].CTXT[0]='\0';  
    }                            
    return 0;                              
}              




int chercher_id(ts tabsymb, char * id,char * contexte){
    for(int i=0;i<128;i++){
        if(strcmp(tabsymb[i].id,id)==0  && strcmp(tabsymb[i].CTXT,contexte)==0){
            return i;
        }
    }
    return -1;
}


int ajouter_id(ts tabsymb,char * id,char * type,char * contexte){
    if (chercher_id(tabsymb, id, contexte) != -1) {
        
        return -1; // Erreur : doublon
    }
    int i;
    for(i=0;i<128;i++){
        if((strcmp(tabsymb[i].id,""))==0){
            strcpy(tabsymb[i].id,id);
            strcpy(tabsymb[i].type,type);
            strcpy(tabsymb[i].CTXT,contexte);
            tabsymb[i].adresse=i;
            return  1;
        }
    }
    
    return -1;
}


void Print_tabsymb(ts tabsymb){
    printf("Table des symboles :\n");
    printf("-------------------------------------------------\n");
    printf("| %-15s | %-8s  | %-8s | %-8s\n", 
           "Identifiant", "CTXT", "Adresse","TYPE" );
    printf("-------------------------------------------------\n");

    for(int i = 0 ; i < 128 ; i++) {
        if(tabsymb[i].id[0]!='\0'){
            
            printf("| %-15s | %-8s | %-8d |%-8s\n", 
                   tabsymb[i].id, 
                   tabsymb[i].CTXT, 
                   tabsymb[i].adresse,
                   tabsymb[i].type);
        }
    }
    printf("-------------------------------------------------\n");
}
   