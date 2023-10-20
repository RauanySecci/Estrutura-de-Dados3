// Karine Cerqueira Nascimento 13718404
// Rauany Martinez Secci 13721217

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "estrutura.h"
#include "funcoesFornecidas.h"
#include "leitura.h"
#include "funcionalidades.h"
#include "registro.h"
#include "arquivo.h"
#include "funcoes.h"

//podemos ver na main quando a pessoa digitar 1 é essa funcionalidade que será chamada
void funcionalidade1(char *a1, char *a2){
    FILE *arquivoEntrada = fopen(a1, "r"); //definição do arquivoEntrada como leitura, passaremos ele como parametro para a funcao create_table
    FILE *arquivoSaida = fopen(a2, "wb"); //definição do arquivoEntrada como escrita binaria, passaremos ele como parametro para a funcao create_table

    if (arquivoEntrada == NULL || arquivoSaida == NULL) {
        printf("Falha no processamento do arquivo.\n"); //verificacao de existencia dos arquivos
    }else{ //caso exista
        Registro *aux_r = malloc(sizeof(Registro)); //cria registro que será passado como parametro para a função create_table
        Cabecalho *aux_c = malloc(sizeof(Cabecalho)); //cria cabeçalho que será passado como parametro para a função create_table
        create_table(arquivoEntrada, arquivoSaida, aux_r, aux_c); //chama a função create_table, explicada no arquivo.c
        
        //libera memoria
        free(aux_r); 
        free(aux_c);
        //fecha arquivo
        fclose(arquivoEntrada); 
        fclose(arquivoSaida);
        binarioNaTela(a2); //chama função fornecida binarionaTela
    }
}

//podemos ver na main quando a pessoa digitar 2 é essa funcionalidade que será chamada
void funcionalidade2(char* a2){
    FILE *arquivoEntrada = fopen(a2, "rb"); //definição do arquivoEntrada como leitura binaria, passaremos ele como parametro para a funcao select_funcao
    if (arquivoEntrada == NULL) {
        printf("Falha no processamento do arquivo.\n"); //verificacao de existencia dos arquivos
    }else{
        Cabecalho *aux_c = malloc(sizeof(Cabecalho)); //cria cabeçalho afim de ler seu primeiro byte, que dirá se o arquivo é consistente '1' ou inconsistente '0'
        fread(&aux_c->status, 1, 1, arquivoEntrada);
        if(aux_c->status == '0'){
            printf("Falha no processamento do arquivo.\n"); //caso seja inconsistente
        }else{ //caso seja consistente
            select_funcao(arquivoEntrada);  //chama a função select_funcao, explicada no arquivo.c
            fclose(arquivoEntrada);//fecha arquivo
        }
        free(aux_c);//libera memoria
    }
}

//podemos ver na main quando a pessoa digitar 3 é essa funcionalidade que será chamada
void funcionalidade3(char* a3, int n){
    FILE *arquivoEntrada = fopen(a3, "rb"); //definição do arquivoEntrada como leitura binaria, passaremos ele como parametro para a funcao select_where
    if (arquivoEntrada == NULL) {
        printf("Falha no processamento do arquivo.\n"); //verificacao de existencia dos arquivos
    }else{
        Cabecalho *aux_c = malloc(sizeof(Cabecalho)); //cria cabeçalho afim de ler seu primeiro byte, que dirá se o arquivo é consistente '1' ou inconsistente '0'
        fread(&aux_c->status, 1, 1, arquivoEntrada);
        if( aux_c->status == '0'){
            printf("Falha no processamento do arquivo.\n"); //caso seja inconsistente
        }else{ //caso seja consistente
            select_where(arquivoEntrada, n); //chama a função select_where, explicada no arquivo.c
            fclose(arquivoEntrada); //fecha arquivo
        }  
        free(aux_c); //libera memoria
    } 
}

//podemos ver na main quando a pessoa digitar 4 é essa funcionalidade que será chamada
void funcionalidade4(char* a4, int nRRN){
    FILE *arquivoEntrada = fopen(a4, "rb"); //definição do arquivoEntrada como leitura binaria, passaremos ele como parametro para a funcao recupera_registro
    
    if (arquivoEntrada == NULL) {
        printf("Falha no processamento do arquivo.\n"); //verificacao de existencia dos arquivos
    }else{
        Cabecalho *aux_c = malloc(sizeof(Cabecalho)); //cria cabeçalho afim de ler seu primeiro byte, que dirá se o arquivo é consistente '1' ou inconsistente '0'
        fread(&aux_c->status, 1, 1, arquivoEntrada);
        if(aux_c->status == '0'){
            printf("Falha no processamento do arquivo.\n"); //caso seja inconsistente
        }else{ //caso seja consistente
            recupera_registro(arquivoEntrada, nRRN); //chama a função recupera_registro, explicada no registro.c
            fclose(arquivoEntrada); //fecha arquivo
        } 
        free(aux_c); //libera memoria
    }  
}