/*
 * Ficheiro: proj.c
 * Autor: Maria Campos
 * Descricao: programa que consiste num sistema de logistica que gere produtos e encomendas.
*/

#include <stdio.h>
#include <string.h>

/* Valores maximos e constantes booleanas guardadas. */
#define MAX_PROD 10000 
#define MAX_ENC 500 
#define PESO_MAX 200
#define MAX_DESCRICAO 64
#define TRUE 1
#define FALSE -1

/* Definicao das estruturas produto e encomenda. */

typedef struct produto
{
    int idp;
    char descricao[MAX_DESCRICAO];
    int preco;
    int peso;
    int qtd_stock;

}Produto;

typedef struct encomenda
{
    int ide;
    int num_prods; /* Numero de produtos numa encomenda. */
    int peso;
    Produto produto[PESO_MAX]; /* Cada encomenda podera ter no maximo 200 produtos de quantidade 1. */

}Encomenda;

/* Prototipos das funcoes principais. */
void comando_a();
void comando_q();
void comando_N();
void comando_A();
void comando_r();
void comando_R();
void comando_p();
void comando_l();
void comando_C();
void comando_E();
void comando_m();
void comando_L();

/* Prototipos das funcoes auxiliares. */
int procura_produto(int ident_prod);
int procura_encomenda(int ident_enc);
int procura_produto_em_encomenda(int ident_enc, int ident_prod);
void mergesort(Produto lista[], int esquerda, int direita);
void merge(Produto lista[],int esquerda, int m, int direita);

/* Atribui o identificador aos produtos que sao criados, bem como informa sobre a quantidade de produtos no sistema. */
int id_produto = 0; 

/* Atribui o identificador as encomendas que sao criadas, bem como informa sobre a quantidade de encomendas no sistema. */
int id_encomenda = 0;

/* Vetor que guarda os produtos registados no sistema e suas caracteristicas. */
Produto lista_produtos_sistema[MAX_PROD];

/* Vetor que guarda as encomendas registadas no sistema e suas caracteristicas */
Encomenda lista_encomendas_sistema[MAX_ENC];

/* Le o comando a ser executado */
int main()
{
    char comando;
    
    while ((comando = getchar()) != 'x') /* Se o comando for x, o programa termina. */
    {
        switch (comando)
        {
        case ('a'):
            comando_a();
            break;

        case ('q'):
            comando_q();
            break;

        case ('N'):
            comando_N();
            break;
        
        case ('A'):
            comando_A();
            break;

        case('r'):
            comando_r();
            break;

        case('R'):
            comando_R();
            break;

        case('C'):
            comando_C();
            break;
        
        case ('p'):
            comando_p();
            break;
        
        case ('E'):
            comando_E();
            break;

        case ('m'):
            comando_m();
            break;
        
        case ('l'):
            comando_l();
            break;
        
        case ('L'):
            comando_L();
            break;
        }
    }
    return 0;
}

/* Definicao das funcoes principais. */

/* Funcao que adiciona um novo produto ao sistema, associando-lhe uma descricao, preco, peso, quantidade e idp, bem como 
o adiciona a lista de produtos registados no sistema. */
void comando_a()
{     
    Produto produto;
    char desc[MAX_DESCRICAO];
    int preco, peso, qtd;

    scanf(" %[^:]:%d:%d:%d", desc, &preco, &peso, &qtd);

    /* Criacao do produto. */
    produto.idp = id_produto;
    strcpy(produto.descricao, desc);
    produto.preco = preco;
    produto.peso = peso;
    produto.qtd_stock = qtd;
   
    printf("Novo produto %d.\n", id_produto);

    /* Guardar o novo produto no sistema. */
    lista_produtos_sistema[id_produto] = produto;

    /* A variavel id_produto aumenta, para atribuir o idp a um eventual proximo produto. */
    id_produto += 1;
}

/* Funcao que adiciona stock a um produto existente no sistema, imprime erro se o id do produto fornecido nao estiver associado 
a nenhum produto do sistema. */
void comando_q()
{
    int ident_prod, qtd;

    scanf(" %d:%d", &ident_prod ,&qtd);

    /* Procurar produto no sistema. */
    if (procura_produto(ident_prod) == TRUE) 
    {
        /* Adicionar stock. */
        lista_produtos_sistema[ident_prod].qtd_stock += qtd;

        return;
    }

    printf("Impossivel adicionar produto %d ao stock. Produto inexistente.\n", ident_prod);
}

