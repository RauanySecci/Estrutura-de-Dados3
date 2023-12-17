#ifndef FUNCOESGRAFOS_H
#define FUNCOESGRAFOS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void principal_cria_grafo(FILE *arquivo);

void grafo_transposto(FILE *arquivo);

void componente_fortemente_conexo(Grafo *grafo, int vertice, int *visitados, int *componentes, int componenteAtual);

void funcao_aux_11(FILE *arquivo);

int caminho_mais_curto(Grafo *grafo, int origem, int destino);

void funcao_aux_12(FILE *arquivo, int numConsultas, char **consultas);

#endif