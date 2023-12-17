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
#include "criaGrafos.h"

void principal_cria_grafo(FILE *arquivo){
    Grafo *grafo = preenche_grafo(arquivo);
    imprimirGrafo(grafo);
    liberarGrafo(grafo);
}

void grafo_transposto(FILE *arquivo){
    Grafo *grafo = preenche_grafo(arquivo);
    Grafo *grafo_transposto = inicializarGrafo();
    grafo_transposto->lista_vertices = (Vertice*) malloc(sizeof(Vertice) * grafo->numVertices);
    grafo_transposto->numVertices = grafo->numVertices;

    // Adiciona os vértices ao grafo transposto
    for (int i = 0; i < grafo->numVertices; i++) {
        grafo_transposto->lista_vertices[i] = adicionarVerticeOrigem(grafo->lista_vertices[i].nomeTecnologia, grafo->lista_vertices[i].grupo);
    }

    // Inverte a direção das arestas
    for (int i = 0; i < grafo->numVertices; i++) {
        Aresta *aux = grafo->lista_vertices[i].inicio;

        while (aux != NULL) {
            int posDestino = encontrarOuCriarVertice(grafo, aux->nomeTecnologiaDestino);
            
            adicionarAresta(grafo_transposto, posDestino, grafo->lista_vertices[i].nomeTecnologia, aux->peso, grafo->lista_vertices[posDestino].grupo);

            aux = aux->proximo;
        }
    }


    // Libera a memória do grafo original
    liberarGrafo(grafo);
    imprimirGrafo(grafo_transposto);
    liberarGrafo(grafo_transposto);
}

void componente_fortemente_conexo(Grafo *grafo, int vertice, int *visitados, int *componentes, int componenteAtual) {
    visitados[vertice] = 1;
    componentes[vertice] = componenteAtual;

    Aresta *aresta = grafo->lista_vertices[vertice].inicio;
    while (aresta != NULL) {
        int vizinho = encontrarOuCriarVertice(grafo, aresta->nomeTecnologiaDestino);
        if (!visitados[vizinho]) {
            componente_fortemente_conexo(grafo, vizinho, visitados, componentes, componenteAtual);
        }
        aresta = aresta->proximo;
    }
}

void funcao_aux_11(FILE *arquivo) {
    Grafo *grafo = preenche_grafo(arquivo);

    int *visitados = (int *)malloc(grafo->numVertices * sizeof(int));
    int *componentes = (int *)malloc(grafo->numVertices * sizeof(int));
    int numComponentes = 0;

    // Inicializa os arrays de visitados e componentes
    for (int i = 0; i < grafo->numVertices; i++) {
        visitados[i] = 0;
        componentes[i] = -1; // Inicializa todos os componentes como -1
    }

    // Identifica os componentes fortemente conexos
    for (int i = 0; i < grafo->numVertices; i++) {
        if (!visitados[i]) {
            componente_fortemente_conexo(grafo, i, visitados, componentes, numComponentes++);
        }
    }

    // Conta o número de componentes
    int *numVerticesComponente = (int *)calloc(numComponentes, sizeof(int));
    for (int i = 0; i < grafo->numVertices; i++) {
        numVerticesComponente[componentes[i]]++;
    }

    // Verifica se o grafo é fortemente conexo e imprime a saída desejada
    if (numComponentes == 1) {
        printf("Sim, o grafo é fortemente conexo e possui 1 componente.\n");
    } else {
        printf("Não, o grafo não é fortemente conexo e possui %d componentes.\n", numComponentes);
    }

    // Liberar memória alocada
    free(visitados);
    free(componentes);
    free(numVerticesComponente);
    liberarGrafo(grafo);
}



int caminho_mais_curto(Grafo *grafo, int origem, int destino) {
    int numVertices = grafo->numVertices;
    int *distancia = (int *)malloc(numVertices * sizeof(int));
    int *processado = (int *)malloc(numVertices * sizeof(int));
    int infinito = 999999;

    // Inicializa as distâncias como infinito e os vértices como não processados
    for (int i = 0; i < numVertices; i++) {
        distancia[i] = infinito;
        processado[i] = 0;
    }

    // A distância da origem para ela mesma é sempre 0
    distancia[origem] = 0;

    // Loop principal do algoritmo de Dijkstra
    for (int count = 0; count < numVertices - 1; count++) {
        // Encontra o vértice com a menor distância não incluído no conjunto de vértices processados
        int u = -1;
        int menorDistancia = infinito;

        for (int i = 0; i < numVertices; i++) {
            if (!processado[i] && distancia[i] < menorDistancia) {
                menorDistancia = distancia[i];
                u = i;
            }
        }

        if (u == -1) {
            // Se não houver vértice válido, interrompe o loop
            break;
        }

        processado[u] = 1;

        // Atualiza as distâncias dos vizinhos do vértice escolhido
        for (Aresta *a = grafo->lista_vertices[u].inicio; a != NULL; a = a->proximo) {
            int v = encontrarOuCriarVertice(grafo, a->nomeTecnologiaDestino);
            int pesoAresta = a->peso;

            if (!processado[v]) {
                int novaDistancia = distancia[u] + pesoAresta;

                if (novaDistancia < distancia[v]) {
                    distancia[v] = novaDistancia;
                }
            }
        }
    }

    int resultado = distancia[destino];

    free(distancia);
    free(processado);

    return resultado;
}

void funcao_aux_12(FILE *arquivo, int n, char **consultas) {

    Grafo *grafo = preenche_grafo(arquivo);

    for (int i = 0; i < n; i +=2) {
        int infinito = 999999;
        char nomeTecnologiaOrigem[100];
        char nomeTecnologiaDestino[100];

        strcpy(nomeTecnologiaOrigem, consultas[i]);
        if(i+1 <= n){
            strcpy(nomeTecnologiaDestino, consultas[i + 1]);
        }
        else{
            printf("Falha na execução da funcionalidade.");
        }

        int posOrigem = encontrarOuCriarVertice(grafo, nomeTecnologiaOrigem);
        int posDestino = encontrarOuCriarVertice(grafo, nomeTecnologiaDestino);

        int distanciaCaminho = caminho_mais_curto(grafo, posOrigem, posDestino);

        printf("%s %s: ", nomeTecnologiaOrigem, nomeTecnologiaDestino);
        if (distanciaCaminho != infinito) {
            printf("%d\n", distanciaCaminho);
        } else {
            printf("CAMINHO INEXISTENTE.\n");
        }
    }
    
    liberarGrafo(grafo);
}