/*
 * Ficheiro: hash_eq.h
 * Autor: Maria Campos
 * Descricao: ficheiro de cabecalho que contem as declaracoes das funcoes que manipulam 
 a hashtable das equipas
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "func_hash.h"

#define MAX_NOME 1024
#define TAMANHO 1049

typedef struct equipa
{
    char* nome;
    int pontos;
    
}*Equipa;

typedef struct nodulo* lEquipa; 

struct nodulo 
{
    Equipa equipa;
    lEquipa prox;
};

/* Funcoes da estrutura equipa. */
Equipa cria_equipa(char *nome, int  pontos);
void destroi_equipa(Equipa equipa);

/* Funcoes da hashtable. */
void apaga_nodulo(lEquipa nodulo);
lEquipa insere_eq_lista(lEquipa inicio, Equipa equipa);
Equipa procura_eq_lista(lEquipa inicio, char nome[MAX_NOME]);
void destroi_lista_eq(lEquipa inicio);
lEquipa* inicializa_eq();
void insere_equipa_hash(lEquipa* equipas, Equipa equipa);
void destroi_hash_eq(lEquipa* equipas);
Equipa procura_eq_hash(lEquipa* equipas,  char nome[MAX_NOME]);
int ordem_alfabetica(const void * pal1, const void * pal2);
int hash_vazia(lEquipa* equipas);
