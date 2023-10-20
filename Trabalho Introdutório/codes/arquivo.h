// Karine Cerqueira Nascimento 13718404
// Rauany Martinez Secci 13721217

#ifndef ARQUIVO_H
#define ARQUIVO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "estrutura.h"
#include "funcoesFornecidas.h"

void escreverCabecalho(FILE *arquivoSaida, Cabecalho *aux_c, int RRN, int i, int x);

void create_table(FILE *arquivoEntrada, FILE *arquivoSaida, Registro *aux_r, Cabecalho *aux_c);

void select_funcao(FILE *arquivo);

void select_where(FILE *arquivo, int n);

#endif