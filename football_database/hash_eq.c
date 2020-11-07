/*
 * Ficheiro: hash_eq.c
 * Autor: Maria Campos
 * Descricao: codigo das funcoes auxiliares que manipulam a hashtable das equipas.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hash_eq.h"

/* Funcao que cria e aloca memoria para uma equipa. */
Equipa cria_equipa(char *nome, int  pontos)
{
    Equipa nova = (Equipa) malloc(sizeof(struct equipa));
    
    nova->nome = str_dup(nome);
    nova->pontos = pontos;
    return nova;
}

/* Funcao que liberta memoria alocada previamente para guardar uma equipa. */
void destroi_equipa(Equipa equipa)
{ 
    free(equipa->nome);
    free(equipa); 
}

/* Funcao que apaga um nodulo de uma lista ligada na hashtable. */
void apaga_nodulo(lEquipa nodulo)
{
    destroi_equipa(nodulo->equipa);
    free(nodulo);
}

/* Funcao que insere uma equipa numa lista ligada na hashtable. */
lEquipa insere_eq_lista(lEquipa inicio, Equipa equipa)
{
    lEquipa novo = (lEquipa) malloc(sizeof(struct nodulo));

    novo->equipa = equipa;
    novo->prox = inicio;
    return novo;
}

/* Funcao que procura uma equipa numa lista ligada na hashtable, dado um nome. */
Equipa procura_eq_lista(lEquipa inicio, char nome[MAX_NOME])
{   
    if (inicio == NULL) 
    {
        return NULL;
    }

    if (strcmp(nome, inicio->equipa->nome) == 0) 
    {
        return inicio->equipa;
    }

    return procura_eq_lista(inicio->prox, nome);
}

/* Funcao que liberta a memoria associada a uma lista ligada na hashtable. */
void destroi_lista_eq(lEquipa inicio)
{
    lEquipa atual, aux;

    if (!inicio)
    {
        return;
    }

    for (atual = inicio, aux = inicio->prox; atual; aux = atual->prox, apaga_nodulo(atual), atual = aux)
    {
        ;
    }
}

/* Funcao que inicializa a hashtable das equipas. */
lEquipa* inicializa_eq() 
{ 
    int i, m = TAMANHO;

    lEquipa* equipas = (lEquipa*) malloc(m*sizeof(lEquipa));

    for(i = 0; i < m; i++)
    {
        equipas[i]=NULL; 
    }

    return equipas;
}

/* Funcao que insere uma equipa na hashtable das equipas. */
void insere_equipa_hash(lEquipa* equipas, Equipa equipa)
{
    int i = hash(equipa->nome, TAMANHO);
    equipas[i] = insere_eq_lista(equipas[i], equipa);
}

/* Funcao que liberta a memoria associada a hashtable das equipas. */
void destroi_hash_eq(lEquipa* equipas)
{
    int i, m = TAMANHO;
   
    for (i = 0; i < m; i++)
    {
        destroi_lista_eq(equipas[i]);
    }

    free(equipas);
}

/* Funcao que procura uma equipa na hashtable, dado um nome. */
Equipa procura_eq_hash(lEquipa* equipas,  char nome[MAX_NOME]) 
{
	int i = hash(nome, TAMANHO);
	return procura_eq_lista(equipas[i], nome);
}

/* Funcao que compara nomes de equipas, para, posteriormente, os ordenar. */
int ordem_alfabetica(const void * pal1, const void * pal2) 
{
    const char ** p1 = (const char **) pal1;
    const char ** p2 = (const char **) pal2;

    return strcmp(*p1,*p2);
}

/* Funcao que retorna 1 se a hashtable estiver vazia e 0, caso contrario. */
int hash_vazia(lEquipa* equipas)
{
    int i;

    for(i = 0; i < TAMANHO; i++)
    {
        if (equipas[i] != NULL)
        {
            return 0;
        }
    }
    return 1;
}
