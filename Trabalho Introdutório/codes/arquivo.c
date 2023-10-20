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

//funcao utilizada para escrever o cabeçalho no arquivo de saída binário
void escreverCabecalho(FILE *arquivoSaida,Cabecalho *aux_c, int RRN, int i, int x){
    fseek(arquivoSaida, 0, SEEK_SET); //voltamos nossa leitura para o primeiro byte
    aux_c->status = '1'; //consistente
    aux_c->proxRRN = RRN; //recebe o valor do RRN recebido por parâmetro
    aux_c->nroTecnologias = i; //recebe o numero de tecnologias distintas recebida por parâmetro
    aux_c->nroParesTecnologias = x; //recebe o numero de pares de tecnologias distintas recebida por parâmetro

    /*utilizamos fwrite para escrever no arquivo de saída
    fwrite funciona da seguinte maneira fwrite(variavel que voce deseja escrever no arquivo, quantidade de bytes 
    que ela tem, quantas vezes você deseja escrever ela, em qual arquivo binário será escrito)*/
    fwrite(&aux_c->status, 1, 1, arquivoSaida);
    fwrite(&aux_c->proxRRN, sizeof(int), 1, arquivoSaida);
    fwrite(&aux_c->nroTecnologias, sizeof(int), 1, arquivoSaida);
    fwrite(&aux_c->nroParesTecnologias, sizeof(int), 1, arquivoSaida);
}

//funcao chamada pela funcionalidade 1, nela iremos chamar a funcao lerArquivo, que é responsável por ler nosso arquivo csv
void create_table(FILE *arquivoEntrada, FILE *arquivoSaida, Registro *aux_r, Cabecalho *aux_c){
    lerArquivo(arquivoEntrada, arquivoSaida, aux_r, aux_c);   
}

