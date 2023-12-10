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

void lerCabecalho(FILE *arquivo, Cabecalho *aux_c){
    fseek(arquivo, 0, SEEK_SET);
    fread(&aux_c->status, 1, 1, arquivo);
    fread(&aux_c->proxRRN, sizeof(int), 1, arquivo);
    fread(&aux_c->nroTecnologias, sizeof(int), 1, arquivo);
    fread(&aux_c->nroParesTecnologias, sizeof(int), 1, arquivo);
}

//a nossa funçao lerArquivo mostrada logo abaixo varia em dois tipos de leitura, são elas: STRING E INTEIRO
//nesse caso essa função é responsável pela leitura da string
void leitura_str(FILE *file, StrVariavel *str) {
    str->tamanho = 0; //definimos o tamanho da string como zero
    str->string = NULL; //definimos a string como nula
    char c; //variavel auxiliar

    while (fread(&c, 1, 1, file) == 1) { //leitura byte a byte
        if (c == ',') {
            break; // Sai do loop quando uma vírgula for encontrada
        }
        str->tamanho++; //a cada interação será adicionada um ao tamanho
        str->string = (char*)realloc(str->string, str->tamanho); //alocação da memoria para salvar a string
        str->string[str->tamanho - 1] = c; //string vai receber o caracter para formar a string final
    }
}

//nesse caso essa função é responsável pela leitura do inteiro
void leitura_int(FILE* file, int* valor) {
    if (fscanf(file, "%d", valor) != 1) { //se o campo for nulo
        *valor = -1; //nosso valor receberá menos 1
    }
    char c; //variavel auxiliar
    if(fread(&c, 1, 1, file) == 1 && c != ',' && c!='\n' && c!='\0') { //se a leitura do meu proximo char c existir e for diferente de , \n ou \0
        fseek(file, -1, SEEK_CUR); // Volta uma posição para não perder o próximo caractere
    }
}

