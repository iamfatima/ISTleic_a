/*
 * Ficheiro: proj2.c
 * Autor: Maria Campos
 * Descricao: ficheiro de codigo de um programa que consiste num sistema de gestao de 
 uma base de dados de jogos de futebol amigaveis.
*/

#include "proj2.h"

/* Le o comando a ser executado */
int main()
{
    char comando;
    
    int NL = 0, *ptr;

    lEquipa* equipas = inicializa_eq();

    lJogo* jogos = inicializa_jg();

    LDupla lst_jgs = inicializa_lista();

    ptr = &NL;

    while ((comando = getchar()) != 'x') /* Se o comando for x, o programa termina. */
    {
        switch (comando)
        {
        case ('a'):
            adiciona_jogo(ptr, jogos, equipas, lst_jgs);
            break;

        case ('A'):
            adiciona_equipa(ptr, equipas);
            break;

        case('l'):
            lista_jogos(ptr, lst_jgs);
            break;

        case('p'):
            procura_jogo(ptr, jogos);
            break;

        case('P'):
            procura_equipa(ptr, equipas);
            break;
        
        case ('r'):
            apaga_jogo(ptr, jogos, equipas, lst_jgs);
            break;

        case ('s'):
            altera_pontos(ptr, jogos, equipas, lst_jgs);
            break;
        
        case ('g'):
            melhores_equipas(ptr, equipas);
            break;
        }
    }

    destroi_hash_eq(equipas); 
    destroi_lista(lst_jgs);
    destroi_hash_jg(jogos);
   
    return 0;
}

/* Funcao que adiciona um novo jogo ao sistema, devolve erro caso o jogo ja existir ou alguma das equipas envolvidas */
/* nao existir. */
void adiciona_jogo(int *ptr, lJogo* jogos, lEquipa* equipas, LDupla lst_jgs)
{
    char nome_jogo[MAX_NOME], equipa1[MAX_NOME], equipa2[MAX_NOME];
    int pontos1, pontos2;
    Jogo jogo;
    Equipa eq1, eq2;

    ++(*ptr);

    scanf(" %[^:\n]:%[^:\n]:%[^:\n]:%d:%d", nome_jogo, equipa1, equipa2,&pontos1,&pontos2);

    jogo = procura_jg_hash(jogos,nome_jogo);
    eq1 = procura_eq_hash(equipas,equipa1);
    eq2 = procura_eq_hash(equipas,equipa2);


    if (jogo != NULL)
    {
        printf("%d Jogo existente.\n",*ptr);
    }
    else if((eq1 == NULL) || (eq2 == NULL))
    {
        printf("%d Equipa inexistente.\n",*ptr);
    }
    else
    {
        jogo = cria_jogo(nome_jogo, equipa1, equipa2, pontos1, pontos2);

        insere_jg_hash(jogos,jogo);
        
        insere_jg_lst(lst_jgs, jogo);

        if (pontos1 > pontos2)
        {
           eq1->pontos +=1;
            
        }
        else if (pontos2 > pontos1)
        {
            eq2->pontos +=1; 
            
        }

    }

}

/* Funcao que adiciona uma nova equipa ao sistema, retorna erro se a equipa ja existir no sistema. */
void adiciona_equipa(int *ptr, lEquipa *equipas)
{
    char nome[MAX_NOME];
    Equipa equipa;

    scanf(" %[^:\n]", nome);

    equipa = procura_eq_hash(equipas,nome);

    ++(*ptr);  

    if (equipa != NULL)
    {
        printf("%d Equipa existente.\n", *ptr);
    }
    else
    {
        insere_equipa_hash(equipas,cria_equipa(nome,0));
    }

}

/* Funcao que lista todos os jogos pela ordem em que foram introduzidos, uma linha por jogo, */
/* nao retorna nada. */
void lista_jogos(int *ptr, LDupla lst_jgs)
{
    ligacao atual;

    ++(*ptr);

    for (atual = lst_jgs->inicio; atual != NULL; atual = atual->prox) 
    {
		printf("%d %s %s %s %d %d\n",*ptr, atual->jogo->nome,atual->jogo->equipa1, 
        atual->jogo->equipa2,atual->jogo->pontos1, atual->jogo->pontos2);
	}
    
}      

/* Funcao que procura um jogo com o nome dado, retorna erro se nao existir nenhum jogo no sistema */
/* com o nome introduzido. */
void procura_jogo(int *ptr, lJogo* jogos)
{
    char nome[MAX_NOME];
    Jogo jogo;

    scanf(" %[^:\n]", nome);

    jogo = procura_jg_hash(jogos, nome);

    ++(*ptr);

    if (jogo == NULL)
    {
        printf("%d Jogo inexistente.\n",*ptr);
    }
    else
    {
        printf("%d %s %s %s %d %d\n",*ptr, jogo->nome,jogo->equipa1,
        jogo->equipa2, jogo->pontos1, 
        jogo->pontos2);
    }
    
}

/* Funcao que procura uma equipa dado um nome, retorna o nome da equipa e o numero de jogos ganhos, */
/* retorna erro se nao existir nenhuma equipa no sistema com o nome introduzido. */
void procura_equipa(int *ptr, lEquipa* equipas)
{
    char nome_equipa[MAX_NOME];
    Equipa equipa;

    scanf(" %[^:\n]", nome_equipa);

    equipa = procura_eq_hash(equipas,nome_equipa);

    ++(*ptr);

    if (equipa == NULL)
    {
        printf("%d Equipa inexistente.\n",*ptr);
    }
    else
    {
        printf("%d %s %d\n",*ptr, equipa->nome, equipa->pontos);
    }
    
}

