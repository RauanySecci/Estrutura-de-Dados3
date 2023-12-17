// Karine Cerqueira Nascimento 13718404
// Rauany Martinez Secci 13721217

#ifndef FUNCOES_H
#define FUNCOES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "estrutura.h"
#include "funcoesFornecidas.h"
#include "funcoesGrafos.h"
#include "criaGrafos.h"

void realoca1(FILE* file);

void realoca2(FILE* file);

int stringExiste(char *lista[], int tamanho, char *novaString);

char* separaPalavra(char *b);

int retorna_pos(FILE *arquivo, NoRegistro *no1, char *chave);

int buscaARVBRecursiva(NoRegistro *no, int inf, int sup, char *chave);

int buscaARVB(NoRegistro *no, char *chave);

void insertionSort(char Csplit[4][55], int PRsplit[4], int Psplit[5]);

void trocarArestas(Aresta *a, Aresta *b);

void bubble_sort_aresta(Vertice *vertice);

void trocarVertices(Vertice *a, Vertice *b);

void bubble_sort_vertice(Grafo *grafo);

#endif