/* Funcao que cria uma encomenda no sistema, associando-lhe um id e registando-a na lista de encomendas do sistema. */
void comando_N()
{
    Encomenda enc;

    enc.ide = id_encomenda;
    enc.peso = 0;
    enc.num_prods = 0;

    /* Guardar encomenda no sistema. */
    lista_encomendas_sistema[id_encomenda] = enc;

    printf("Nova encomenda %d.\n", id_encomenda);  

    /* Variavel id_encomenda eh icrementada, para atribuir o ide a uma proxima encomenda. */
    id_encomenda += 1;
}

/* Funcao que adiciona um produto do sistema a uma encomenda, imprime erro se a encomenda nao existir,
se o produto nao existir, se a quantidade encomendada exceder o stock disponivel ou se o peso da encomenda exceder 200. */
void comando_A()
{
    int ident_enc, ident_prod, qtd;

    scanf(" %d:%d:%d", &ident_enc, &ident_prod, &qtd);

    /* Verificar se encomenda existe no sistema. */
    if (procura_encomenda(ident_enc) == TRUE)
    {     
        /* Verificar se produto existe no sistema. */
        if (procura_produto(ident_prod) == TRUE)
        {
            /* Calculo do peso da encomenda com o peso do produto a ser adicionado*/

            int peso = lista_encomendas_sistema[ident_enc].peso + lista_produtos_sistema[ident_prod].peso * qtd;

            if ((lista_produtos_sistema[ident_prod].qtd_stock - qtd) < 0)
            {
                printf("Impossivel adicionar produto %d a encomenda %d. Quantidade em stock insuficiente.\n", ident_prod, ident_enc);
            }
            else if (peso > PESO_MAX)
            {
                printf("Impossivel adicionar produto %d a encomenda %d. Peso da encomenda excede o maximo de 200.\n", ident_prod, ident_enc);
            }
            else if (procura_produto_em_encomenda(ident_enc, ident_prod) != FALSE)  /* Verificar se produto ja estava na encomenda. */
            {
                /* Posicao do produto no na encomenda. */
                int i = procura_produto_em_encomenda(ident_enc, ident_prod);

                /* Atualiza-se o quantidade do produto na encomenda, o peso da encoemnda e a quantidade desse produto no stock do sistema. */
                lista_encomendas_sistema[ident_enc].produto[i].qtd_stock += qtd;

                lista_encomendas_sistema[ident_enc].peso = peso;

                lista_produtos_sistema[ident_prod].qtd_stock -= qtd; 
            }
            else
            {   
                /* Posicao do produto na encomenda. */
                int indice = lista_encomendas_sistema[ident_enc].num_prods;

                /* Atualizar o peso da encomenda. */
                lista_encomendas_sistema[ident_enc].peso = peso;

                /* Adicionar o produto a encomenda. */
                lista_encomendas_sistema[ident_enc].produto[indice].idp = lista_produtos_sistema[ident_prod].idp;
                lista_encomendas_sistema[ident_enc].produto[indice].qtd_stock = qtd;
                strcpy(lista_encomendas_sistema[ident_enc].produto[indice].descricao, lista_produtos_sistema[ident_prod].descricao);
                lista_encomendas_sistema[ident_enc].produto[indice].peso = lista_produtos_sistema[ident_prod].peso;
                lista_encomendas_sistema[ident_enc].produto[indice].preco = lista_produtos_sistema[ident_prod].preco;

                /* Retirar ao stock do sistema a quantidade adiconada a encomenda. */
                lista_produtos_sistema[ident_prod].qtd_stock -= qtd;

                /* Numero de produtos na encomenda aumenta */
                lista_encomendas_sistema[ident_enc].num_prods +=1;
            }  
        }
        else
        {
            printf("Impossivel adicionar produto %d a encomenda %d. Produto inexistente.\n", ident_prod, ident_enc);
        }
    }
    else
    {
        printf("Impossivel adicionar produto %d a encomenda %d. Encomenda inexistente.\n", ident_prod, ident_enc);
    }
}

