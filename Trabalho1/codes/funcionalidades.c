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
    FILE *arquivoEntrada = fopen(a1, "r"); //definição do arquivoEntrada como leitura, passaremos ele como parametro para a funcao criar_tabela
    FILE *arquivoSaida = fopen(a2, "wb"); //definição do arquivoEntrada como escrita binaria, passaremos ele como parametro para a funcao criar_tabela

    if (arquivoEntrada == NULL)  {
        printf("Falha no processamento do arquivo.\n"); //verificacao de existencia dos arquivos
        return;
    }
    if(arquivoSaida == NULL){
        printf("Falha no processamento do arquivo.\n"); //verificacao de existencia dos arquivos
        return;
    }
    
        Registro *aux_r = malloc(sizeof(Registro)); //cria registro que será passado como parametro para a função criar_tabela
        Cabecalho *aux_c = malloc(sizeof(Cabecalho)); //cria cabeçalho que será passado como parametro para a função criar_tabela
        criar_tabela(arquivoEntrada, arquivoSaida, aux_r, aux_c); //chama a função criar_tabela, explicada no arquivo.c
        
        //libera memoria
        free(aux_r); 
        free(aux_c);
        //fecha arquivo
        fclose(arquivoEntrada); 
        fclose(arquivoSaida);
        binarioNaTela(a2); //chama função fornecida binarionaTela
    
}

//podemos ver na main quando a pessoa digitar 2 é essa funcionalidade que será chamada
void funcionalidade2(char* a2){
    FILE *arquivoEntrada = fopen(a2, "rb"); //definição do arquivoEntrada como leitura binaria, passaremos ele como parametro para a funcao selecionar_funcao
    if (arquivoEntrada == NULL) {
        printf("Falha no processamento do arquivo.\n"); //verificacao de existencia dos arquivos
        return;
    }
        Cabecalho *aux_c = malloc(sizeof(Cabecalho)); //cria cabeçalho afim de ler seu primeiro byte, que dirá se o arquivo é consistente '1' ou inconsistente '0'
        fread(&aux_c->status, 1, 1, arquivoEntrada);
        if(aux_c->status == '0'){
            printf("Falha no processamento do arquivo.\n"); //caso seja inconsistente
        }else{ //caso seja consistente
            selecionar_funcao(arquivoEntrada);  //chama a função selecionar_funcao, explicada no arquivo.c
            
        }
        fclose(arquivoEntrada);//fecha arquivo
        free(aux_c);//libera memoria
    
}

//podemos ver na main quando a pessoa digitar 3 é essa funcionalidade que será chamada
void funcionalidade3(char* a3, int n){
    FILE *arquivoEntrada = fopen(a3, "rb"); //definição do arquivoEntrada como leitura binaria, passaremos ele como parametro para a funcao selecionar_onde
    if (arquivoEntrada == NULL) {
        printf("Falha no processamento do arquivo.\n"); //verificacao de existencia dos arquivos
        return;
    }
        Cabecalho *aux_c = malloc(sizeof(Cabecalho)); //cria cabeçalho afim de ler seu primeiro byte, que dirá se o arquivo é consistente '1' ou inconsistente '0'
        fread(&aux_c->status, 1, 1, arquivoEntrada);
        if( aux_c->status == '0'){
            printf("Falha no processamento do arquivo.\n"); //caso seja inconsistente
        }else{ //caso seja consistente
            selecionar_onde(arquivoEntrada, n); //chama a função selecionar_onde, explicada no arquivo.c
           
        } 
        fclose(arquivoEntrada); //fecha arquivo 
        free(aux_c); //libera memoria
    
}

//podemos ver na main quando a pessoa digitar 4 é essa funcionalidade que será chamada
void funcionalidade4(char* a4, int nRRN){
    FILE *arquivoEntrada = fopen(a4, "rb"); //definição do arquivoEntrada como leitura binaria, passaremos ele como parametro para a funcao recupera_registro
    
    if (arquivoEntrada == NULL) {
        printf("Falha no processamento do arquivo.\n"); //verificacao de existencia dos arquivos
        return;
    }
        Cabecalho *aux_c = malloc(sizeof(Cabecalho)); //cria cabeçalho afim de ler seu primeiro byte, que dirá se o arquivo é consistente '1' ou inconsistente '0'
        fread(&aux_c->status, 1, 1, arquivoEntrada);
        if(aux_c->status == '0'){
            printf("Falha no processamento do arquivo.\n"); //caso seja inconsistente
        }else{ //caso seja consistente
            recupera_registro(arquivoEntrada, nRRN); //chama a função recupera_registro, explicada no registro.c
            
        } 
        fclose(arquivoEntrada); //fecha arquivo
        free(aux_c); //libera memoria
    
}

