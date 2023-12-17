#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "estrutura.h"
#include "funcoesFornecidas.h"
#include "leitura.h"
#include "funcionalidades.h"
#include "registro.h"
#include "arquivo.h"
#include "funcoes.h"

Grafo* inicializarGrafo() {
    Grafo *grafo = (Grafo *)malloc(sizeof(Grafo));
    grafo->numVertices = 0; // Você pode inicializar o número de vértices conforme necessário
    grafo->lista_vertices = NULL;
    return grafo;
}

// Função para encontrar ou criar um vértice com o nome especificado
int encontrarOuCriarVertice(Grafo *grafo, char *nomeTecnologia) {
    for (int i = 0; i < grafo->numVertices; i++) {
        if (grafo->lista_vertices[i].nomeTecnologia != NULL &&
            strcmp(grafo->lista_vertices[i].nomeTecnologia, nomeTecnologia) == 0) {
            return i;  // Vértice já existe, retorna seu índice
        }
    }

    // Vértice não encontrado, criar um novo
    grafo->numVertices++;
    grafo->lista_vertices = (Vertice *)realloc(grafo->lista_vertices, grafo->numVertices * sizeof(Vertice));
    int novoIndice = grafo->numVertices - 1;
    grafo->lista_vertices[novoIndice].nomeTecnologia = strdup(nomeTecnologia);
    
    // Inicializar os campos do novo vértice
    grafo->lista_vertices[novoIndice].grupo = -4;
    grafo->lista_vertices[novoIndice].grauEntrada = 0;
    grafo->lista_vertices[novoIndice].grauSaida = 0;
    grafo->lista_vertices[novoIndice].grau = 0;
    grafo->lista_vertices[novoIndice].inicio = NULL;
    grafo->lista_vertices[novoIndice].numArestas = 0;

    return novoIndice;
}

// Função para adicionar uma aresta ao grafo
void adicionarAresta(Grafo *grafo, int verticeOrigem, char *nomeTecnologiaDestino, int peso, int grupo) {
    // Encontrar ou criar o vértice destino
    int verticeDestino = encontrarOuCriarVertice(grafo, nomeTecnologiaDestino);
    
    Aresta *novaAresta = (Aresta *)malloc(sizeof(Aresta));
    novaAresta->nomeTecnologiaDestino = strdup(nomeTecnologiaDestino);
    novaAresta->peso = peso;
    novaAresta->proximo = NULL;

    Aresta *ultimaAresta = grafo->lista_vertices[verticeOrigem].inicio;
   
    if (ultimaAresta == NULL) {
        grafo->lista_vertices[verticeOrigem].inicio = novaAresta;
    } else {
    // Se a lista não estiver vazia, percorrer até o final e adicionar a nova aresta
        while (ultimaAresta->proximo != NULL) {
            ultimaAresta = ultimaAresta->proximo;
        }

        ultimaAresta->proximo = novaAresta;
    }

    grafo->lista_vertices[verticeOrigem].grupo = grupo;
    grafo->lista_vertices[verticeOrigem].numArestas++;
    grafo->lista_vertices[verticeOrigem].grauSaida++;
    grafo->lista_vertices[verticeOrigem].grau++;
    grafo->lista_vertices[verticeDestino].grauEntrada++;
    grafo->lista_vertices[verticeDestino].grau++;
}

Vertice adicionarVerticeOrigem(char *nomeTecnologia, int grupo) {
    Vertice novoVertice;

    // Alocar memória para o nome da tecnologia
    novoVertice.nomeTecnologia = strdup(nomeTecnologia);

    // Inicializar os campos do novo vértice
    novoVertice.grupo = grupo;
    novoVertice.grauEntrada = 0;
    novoVertice.grauSaida = 0;
    novoVertice.grau = 0;
    novoVertice.inicio = NULL;
    novoVertice.numArestas = 0;

    return novoVertice;
}


void imprimirGrafo(Grafo *grafo) {
    for (int i = 0; i < grafo->numVertices; i++){
    
    
        // Percorrendo a lista de arestas
        Aresta *a = grafo->lista_vertices[i].inicio;

        while (a != NULL){
            // Imprimindo os atributos do vértice
            printf("%s %d %d %d %d ", grafo->lista_vertices[i].nomeTecnologia, grafo->lista_vertices[i].grupo, grafo->lista_vertices[i].grauEntrada, grafo->lista_vertices[i].grauSaida, grafo->lista_vertices[i].grau);
            printf("%s %d", a->nomeTecnologiaDestino, a->peso);
            printf("\n");
            a = a->proximo;
        }

    }
}

