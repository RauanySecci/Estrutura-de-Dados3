// Karine Cerqueira Nascimento 13718404
// Rauany Martinez Secci 13721217

#ifndef ESTRUTURA_H
#define ESTRUTURA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct{
    char status; // consistência do arquivo '0' para inconsistente e '1' para consistente, ao abrir arquivo para escrita status = 0
    int proxRRN; // armazena o valor do proximo RRN disponivel, iniciado com 0
    int nroTecnologias; // quantidade de diferentes tecnologias armazenadas iniciado com 0
    int nroParesTecnologias; // indica a quantidade de pares de tecnologias diferentes iniciado com 0
} Cabecalho;

typedef struct{ // para campos de tamanho variavel
    int tamanho;
    char* string;
} StrVariavel;

typedef struct{
    char removido; // 1 está logicamente removido e 0 não está removido
    int grupo; // grupo ao qual a tecnologia pertence
    int popularidade; // acessos no Stack Overflow
    int peso; // frequencia com que as tecnologias aparecem juntas
    StrVariavel nomeTecnologiaOrigem;
    StrVariavel nomeTecnologiaDestino;
} Registro;

#endif
