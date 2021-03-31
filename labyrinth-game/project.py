# Maria Campos

#REPRESENTACAO DO LABIRINTO

'''
eh_labirinto devolve True se o argumento corresponde a um labirinto(tuplo com Nx tuplos, cada um com Ny inteiros com valores 0 ou 1, Nx e Ny tem que ter pelo menos tres elementos, os elementos das extremidades dos tuplos de Nx tem que ser 1 correspondendo a paredes) e False caso contrario
'''

def eh_labirinto(maze):

    if not isinstance(maze,tuple): #valida se maze e tuplo ou nao
        return False

    if len (maze) < 3: #Nx tem que ser no minimo 3
        return False

    for i in range(0,len(maze)): #aceder aos Nx

        if not isinstance(maze[i],tuple): #Nx e tuplo ou nao
            return False
        if len(maze[i]) < 3: #Ny no minimo 3
            return False
        if len(maze[i]) != len(maze[i-1]): #validar se os tuplos tem o mesmo numero de elementos
            return False
        if maze[i][0] != 1 or maze[i][-1] != 1: #valida se os numeros das extremidades sao 1
            return False
        #valida se os numeros do meio sao 1 ou 0
        for j in range(1,len(maze[i])-1):
            if isinstance(maze[i][j],int) is False:
                return False
            if maze [i][j] != 0 and maze[i][j] !=1:
                return False
    else:
        return True
'''
eh_posicao devolve True se o argumento e uma posicao (tuplo com dois valores inteiros nao negativos(x,y)) e False caso contrario
'''
def eh_posicao(posicao):

    if not isinstance(posicao,tuple) or len(posicao) != 2 : # se nao for introduzido um tuplo ou se o tuplo tiver mais do que 2 elementos
        return False
    for i in range(0,2):
        if not isinstance(posicao[i], int) or posicao[i] < 0:
            return False
    else:
        return True
'''
eh_conj_posicoes devolve True se o argumento corresponde a um conjunto de posicoes unicas (nao ha posicoes repetidas)
'''
def eh_conj_posicoes(conj_posicoes):

    if not isinstance(conj_posicoes,tuple): #verifica se e tuplo
        return False
    for j in range(0,len(conj_posicoes)):
        if not isinstance(conj_posicoes[j],tuple): #se o elemento do tuplo na posicao j nao for um tuplo
            return False
        if eh_posicao(conj_posicoes[j]) is False: #verifica se o elemento do tuplo na posicao j e uma posicao valida
            return False
        if conj_posicoes[j] in conj_posicoes[j+1:]:
            return False
    else:
        return True
'''
tamanho_labirinto recebe um labirinto e devolve tuplo com dois valores, o primeiro corresponde a Nx e o segundo a Ny
'''
def tamanho_labirinto(maze):
    
    if eh_labirinto(maze) is False:
        raise ValueError('tamanho_labirinto: argumento invalido')
    
    else:
        nY = len(maze[0])
        nX = len(maze)
        tuploFinal = (nX,nY)
        return tuploFinal
'''
eh_mapa_valido recebe um labirinto e um conjunto de posicoes(unidades no labirinto), devolve True se as posicoes nao estao ocupadas por paredes e False caso contrario
'''

def eh_mapa_valido(maze,conj_posicoes):

    #validar argumentos da funcao
    if eh_labirinto(maze) is False or eh_conj_posicoes(conj_posicoes) is False:
        raise ValueError ('eh_mapa_valido: algum dos argumentos e invalido')

    dimensoes = tamanho_labirinto(maze)

    #segundo argumento corresponde a posicoes ocupadas por paredes
    for n in range(0,len(conj_posicoes)): #percorre os elementos dos elementos de conj_posicoes
        x = conj_posicoes[n][0]
        y = conj_posicoes[n][1]
        if x > dimensoes[0]: #o primeiro elemento nao pode ser maior do que o maior indice de maze
            return False
        if x > dimensoes[1]:
            return False
        if maze[x][y] == 1:
            return False

    return True

'''
eh_posicao_livre recebe um labirinto, um conjunto de posicoes(unidades no labirinto) e uma posicao. Devolve True se a posicao nao e ocupada por paredes nem unidades e False caso contrario
'''

def eh_posicao_livre(maze,unidades,posicao):
    

    if eh_labirinto(maze) is False or eh_conj_posicoes(unidades) is False or eh_posicao(posicao) is False:
        raise ValueError('eh_posicao_livre: algum dos argumentos e invalido')
    if eh_mapa_valido(maze,unidades) is False:
        raise ValueError('eh_posicao_livre: algum dos argumentos e invalido')
    for i in range(0,len(unidades)): #verifica se a posicao dada esta nas unidades
        if unidades[i] == posicao:
                return False
    salvaIndice = posicao[0] #guarda se o numero do tuplo
    salvaIndice2 = posicao[1]
    if maze[salvaIndice][salvaIndice2] == 1: #verifica se e parede ou nao
        return False
    else:
        return True
    
'''
posicoes_adjacentes recebe uma posicao e devolve o conjunto de posicoes adjacentes a essa posicao pela ordem de leitura do labirinto
'''

def posicoes_adjacentes(posicao):
    

    if eh_posicao(posicao) is False:
        raise ValueError('posicoes_adjacentes: argumento invalido')
    x = posicao[0]
    y = posicao[1]
    if x-1 < 0 and y-1 > 0:
        return ((x,y-1),(x+1,y),(x,y+1))
    if y-1 < 0 and x-1 > 0:
        return ((x-1,y),(x+1,y),(x,y+1))
    if y-1 < 0 and x-1 < 0:
        return ((x+1,y),(x,y+1))
    else:
        return ((x,y-1),(x-1,y),(x+1,y),(x,y+1))
