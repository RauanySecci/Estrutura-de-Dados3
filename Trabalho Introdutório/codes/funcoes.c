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

/*função auxiliar para quando iremos ler o peso
essa função auxiliar é chamada quando o peso existe, ou seja, é diferente de -1*/
void realoca1(FILE* file) {
    char c; //variavel auxiliar
    if(fread(&c, 1, 1, file) == 1 && c != ',' && c!='\n' && c!='\0') { //se a leitura do meu proximo char c existir e for diferente de , \n ou \0
        fseek(file, +1, SEEK_CUR); // Adianta uma posição para não perder o próximo caractere (excluindo o \n que ficava junto com a proxima variavel nomeTecnologia Origem)
    }
}

/*função auxiliar para quando iremos ler o peso
essa função auxiliar é chamada quando o peso não existe, ou seja, é -1*/
void realoca2(FILE* file) {
    char c; //variavel auxiliar
    if(fread(&c, 1, 1, file) == 1 && c != ',' && c!='\n' && c!='\0') { //se a leitura do meu proximo char c existir e for diferente de , \n ou \0
        fseek(file, -1, SEEK_CUR); // Volta uma posição para não perder o próximo caractere de nomeTecnologiaOrigem
    }
}

//função para auxiliar na contagem de quantas tecnologias diferntes temos, armazenadas no cabeçalho
int stringExiste(char *lista[], int tamanho, char *novaString) {
    for (int i = 0; i < tamanho; i++) { //percorre a lista
        if (strcmp(lista[i], novaString) == 0) { //compara duas strings e verificam se elas são iguais
            return 1; // A string já existe na lista
        }
    }
    return 0; // A string não existe na lista
}

