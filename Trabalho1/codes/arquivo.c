// Karine Cerqueira Nascimento 13718404
// Rauany Martinez Secci 13721217

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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
void criar_tabela(FILE *arquivoEntrada, FILE *arquivoSaida, Registro *aux_r, Cabecalho *aux_c){
    lerArquivo(arquivoEntrada, arquivoSaida, aux_r, aux_c);   
}

//funcao chamada pela funcionalidade 2
void selecionar_funcao(FILE *arquivo){
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
            printf("Registro inexistente.\n"); //caso o registro seja removido
            break;
        }
    }
}

/*verificar qual valor ele quer procurar e no caso de strings a leitura é feita com a função dada chamada 
scan_quote_string para retirarmos as aspas duplas escritas pelo usuário, logo em seguida chama outra função, 
que vai fazer uma leitura personalizada no arquivo de acordo com o parametro desejado*/

void busca_no_bin(FILE *arquivo, char *tipo){
        fseek(arquivo, 0, SEEK_SET); //volta para o inicio do arquivo

        char a1[50];
        int a2;

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
}

//funcao chamada pela funcionalidade 3 
void selecionar_onde(FILE *arquivo, int n){
    //enquanto a quantidade de vezes que o usuário deseja digitar (n) para encontrar um determinado registro
    while(n>0){
        //criação de variáveis auxiliares para a função
        char tipo[50];
        
        scanf("%s ", tipo); //leitura do tipo de campo que ele deseja ler
        busca_no_bin(arquivo, tipo);
        n--; //decresce o n em menos 1
    }
}


int busca_na_arv_recursiva(FILE *arquivoIndEntrada, char *a1, int RRN){
     if(RRN == -1){
        return -1;
    }

    int tamanho = 205* (RRN+1); 
    fseek(arquivoIndEntrada, tamanho, SEEK_SET); //pula para a posição desejada
    
    NoRegistro no_r; //cria no
    ler_no(arquivoIndEntrada, &no_r); //faz a leitura do nó
    
    int i;
    for(i = 0; i < no_r.nroChavesNo; i++){
        if(strcmp(a1, no_r.C[i]) <= 0){ //compara a chave com o procurado
            if(strcmp(a1, no_r.C[i]) == 0){
                return no_r.PR[i]; //se for igual retorna o RRN correspondente do arquivo de dados
            }
            else{
                return busca_na_arv_recursiva(arquivoIndEntrada, a1, no_r.P[i]); //chama busca recursiva
            }
        }
        else{
            continue;
        }
    }
        return busca_na_arv_recursiva(arquivoIndEntrada, a1, no_r.P[no_r.nroChavesNo]); //chama busca recursiva
}

//busca feita com a arvore B chamada pela funcionalidade 6
void busca_na_arv(FILE *arquivoIndEntrada, FILE *arquivoBinEntrada, char *a1){
    fseek(arquivoIndEntrada, 0, SEEK_SET); //volta para o inicio do arquivo

    //faz a leitura do cabeçalho da arvore
    CabecalhoArvore *aux_c = malloc(sizeof(CabecalhoArvore));
    fread(&aux_c->status, 1, 1, arquivoIndEntrada);
    fread(&aux_c->noRaiz, sizeof(int), 1, arquivoIndEntrada);
    fread(&aux_c->RRNproxNo, sizeof(int), 1, arquivoIndEntrada);
    fseek(arquivoIndEntrada, 196,SEEK_CUR); //pularemos o tamanho total (que seria o nosso lixo)
    
    //faz uma busca recursiva na arvore e encontra o rrn do nó
    int RRN_busca = busca_na_arv_recursiva(arquivoIndEntrada, a1, aux_c->noRaiz);
    free(aux_c);

    if(RRN_busca!=-1){ //se encontrar o RRN
        imprimeRRN(arquivoBinEntrada, RRN_busca); //chama a função para imprimir os campos do RRN correspondente
    }else{
        printf("Registro inexistente.\n");
    }
}

