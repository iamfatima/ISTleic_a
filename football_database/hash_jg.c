/*
 * Ficheiro: hash_jg.c
 * Autor: Maria Campos
 * Descricao: ficheiro que contem o codigo das funcoes que manipulam a 
 estrutura jogo, a hashtable dos jogos e a lista duplamente ligada.
*/

#include "hash_jg.h"

/* Funcao que cria e aloca memoria para um jogo. */
Jogo cria_jogo(char *nome, char *equipa1, char *equipa2, int pts1, int pts2)
{
  Jogo novo = (Jogo) malloc(sizeof(struct jogo));

  novo->nome = str_dup(nome);
  novo->equipa1 = str_dup(equipa1);
  novo->equipa2 = str_dup(equipa2);
  novo->pontos1 = pts1;
  novo->pontos2 = pts2;

  return novo;
}

/* Funcao que liberta memoria alocada previamente para guardar um jogo. */
void destroi_jogo(Jogo jogo)
{
  if (jogo)
  {
    free(jogo->nome);
    free(jogo->equipa1);
    free(jogo->equipa2);
    free(jogo);
  }
}

/* Funcao que apaga um nodulo de uma lista ligada na hashtable. */
void apaga_nodulos(lJogo nodulos)
{
  destroi_jogo(nodulos->jogo);
  free(nodulos);
}

/* Funcao que procura um jogo numa lista ligada na hashtable, dado um nome. */
Jogo procura_jg_lista(lJogo inicio, char nome[MAX_NOME])
{
  if (inicio == NULL) 
  {
    return NULL;
  }

  if (strcmp(nome, inicio->jogo->nome) == 0) 
  {
    return inicio->jogo;
  }

  return procura_jg_lista(inicio->prox,nome);
}

/* Funcao que insere um jogo numa lista ligada na hashtable. */
lJogo insere_jg_lista(lJogo inicio, Jogo jogo)
{
  lJogo novo = (lJogo) malloc(sizeof(struct nodulos));
  novo->jogo= jogo;
  novo->prox = inicio;

  return novo;
}

/* Funcao que apaga um jogo da lista ligada na hashtable. */
lJogo apaga_jg_lista(lJogo inicio, char nome[MAX_NOME])
{
  lJogo t, ant; 

  for(t = inicio, ant = NULL; t != NULL; ant = t, t = t->prox)
  {
    if(strcmp(t->jogo->nome, nome) == 0) 
    {
      if(t == inicio) 
      {
        inicio = t->prox;
      }
      else
      {
        ant->prox = t->prox;
      }

      apaga_nodulos(t);

      return inicio;
    }
  }
  return inicio;
}

/* Funcao que liberta a memoria associada a uma lista ligada na hashtable. */
void destroi_lista_jg(lJogo inicio)
{
  lJogo atual, aux;

  if (!inicio)
  {
    return;
  }

  for (atual = inicio, aux = inicio->prox; atual; 
  aux = atual->prox, apaga_nodulos(atual), atual = aux)
  {
    ;
  }
}

/* Funcao que inicializa a hashtable dos jogos. */
lJogo* inicializa_jg()
{ 
  int i, m = TAMANHO;

  lJogo* jogos = (lJogo*) malloc(m*sizeof(lJogo));

  for(i = 0; i < m; i++)
  {
    jogos[i] = NULL; 
  }

  return jogos;
}

/* Funcao que insere um jogo na hashtable dos jogos. */
void insere_jg_hash(lJogo* jogos, Jogo jogo)
{
  int i = hash(jogo->nome, TAMANHO);
  jogos[i] = insere_jg_lista(jogos[i], jogo);
}

/* Funcao que apaga um jogo na hashtable, dado um nome. */
void apaga_jg_hash(lJogo* jogos, char nome[MAX_NOME]) 
{
  int i = hash(nome, TAMANHO);
  jogos[i] = apaga_jg_lista(jogos[i], nome);
}

/* Funcao que procura um jogo na hashtable, dado um nome. */
Jogo procura_jg_hash(lJogo* jogos,char nome[MAX_NOME])
{
  int i = hash(nome, TAMANHO);

  return procura_jg_lista(jogos[i], nome);
}

