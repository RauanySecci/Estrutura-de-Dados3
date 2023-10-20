// Karine Cerqueira Nascimento 13718404
// Rauany Martinez Secci 13721217

#ifndef REGISTRO_H
#define REGISTRO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "estrutura.h"
#include "funcoesFornecidas.h"

void escreverRegistro(FILE *arquivoSaida, Registro *aux_r);

int contarRRN(FILE *arquivo);

void imprimeRRN(FILE *arquivo, int valor);

int contarRegistros(FILE *arquivo);

void recupera_registro(FILE *arquivo, int RRN);

#endif