//essa função tem a responsabilidade de ler um arquivo binario e a partir disso criar um arquivo de indices (que será nossa Arvore B)
void funcionalidade5(char *a1, char *a2){
    FILE *arquivoBinEntrada = fopen(a1, "rb"); //definição do arquivoEntrada como leitura, passaremos ele como parametro para a funcao criar_tabela
    FILE *arquivoIndSaida = fopen(a2, "wb+"); //definição do arquivoEntrada como escrita binaria, passaremos ele como parametro para a funcao criar_tabela

    if (arquivoBinEntrada == NULL) {
        printf("Falha no processamento do arquivo.\n"); //verificacao de existencia dos arquivos
        return;
    }
    if(arquivoIndSaida == NULL){
        printf("Falha no processamento do arquivo.\n"); //verificacao de existencia dos arquivos
        return;
    }
        //CODIGO EM CONSTRUÇÂO
        cria_indice(arquivoBinEntrada, arquivoIndSaida); //chama a função criar_tabela, explicada no arquivo.c
    
        //fecha arquivo
        fclose(arquivoBinEntrada); 
        fclose(arquivoIndSaida);
        binarioNaTela(a2); //chama função fornecida binarionaTela
    
}

//essa funcionalidade tem a responsabilidade de fazer busca, porém ela pode ser como foi vista na funcionalidade 3 
//mas tambem pode ser feita com o arquivo de indice, ou seja, fazendo uma busca na arvore B
void funcionalidade6(char *a1, char *a2, int n){
    FILE *arquivoBinEntrada = fopen(a1, "rb"); //definição do arquivoBinEntrada como leitura binaria
    FILE *arquivoIndEntrada = fopen(a2, "rb"); //definição do arquivoIndEntrada como leitura binaria
    if (arquivoBinEntrada == NULL) {
        printf("Falha no processamento do arquivo.\n"); //verificacao de existencia dos arquivos
        return;
    }
    if(arquivoIndEntrada == NULL){
        printf("Falha no processamento do arquivo.\n"); //verificacao de existencia dos arquivos
        return;
    }
    
    CabecalhoArvore *aux_ca = malloc(sizeof(CabecalhoArvore)); //cria cabeçalho arvore
    Cabecalho *aux_c = malloc(sizeof(Cabecalho)); //cria cabeçalho 
    fread(&aux_c->status, 1, 1, arquivoBinEntrada); //le status 
    fread(&aux_ca->status, 1, 1, arquivoIndEntrada);//le status

    if(aux_c->status == '0' || aux_ca->status == '0'){
        printf("Falha no processamento do arquivo.\n"); //caso seja inconsistente
    }else{ //caso seja consistente
        selec_func6(arquivoBinEntrada, arquivoIndEntrada, n); 
        
    }
    free(aux_c); //libera memoria
    free(aux_ca); //libera memoria
    fclose(arquivoBinEntrada); //fecha arquivo
    fclose(arquivoIndEntrada); //fecha arquivo  
        
     
}

//funcionalidade responsavel pela inserção de novos registros
void funcionalidade7(char *a1, char *a2, int n){
    FILE *arquivoBinEntrada = fopen(a1, "rb+"); //definição do arquivoBinEntrada como leitura binaria
    FILE *arquivoIndEntrada = fopen(a2, "rb+"); //definição do arquivoIndEntrada como leitura binaria
    if (arquivoBinEntrada == NULL ) {
        printf("Falha no processamento do arquivo.\n"); //verificacao de existencia dos arquivos
        return;
    }
    if(arquivoIndEntrada == NULL){
        printf("Falha no processamento do arquivo.\n"); //verificacao de existencia dos arquivos
        return;
    }
    
    CabecalhoArvore *aux_ca = malloc(sizeof(CabecalhoArvore)); //cria cabeçalho arvore
    Cabecalho *aux_c = malloc(sizeof(Cabecalho)); //cria cabeçalho 
    fread(&aux_c->status, 1, 1, arquivoBinEntrada); //le status 
    fread(&aux_ca->status, 1, 1, arquivoIndEntrada);//le status

    if(aux_c->status == '0' || aux_ca->status == '0'){
        printf("Falha no processamento do arquivo.\n"); //caso seja inconsistente
    }else{ //caso seja consistente
        insere(arquivoBinEntrada, arquivoIndEntrada, n); 
        
    }
    free(aux_c); //libera memoria
    free(aux_ca); //libera memoria
    fclose(arquivoBinEntrada); //fecha arquivo
    fclose(arquivoIndEntrada); //fecha arquivo  
    
    binarioNaTela(a1); //chama função fornecida binarionaTela
    
}