/* Funcao que apaga um jogo dado um nome, nao retorna nada exceto erro se nao existir no sistema */
/* nenhum jogo com o nome introduzido. */
void apaga_jogo(int *ptr, lJogo* jogos, lEquipa* equipas, LDupla lst_jgs)
{
    char nome[MAX_NOME];
    Jogo jogo;

    scanf(" %[^:\n]", nome);

    jogo = procura_jg_hash(jogos, nome);

    ++(*ptr);

    if (jogo == NULL)
    {
        printf("%d Jogo inexistente.\n",*ptr);
    }
    else
    {
        if (jogo->pontos1 > jogo->pontos2)
        {
            procura_eq_hash(equipas,jogo->equipa1)->pontos -=1;
        }
        else if (jogo->pontos2 > jogo->pontos1)
        {
            procura_eq_hash(equipas,jogo->equipa2)->pontos -=1;
        }
        
        apaga_jg_lst(lst_jgs, nome);
        apaga_jg_hash(jogos, nome);
        
    }
    

}

/* Funcao que altera os pontos de um jogo dado um nome, retorna erro se nao existir no sistema */
/* nenhum jogo com o nome introduzido. */
void altera_pontos(int *ptr, lJogo* jogos, lEquipa* equipas, LDupla lst_jgs)
{
    char nome_jogo[MAX_NOME];
    int pontos1, pontos2, pontos_ant1 = 0, pontos_ant2 = 0;
    Jogo jogo;

    scanf(" %[^:\n]:%d:%d", nome_jogo, &pontos1, &pontos2);

    jogo = procura_jg_hash(jogos,nome_jogo);

    ++(*ptr);

    if (jogo == NULL)
    {
        printf("%d Jogo inexistente.\n",*ptr);
    }
    else
    {

        pontos_ant1 = jogo->pontos1;
        pontos_ant2 = jogo->pontos2;

        /* Empate anterior. */
        if (pontos_ant1 == pontos_ant2)  
        {
            
            if (pontos1 > pontos2) 
            {
                procura_eq_hash(equipas,jogo->equipa1)->pontos +=1;
            }
            else if (pontos1 < pontos2)
            {
                procura_eq_hash(equipas,jogo->equipa2)->pontos +=1;
            }
            
            
        }
        /* Empate atual. */
        else if (pontos1 == pontos2)
        {
            if (pontos_ant1 > pontos_ant2)
            {
                procura_eq_hash(equipas,jogo->equipa1)->pontos -=1;
            }
            else if (pontos_ant2 > pontos_ant1)
            {
                procura_eq_hash(equipas,jogo->equipa2)->pontos -=1;
            }
            
        }
        /* Equipa 1 ganha jogo anterior. */
        else if (pontos_ant1 > pontos_ant2)
        {
            if (pontos2 > pontos1)
            {
                procura_eq_hash(equipas,jogo->equipa1)->pontos -=1;
                procura_eq_hash(equipas, jogo->equipa2)->pontos +=1;
            }

        }

         /* Equipa 2 ganha jogo anterior. */
        else if (pontos_ant2 > pontos_ant1)
        {
            if (pontos1 > pontos2) 
            {
                procura_eq_hash(equipas,jogo->equipa2)->pontos -=1;
                procura_eq_hash(equipas, jogo->equipa1)->pontos +=1;
            }
            
        }

        jogo->pontos1 = pontos1;
        jogo->pontos2 = pontos2;

        procura_jg_lst(lst_jgs, nome_jogo)->jogo->pontos1 = pontos1;
        procura_jg_lst(lst_jgs, nome_jogo)->jogo->pontos2 = pontos2;

        
    }
    
}

/* Funcao que encontra as equipas que ganharam mais jogos e lista por ordem alfabetica. */
void melhores_equipas(int *ptr, lEquipa* equipas)
{   
    int i,j = 0, max = 0, num_els = 0;

    ++(*ptr);

    if (hash_vazia(equipas) != 1)
    {
        char** tabela;

        lEquipa atual, prox;

        /* Descobrir pontuacao max e numero de equipas com a mesma.*/
        for (i = 0; i < TAMANHO; i++)
        {
            for (atual = equipas[i]; atual != NULL; atual = prox)
            {

                if(atual->equipa->pontos > max)
                {
                    max = atual->equipa->pontos;
                    num_els = 1;
                }
                else if(atual->equipa->pontos == max)
                {
                    num_els++;
                }

                prox = atual->prox;

            }

        }

        tabela = (char**) malloc(sizeof(char*)*num_els);

        for (i = 0; i < TAMANHO; i++)
        {
            for (atual = equipas[i]; atual != NULL; atual = prox)
            {

                if(atual->equipa->pontos == max)
                {
                    tabela[j] = str_dup(atual->equipa->nome);
                    j++;
                }

                if (j == num_els)
                {
                    break;
                }
                
                prox = atual->prox;

            }

            if (j == num_els)
            {
                break;
            }
        }

        qsort(tabela,num_els,sizeof(tabela[0]),ordem_alfabetica);
        
        
        printf("%d Melhores %d\n", *ptr, max);

        for (i = 0; i < num_els; i++)
        {
            printf("%d * %s\n",*ptr, tabela[i]);
            free(tabela[i]);
        }

        free(tabela);

    }
    
}