'''
mapa_str recebe um labirinto e as posicoes das unidades que estao no labirinto e devolve a representacao externa do mapa, isto e faz um desenho do mapa em que  O = posicao, . = 0 e 1 = paredes.
'''
def mapa_str(maze,unidades):

    # validar argumentos
    if eh_labirinto(maze) is False or eh_conj_posicoes(unidades) is False:
        raise ValueError ('mapa_str: algum dos argumentos e invalido')
    if eh_mapa_valido(maze,unidades) is False:
        raise ValueError ('mapa_str: algum dos argumentos e invalido')

    # construir o mapa ( O = posicao, . = 0, 1 = paredes)

    converteTexto = '' #variavel que guarda a estrutura do mapa
    for i in range(0,len(maze[0])):

        for w in range(0,len(maze)):
            for n in range(0,len(unidades)):
                if unidades[n][0] == w and unidades[n][1] == i:
                    converteTexto = converteTexto + 'O'

            if maze[w][i] == 1:
                converteTexto = converteTexto + '#'

            elif maze[w][i] == 0 and (w,i) not in unidades:
                converteTexto = converteTexto + '.'

        if i!= len(maze[0])-1:
            converteTexto = converteTexto +'\n'
    return converteTexto

#FUNCOES DE MOVIMENTO
'''
obter_objetivos recebe um labirinto, um conjunto de posicoes, uma posicao (que tem de estar no conjunto de posicoes) e devolve o conjunto de posicoes nao ocupadas no labirinto, que correspondem aos objetivos.

'''

def obter_objetivos(maze,unidades,posicao):
    # validar argumentos
    if eh_labirinto(maze) is False or eh_conj_posicoes(unidades) is False or eh_posicao(posicao) is False:
        raise ValueError('obter_objetivos: algum dos argumentos e invalido')

    if eh_mapa_valido(maze,unidades) is False:
        raise ValueError('obter_objetivos: algum dos argumentos e invalido')

    if posicao in unidades is False:
        raise ValueError('obter_objetivos: algum dos argumentos e invalido')

    tuplo_obj = ()
    for i in range(0,len(unidades)): #indices dos elementos em unidades
        if unidades[i] != posicao:
            pos_adjacentes = posicoes_adjacentes(unidades[i])
            for adj in pos_adjacentes:
                if (eh_posicao_livre(maze,unidades,adj) is True):
                    tuplo_obj = tuplo_obj + (adj,)

    return tuplo_obj

'''
obter_caminho recebe um labirinto, um conjunto de posicoes e uma posicao(que tem de pertencer ao conjunto de posicoes), o caminho corresponde ao conjunto de posicoes adjacentes, comecando na posicao dada e acabando na posicao objetivo (obtida de obter objetivos)
'''

def obter_caminho(labirinto,unidades,posicao):

    if eh_labirinto(labirinto) is False or eh_conj_posicoes(unidades) is False or eh_posicao(posicao) is False:
        raise ValueError('obter_caminho: algum dos argumentos e invalido')
    if eh_mapa_valido(labirinto,unidades) is False:
        raise ValueError('obter_caminho: algum dos argumentos e invalido')
    if posicao in unidades is False:
        raise ValueError('obter_caminho: algum dos argumentos e invalido')

    objetivos = obter_objetivos(labirinto,unidades,posicao)
    listaExploracao = (posicao,())
    posicao_atual = ()
    caminho_atual = ()
    posicoesVisitadas = ()
    pos_adj = ()

    while listaExploracao != ():
        posicao_atual = listaExploracao[0] 
        caminho_atual = listaExploracao[1] 
        listaExploracao = listaExploracao[2:]
        if posicao_atual not in posicoesVisitadas:
            posicoesVisitadas = posicoesVisitadas + (posicao_atual,)
            caminho_atual = caminho_atual + (posicao_atual,) #caminho_atual e atualizado
            if posicao_atual in objetivos:
                return caminho_atual
            else:
                pos_adj = posicoes_adjacentes(posicao_atual)
                for pos in pos_adj:
                    if eh_posicao_livre(labirinto,unidades,pos) is True:
                        listaExploracao = listaExploracao + (pos,) + (caminho_atual,)
    return ()

'''
a funcao mover_unidade recebe um labirinto, um conjunto de posicoes(unidades) e uma posicao que tem de estar no conjunto de posicoes, a unidade nao se move se estiver ao lado de outra unidade ou se nao houver unidade atingivel, caso contrario unidade avanca de acordo com o caminho de numero minimo de passos (obtido da funcao obter_caminho)
'''
def mover_unidade(labirinto,unidades,posicao):
    
    #validar argumentos
    if eh_labirinto(labirinto) is False or eh_conj_posicoes(unidades) is False or eh_posicao(posicao) is False:
        raise ValueError('mover_unidade: algum dos argumentos e invalido')
    
    if posicao in unidades is False:
        raise ValueError('mover_unidade: algum dos argumentos e invalido')
    
    if eh_mapa_valido(maze,unidades) is False:
        raise ValueError('mover_unidade: algum dos argumentos e invalido')

    #verificar se unidade unidade e adjacente a uma outra unidade
    for i in range(0,len(unidades)):
        adjacentes = posicoes_adjacentes(unidades[i])
        
        if posicao in adjacentes:
            return unidades
        
    caminho = obter_caminho(labirinto,unidades,posicao)
    if caminho == ():
        return unidades
    elif len(caminho) == 1:
        return unidades
    else:
        pos_atualizada = ()
        for i in range(0,len(unidades)):
            if unidades[i] == posicao:
                pos_atualizada = unidades[:i]+(caminho[1],)+unidades[i+1:]
        return pos_atualizada