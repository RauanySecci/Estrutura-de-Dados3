// Karine Cerqueira Nascimento 13718404
// Rauany Martinez Secci 13721217

#ifndef LEITURA_H
#define LEITURA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "estrutura.h"
#include "funcoesFornecidas.h"

void lerCabecalho(FILE *arquivo, Cabecalho *aux_c);

void leitura_str(FILE *file, StrVariavel *str);

void leitura_int(FILE* file, int* valor);

void lerArquivo(FILE *arquivoEntrada, FILE *arquivoSaida, Registro *aux_r, Cabecalho *aux_c);

void lerOrigem(FILE *arquivo, char *a1);

void lerDestino(FILE *arquivo, char *a1);

void lerGrupo(FILE *arquivo, int a2);

void lerPopularidade(FILE *arquivo, int a2);

void lerPeso(FILE *arquivo, int a2);

int aux_origem(FILE *arquivo, char *a1);

int aux_destino(FILE *arquivo, char *a1);

int aux_concatenado(FILE *arquivo, char *a1, char *a2);

void ler_no(FILE *arquivoIndEntrada, NoRegistro *no_r);

void caminha_arvore(FILE *arquivo, NoRegistro *no, int tamanho);



#endif