/* Funcao que remove stock a um produto existente, imprime erro se o produto nao exisitr ou se a remocao do stock pedido implicar um stock negativo. */
void comando_r()
{
    int ident_prod, qtd;

    scanf(" %d:%d",&ident_prod, &qtd);

    /* Verificar se produto esta no sistema. */
    if (procura_produto(ident_prod) == TRUE)
    {
        int remocao = lista_produtos_sistema[ident_prod].qtd_stock - qtd;

        if (remocao < 0)
        {
            printf("Impossivel remover %d unidades do produto %d do stock. Quantidade insuficiente.\n", qtd, ident_prod);
        }
        else
        {
            lista_produtos_sistema[ident_prod].qtd_stock = remocao;
        }  
    }
    else
    {
        printf("Impossivel remover stock do produto %d. Produto inexistente.\n", ident_prod);
    }
}

/* Funcao que remove um produto de uma encomenda, imprime erro se a encomenda nao estiver registada no sistema ou se o produto nao existir na encomenda. */
void comando_R()
{
    int ident_enc, ident_prod;

    scanf(" %d:%d", &ident_enc, &ident_prod);

    /* Verificar se encomenda existe no sistema. */
    if (procura_encomenda(ident_enc) == TRUE)
    {
        /* Verificar se produto existe no sistema. */
        
        if (procura_produto(ident_prod) == TRUE)
        {
            if (procura_produto_em_encomenda(ident_enc, ident_prod) != FALSE)
            {
                /* Eh a posicao do produto no vetor de produtos na encomenda que se quer apagar. */
                int pos_remove = procura_produto_em_encomenda(ident_enc, ident_prod);

                /* Atualizar peso da encomenda. */
                lista_encomendas_sistema[ident_enc].peso -= (lista_produtos_sistema[ident_prod].peso * lista_encomendas_sistema[ident_enc].produto[pos_remove].qtd_stock); 

                /*Reposicao do stock removido na lista de produtos do sistema. */
                lista_produtos_sistema[ident_prod].qtd_stock += lista_encomendas_sistema[ident_enc].produto[pos_remove].qtd_stock;

                /* Remocao do produto. */
                lista_encomendas_sistema[ident_enc].produto[pos_remove].qtd_stock = 0;
            }
        }
        else
        {
            printf("Impossivel remover produto %d a encomenda %d. Produto inexistente.\n", ident_prod, ident_enc);
        }
    }
    else
    {
        printf("Impossivel remover produto %d a encomenda %d. Encomenda inexistente.\n", ident_prod, ident_enc);
    } 
}

/* Funcao que calcula o custo de uma encomenda, imprime o custo da encomenda ou imprime erro se a encomenda nao estiver registada no sistema. */
void comando_C()
{
    int ident_enc;

    scanf(" %d", &ident_enc);
    
    /*  Verficar se encomenda existe no sistema. */
    if (procura_encomenda(ident_enc) == TRUE)
    {
        int i, lim_enc = lista_encomendas_sistema[ident_enc].num_prods, custo_total = 0;

        for (i = 0; i < lim_enc ; i++)
        {
            custo_total += (lista_encomendas_sistema[ident_enc].produto[i].preco * lista_encomendas_sistema[ident_enc].produto[i].qtd_stock);
    
        }
        printf("Custo da encomenda %d %d.\n", ident_enc, custo_total);
    }
    else
    {
        printf("Impossivel calcular custo da encomenda %d. Encomenda inexistente.\n", ident_enc);
    }
}

/* Funcao que altera o preco de um produto existente no sistema, imprime erro se o produto nao existir no sistema. */
void comando_p()
{
    int i, ident_prod, preco;

    scanf(" %d:%d", &ident_prod ,&preco);
    
    /* Verificar se produto existe no sistema. */
    if (procura_produto(ident_prod) == TRUE)
    {
        lista_produtos_sistema[ident_prod].preco = preco;

        /* Atualizar preco do produto se este estiver em alguma encomenda do sistema. */
        for (i = 0; i < id_encomenda; i++)
        {
            if (procura_produto_em_encomenda(i, ident_prod) != FALSE) 
            {
                /* Encontra a posicao do produto na encomenda. */
                int pos_produto = procura_produto_em_encomenda(i, ident_prod);

                lista_encomendas_sistema[i].produto[pos_produto].preco = preco;
            }
        }
    }
    else
    {
        printf("Impossivel alterar preco do produto %d. Produto inexistente.\n", ident_prod);
    }
}