//funcao chamada em criar_tabela (funcionalidade 1), responsável pela leitura do arquivo csv de strings e inteiros
void lerArquivo(FILE *arquivoEntrada, FILE *arquivoSaida, Registro *aux_r, Cabecalho *aux_c){
    int l = contarRRN(arquivoEntrada); // chama a função contar RRN responsavel por contar quantos registros temos no nosso arquivo csv
    fseek(arquivoEntrada, 0, SEEK_SET); //volta ao inicio do arquivo
    //variavel auxiliar
    char c; 
    
    int j = 0;
    int RRN = 0; //quantidade de registros
    int i = 0; //quantidade de tecnologias diferentes, que será armazenada no cabeçalho
    int x = l; //quantidade de duplas tecnologias diferentes, que será armazenada no cabeçalho
    //vetores auxiliares para armazenarmos as tecnologias
    char concatenada[100];
    char *tec_unica[5000];
    char *tec_p[5000];
  
    while (fread(&c, 1, 1, arquivoEntrada) == 1) {
        if (c == '\n') {
            break; //verifica se o caracter é \n se isso ocorrer, chegamos ao fim do registro
        }
    }

    while (!feof(arquivoEntrada)) {
        aux_r->removido = '0'; // Define 'removido' como '0' (não removido)
        leitura_str(arquivoEntrada, &aux_r->nomeTecnologiaOrigem); //chama a função leitura_str para ler nomeTecnologiaOrigem
        leitura_int(arquivoEntrada, &aux_r->grupo); //chama a função leitura_int para ler grupo
        leitura_int(arquivoEntrada, &aux_r->popularidade); //chama a função leitura_int para ler popularidade
        leitura_str(arquivoEntrada, &aux_r->nomeTecnologiaDestino); //chama a função leitura_str para ler nomeTecnologiaDestino
        leitura_int(arquivoEntrada, &aux_r->peso); //chama a função leitura_int para ler peso
        if(aux_r->peso == -1){
            realoca2(arquivoEntrada); //chama a função realoca2 explicada em funcoes.c    
        }else{
            realoca1(arquivoEntrada); //chama a função realoca1 explicada em funcoes.c 
        }

        if(RRN == 0){ //caso for o primeiro RRN
            fseek(arquivoSaida, 13, SEEK_SET); //pulando os 13 bytes do cabecalho
            escreverRegistro(arquivoSaida, aux_r); //chama a função que escreve o reegistro (explicada em registro.c) lido do arquivo csv no arquivo binario
        } else{
            escreverRegistro(arquivoSaida, aux_r); //chama a função que escreve o reegistro (explicada em registro.c) lido do arquivo csv no arquivo binario
        }
        
        //verificação das tecnologias diferentes para armazenarmos no cabeçalho
        if(aux_r->nomeTecnologiaOrigem.tamanho!= 0){ //se o tamanho da string for diferente de zero quer dizer que ela existe
            if (!stringExiste(tec_unica, i, aux_r->nomeTecnologiaOrigem.string) ) { //caso nao exista nenhuma tecnologia salva no vetor igual a comparada
                tec_unica[i] = aux_r->nomeTecnologiaOrigem.string; //salvar a tecnologia no vetor
                i++; //aumentar o tamanho do vetor
            }
        }
        //faz a segunda verificação pois em um mesmo registro temos tecnologia de origem e de destino.
        if(aux_r->nomeTecnologiaDestino.tamanho != 0){
            if (!stringExiste(tec_unica, i, aux_r->nomeTecnologiaDestino.string)) {
                tec_unica[i] = aux_r->nomeTecnologiaDestino.string;
                i++;
            }
        }

        //criando lista com a tecnologia de origem e de destino concatenadas
         if(aux_r->nomeTecnologiaOrigem.tamanho != 0 && aux_r->nomeTecnologiaDestino.tamanho != 0){ //se ambas tecnlogias existirem
            strcpy(concatenada, aux_r->nomeTecnologiaOrigem.string); // Copie string1 para a nova variável
            strcat(concatenada, aux_r->nomeTecnologiaDestino.string); //concatena string
            tec_p[j] = concatenada; //passa a string concatenada para o vetor auxiliar 
            
            if (stringExiste(tec_p, j, concatenada)) { //caso exista uma string igual a comparada no vetor  
                x--; //subtrai um no tamanho das duplas tecnologias diferentes 
            }      
        }else{
            x--; //caso alguma delas não exista, quer dizer que não existe uma dupla com ambas tecnologias, por este motivo tiramos 1 do tamanho das duplas tecnologias diferentes 
        }
        
        //quantidade de RRN que temos
        RRN++;
    }
    /*chama a função escreve cabeçalho passando como parametro o arquivo de saida, o cabeçalho, a quantidade de registros, 
    a quantidade de tecnologias diferentes e a quantidade de duplas tecnologias diferentes*/
    escreverCabecalho(arquivoSaida, aux_c, RRN, i, x); 
}

