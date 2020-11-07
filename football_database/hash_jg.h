/*
 * Ficheiro: hash_jg.h
 * Autor: Maria Campos
 * Descricao: ficheiro de cabecalho que contem as declaracoes das funcoes que manipulam 
 a estrutura jogo, a hashtable de jogos e a lista duplamente ligada.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "func_hash.h"

#define MAX_NOME 1024
#define TAMANHO 1049

typedef struct jogo
{
    char * nome;
    char * equipa1;
    char * equipa2;
    int pontos1;
    int pontos2;

}*Jogo;

typedef struct nodulos* lJogo; 

struct nodulos
{
    Jogo jogo;
    lJogo prox;
};

typedef struct noduloJ* ligacao;

/* Estruturas da lista duplamente ligada. */
struct noduloJ 
{
	Jogo jogo;
	ligacao prox;
	ligacao ant;
};

typedef struct nodulo_L 
{
	int tamanho;
	ligacao inicio;
	ligacao resto;
}* LDupla;

/* Funcoes da estrutura jogo. */
Jogo cria_jogo(char *nome, char *equipa1, char *equipa2, int pts1, int pts2);
void destroi_jogo(Jogo jogo);

/* Funcoes da hashtable. */
void apaga_nodulos(lJogo nodulos);
void destroi_hash_jg(lJogo* jogos);
Jogo procura_jg_lista(lJogo inicio, char nome[MAX_NOME]);
lJogo insere_jg_lista(lJogo inicio, Jogo jogo);
lJogo apaga_jg_lista(lJogo inicio, char nome[MAX_NOME]);
void destroi_lista_jg(lJogo inicio);
lJogo* inicializa_jg();
void insere_jg_hash(lJogo* jogos, Jogo jogo);
void apaga_jg_hash(lJogo* jogos, char nome[MAX_NOME]) ;
Jogo procura_jg_hash(lJogo* jogos,char nome[MAX_NOME]);

/* Funcoes da lista duplamente ligada. */
LDupla inicializa_lista();
ligacao cria_nodulo(ligacao prox, ligacao ant, Jogo jogo);
void apaga_nodulo_jg(ligacao nodulo);
ligacao procura_jg_lst(LDupla lista, char nome[MAX_NOME]);
void insere_jg_lst(LDupla lista, Jogo jogo);
void primeiro(LDupla lista); /* pop */
void ultimo(LDupla lista);
void apaga_jg_lst(LDupla lista, char nome[MAX_NOME]);
void destroi_lista(LDupla lista);