//funcao chamada pela funcionalidade 2
void select_funcao(FILE *arquivo){
    fseek(arquivo, 0, SEEK_SET); //voltamos nossa leitura para o primeiro byte
    //parte responsável por pular a leitura do cabeçalho
    char cabecalho[13];
    fread(cabecalho, sizeof(cabecalho), 1, arquivo);
        
    while (1) {
        Registro *aux_r = malloc(sizeof(Registro)); //alocamos memoria para o registro criado a cada linha que vamos ler do arquivo binario

        // Tentativa de leitura dos dados
        size_t lidos_removido = fread(&aux_r->removido, sizeof(char), 1, arquivo);

        // Se não foi possível ler o campo 'removido', sair do loop
        if (lidos_removido != 1) {
            free(aux_r);
            break;
        }
        /*verifica se o registro está removido ou não (nesse caso quando é igual a '0' é não removido e quando
        é igual a '1' é removido)*/
        if(aux_r->removido=='0'){
            /*nessa parte vou ler os campos do meu registro com o fread
            ele funciona da seguinte forma fread(variável que vai armazenar o que foi lido, 
            tamanho do campo, quantas vezes será lido, está lendo de qual arquivo binario)*/
            fread(&aux_r->grupo, sizeof(int), 1, arquivo); 
            fread(&aux_r->popularidade, sizeof(int), 1, arquivo);
            fread(&aux_r->peso, sizeof(int), 1, arquivo);
            fread(&aux_r->nomeTecnologiaOrigem.tamanho, sizeof(int), 1, arquivo);
            aux_r->nomeTecnologiaOrigem.string = (char *)malloc(aux_r->nomeTecnologiaOrigem.tamanho + 1); //aloca memoria para salvar o nome Tecnologia Origem
            fread(aux_r->nomeTecnologiaOrigem.string, aux_r->nomeTecnologiaOrigem.tamanho, 1, arquivo);
            fread(&aux_r->nomeTecnologiaDestino.tamanho, sizeof(int), 1, arquivo);
            aux_r->nomeTecnologiaDestino.string = (char *)malloc(aux_r->nomeTecnologiaDestino.tamanho + 1);
            fread(aux_r->nomeTecnologiaDestino.string, aux_r->nomeTecnologiaDestino.tamanho, 1, arquivo); //aloca memoria para salvar o nome Tecnologia Destino
            aux_r->nomeTecnologiaOrigem.string[aux_r->nomeTecnologiaOrigem.tamanho] = '\0'; //garante o final da string
            aux_r->nomeTecnologiaDestino.string[aux_r->nomeTecnologiaDestino.tamanho] = '\0';//garante o final da string

            /*para a leitura do lixo, iremos calcular o tamanho total, ou seja, retiraremos do tamanho total do registro, que é 76,
            tudo o que já foi lido anteriormente*/
            int tamanhototal = 76 - (1 + 5 * sizeof(int) + aux_r->nomeTecnologiaDestino.tamanho + aux_r->nomeTecnologiaOrigem.tamanho);
            char lixo[tamanhototal];
            fread(lixo, tamanhototal, 1, arquivo);
        

            //printando a tecnologia de origem
            if(aux_r->nomeTecnologiaOrigem.tamanho==0){
                printf("NULO, "); //caso aux_r->nomeTecnologiaOrigem.string for null
            }else{
                printf("%s, ", aux_r->nomeTecnologiaOrigem.string);
            }

            //printando o grupo
            if(aux_r->grupo==-1){
                printf("NULO, "); //caso aux_r->grupo for -1
            }else{
                printf("%d, ", aux_r->grupo);
            }

            //printando a popularidade
            if(aux_r->popularidade==-1){
                printf("NULO, "); //caso aux_r->popularidade for -1
            }else{
                printf("%d, ", aux_r->popularidade);
            }

            //printando a tecnologia de Destino
            if(aux_r->nomeTecnologiaDestino.tamanho==0){
                printf("NULO, "); //caso aux_r->nomeTecnologiaDestino.string for null
            }else{
                printf("%s, ", aux_r->nomeTecnologiaDestino.string);
            }
            //printando o peso
            if(aux_r->peso==-1){
                printf("NULO\n"); //caso aux_r->popularidade for -1
            }else{
                printf("%d\n", aux_r->peso);
            }
            
            //libera memoria alocada anteriormente
            free(aux_r->nomeTecnologiaDestino.string);
            free(aux_r->nomeTecnologiaOrigem.string);
            free(aux_r);
    
        }else if(aux_r->removido=='1'){
            printf("Registro inexistente\n"); //caso o registro seja removido
        }
        }
}

//funcao chamada pela funcionalidade 3 
void select_where(FILE *arquivo, int n){
    //enquanto a quantidade de vezes que o usuário deseja digitar (n) para encontrar um determinado registro
    while(n>0){
        fseek(arquivo, 0, SEEK_SET); //volta para o inicio do arquivo

        //criação de variáveis auxiliares para a função
        char tipo[20];
        char a1[20];
        int a2;
        
        scanf("%s ", tipo); //leitura do tipo de campo que ele deseja ler

        /*verificar qual valor ele quer procurar e no caso de strings a leitura é feita com a função dada chamada 
        scan_quote_string para retirarmos as aspas duplas escritas pelo usuário, logo em seguida chama outra função, 
        que vai fazer uma leitura personalizada no arquivo de acordo com o parametro desejado*/
        if(strcmp(tipo, "nomeTecnologiaOrigem") == 0){
            scan_quote_string(a1); 
            lerOrigem(arquivo, a1); 
        
        }else if(strcmp(tipo, "nomeTecnologiaDestino") == 0){
            scan_quote_string(a1);
            lerDestino(arquivo, a1);
        
        }else if (strcmp(tipo, "grupo") == 0){
            scanf("%d", &a2);
            lerGrupo(arquivo, a2);

        }else if (strcmp(tipo, "popularidade") == 0){
            scanf("%d", &a2);
            lerPopularidade(arquivo, a2);

        }else if (strcmp(tipo, "peso") == 0){
            scanf("%d", &a2);
            lerPeso(arquivo, a2);

        }else{
            printf("Registro Inexistente\n"); //caso não exista o tipo de campo que o usuário deseja ler
        }
            n--; //decresce o n em menos 1
    }
}