//na nossa função select-where, explicada em arquivo.c, chamamos as proximas funções de acordo com o parametro que o usuario busca
//responsável por buscar em nomeTecnologiaOrigem
void lerOrigem(FILE *arquivo, char *a1){
    int flag = 0; //variavel auxiliar
    int valor=-1; //valor de RRN

    //pula cabeçalho
    char cabecalho[13];
    fread(cabecalho, sizeof(cabecalho), 1, arquivo);  

    while (!feof(arquivo)) {
        Registro *aux_r = malloc(sizeof(Registro)); //aloca memoria para Registro
        // Tentativa de leitura dos dados
        size_t lidos_removido = fread(&aux_r->removido, sizeof(char), 1, arquivo);
        if (lidos_removido != 1) {
            // Se não foi possível ler o campo 'removido', sair do loop
            free(aux_r);
            break;
        }
        valor++; //RRN é somado mais um
        
        if(aux_r->removido=='0'){ //caso o registro exista
            
            fseek(arquivo, 12, SEEK_CUR); //pulamos as doze posiçoes dos inteiros anteriores (grupo, popularidade, peso)
            fread(&aux_r->nomeTecnologiaOrigem.tamanho, sizeof(int), 1, arquivo); //lemos o tamanho da tecnologia origem no arquivo binario
            aux_r->nomeTecnologiaOrigem.string = (char *)malloc(aux_r->nomeTecnologiaOrigem.tamanho + 1); //alocamos memoria para salvarmos essa variavel
            fread(aux_r->nomeTecnologiaOrigem.string, aux_r->nomeTecnologiaOrigem.tamanho, 1, arquivo); //lemos e armazenamos o nomeTecnologiaOrigem
            aux_r->nomeTecnologiaOrigem.string[aux_r->nomeTecnologiaOrigem.tamanho] = '\0'; //garante o final da string
           
            /*para a leitura do lixo, iremos calcular o tamanho total, ou seja, retiraremos do tamanho total do registro, que é 76,
            tudo o que já foi lido anteriormente*/
            int tamanhototal = 76 - (1 + 4 * sizeof(int) + aux_r->nomeTecnologiaOrigem.tamanho);
            fseek(arquivo, tamanhototal, SEEK_CUR); //pularemos o tamanho total (que seria o nosso lixo)

             /*para printarmos a resposta do que está sendo procurado, verificaremos que a string 
             a1 dada é igual a nomeTecnologiaOrigem que acabamos de ler*/
             if(aux_r->nomeTecnologiaOrigem.tamanho!=0){
                if(strcmp(a1, aux_r->nomeTecnologiaOrigem.string) == 0){ //caso for igual
                    fseek(arquivo, 0, SEEK_SET); //voltamos para o inicio do arquivo para chamarmos o imprime RRN
                    imprimeRRN(arquivo, valor); //chamada imprime RRN (função utilizada na funcionalidade 3 e 4) explicada em registro.c
                    flag = 1; //variavel auxiliar recebe um, simbolizando que encontramos o que estavamos procurando
                
            }}
            //libera memoria
            free(aux_r->nomeTecnologiaOrigem.string); 
            free(aux_r);
        }
        else{
            fseek(arquivo, 75, SEEK_CUR);
        }
    }
    if(flag == 0){ //caso nossa flag seja zero, isso significa que nada foi encontrado, por este motivo nosso registro é inexistente
        printf("Registro inexistente.\n");
    }
}

//responsável por buscar em nomeTecnologiaDestino
void lerDestino(FILE *arquivo, char *a1){
    int flag = 0; //variavel auxiliar
    int valor=-1; //valor de RRN

    //pula cabeçalho
    char cabecalho[13];
    fread(cabecalho, sizeof(cabecalho), 1, arquivo); 
        
    while (!feof(arquivo)) {
        Registro *aux_r = malloc(sizeof(Registro));//aloca memoria para Registro
        // Tentativa de leitura dos dados
        size_t lidos_removido = fread(&aux_r->removido, sizeof(char), 1, arquivo);
        if (lidos_removido != 1) {
            // Se não foi possível ler o campo 'removido', sair do loop
            free(aux_r);
            break;
        }
        valor++; //RRN é somado mais um
        if(aux_r->removido=='0'){//caso o registro exista
            fseek(arquivo, 12, SEEK_CUR); //pulamos as doze posiçoes dos inteiros anteriores (grupo, popularidade, peso)
            fread(&aux_r->nomeTecnologiaOrigem.tamanho, sizeof(int), 1, arquivo); //lemos o tamanho da tecnologia origem no arquivo binario
            fseek(arquivo, aux_r->nomeTecnologiaOrigem.tamanho, SEEK_CUR); //pulamos o tamanho do nomeTecnologiaOrigem
            fread(&aux_r->nomeTecnologiaDestino.tamanho, sizeof(int), 1, arquivo); //lemos o tamanho da tecnologia destino no arquivo binario
            aux_r->nomeTecnologiaDestino.string = (char *)malloc(aux_r->nomeTecnologiaDestino.tamanho + 1); //alocamos memoria para salvarmos essa variavel
            fread(aux_r->nomeTecnologiaDestino.string, aux_r->nomeTecnologiaDestino.tamanho, 1, arquivo); //lemos e armazenamos o nomeTecnologiaDestino
            aux_r->nomeTecnologiaDestino.string[aux_r->nomeTecnologiaDestino.tamanho] = '\0'; //garante o final da string
           
            /*para a leitura do lixo, iremos calcular o tamanho total, ou seja, retiraremos do tamanho total do registro, que é 76,
            tudo o que já foi lido anteriormente*/
            int tamanhototal = 76 - (1 + 5 * sizeof(int) + aux_r->nomeTecnologiaDestino.tamanho + aux_r->nomeTecnologiaOrigem.tamanho);
            fseek(arquivo, tamanhototal, SEEK_CUR); //pularemos o tamanho total (que seria o nosso lixo)

             /*para printarmos a resposta do que está sendo procurado, verificaremos que a string 
             a1 dada é igual a nomeTecnologiaDestino que acabamos de ler*/
             if(aux_r->nomeTecnologiaDestino.tamanho!=0){ 
                if(strcmp(a1, aux_r->nomeTecnologiaDestino.string) == 0){ //caso for igual
                    fseek(arquivo, 0, SEEK_SET); //voltamos para o inicio do arquivo para chamarmos o imprime RRN
                    imprimeRRN(arquivo, valor); //chamada imprime RRN (função utilizada na funcionalidade 3 e 4) explicada em registro.c
                    flag = 1; //variavel auxiliar recebe um, simbolizando que encontramos o que estavamos procurando
            }}
            //libera memoria
            free(aux_r->nomeTecnologiaDestino.string);
            free(aux_r);
    
        }
        else{
            fseek(arquivo, 75, SEEK_CUR);
        }
    }
    if(flag == 0){ //caso nossa flag seja zero, isso significa que nada foi encontrado, por este motivo nosso registro é inexistente
        printf("Registro inexistente.\n");
    }
}