/* Funcao que lista a descricao e a quantidade de um produto numa encomenda, imprime erro se a encomenda ou o produto nao estiverem registados no sistema. */
void comando_E()
{
    int ident_enc, ident_prod;

    scanf(" %d:%d", &ident_enc, &ident_prod);

    /* Verificar se encomenda existe no sistema. */
    if (procura_encomenda(ident_enc)  == TRUE)
    {
        /* Verificar se produto existe no sistema. */
        if (procura_produto(ident_prod) == TRUE)
        {
            if (procura_produto_em_encomenda(ident_enc,ident_prod) != FALSE)
            {
                /* Eh a posicao do produto na encomenda. */
                int pos_produto = procura_produto_em_encomenda(ident_enc,ident_prod);

                printf("%s %d.\n",lista_encomendas_sistema[ident_enc].produto[pos_produto].descricao, lista_encomendas_sistema[ident_enc].produto[pos_produto].qtd_stock);
            }
            else
            {
                /* O produto nao esta na encomenda, logo quantidade eh 0. */
                printf("%s %d.\n", lista_produtos_sistema[ident_prod].descricao, 0);
            }
        }
        else
        {
            printf("Impossivel listar produto %d. Produto inexistente.\n", ident_prod);
        }
    }
    else
    {
        printf("Impossivel listar encomenda %d. Encomenda inexistente.\n", ident_enc);
    }
}

/* Funcao que lista o identificador da encomenda em que o produto dado ocorre mais vezes, imprime erro se o produto nao existir no sistema ou o maximo desse produto, 
se existirem duas encomendas em que o produto ocorre igual numero de vezes imprime a que tiver menor identificador. */
void comando_m()
{
    int ident_prod;

    scanf(" %d",&ident_prod);

    /* Verificar se produto existe no sistema. */
    if (procura_produto(ident_prod) == TRUE)
    {
        int i, ide, max = 0;

        for (i = 0; i < id_encomenda; i++)
        {
            /* Verificar se produto existe na encomenda. */
            if (procura_produto_em_encomenda(i,ident_prod) != -1)
            {
                /* Posicao do produto na encomenda. */
                int pos_produto = procura_produto_em_encomenda(i,ident_prod);

                if (max < lista_encomendas_sistema[i].produto[pos_produto].qtd_stock)
                {
                    max = lista_encomendas_sistema[i].produto[pos_produto].qtd_stock;
                    ide = i;
                }
            }
        }
        if (max > 0)
        {
            printf("Maximo produto %d %d %d.\n", ident_prod, ide, max);
        }
    }
    else
    {
        printf("Impossivel listar maximo do produto %d. Produto inexistente.\n", ident_prod);
    }
}

/* Funcao que lista todos os produtos registados no sistema por ordem crescente de peso primeiro, e se 
houver 2 ou mais produtos com o mesmo preco, imprime por ordem crescente de identificador. */
void comando_l()
{
    int i;

    Produto lista_prods_ordenada[MAX_PROD];

    /* Copia do vetor lista de produtos do sistema para um outro vetor que vai ser ordenado. */
    for (i = 0; i < id_produto; i++)
    {
        lista_prods_ordenada[i] = lista_produtos_sistema[i];
    }

    mergesort(lista_prods_ordenada,0,id_produto -1);
   
    printf("Produtos\n");

    for (i = 0; i < id_produto; i++)
    {
        printf("* %s %d %d\n", lista_prods_ordenada[i].descricao, lista_prods_ordenada[i].preco, lista_prods_ordenada[i].qtd_stock);
    }
}

