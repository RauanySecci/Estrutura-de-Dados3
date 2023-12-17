#ifndef CRIAGRAFOS_H
#define CRIAGRAFOS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Grafo* inicializarGrafo();

int encontrarOuCriarVertice(Grafo *grafo, char *nomeTecnologia);

void adicionarAresta(Grafo *grafo, int verticeOrigem, char *nomeTecnologiaDestino, int peso, int grupo);

Vertice adicionarVerticeOrigem(char *nomeTecnologia, int grupo);

void imprimirGrafo(Grafo *grafo);

void liberarGrafo(Grafo *grafo) ;

Grafo* preenche_grafo(FILE *arquivo);

#endif