// Função para liberar a memória alocada para o grafo
void liberarGrafo(Grafo *grafo) {
    for (int i = 0; i < grafo->numVertices; i++) {
        Aresta *arestaAtual = grafo->lista_vertices[i].inicio;
        while (arestaAtual != NULL) {
            Aresta *proximo = arestaAtual->proximo;
            free(arestaAtual->nomeTecnologiaDestino);
            free(arestaAtual);
            arestaAtual = proximo;
        }
        free(grafo->lista_vertices[i].nomeTecnologia);
    }

    free(grafo->lista_vertices);
    free(grafo);
}

Grafo* preenche_grafo(FILE *arquivo){
    fseek(arquivo, 0, SEEK_SET); 

    Grafo *grafo = inicializarGrafo();

    char cabecalho[13];
    fread(cabecalho, sizeof(cabecalho), 1, arquivo);

    while (1) {
        Registro *aux_r = malloc(sizeof(Registro)); //alocamos memoria para o registro criado a cada linha que vamos ler do arquivo binario

        // Tentativa de leitura dos dados
        size_t lidos_removido = fread(&aux_r->removido, sizeof(char), 1, arquivo);

        // Se não foi possível ler o campo 'removido', sair do loop
        if (lidos_removido != 1) {
            free(aux_r);
            break;
        }
        /*verifica se o registro está removido ou não (nesse caso quando é igual a '0' é não removido e quando
        é igual a '1' é removido)*/
        if(aux_r->removido=='0'){
            /*nessa parte vou ler os campos do meu registro com o fread
            ele funciona da seguinte forma fread(variável que vai armazenar o que foi lido, 
            tamanho do campo, quantas vezes será lido, está lendo de qual arquivo binario)*/
            fread(&aux_r->grupo, sizeof(int), 1, arquivo); 
            fread(&aux_r->popularidade, sizeof(int), 1, arquivo);
            fread(&aux_r->peso, sizeof(int), 1, arquivo);
            fread(&aux_r->nomeTecnologiaOrigem.tamanho, sizeof(int), 1, arquivo);
            aux_r->nomeTecnologiaOrigem.string = (char *)malloc(aux_r->nomeTecnologiaOrigem.tamanho + 1); //aloca memoria para salvar o nome Tecnologia Origem
            fread(aux_r->nomeTecnologiaOrigem.string, aux_r->nomeTecnologiaOrigem.tamanho, 1, arquivo);
            fread(&aux_r->nomeTecnologiaDestino.tamanho, sizeof(int), 1, arquivo);
            aux_r->nomeTecnologiaDestino.string = (char *)malloc(aux_r->nomeTecnologiaDestino.tamanho + 1);
            fread(aux_r->nomeTecnologiaDestino.string, aux_r->nomeTecnologiaDestino.tamanho, 1, arquivo); //aloca memoria para salvar o nome Tecnologia Destino
            aux_r->nomeTecnologiaOrigem.string[aux_r->nomeTecnologiaOrigem.tamanho] = '\0'; //garante o final da string
            aux_r->nomeTecnologiaDestino.string[aux_r->nomeTecnologiaDestino.tamanho] = '\0';//garante o final da string

            /*para a leitura do lixo, iremos calcular o tamanho total, ou seja, retiraremos do tamanho total do registro, que é 76,
            tudo o que já foi lido anteriormente*/
            int tamanhototal = 76 - (1 + 5 * sizeof(int) + aux_r->nomeTecnologiaDestino.tamanho + aux_r->nomeTecnologiaOrigem.tamanho);
            char lixo[tamanhototal];
            fread(lixo, tamanhototal, 1, arquivo);
            
        if(aux_r->nomeTecnologiaOrigem.tamanho!=0 && aux_r->nomeTecnologiaDestino.tamanho!=0){
            if(grafo->numVertices == 0){

                grafo->lista_vertices = (Vertice*)malloc(1 * sizeof(Vertice));
                grafo->numVertices = 1;

                grafo->lista_vertices[0] = adicionarVerticeOrigem(aux_r->nomeTecnologiaOrigem.string, aux_r->grupo);
                adicionarAresta(grafo, 0, aux_r->nomeTecnologiaDestino.string, aux_r->peso, aux_r->grupo);
            }

            else {
                
                int posOrigem = encontrarOuCriarVertice(grafo, aux_r->nomeTecnologiaOrigem.string);
                adicionarAresta(grafo, posOrigem, aux_r->nomeTecnologiaDestino.string, aux_r->peso, aux_r->grupo);
            }
        }

            //libera memoria alocada anteriormente
            free(aux_r->nomeTecnologiaDestino.string);
            free(aux_r->nomeTecnologiaOrigem.string);
            free(aux_r);
           
        }else if(aux_r->removido=='1'){
            printf("Registro inexistente.\n"); //caso o registro seja removido
        }
    }

    for(int i = 0; i < grafo->numVertices; i++){

        bubble_sort_aresta(&grafo->lista_vertices[i]);
    }
    bubble_sort_vertice(grafo);
    return grafo;
}