//responsável por buscar em grupo
void lerGrupo(FILE *arquivo, int a2){
    int flag = 0; //variavel auxiliar
    int valor=-1; //valor de RRN

    //pula cabeçalho
    char cabecalho[13];
    fread(cabecalho, sizeof(cabecalho), 1, arquivo); 
        
    while (!feof(arquivo)) {
        Registro *aux_r = malloc(sizeof(Registro)); //aloca memoria para Registro

        // Tentativa de leitura dos dados
        size_t lidos_removido = fread(&aux_r->removido, sizeof(char), 1, arquivo);

        if (lidos_removido != 1) {
            // Se não foi possível ler o campo 'removido', sair do loop
            free(aux_r);
            break;
        }
        valor++; //RRN é somado mais um
        if(aux_r->removido=='0'){//caso o registro exista
            fread(&aux_r->grupo, sizeof(int), 1, arquivo); //lemos e armazenamos o grupo
            
            /*para a leitura do lixo, iremos calcular o tamanho total, ou seja, retiraremos do tamanho total do registro, que é 76,
            tudo o que já foi lido anteriormente, nesse caso foi lido o primeiro byte e o grupo*/
            int tamanhototal = 76 - (1 + 1 * sizeof(int)); 
            fseek(arquivo, tamanhototal, SEEK_CUR); //pularemos o tamanho total (que seria o nosso lixo)

             /*para printarmos a resposta do que está sendo procurado, verificaremos se a2 é igual ao grupo lido*/
            if(a2 == aux_r->grupo){
                fseek(arquivo, 0, SEEK_SET); //voltamos para o inicio do arquivo para chamarmos o imprime RRN
                imprimeRRN(arquivo, valor); //chamada imprime RRN (função utilizada na funcionalidade 3 e 4) explicada em registro.c
                flag = 1; //variavel auxiliar recebe um, simbolizando que encontramos o que estavamos procurando
            }
            //libera memoria
            free(aux_r);
        }
        else{
            fseek(arquivo, 75, SEEK_CUR);
        }
    }
    if(flag == 0){ //caso nossa flag seja zero, isso significa que nada foi encontrado, por este motivo nosso registro é inexistente
        printf("Registro inexistente.\n");
    }
}

