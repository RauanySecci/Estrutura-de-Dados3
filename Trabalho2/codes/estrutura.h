// Karine Cerqueira Nascimento 13718404
// Rauany Martinez Secci 13721217

#ifndef ESTRUTURA_H
#define ESTRUTURA_H
#define LIXO "$"

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

typedef struct{
    char status; // consistência do arquivo '0' para inconsistente e '1' para consistente, ao abrir arquivo para escrita status = 0
    int noRaiz; //armazena o RRN do no raiz
    int RRNproxNo; // armazena o valor do proximo RRN disponivel, iniciado com 0 
} CabecalhoArvore;

typedef struct{
    int nroChavesNo; //numeros de chaves de busca indexadas no nó
    int alturaNo; //altura do nó na arvore nó-folha, alturaNo deve ser igual a 1. Quando o nó é pai de um nó-folha, então alturaNo deve ser igual a 2
    int RRNdoNo; //RRN referente ao nó
    int P[4]; //RRN do Nó filho
    char C[3][55]; //Chave de busca
    int PR[3]; //RRN no arquivo de dados
   
} NoRegistro;

typedef struct{
    int RRN; 
    char chave[55]; 
    int filho; 
} Dados;

typedef struct Aresta {
    char *nomeTecnologiaDestino;
    int peso;
    struct Aresta *proximo;
} Aresta;

typedef struct {
    char *nomeTecnologia;
    int grupo;
    int grauEntrada;
    int grauSaida;
    int grau;
    Aresta *inicio;
    int numArestas;
} Vertice;

typedef struct {
    Vertice *lista_vertices;
    int numVertices;
} Grafo;

#endif