//funcao chamada pela funcionalidade 6 que determinará qual tipo de busca iremos fazer
void selec_func6(FILE *arquivoBinEntrada, FILE *arquivoIndEntrada, int n){
    while(n>0){    
        //criação de variáveis auxiliares para a função
        char tipo[50];
        char a1[55];
        scanf("%s ", tipo); //leitura do tipo de campo que ele deseja ler

        //caso o tipo for nomeTecnologiaOrigemDestino utilizaremos a busca com arvore B
        if(strcmp(tipo, "nomeTecnologiaOrigemDestino") == 0){
            scan_quote_string(a1);
            busca_na_arv(arquivoIndEntrada, arquivoBinEntrada, a1);
        }
        else{  //caso contrário busca como a funcionalidade 3
            busca_no_bin(arquivoBinEntrada, tipo);   
        }
        n--; //decresce o n em menos 1
    }
}

//funcao chamada pela funcionalidade 7 onde irá adicionar novos registros no arquivo de dados
//a parte de adicionar no arquivo de indice não esta pronta por conta da funcionalidade 5
void insere(FILE *arquivoBinEntrada, FILE *arquivoIndEntrada, int n) {
    //cria variaveis auxiliares
    char nomeTecnologiaOrigem[50];
    char grupo[5];
    char popularidade[5];
    char nomeTecnologiaDestino[50];
    char peso[5];
    Cabecalho *aux_c = malloc(sizeof(Cabecalho));//cria cabeçalho
    Registro *aux_r = malloc(sizeof(Registro));//cria registro
    aux_r->nomeTecnologiaOrigem.string = malloc(50 * sizeof(char));
    aux_r->nomeTecnologiaDestino.string = malloc(50 * sizeof(char));

    while (n > 0) {
        // Ler os campos da estrutura
        scanf(" %[^,], %[^,], %[^,], %[^,], %[^\n]", nomeTecnologiaOrigem, grupo, popularidade, nomeTecnologiaDestino, peso);
       
       //campo origem
       if(strcmp(nomeTecnologiaOrigem, "NULO") != 0){
        aux_r->nomeTecnologiaOrigem.tamanho = strlen(nomeTecnologiaOrigem);
        aux_r->nomeTecnologiaOrigem.string = malloc(aux_r->nomeTecnologiaOrigem.tamanho);
        strcpy(aux_r->nomeTecnologiaOrigem.string, nomeTecnologiaOrigem);
       }else{
        aux_r->nomeTecnologiaOrigem.tamanho = 0;
       

       }

        //campo grupo
        if(strcmp(grupo, "NULO") == 0){
            aux_r->grupo = -1;
        }else{
            aux_r->grupo =atoi(grupo);
        }
        
        //campo popularidade
        if(strcmp(popularidade, "NULO") == 0){
            aux_r->popularidade = -1;
        }else{
            aux_r->popularidade =atoi(popularidade);
        }

        //campo destino
        if(strcmp(nomeTecnologiaDestino, "NULO") != 0){
        aux_r->nomeTecnologiaDestino.tamanho = strlen(nomeTecnologiaDestino);
        aux_r->nomeTecnologiaDestino.string = malloc(aux_r->nomeTecnologiaDestino.tamanho);
        strcpy(aux_r->nomeTecnologiaDestino.string, nomeTecnologiaDestino);
        }else{
        aux_r->nomeTecnologiaDestino.tamanho = 0;
        

       }

        // campo peso
        if(strcmp(peso, "NULO") == 0){
            aux_r->peso = -1;
        }else{
            aux_r->peso =atoi(peso);
        }
        insereRegistro(arquivoBinEntrada, aux_r, aux_c);
        n--;
    }

    free(aux_r->nomeTecnologiaOrigem.string);
    free(aux_r->nomeTecnologiaDestino.string);
    free(aux_r);
    free(aux_c);
}