//responsável por buscar em popularidade
void lerPopularidade(FILE *arquivo, int a2){
    int flag = 0; //variavel auxiliar
    int valor=-1; //valor de RRN

    //pula cabeçalho
    char cabecalho[13];
    fread(cabecalho, sizeof(cabecalho), 1, arquivo); 
        
    while (!feof(arquivo)) {
        Registro *aux_r = malloc(sizeof(Registro)); //aloca memoria para Registro

        // Tentativa de leitura dos dados
        size_t lidos_removido = fread(&aux_r->removido, sizeof(char), 1, arquivo);
        if (lidos_removido != 1) {
            // Se não foi possível ler o campo 'removido', sair do loop
            free(aux_r);
            break;
        }
        valor++; //RRN é somado mais um
        if(aux_r->removido=='0'){//caso o registro exista
            fseek(arquivo, 4, SEEK_CUR); //pula a leitura do grupo (inteiro)
            fread(&aux_r->popularidade, sizeof(int), 1, arquivo); //lemos e armazenamos a popularidade 

            /*para a leitura do lixo, iremos calcular o tamanho total, ou seja, retiraremos do tamanho total do registro, que é 76,
            tudo o que já foi lido anteriormente, nesse caso foi lido o primeiro byte, grupo e a popularidade*/
            int tamanhototal = 76 - (1 + 2 * sizeof(int) );
            fseek(arquivo, tamanhototal, SEEK_CUR); //pularemos o tamanho total (que seria o nosso lixo)

            /*para printarmos a resposta do que está sendo procurado, verificaremos se a2 é igual a popularidade lida*/
            if(a2 == aux_r->popularidade){
                fseek(arquivo, 0, SEEK_SET); //voltamos para o inicio do arquivo para chamarmos o imprime RRN
                imprimeRRN(arquivo, valor); //chamada imprime RRN (função utilizada na funcionalidade 3 e 4) explicada em registro.c
                flag = 1; //variavel auxiliar recebe um, simbolizando que encontramos o que estavamos procurando
            }
            //libera memoria
            free(aux_r);
    
        }
        else{
            fseek(arquivo, 75, SEEK_CUR);
        }
    }
    if(flag == 0){ //caso nossa flag seja zero, isso significa que nada foi encontrado, por este motivo nosso registro é inexistente
        printf("Registro inexistente.\n");
    }
}

//responsável por buscar em peso
void lerPeso(FILE *arquivo, int a2){
    int flag = 0; //variavel auxiliar
    int valor=-1; //valor de RRN

    //pula cabeçalho
    char cabecalho[13];
    fread(cabecalho, sizeof(cabecalho), 1, arquivo); 
        
    while (!feof(arquivo)) {
        Registro *aux_r = malloc(sizeof(Registro)); //aloca memoria para Registro
        // Tentativa de leitura dos dados
        size_t lidos_removido = fread(&aux_r->removido, sizeof(char), 1, arquivo);
        if (lidos_removido != 1) {
            // Se não foi possível ler o campo 'removido', sair do loop
            free(aux_r);
            break;
        }
        valor++; //RRN é somado mais um
        if(aux_r->removido=='0'){//caso o registro exista
            fseek(arquivo, 8, SEEK_CUR); //pula o grupo e a popularidade (inteiros)
            fread(&aux_r->peso, sizeof(int), 1, arquivo);  //lemos e armazenamos o peso
            
            /*para a leitura do lixo, iremos calcular o tamanho total, ou seja, retiraremos do tamanho total do registro, que é 76,
            tudo o que já foi lido anteriormente, nesse caso foi lido o primeiro byte, grupo, popularidade e peso*/
            int tamanhototal = 76 - (1 + 3 * sizeof(int));
            fseek(arquivo, tamanhototal, SEEK_CUR); //pularemos o tamanho total (que seria o nosso lixo)

            /*para printarmos a resposta do que está sendo procurado, verificaremos se a2 é igual ao peso lido*/
            if(a2 == aux_r->peso){
                fseek(arquivo, 0, SEEK_SET); //voltamos para o inicio do arquivo para chamarmos o imprime RRN
                imprimeRRN(arquivo, valor); //chamada imprime RRN (função utilizada na funcionalidade 3 e 4) explicada em registro.c
                flag = 1; //variavel auxiliar recebe um, simbolizando que encontramos o que estavamos procurando
            }
            //libera memoria
            free(aux_r);
    
        }
        else{
            fseek(arquivo, 75, SEEK_CUR);
        }
    }
    if(flag == 0){ //caso nossa flag seja zero, isso significa que nada foi encontrado, por este motivo nosso registro é inexistente
        printf("Registro inexistente.\n");
    }
}

