// Karine Cerqueira Nascimento 13718404
// Rauany Martinez Secci 13721217

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "estrutura.h"
#include "funcoesFornecidas.h"
#include "leitura.h"
#include "funcionalidades.h"
#include "registro.h"
#include "arquivo.h"
#include "funcoes.h"

/*funcao que faz parte da funcionalidade 1 chamada dentro da função LerArquivo (vista no leitura.c), 
responsavel por escrever o que foi lido do arquivo csv em um arquivo binario*/
void escreverRegistro(FILE *arquivoSaida, Registro *aux_r){
    char lixo = '$'; //caracter de lixo

    /*fwrite funciona da seguinte maneira fwrite(variavel que voce deseja escrever no arquivo binario, quantidade de bytes 
    que ela tem, quantas vezes você deseja escrever ela, em qual arquivo binário será escrito)*/
    fwrite(&aux_r->removido, 1, 1, arquivoSaida); //escrevendo o campo removido
    fwrite(&aux_r->grupo, sizeof(int), 1, arquivoSaida); //escrevendo o grupo
    fwrite(&aux_r->popularidade, sizeof(int), 1, arquivoSaida); //escrevendo popularidade 
    fwrite(&aux_r->peso, sizeof(int), 1, arquivoSaida); //escrevendo peso
    fwrite(&aux_r->nomeTecnologiaOrigem.tamanho, sizeof(int), 1, arquivoSaida); //escrevendo tamanho do nomeTecnologiaOrigem
    fwrite(aux_r->nomeTecnologiaOrigem.string, aux_r->nomeTecnologiaOrigem.tamanho, 1, arquivoSaida); //escrevendo o nomeTecnologiaOrigem
    fwrite(&aux_r->nomeTecnologiaDestino.tamanho, sizeof(int), 1, arquivoSaida); //escrevendo tamanho do nomeTecnologiaDestino
    fwrite(aux_r->nomeTecnologiaDestino.string, aux_r->nomeTecnologiaDestino.tamanho, 1, arquivoSaida); //escrevendo NomeTecnologiaTamanho

    //transformando o lixo para $, para isso precisaremos descontar do posso tamanho total (76), tudo o que já foi lido
    int tamanhototal = 76 - (1 + 5 * sizeof(int) + aux_r->nomeTecnologiaDestino.tamanho+ aux_r->nomeTecnologiaOrigem.tamanho);
   
   //acrescentamos a váriavel de lixo $ para completar os 76 bytes do registro
    while(tamanhototal>0){
        fwrite(&lixo, 1, 1, arquivoSaida); 
        tamanhototal--;
    }

    
}

/*funcao responsável por contar quantos RRN temos em um arquivo csv, nesse caso ela foi chamada na função 
lerArquivo (vista em leitura.c) ela é necessária para contarmos a quantidade de duplas tecnologias que temos no arquivo*/
int contarRRN(FILE *arquivo) {
    //variaveis auxiliares
    int reg = 0;
    char c;

    while ((c = fgetc(arquivo)) != EOF) { //faz a leitura dos caracteres no arquivo csv
        if (c == '\n') {
            reg++; //contabiliza mais um toda vez que chega ao final de um registro
        }
    }

    return reg; //retorna a quantidade de registros
}