/* Funcao que lista todos os produtos de uma encomenda por ordem alfabetica
da descricao, imprime erro se a encomenda nao estiver registada no sistema. */
void comando_L()
{
    int ident_enc;

    scanf(" %d", &ident_enc);

    /* Verifica se encomenda esta no sistema. */
    if (procura_encomenda(ident_enc) == TRUE) 
    {
        int i,j, prods_enc = lista_encomendas_sistema[ident_enc].num_prods;
        Encomenda encomenda_ordenada;

        /* Copia das descricoes, precos e quantidades dos produtos na encomenda. */
        for (i = 0; i < prods_enc; i++)
        {
            strcpy(encomenda_ordenada.produto[i].descricao,lista_encomendas_sistema[ident_enc].produto[i].descricao);
            encomenda_ordenada.produto[i].preco = lista_encomendas_sistema[ident_enc].produto[i].preco;
            encomenda_ordenada.produto[i].qtd_stock = lista_encomendas_sistema[ident_enc].produto[i].qtd_stock;
        }
        
        printf("Encomenda %d\n", ident_enc);

        for (i = 1; i < prods_enc; i++)
        {
            Produto v = encomenda_ordenada.produto[i];
            j = i - 1;

            while ((j >= 0) && (strcmp(v.descricao,encomenda_ordenada.produto[j].descricao) < 0))
            {
                encomenda_ordenada.produto[j+1] = encomenda_ordenada.produto[j];
                j--;
            }

            encomenda_ordenada.produto[j+1] = v; 
        }

        for (i = 0; i < prods_enc; i++)
        {
            if (encomenda_ordenada.produto[i].qtd_stock != 0) /* So sao listados produtos com quantidade que nao seja 0. */
            {
                printf("* %s %d %d\n", encomenda_ordenada.produto[i].descricao, encomenda_ordenada.produto[i].preco, encomenda_ordenada.produto[i].qtd_stock);
            }
        }
    }
    else
    {
        printf("Impossivel listar encomenda %d. Encomenda inexistente.\n", ident_enc);
    }
}

/* Definicao das funcoes auxiliares. */

/* Funcao que verfica se um produto existe no sistema, recebe um identificador de um produto, 
retorna TRUE caso o produto exista. Caso contrario, retorna FALSE. */
int procura_produto(int ident_prod)
{
    /* id_produto conta quantos produtos ja foram adicionados ao sistema. */ 
    if (ident_prod < id_produto)
    {
        return TRUE;
    }

    return FALSE;
}

/* Funcao que recebe um ide e retorna TRUE caso encontre a encomenda com o ide dado. Caso contrario, retorna FALSE. */
int procura_encomenda(int ident_enc)
{
    
    if (ident_enc < id_encomenda)
    {
        return TRUE;
    }
    else
    {
        return FALSE;  
    }
}

/* Funcao que recebe um identificador de encomenda e um identificador de produto, se encontrar o produto devolve a sua posicao dele no vetor produtos da encomenda,
caso contrario retorna FALSE. */
int procura_produto_em_encomenda(int ident_enc, int ident_prod)
{
    int i;
    char descricao_prod[MAX_DESCRICAO];

    strcpy(descricao_prod, lista_produtos_sistema[ident_prod].descricao);

    for (i = 0; i < lista_encomendas_sistema[ident_enc].num_prods; i++)
    {
        if (strcmp(lista_encomendas_sistema[ident_enc].produto[i].descricao,descricao_prod) == 0)
        {
           return i;
        }
        
    }

    return FALSE;
    
}

/* Funcao usada no comando_l para organizar os produtos por preco decrescente. */
void mergesort(Produto lista[], int esquerda, int direita)
{
    int m = (esquerda+direita)/2;

    if(direita <= esquerda)
    {
        return;
    }

    mergesort(lista,esquerda,m);
    mergesort(lista,m+1,direita);
    merge(lista,esquerda,m,direita); 
}

/* Funcao usada em conjunto com a funcao mergesort no comando_l. */
void merge(Produto lista[],int esquerda, int m, int direita)
{
    int i,j,k;
    Produto aux[MAX_PROD];

    for (i = m+1; i > esquerda; i--)
    {
        aux[i-1] = lista[i-1];
        
    }

    for (j = m; j < direita; j++)
    {
        aux[direita+m-j] = lista[j+1];
    }

    for (k = esquerda; k <= direita; k++)
    {
        if ((aux[j].preco < aux[i].preco))
        {
            lista[k] = aux[j--];
        }
        else if (aux[j].preco == aux[i].preco)
        {
            if (aux[j].idp < aux[i].idp) /* Se os precos forem iguais, o fator de desempate eh o idp. */
            {
                lista[k] = aux[j--];
            }
            else
            {
                lista[k] = aux[i++];
            }
            
        }
        else
        {
            lista[k] = aux[i++];
        }
    }
}