//funcao auxiliar para ver se ja existe a tecnologiaOrigem que estamos tentando adicionar
int aux_origem(FILE *arquivo, char *a1){
    fseek(arquivo, 0, SEEK_SET);
    int valor = -1;
    int flag = 0; //variavel auxiliar
    
    //pula cabeçalho
    char cabecalho[13];
    fread(cabecalho, sizeof(cabecalho), 1, arquivo);  

    while (!feof(arquivo)) {
        Registro *aux_r = malloc(sizeof(Registro)); //aloca memoria para Registro
        // Tentativa de leitura dos dados
        size_t lidos_removido = fread(&aux_r->removido, sizeof(char), 1, arquivo);
        if (lidos_removido != 1) {
            // Se não foi possível ler o campo 'removido', sair do loop
            free(aux_r);
            break;
        }
        valor++; //RRN é somado mais um
        
        if(aux_r->removido=='0'){ //caso o registro exista
            
            fseek(arquivo, 12, SEEK_CUR); //pulamos as doze posiçoes dos inteiros anteriores (grupo, popularidade, peso)
            fread(&aux_r->nomeTecnologiaOrigem.tamanho, sizeof(int), 1, arquivo); //lemos o tamanho da tecnologia origem no arquivo binario
            aux_r->nomeTecnologiaOrigem.string = (char *)malloc(aux_r->nomeTecnologiaOrigem.tamanho + 1); //alocamos memoria para salvarmos essa variavel
            fread(aux_r->nomeTecnologiaOrigem.string, aux_r->nomeTecnologiaOrigem.tamanho, 1, arquivo); //lemos e armazenamos o nomeTecnologiaOrigem
            aux_r->nomeTecnologiaOrigem.string[aux_r->nomeTecnologiaOrigem.tamanho] = '\0'; //garante o final da string
           
            /*para a leitura do lixo, iremos calcular o tamanho total, ou seja, retiraremos do tamanho total do registro, que é 76,
            tudo o que já foi lido anteriormente*/
            int tamanhototal = 76 - (1 + 4 * sizeof(int) + aux_r->nomeTecnologiaOrigem.tamanho);
            fseek(arquivo, tamanhototal, SEEK_CUR); //pularemos o tamanho total (que seria o nosso lixo)

             /*para printarmos a resposta do que está sendo procurado, verificaremos que a string 
             a1 dada é igual a nomeTecnologiaOrigem que acabamos de ler*/
             if(aux_r->nomeTecnologiaOrigem.tamanho!=0){
                if(strcmp(a1, aux_r->nomeTecnologiaOrigem.string) == 0){ //caso for igual
                    flag = 1; //variavel auxiliar recebe um, simbolizando que encontramos o que estavamos procurando
                    return 1;
            }}
            //libera memoria
            free(aux_r->nomeTecnologiaOrigem.string); 
            free(aux_r);
        }
        else{
            fseek(arquivo, 75, SEEK_CUR);
        }
    }
    if(flag == 0){ //caso nossa flag seja zero, isso significa que nada foi encontrado, por este motivo nosso registro é inexistente
        return 0;
    }
}

//funcao auxiliar para ver se ja existe a tecnologiaDestino que estamos tentando adicionar