/*essa função foi utilizada na funcionalidade 3 e 4, de forma que é passado para ela qual o RRN e 
com isso é printado todo o registro separado por vírgulas*/
void imprimeRRN(FILE *arquivo, int valor){  
    int prox; //variavel auxiliar
    prox = 13 + (76*valor); //somado 13(quantidade de bytes do cabeçalho) + 76(tamanho do registro)*(RRN)
    fseek(arquivo, prox, SEEK_SET); //pula o valor de prox para a leitura do registro desejado
    Registro *aux_r = malloc(sizeof(Registro)); //aloca memória para registro

    /*nessa parte vou ler os campos do meu registro com o fread
    ele funciona da seguinte forma fread(variável que vai armazenar o que foi lido, 
    tamanho do campo, quantas vezes será lido, está lendo de qual arquivo binario)*/
    fread(&aux_r->removido, 1, 1, arquivo); //leitura do primeiro bite do registro que indica se ele existe ou não
    fread(&aux_r->grupo, sizeof(int), 1, arquivo); //leitura do grupo
    fread(&aux_r->popularidade, sizeof(int), 1, arquivo); //leitura da popularidade
    fread(&aux_r->peso, sizeof(int), 1, arquivo); //leitura do peso
    fread(&aux_r->nomeTecnologiaOrigem.tamanho, sizeof(int), 1, arquivo); //leitura do tamamnho da Tecnologia de origem 
    aux_r->nomeTecnologiaOrigem.string = (char *)malloc(aux_r->nomeTecnologiaOrigem.tamanho + 1); //aloca memoria para tecnologia de origem
    fread(aux_r->nomeTecnologiaOrigem.string, aux_r->nomeTecnologiaOrigem.tamanho, 1, arquivo); //leitura do nomeTecnologiaOrigem
    fread(&aux_r->nomeTecnologiaDestino.tamanho, sizeof(int), 1, arquivo); //leitura do tamamnho da Tecnologia destino
    aux_r->nomeTecnologiaDestino.string = (char *)malloc(aux_r->nomeTecnologiaDestino.tamanho + 1);//aloca memoria para tecnologia destino
    fread(aux_r->nomeTecnologiaDestino.string, aux_r->nomeTecnologiaDestino.tamanho, 1, arquivo); //leitura do nomeTecnologiaDestino
    aux_r->nomeTecnologiaDestino.string[aux_r->nomeTecnologiaDestino.tamanho] = '\0'; //garante o final da string
    aux_r->nomeTecnologiaOrigem.string[aux_r->nomeTecnologiaOrigem.tamanho] = '\0'; //garante o final da string
    int tamanhototal = 76 - (1 + 5 * sizeof(int) + aux_r->nomeTecnologiaOrigem.tamanho + aux_r->nomeTecnologiaDestino.tamanho);
    fseek(arquivo, tamanhototal, SEEK_CUR); //pularemos o tamanho total (que seria o nosso lixo)
    //printando o nomeTecnologiaOrigem
    if(aux_r->nomeTecnologiaOrigem.tamanho==0){
        printf("NULO, ");
    }else{
        printf("%s, ", aux_r->nomeTecnologiaOrigem.string);
    }

    //printando o grupo
    if(aux_r->grupo==-1){
        printf("NULO, ");
    }else{
        printf("%d, ", aux_r->grupo);
    }

     //printando a popularidade
    if(aux_r->popularidade==-1){
        printf("NULO, ");
    }else{
        printf("%d, ", aux_r->popularidade);
    }

    //printando a tecnologia de Destino
    if(aux_r->nomeTecnologiaDestino.tamanho==0){
        printf("NULO, ");
    }else{
        printf("%s, ", aux_r->nomeTecnologiaDestino.string);
    }
    //printando o peso
    if(aux_r->peso==-1){
        printf("NULO\n");
    }else{
        printf("%d\n", aux_r->peso);
    }

    //libera memoria       
    free(aux_r->nomeTecnologiaDestino.string);
    free(aux_r->nomeTecnologiaOrigem.string);
    free(aux_r);    
}

//chamada pela funcionalidade 4, de forma que é passado o RRN fornecido pelo usuário para ela fazer as verificações seguintes
void recupera_registro(FILE *arquivo, int RRN) {
    fseek(arquivo, 0, SEEK_SET); //vai para o inicio do arquivo

    //le o cabeçalho
    char cabecalho[13];
    fread(cabecalho, sizeof(char), 13, arquivo); 

    //calcula a quantidade de RRN do arquivo binario
    int tA, tR;//variaveis auxiliares
    fseek(arquivo, 0, SEEK_END); //pula para o final do arquivo
    tA = ftell(arquivo); //tamanho total do arquivo
    tR = 76; //tamanho do registro

    int nR = (tA - 13) / tR; //descobre o numero de registros

    if(RRN>=nR || RRN<0){ //se o numero de registro dado pelo usuário for menor que zero ou extrapolar a quantidade de RRN que temos no arquivo binário o registro será inexistente
        printf("Registro inexistente.\n");
    }else{
        fseek(arquivo, 0, SEEK_SET);//caso contrario volta ao inicio do arquivo
        imprimeRRN(arquivo, RRN); //chama a função impre RRN falada anteriormente
    }
      
}