/*FUNCOES DA FUNCIONALIDADE 5 QUE AINDA ESTÃO EM PROCESSO DE CONSTRUÇÃO*/
void cria_indice(FILE *arquivoBinEntrada, FILE *arquivoSaida){

    int RRN = 0;
    fseek(arquivoBinEntrada, 13, SEEK_SET); // Pule o cabeçalho
    CabecalhoArvore *aux_c = criaCabecalhoArvore();
    escreveCabecalhoArvore(aux_c, arquivoSaida);

    while (1) {

        Registro *aux_r = malloc(sizeof(Registro));

        size_t lidos_removido = fread(&aux_r->removido, sizeof(char), 1, arquivoBinEntrada);
        

        if(feof (arquivoBinEntrada)){
            free(aux_r);
             break;
        }

        /*verifica se o registro está removido ou não (nesse caso quando é igual a '0' é não removido e quando
        é igual a '1' é removido)*/
        if(aux_r->removido=='0'){
            /*nessa parte vou ler os campos do meu registro com o fread
            ele funciona da seguinte forma fread(variável que vai armazenar o que foi lido, 
            tamanho do campo, quantas vezes será lido, está lendo de qual arquivo binario)*/
            fseek(arquivoBinEntrada, 12, SEEK_CUR);
            fread(&aux_r->nomeTecnologiaOrigem.tamanho, sizeof(int), 1, arquivoBinEntrada);
            
            aux_r->nomeTecnologiaOrigem.string = (char *)malloc(aux_r->nomeTecnologiaOrigem.tamanho + 1); //aloca memoria para salvar o nome Tecnologia Origem
            fread(aux_r->nomeTecnologiaOrigem.string, aux_r->nomeTecnologiaOrigem.tamanho, 1, arquivoBinEntrada);
            fread(&aux_r->nomeTecnologiaDestino.tamanho, sizeof(int), 1, arquivoBinEntrada);
            
            
            aux_r->nomeTecnologiaDestino.string = (char *)malloc(aux_r->nomeTecnologiaDestino.tamanho + 1);
            
            fread(aux_r->nomeTecnologiaDestino.string, aux_r->nomeTecnologiaDestino.tamanho, 1, arquivoBinEntrada); //aloca memoria para salvar o nome Tecnologia Destino
            aux_r->nomeTecnologiaOrigem.string[aux_r->nomeTecnologiaOrigem.tamanho] = '\0'; //garante o final da string
            aux_r->nomeTecnologiaDestino.string[aux_r->nomeTecnologiaDestino.tamanho] = '\0';//garante o final da string

            /*para a leitura do lixo, iremos calcular o tamanho total, ou seja, retiraremos do tamanho total do registro, que é 76,
            tudo o que já foi lido anteriormente*/
            int tamanhototal = 76 - (1 + 5 * sizeof(int) + aux_r->nomeTecnologiaDestino.tamanho + aux_r->nomeTecnologiaOrigem.tamanho);
            char lixo[tamanhototal];
            fread(lixo, tamanhototal, 1, arquivoBinEntrada);
        
            //printando a tecnologia de origem
            if(aux_r->nomeTecnologiaOrigem.tamanho != 0 && aux_r->nomeTecnologiaDestino.tamanho != 0) {
                // Alocar dinamicamente para a chave

                Dados *aux_d = malloc(sizeof(Dados));
                Dados *aux_d2 = malloc(sizeof(Dados));


                strcpy(aux_d->chave, aux_r->nomeTecnologiaOrigem.string);
                strcat(aux_d->chave, aux_r->nomeTecnologiaDestino.string);


                aux_d->RRN = RRN;
               

                insereNo_ARVB(arquivoBinEntrada, arquivoSaida, aux_d, aux_d2, aux_c, aux_c->noRaiz);

                //if(aux_d->chave != NULL) free(aux_d->chave);
                //if(aux_d != NULL) free(aux_d);
            }
    
            free(aux_r->nomeTecnologiaDestino.string);
            free(aux_r->nomeTecnologiaOrigem.string);
        }else{
            fseek(arquivoBinEntrada, 75, SEEK_CUR);
        }
        RRN++;
        free(aux_r);
    }
    aux_c->status = '1';
    escreveCabecalhoArvore(aux_c, arquivoSaida);

    free(aux_c);
}

CabecalhoArvore *criaCabecalhoArvore(){
    CabecalhoArvore *aux_c = malloc(sizeof(CabecalhoArvore));
    aux_c->status = '0';
    aux_c->noRaiz = -1;
    aux_c->RRNproxNo = 0;
    return aux_c;
}

void escreveCabecalhoArvore(CabecalhoArvore *c_aux, FILE *arquivo){
    // volta início do arquivo
    fseek(arquivo, 0, SEEK_SET);
    fwrite(&c_aux->status, sizeof(char), 1, arquivo);
    fwrite(&c_aux->noRaiz, sizeof(int), 1, arquivo);
    fwrite(&c_aux->RRNproxNo, sizeof(int), 1, arquivo);
    for(int i = 0; i < 196; i++){
        fwrite(LIXO, sizeof(char), 1, arquivo);
    }
        
}

