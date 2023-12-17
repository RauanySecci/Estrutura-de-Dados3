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
#include "funcoesGrafos.h"
#include "criaGrafos.h"

/*função auxiliar para quando iremos ler o peso
essa função auxiliar é chamada quando o peso existe, ou seja, é diferente de -1*/
void realoca1(FILE* file) {
    char c; //variavel auxiliar
    if(fread(&c, 1, 1, file) == 1 && c != ',' && c!='\n' && c!='\0') { //se a leitura do meu proximo char c existir e for diferente de , \n ou \0
        fseek(file, +1, SEEK_CUR); // Adianta uma posição para não perder o próximo caractere (excluindo o \n que ficava junto com a proxima variavel nomeTecnologia Origem)
    }
}

/*função auxiliar para quando iremos ler o peso
essa função auxiliar é chamada quando o peso não existe, ou seja, é -1*/
void realoca2(FILE* file) {
    char c; //variavel auxiliar
    if(fread(&c, 1, 1, file) == 1 && c != ',' && c!='\n' && c!='\0') { //se a leitura do meu proximo char c existir e for diferente de , \n ou \0
        fseek(file, -1, SEEK_CUR); // Volta uma posição para não perder o próximo caractere de nomeTecnologiaOrigem
    }
}

//função para auxiliar na contagem de quantas tecnologias diferntes temos, armazenadas no cabeçalho
int stringExiste(char *lista[], int tamanho, char *novaString) {
    for (int i = 0; i < tamanho; i++) { //percorre a lista
        if (strcmp(lista[i], novaString) == 0) { //compara duas strings e verificam se elas são iguais
            return 1; // A string já existe na lista
        }
    }
    return 0; // A string não existe na lista
}

char* separaPalavra(char *b){
    char *c = (char*)malloc(500);   // Alocando memória suficiente para armazenar os caracteres
    int j = 0;
    for (int i = 0; i < strlen(b) && b[i] != '$'; i++) {
        c[j++] = b[i];
    }

    c[j] = '\0';  // Adicionando o caractere nulo ao final da string
    
    return c;
}

int retorna_pos(FILE *arquivo, NoRegistro *no1, char *chave){
     int aux = -1;
    for (int i = 0; i < 3; i++) {
        fread(&no1->P[i], sizeof(int), 1, arquivo);
        Dados *aux_d = malloc(sizeof(Dados));

        if (strcmp(aux_d->chave, chave) == 0) {
            return -2; // Chave encontrada
        } else if (strcmp(aux_d->chave, chave) > 0 && i < no1->nroChavesNo && aux == -1) {
            aux = i; // Retorna o nó filho à esquerda
        }
    }
    fread(&no1->P[3], sizeof(int), 1, arquivo);

    if (aux == -1) {
        aux = no1->nroChavesNo;
    }

    return aux; // Retorna a posição onde a chave deve ser buscada
}

int buscaARVBRecursiva(NoRegistro *no, int inf, int sup, char *chave){
    if(inf >= sup){ // Se os índices se cruzaram, é por que o elemento deverá ser comparado com esse índice
        return inf; // Retorna a posição onde a chave deveria ser inserida
    }

    int meio = inf + (sup - inf) / 2;
    if (strcmp(chave, no->C[meio]) == 0){

        return meio;
    }
    return (strcmp(chave, no->C[meio]) < 0) ? buscaARVBRecursiva(no, inf, meio - 1, chave) : buscaARVBRecursiva(no, meio + 1, sup, chave);
}

int buscaARVB(NoRegistro *no, char *chave){

    int posicao = buscaARVBRecursiva(no, 0, no->nroChavesNo - 1, chave);
  
    if (strcmp(chave, no->C[posicao]) > 0) {
            

         posicao++;
     }
     return posicao;

    
}

void insertionSort(char Csplit[4][55], int PRsplit[4], int Psplit[5]) {
    int i, j;
    char keyC[55];
    int keyPR, keyP;

    for (i = 1; i < 4; i++) {
        strcpy(keyC, Csplit[i]);
        keyPR = PRsplit[i];
        keyP = Psplit[i+1];
        j = i - 1;

        while (j >= 0 && strcmp(Csplit[j], keyC) > 0) {
            strcpy(Csplit[j + 1], Csplit[j]);
            PRsplit[j + 1] = PRsplit[j];
            Psplit[j + 2] = Psplit[j + 1];
            j = j - 1;
        }

        strcpy(Csplit[j + 1], keyC);
        PRsplit[j + 1] = keyPR;
        Psplit[j + 2] = keyP;
    }
}

// Função auxiliar para trocar duas arestas
void trocarArestas(Aresta *a, Aresta *b) {
    char *temp_nome = a->nomeTecnologiaDestino;
    int temp_peso = a->peso;
    a->nomeTecnologiaDestino = b->nomeTecnologiaDestino;
    a->peso = b->peso;
    b->nomeTecnologiaDestino = temp_nome;
    b->peso = temp_peso;
}

// Função para ordenar as arestas alfabeticamente em um vértice usando Bubble Sort
void bubble_sort_aresta(Vertice *vertice) {
    for (Aresta *i = vertice->inicio; i != NULL; i = i->proximo) {
        for (Aresta *j = vertice->inicio; j->proximo != NULL; j = j->proximo) {
            // Comparação com base no nomeTecnologiaDestino
            if (strcmp(j->nomeTecnologiaDestino, j->proximo->nomeTecnologiaDestino) > 0) {
                trocarArestas(j, j->proximo);
            }
        }
    }
}

void trocarVertices(Vertice *a, Vertice *b) {
    Vertice temp = *a;
    *a = *b;
    *b = temp;
}

void bubble_sort_vertice(Grafo *grafo) {
    for (int i = 0; i < grafo->numVertices - 1; i++) {
        for (int j = 0; j < grafo->numVertices - i - 1; j++) {
            // Comparação com base no nomeTecnologia, ajuste conforme necessário
            if (strcmp(grafo->lista_vertices[j].nomeTecnologia, grafo->lista_vertices[j + 1].nomeTecnologia) > 0) {
                trocarVertices(&grafo->lista_vertices[j], &grafo->lista_vertices[j + 1]);
            }
        }
    }
}