int aux_destino(FILE *arquivo, char *a1){
    fseek(arquivo, 0, SEEK_SET);
    int valor = -1;
    int flag = 0; //variavel auxiliar

    //pula cabeçalho
    char cabecalho[13];
    fread(cabecalho, sizeof(cabecalho), 1, arquivo); 
        
    while (!feof(arquivo)) {
        Registro *aux_r = malloc(sizeof(Registro));//aloca memoria para Registro
        // Tentativa de leitura dos dados
        size_t lidos_removido = fread(&aux_r->removido, sizeof(char), 1, arquivo);
        if (lidos_removido != 1) {
            // Se não foi possível ler o campo 'removido', sair do loop
            free(aux_r);
            break;
        }
        valor++; //RRN é somado mais um
        if(aux_r->removido=='0'){//caso o registro exista
            fseek(arquivo, 12, SEEK_CUR); //pulamos as doze posiçoes dos inteiros anteriores (grupo, popularidade, peso)
            fread(&aux_r->nomeTecnologiaOrigem.tamanho, sizeof(int), 1, arquivo); //lemos o tamanho da tecnologia origem no arquivo binario
            fseek(arquivo, aux_r->nomeTecnologiaOrigem.tamanho, SEEK_CUR); //pulamos o tamanho do nomeTecnologiaOrigem
            fread(&aux_r->nomeTecnologiaDestino.tamanho, sizeof(int), 1, arquivo); //lemos o tamanho da tecnologia destino no arquivo binario
            aux_r->nomeTecnologiaDestino.string = (char *)malloc(aux_r->nomeTecnologiaDestino.tamanho + 1); //alocamos memoria para salvarmos essa variavel
            fread(aux_r->nomeTecnologiaDestino.string, aux_r->nomeTecnologiaDestino.tamanho, 1, arquivo); //lemos e armazenamos o nomeTecnologiaDestino
            aux_r->nomeTecnologiaDestino.string[aux_r->nomeTecnologiaDestino.tamanho] = '\0'; //garante o final da string
           
            /*para a leitura do lixo, iremos calcular o tamanho total, ou seja, retiraremos do tamanho total do registro, que é 76,
            tudo o que já foi lido anteriormente*/
            int tamanhototal = 76 - (1 + 5 * sizeof(int) + aux_r->nomeTecnologiaDestino.tamanho + aux_r->nomeTecnologiaOrigem.tamanho);
            fseek(arquivo, tamanhototal, SEEK_CUR); //pularemos o tamanho total (que seria o nosso lixo)

             /*para printarmos a resposta do que está sendo procurado, verificaremos que a string 
             a1 dada é igual a nomeTecnologiaDestino que acabamos de ler*/
             if(aux_r->nomeTecnologiaDestino.tamanho!=0){ 
                if(strcmp(a1, aux_r->nomeTecnologiaDestino.string) == 0){ //caso for igual
                    flag = 1; //variavel auxiliar recebe um, simbolizando que encontramos o que estavamos procurando
                    return 1;
            }}
            //libera memoria
            free(aux_r->nomeTecnologiaDestino.string);
            free(aux_r);
    
        }
        else{
            fseek(arquivo, 75, SEEK_CUR);
        }
    }
    if(flag == 0){ //caso nossa flag seja zero, isso significa que nada foi encontrado, por este motivo nosso registro é inexistente
        return 0;
    }
}

//funcao auxiliar para ver se ja existe o par de tecnologias que estamos tentando adicionar

int aux_concatenado(FILE *arquivo, char *a1, char *a2){
     fseek(arquivo, 0, SEEK_SET);
    int valor = -1;
    int flag = 0; //variavel auxiliar
    char concatenada1[100];
    char concatenada2[100];
    if(strlen(a1)!=0 && strlen(a2)!=0){
            strcpy(concatenada1, a1); // Copie string1 para a nova variável
            strcat(concatenada1, a2); //concatena string
    }
    //pula cabeçalho
    char cabecalho[13];
    fread(cabecalho, sizeof(cabecalho), 1, arquivo);  

    while (!feof(arquivo)) {
        Registro *aux_r = malloc(sizeof(Registro)); //aloca memoria para Registro
        // Tentativa de leitura dos dados
        size_t lidos_removido = fread(&aux_r->removido, sizeof(char), 1, arquivo);
        if (lidos_removido != 1) {
            // Se não foi possível ler o campo 'removido', sair do loop
            free(aux_r);
            break;
        }
        valor++; //RRN é somado mais um
        
        if(aux_r->removido=='0'){ //caso o registro exista
            
            fseek(arquivo, 12, SEEK_CUR); //pulamos as doze posiçoes dos inteiros anteriores (grupo, popularidade, peso)
            fread(&aux_r->nomeTecnologiaOrigem.tamanho, sizeof(int), 1, arquivo); //lemos o tamanho da tecnologia origem no arquivo binario
            aux_r->nomeTecnologiaOrigem.string = (char *)malloc(aux_r->nomeTecnologiaOrigem.tamanho + 1); //alocamos memoria para salvarmos essa variavel
            fread(aux_r->nomeTecnologiaOrigem.string, aux_r->nomeTecnologiaOrigem.tamanho, 1, arquivo); //lemos e armazenamos o nomeTecnologiaOrigem
            aux_r->nomeTecnologiaOrigem.string[aux_r->nomeTecnologiaOrigem.tamanho] = '\0'; //garante o final da string
            fread(&aux_r->nomeTecnologiaDestino.tamanho, sizeof(int), 1, arquivo); //lemos o tamanho da tecnologia destino no arquivo binario
            aux_r->nomeTecnologiaDestino.string = (char *)malloc(aux_r->nomeTecnologiaDestino.tamanho + 1); //alocamos memoria para salvarmos essa variavel
            fread(aux_r->nomeTecnologiaDestino.string, aux_r->nomeTecnologiaDestino.tamanho, 1, arquivo); //lemos e armazenamos o nomeTecnologiaDestino
            aux_r->nomeTecnologiaDestino.string[aux_r->nomeTecnologiaDestino.tamanho] = '\0';
            /*para a leitura do lixo, iremos calcular o tamanho total, ou seja, retiraremos do tamanho total do registro, que é 76,
            tudo o que já foi lido anteriormente*/
            int tamanhototal = 76 - (1 + 5 * sizeof(int) + aux_r->nomeTecnologiaDestino.tamanho + aux_r->nomeTecnologiaOrigem.tamanho);
            fseek(arquivo, tamanhototal, SEEK_CUR); //pularemos o tamanho total (que seria o nosso lixo)

             /*para printarmos a resposta do que está sendo procurado, verificaremos que a string 
             a1 dada é igual a nomeTecnologiaOrigem que acabamos de ler*/
             if(aux_r->nomeTecnologiaOrigem.tamanho!=0 && aux_r->nomeTecnologiaDestino.tamanho!=0){
                strcpy(concatenada2, aux_r->nomeTecnologiaOrigem.string); // Copie string1 para a nova variável
                strcat(concatenada2, aux_r->nomeTecnologiaDestino.string); //concatena string
                if(strcmp(concatenada1, concatenada2) == 0){ //caso for igual
                    flag = 1; //variavel auxiliar recebe um, simbolizando que encontramos o que estavamos procurando
                    return 1;
            }}
            //libera memoria
            free(aux_r->nomeTecnologiaOrigem.string); 
            free(aux_r);
        }
        else{
            fseek(arquivo, 75, SEEK_CUR);
        }
    }
    if(flag == 0){ //caso nossa flag seja zero, isso significa que nada foi encontrado, por este motivo nosso registro é inexistente
        return 0;
    }
}