NoRegistro criaNo(){
    NoRegistro no;
    no.nroChavesNo = 0;
    no.alturaNo = 1;
    no.RRNdoNo = -1;

    for (int i = 0; i < 4; i++) {
        no.P[i] = -1;
        if (i < 3) {
            strcpy(no.C[i], "");
            no.PR[i] = -1;
        }
    }
    return no;
}


void gravaNoArvoreB(NoRegistro *no, FILE *arquivoIND, int byteInicial){
    fseek(arquivoIND, byteInicial, SEEK_SET);
    fwrite(&no->nroChavesNo, sizeof(int), 1, arquivoIND); // Inserindo o nro de chaves
    fwrite(&no->alturaNo, sizeof(int), 1, arquivoIND); // Inserindo a altura
    fwrite(&no->RRNdoNo, sizeof(int), 1, arquivoIND); // Inserindo o RRN do nó

    for (int i = 0; i < 4; i++) {
        fwrite(&no->P[i], sizeof(int), 1, arquivoIND);
        if (i < 3) {
            fwrite(no->C[i], sizeof(char), strlen(no->C[i]), arquivoIND);
            int len = strlen(no->C[i]);
            for (int j = len; j < 55; j++) {
                fputc('$', arquivoIND);
            }

            fwrite(&no->PR[i], sizeof(int), 1, arquivoIND);
        }
    }
}

void adicionaChave(FILE *arquivo, NoRegistro no, Dados *d, int posicao){
    for(int i = no.nroChavesNo - 1; i >= posicao; i--) {
        no.P[i+2] = no.P[i+1];
        no.PR[i+1] = no.PR[i];
        strcpy(no.C[i + 1], no.C[i]);
    }

    no.P[posicao+1] = d->filho;
    no.PR[posicao] = d->RRN;
    strcpy(no.C[posicao], d->chave);
    no.nroChavesNo++;

    int tamanho = 205* (no.RRNdoNo+1);
    fseek(arquivo, tamanho, SEEK_SET); //pula para a posição desejada
    
    fwrite(&no.nroChavesNo, sizeof(int), 1, arquivo); // Inserindo o nro de chaves
    fwrite(&no.alturaNo, sizeof(int), 1, arquivo); // Inserindo a altura
    fwrite(&no.RRNdoNo, sizeof(int), 1, arquivo); // Inserindo o RRN do nó

    for (int i = 0; i < 4; i++) {
        fwrite(&no.P[i], sizeof(int), 1, arquivo);
        if (i < 3) {
            fwrite(no.C[i], sizeof(char), strlen(no.C[i]), arquivo);
            int len = strlen(no.C[i]);
            for (int j = len; j < 55; j++) {
                fputc('$', arquivo);
            }
            fwrite(&no.PR[i], sizeof(int), 1, arquivo);
        }
    }
}


void insereNaRaiz(Dados *dados, FILE *arquivoIND, CabecalhoArvore *aux_c){
    fseek(arquivoIND, 205, SEEK_SET);
    NoRegistro no = criaNo();
    no.nroChavesNo = 1;
    no.RRNdoNo = 0;
    strcpy(no.C[0], dados->chave);
    no.PR[0] = dados->RRN;
    gravaNoArvoreB(&no, arquivoIND, 205 * (1 + no.RRNdoNo)); 
}