/* Funcao que liberta a memoria associada a hashtable dos jogos. */
void destroi_hash_jg(lJogo* jogos)
{
  int i, m = TAMANHO;

  for (i = 0; i < m; i++)
  {
    destroi_lista_jg(jogos[i]);
  }

  free(jogos);
}

/* Funcao que inicializa a lista duplamente ligada. */
LDupla inicializa_lista()
{
	LDupla lista = malloc(sizeof(struct nodulo_L));
  
  lista->tamanho = 0;
	lista->inicio = NULL;
	lista->resto = NULL;

	return lista;
}

/* Funcao que cria um novo nodulo na lista duplamente ligada. */
ligacao cria_nodulo(ligacao prox, ligacao ant, Jogo jogo) 
{
	ligacao novo = malloc(sizeof(struct noduloJ));

	novo->jogo = jogo;
	novo->ant = ant;
	novo->prox = prox;

	if (ant != NULL)
  {
    ant->prox = novo;
  } 

	if (prox != NULL)
  {
    prox->ant = novo;
  } 

	return novo;
}

/* Funcao que liberta memoria associada as ligacoes da lista duplamente ligada. */
void apaga_nodulo_jg(ligacao nodulo) 
{
	free(nodulo);
}

/* Funcao que insere um jogo na lista duplamente ligada. */
void insere_jg_lst(LDupla lista, Jogo jogo) 
{
	ligacao novo = cria_nodulo(NULL, lista->resto, jogo);

	lista->resto = novo;

	if (lista->inicio == NULL)
  {
    lista->inicio = novo;
  } 

	lista->tamanho++;

}

/* Funcao que remove o ultimo jogo da lista duplamente ligada. */
void ultimo(LDupla lista) 
{
  ligacao aux = lista->resto;

	if (lista == NULL || lista->tamanho == 0)
  {
    return;
  } 
	
	lista->resto = aux->ant;

	if (lista->resto != NULL)
  {
    lista->resto->prox = NULL;
  } 

	else
  {
    lista->inicio = NULL;
  } 

	apaga_nodulo_jg(aux);

	lista->tamanho--;
}

/* Funcao que remove o primeiro jogo da lista duplamente ligada. */
void primeiro(LDupla lista) 
{
  ligacao aux = lista->inicio;

	if (lista->inicio == NULL || lista->tamanho == 0) 
  {
    return;
  }

	lista->inicio = aux->prox;

	if(lista->inicio != NULL)
  {
    lista->inicio->ant = NULL;
  } 

	else
  {
    lista->resto = NULL;
  } 

	apaga_nodulo_jg(aux);

	lista->tamanho--;
}

/* Funcao que procura um jogo na lista duplamente ligada, dado um nome. */
ligacao procura_jg_lst(LDupla lista, char nome[MAX_NOME])
{
  ligacao atual;

	for (atual = lista->inicio; atual != NULL; atual = atual->prox) 
  {
		if (strcmp(nome,atual->jogo->nome) == 0)
    {
      return atual;
    }
	}

	return NULL;
}

/* Funcao que apaga um jogo da lista duplamente ligada. */
void apaga_jg_lst(LDupla lista, char nome[MAX_NOME]) 
{
  ligacao atual = procura_jg_lst(lista, nome);

	if ((lista == NULL) || (lista->tamanho == 0))
  {
    return;
  } 
	
  /* Elemento eh o primeiro da lista. */
	if (atual == lista->inicio)
  {
    primeiro(lista);
  } 
  /* Elemento eh o ultimo da lista. */
	else if (atual == lista->resto)
  {
    ultimo(lista);
  } 
  /* Elemento eh qualquer outro da lista */
	else 
  {
		atual->ant->prox = atual->prox;

		atual->prox->ant = atual->ant;

		apaga_nodulo_jg(atual);

		lista->tamanho--;
	}
}

/* Funcao que liberta a memoria associada a lista duplamente ligada. */
void destroi_lista(LDupla lista) 
{
	ligacao atual, aux;
	if (lista == NULL)
  {
    return;
  } 
	if (lista->tamanho != 0)
  {
    for (atual = lista->inicio, aux = lista->inicio->prox; atual != NULL; 
    aux = atual->prox, apaga_nodulo_jg(atual), atual = aux)
    {
      ;
    }
  }

	free(lista);
}


