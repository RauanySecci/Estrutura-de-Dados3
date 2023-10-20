// Karine Cerqueira Nascimento 13718404
// Rauany Martinez Secci 13721217

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "funcionalidades.h"

int main() {
    int num1; //funcionalidade chamada
    char a1[20]; // Array para a primeira string
    char a2[20]; // Array para a segunda string
    int nRRN; //RRN dado pelo usuário
    int n; //quantidade de vezes que será buscado na funcionalidade 3
    scanf("%d ", &num1); //leitura de num1 que faz a escolha da funcionalidade

    //le os parametros de cada funcionalidade
    if (num1 == 1){
        scanf("%s %s", a1, a2); //nome do arquivo csv e do arquivo binario
        funcionalidade1(a1, a2); //chama funcionalidade 1
        
    }else if (num1 == 2){
        scanf("%s", a2); //nome do arquivo binario
        funcionalidade2(a2); //chama funcionalidade 2

    }else if (num1==3){
        scanf("%s %d", a2, &n); //nome do arquivo binario e quantidade de vezes que será buscado na funcionalidade 3
        funcionalidade3(a2, n); //chama funcionalidade 3

    }else if(num1==4){
        scanf("%s %d", a2, &nRRN); //nome do arquivo binario e RRN do registro que será buscado
        funcionalidade4(a2, nRRN); //chama funcionalidade 4
    }  
}