//FUNÇOES DA FUNCIONALIDADE 5 QUE AINDA ESTA EM CONSTRUÇÃO
//faz a leitura de um nó, o nó escolhido é passado como parâmetro
void ler_no(FILE *arquivoIndEntrada, NoRegistro *no_r){
    char aux1[55], aux2[55], aux3[55];
    
    fread(&no_r->nroChavesNo, sizeof(int), 1, arquivoIndEntrada);
    fread(&no_r->alturaNo, sizeof(int), 1, arquivoIndEntrada);
    fread(&no_r->RRNdoNo, sizeof(int), 1, arquivoIndEntrada);

    fread(&no_r->P[0], sizeof(int), 1, arquivoIndEntrada);
    fread(aux1, 55, 1, arquivoIndEntrada);
    int i;
    for(i =0; i<55; i++){
        if(aux1[i]=='$'){
            break;
        }
    }
    aux1[i] = '\0';

 
    strcpy(no_r->C[0], separaPalavra(aux1));
    fread(&no_r->PR[0], sizeof(int), 1, arquivoIndEntrada);

    fread(&no_r->P[1], sizeof(int), 1, arquivoIndEntrada);
    fread(aux2, 55, 1, arquivoIndEntrada);
    
    for(i =0; i<55; i++){
        if(aux2[i]=='$'){
            break;
        }
    }
    aux2[i] = '\0';
    strcpy(no_r->C[1], separaPalavra(aux2));
    fread(&no_r->PR[1], sizeof(int), 1, arquivoIndEntrada);

    fread(&no_r->P[2], sizeof(int), 1, arquivoIndEntrada);
    fread(aux3, 55, 1, arquivoIndEntrada);
   
    for(i =0; i<55; i++){
        if(aux3[i]=='$'){
            break;
        }
    }
    aux3[i] = '\0';
    strcpy(no_r->C[2], separaPalavra(aux3));

    
    fread(&no_r->PR[2], sizeof(int), 1, arquivoIndEntrada);

    fread(&no_r->P[3], sizeof(int), 1, arquivoIndEntrada);
}

void caminha_arvore(FILE *arquivo, NoRegistro *no, int tamanho){
    fseek(arquivo, tamanho, SEEK_SET); //pula para a posição desejada
    ler_no(arquivo, no); //faz a leitura do nó
}