Dados splitNo_ARVB(FILE *arquivo, NoRegistro *no, Dados *chave, CabecalhoArvore *aux_c) {
    NoRegistro novoNo = criaNo();
    novoNo.alturaNo = no->alturaNo;
    novoNo.nroChavesNo = no->alturaNo;
    novoNo.RRNdoNo = aux_c->RRNproxNo++; // O RRN do nó irmão é o próximo RRN disponível que é atualizado no cabeçalho
    char c[4][55];
    int p[5];
    int pr[4];

    for (int i = 0; i < 3; i++) {
        strcpy(c[i], no->C[i]);
        pr[i] = no->PR[i];
        p[i] = no->P[i];
    }
    p[3] = no->P[3];

    strcpy(c[3], chave->chave);
    pr[3] = chave->RRN;
    p[4] = chave->filho;
    
    insertionSort(c, pr, p);
   
    

    for(int i=0; i<2; i++){
        no->P[i] = p[i];
        strcpy(no->C[i], c[i]);
        no->PR[i] = pr[i];
    }
    no->P[2] = p[2];
    no->nroChavesNo = 2;

    for(int i=3; i<4; i++){
        novoNo.P[i-3] = p[i];
        strcpy(novoNo.C[i-3], c[i]);
        novoNo.PR[i-3] = pr[i];
    }
    novoNo.P[1] = p[4];
    novoNo.nroChavesNo = 1;

    strcpy(no->C[2], "");
    no->PR[2] = -1;
    no->P[3] = -1;
    
    gravaNoArvoreB(no, arquivo, 205 + (205 * no->RRNdoNo));
    gravaNoArvoreB(&novoNo, arquivo, 205 + (205 * novoNo.RRNdoNo));

    escreveCabecalhoArvore(aux_c, arquivo);


    // Retorna a chave promovida, seu RRN e o RRN do novo nó
    Dados *chavePromovida = malloc(sizeof(Dados));
    strcpy(chavePromovida->chave, c[2]);
    chavePromovida->RRN = pr[2];
    chavePromovida->filho = novoNo.RRNdoNo; // Considerando que aux_c->RRNproxNo é atualizado corretamente
    
    return *chavePromovida;
}


//essa funcionalidade foi feita conforme o livro recomendado no trabalho e od ifs baseados no pseudocodigo do monitor
int insereNo_ARVB(FILE *arquivoBinEntrada, FILE *arquivoIndSaida, Dados *dados, Dados *dados2, CabecalhoArvore *aux_c, int RRN){
     if(aux_c->noRaiz == -1){ //como o arquivo esta vazio insere na raiz
        insereNaRaiz(dados, arquivoIndSaida, aux_c);
        aux_c->noRaiz = 0;
        aux_c->RRNproxNo++;
        escreveCabecalhoArvore(aux_c, arquivoIndSaida); 
    }

    if(RRN == -1) { //condição de parada
        dados2->RRN = dados->RRN;
        strcpy(dados2->chave, dados->chave);
        dados2->filho = -1;
        return 1;
    }
    
    NoRegistro no1;
    int tamanho = 205*(1+RRN);
    caminha_arvore(arquivoIndSaida, &no1, tamanho);
    int posicao = buscaARVB(&no1, dados->chave);
    
    int pos_env = no1.P[posicao];

    int valor = insereNo_ARVB(arquivoBinEntrada, arquivoIndSaida, dados, dados2, aux_c, pos_env);
    int pos = buscaARVB(&no1, dados2->chave);
  
    if(valor == 0) {  // se a chave já foi inserida e não tem promoção de uma chave
        return 0;
    } 

    else if(no1.nroChavesNo < 3) { 
        adicionaChave(arquivoIndSaida, no1, dados2, pos);
        escreveCabecalhoArvore(aux_c, arquivoIndSaida);
        return 0;
    }
   
    else if(RRN != aux_c->noRaiz) { // Split em um nó interno
        
        Dados d3;
        *dados2 = splitNo_ARVB(arquivoIndSaida, &no1, dados2, aux_c);
        return 1;
    } 
    
    else{ // Split na raiz
        Dados dNovo;
       dNovo = splitNo_ARVB(arquivoIndSaida, &no1, dados2, aux_c);
        NoRegistro novo = criaNo(aux_c);
       strcpy(novo.C[0], dNovo.chave);
            novo.PR[0] = dNovo.RRN;
            novo.P[0] = no1.RRNdoNo;
            novo.P[1] = dNovo.filho; // ********

            novo.nroChavesNo = 1;
            novo.RRNdoNo = aux_c->RRNproxNo++; // **********
            novo.alturaNo = no1.alturaNo + 1;

            aux_c->noRaiz = novo.RRNdoNo;
        gravaNoArvoreB(&novo, arquivoIndSaida, 205 * (1 + novo.RRNdoNo));
        escreveCabecalhoArvore(aux_c, arquivoIndSaida);
        
        return 0;
    }
    
   
}

