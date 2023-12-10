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

void criar_tabela(FILE *arquivoEntrada, FILE *arquivoSaida, Registro *aux_r, Cabecalho *aux_c);

void selecionar_funcao(FILE *arquivo);

void selecionar_onde(FILE *arquivo, int n);

void selec_func6(FILE *arquivoBinEntrada, FILE *arquivoIndEntrada, int n);

void busca_no_bin(FILE *arquivo, char *tipo);

void busca_na_arv(FILE *arquivoIndEntrada, FILE *arquivoBinEntrada, char* a1);

void insere(FILE *arquivoBinEntrada, FILE *arquivoIndEntrada, int n);

CabecalhoArvore *criaCabecalhoArvore();

void escreveCabecalhoArvore(CabecalhoArvore *c_aux, FILE *arquivo);


NoRegistro criaNo();

void cria_indice(FILE *arquivoBinEntrada, FILE *arquivoSaida);

void adicionaChave(FILE *arquivo, NoRegistro no, Dados *d, int posicao);

Dados splitNo_ARVB(FILE *arquivo, NoRegistro *no, Dados *dados, CabecalhoArvore *cabecalho);


int insereNo_ARVB(FILE *arquivoBinEntrada, FILE *arquivoIndSaida, Dados *dados, Dados *dados2, CabecalhoArvore *aux_c, int RRN);



#endif