//funcao responsavel por inserir um novo registro no arquivo de dados binario, faz parte da funcionalidade 7
void insereRegistro(FILE *arquivoBinEntrada, Registro *aux_r, Cabecalho *aux_c){ 
    lerCabecalho(arquivoBinEntrada, aux_c);
    int RRN = aux_c->proxRRN; //quantidade de registros
    int i = aux_c->nroTecnologias; //quantidade de tecnologias diferentes, que será armazenada no cabeçalho
    int x = aux_c->nroParesTecnologias; //quantidade de duplas tecnologias diferentes, que será armazenada no cabeçalho
    //vetores auxiliares para armazenarmos as tecnologias
    fseek(arquivoBinEntrada, 0, SEEK_END); //pula para o final do arquivo
    aux_r->removido = '0'; // Define 'removido' como '0' (não removido)
   //fazemos isso para ver se temos tecnologias repetidas e duplas tecnologias repetidas
    //se o tecnologiaOrigem e tecnologiaDestino forem nulos
    if(aux_r->nomeTecnologiaOrigem.tamanho==0 && aux_r->nomeTecnologiaDestino.tamanho==0){ 
        RRN++;
        escreverRegistro(arquivoBinEntrada, aux_r);
        escreverCabecalho(arquivoBinEntrada, aux_c, RRN, i, x);

//se o tecnologiaOrigem for nulo
    }else if(aux_r->nomeTecnologiaOrigem.tamanho==0){
        int a2 = aux_origem(arquivoBinEntrada, aux_r->nomeTecnologiaDestino.string);
        int a4 = aux_destino(arquivoBinEntrada, aux_r->nomeTecnologiaDestino.string);
        
        if(a2==0 && a4==0){
            i++; //tecnologia soma 1
        }
        RRN++;
        escreverRegistro(arquivoBinEntrada, aux_r);

        escreverCabecalho(arquivoBinEntrada, aux_c, RRN, i, x);

        //se o tecnologiaDestino for nulo

    }else if(aux_r->nomeTecnologiaDestino.tamanho==0){
        int a1 = aux_origem(arquivoBinEntrada, aux_r->nomeTecnologiaOrigem.string);
        int a3 = aux_destino(arquivoBinEntrada, aux_r->nomeTecnologiaOrigem.string);
    
        if(a1==0 && a3==0){
            i++; //tecnologia soma 1
        }
        RRN++;
        escreverRegistro(arquivoBinEntrada, aux_r);

        escreverCabecalho(arquivoBinEntrada, aux_c, RRN, i, x);
    
    }else{ //se nenhum for nulo 
        //chama a função que retornará zero se não existir tecnologia igual e um para tecnologias iguais
        int a1 = aux_origem(arquivoBinEntrada, aux_r->nomeTecnologiaOrigem.string);
        int a2 = aux_origem(arquivoBinEntrada, aux_r->nomeTecnologiaDestino.string);
        int a3 = aux_destino(arquivoBinEntrada, aux_r->nomeTecnologiaOrigem.string);
        int a4 = aux_destino(arquivoBinEntrada, aux_r->nomeTecnologiaDestino.string);
        int a5 = aux_concatenado(arquivoBinEntrada, aux_r->nomeTecnologiaOrigem.string, aux_r->nomeTecnologiaDestino.string);
        
        if(a1==0 || a2==0){
            x++;
            escreverRegistro(arquivoBinEntrada, aux_r);
        }
        if(a1==0 && a3==0){
            i++;
        }
        if(a2==0 && a4==0){
            i++;
        }
        if(a1==1 && a2==1){
            if(a5==0){
                escreverRegistro(arquivoBinEntrada, aux_r);
                x++;
            }
        }
        RRN++;

        escreverCabecalho(arquivoBinEntrada, aux_c, RRN, i, x);
    
    }
}


