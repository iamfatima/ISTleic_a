# Maria Campos

'''
TAD posicao 

vai representar uma posicao (x,y) num labirinto

uma posicao eh representada por um tuplo em que o primeiro elemento do mesmo corresponde a x e o segundo corresponde ao valor y. Representacao do tipo (x,y)
'''

# construtor

def cria_posicao(x,y):
    '''
    cria_posicao recebe dois valores que correspondem a coordenada de uma posicao
    e devolve um tuplo com esses dois valores, x e y precisam de ser valores inteiros nao negativos
    
    cria_posicao: x x y -> posicao
    '''    
    # x ou y nao sao inteiros? x e y sao inteiros nao negativos?
    if not isinstance(x,int) or not isinstance(y,int) or x < 0 or y < 0:
        raise ValueError('cria_posicao: argumentos invalidos')
    
    else:
        return (x,y)

def cria_copia_posicao(posicao):
    '''
    cria_copia_posicao recebe uma posicao e devolve copia dessa posicao
    
    cria_copia_posicao: posicao -> posicao
    '''
    return cria_posicao(posicao[0],posicao[1])
    
# seletores

def obter_pos_x(posicao): 
    '''
    obter_pos_x recebe uma posicao e retorna a componente x, isto eh, o elemento do tuplo com indice 0
    
    obter_pos_x: posicao -> x
    '''    
    return posicao[0]


def obter_pos_y(posicao): 
    '''
    obter_pos_y recebe uma posicao e retorna a componente y, isto eh, o elemento do tuplo com indice 1
    
    obter_pos_y: posicao -> y
    '''
    return posicao[1]

# reconhecedor

def eh_posicao(arg):
    '''
    eh_posicao recebe um argumento qualquer e devolve True se o seu argumento eh um TAD posicao (um tuplo com dois elementos inteiros e nao negativos)
    
    eh_posicao: universal -> boolenao
    '''
    return isinstance(arg,tuple) and arg != () and len(arg) == 2 and isinstance(arg[0],int) and isinstance(arg[1],int) and arg[0] >= 0 and arg[1] >= 0 

# teste

def posicoes_iguais(p1,p2):  
    '''
    posicoes_iguais recebe duas posicoes e devolve True se p1 == p2, e False caso contrario
    
    posicoes_iguais: posicao x posicao -> booleano
    '''
    return p1[0]==p2[0] and p1[1]==p2[1]
    
# transformador

def posicao_para_str(posicao):
    '''
    posicao_para_str recebe uma posicao e devolve cadeia de caracteres que representa os valores x e y (coordenadas de posicao), do tipo '(x,y)'
    
    posicao_para_str: posicao -> str
    '''   
    return str(posicao)

# funcoes de alto nivel

def obter_posicoes_adjacentes(posicao): 
    '''
    obter_posicoes_adjacentes recebe uma posicao e devolve um tuplo com as posicoes adjacentes a posicao, respeitando a ordem de leitura do labirinto
    
    obter_posicoes_adjacentes posicao -> tuplo de posicoes
    '''    
    pos_adjacentes = ()
    
    x = obter_pos_x(posicao)
    y = obter_pos_y(posicao)
    
    # calcular posicao acima -> x == x  y = y - 1
    if y != 0: 
        pos_acima = cria_posicao(x,y-1) 
        pos_adjacentes += (pos_acima,) 
    
    # calcular posicao a esquerda -> x = x - 1, y == y
    if x != 0:
        pos_esquerda = cria_posicao(x-1,y)
        pos_adjacentes += (pos_esquerda,)
    
    # calcular posicao a direita x = x + 1, y == y
    pos_direita = cria_posicao(x+1,y)
    pos_adjacentes += (pos_direita,)
    
    # calcular posicao abaixo  x == x, y = y + 1
    pos_abaixo = cria_posicao(x,y+1)
    pos_adjacentes += (pos_abaixo,)
    
    return pos_adjacentes


'''
TAD unidade

representa as unidades de combate num simulador de batalhas 
cada unidade eh caracterizada por: posicao, forca de ataque, pontos de vida e exercito

uma unidade eh representada por um dicionario com quatro chaves: 'p' a qual se associa a posicao(representada por um tuplo), 'v' a qual se associa o valor que corresponde aos pontos de vida, 'f' a qual se associa o valor da forca de ataque e 'e' ao qual se associa uma cadeia de caracteres que corresponde ao nome do exercito
'''
# construtor

def cria_unidade(posicao,vida,forca,exercito):
    '''
    cria_unidade recebe uma posicao, dois valores inteiros maiores que 0 (vida e forca) e uma cadeia de caracteres nao vazia e devolve a unidade (tipo representado por um dicionario)
    
    cria_unidade: posicao x n x n x str -> unidade
    '''
    # validar argumentos
    if eh_posicao(posicao) is True and isinstance(vida,int) and isinstance(forca,int) and isinstance(exercito,str) and exercito != ''  and vida > 0 and forca > 0:
        
        return {'p': posicao, 'v': vida, 'f': forca, 'e': exercito}
    
    else:
        raise ValueError('cria_unidade: argumentos invalidos')


def cria_copia_unidade(unidade):
    '''
    cria_copia_unidade recebe unidade e devolve uma nova copia de unidade
    
    cria_copia_unidade: unidade -> unidade
    '''
    return cria_unidade(unidade['p'],unidade['v'],unidade['f'],unidade['e'])

# seletores

def obter_posicao(unidade):
    '''
    obter_posicao recebe unidade e retorna a posicao da unidade, que eh o tuplo associado a chave 'p' do dicionario
    
    obter_posicao: unidade -> posicao
    '''
    return unidade['p']

def obter_exercito(unidade):
    '''
    obter_exercito recebe unidade e retorna a cadeia de caracteres que eh o nome do execito associado a chave 'e' do dicionario
    
    obter_exercito: unidade -> str
    '''
    return unidade['e']

def obter_forca(unidade):
    '''
    obter_forca recebe unidade e retorna a forca de ataque da unidade, que eh o valor associado a chave 'f' do dicionario
    
    obter_forca: unidade -> n
    '''    
    return unidade['f']

def obter_vida(unidade):
    '''
    obter_vida recebe unidade e retorna os pontos de vida da unidade que eh o valor associado a chave 'v' do dicionario
    
    obter_vida: unidade -> n
    '''    
    return unidade['v']

# modificadores

def muda_posicao(unidade,posicao):
    '''
    muda_posicao substitui posicao de unidade por uma nova posicao e devolve unidade
    
    muda_posicao: unidade x posicao -> unidade
    '''
    unidade['p'] = posicao
    
    return unidade

def remove_vida(unidade,valor): 
    '''
    remove_vida subtrai aos pontos de vida de unidade o valor v e devolve unidade
    
    remove_vida: unidade x n -> unidade
    '''
    if valor >= unidade['v']:
        unidade['v'] = 0
        return unidade
    
    else:
        unidade['v'] = unidade['v'] - valor
        return unidade

# reconhecedor

def eh_unidade(arg): 
    '''
    eh_unidade devolve True se o seu argumento corresponde a um dicionario com 4 chaves ('p','v','f','e'), sendo que a 'p' vem associado uma posicao valida, a 'v' e 'f' vem associados valores inteiros positivos e a 'e' vem associada uma cadeia de caracteres nao vazia
    
    eh_unidade: universal -> booleano
    '''
    return isinstance(arg,dict) and len(arg) == 4 and 'p' in arg and 'v' in arg and 'f' in arg and 'e' in arg and eh_posicao(arg['p']) and isinstance(arg['f'],int) and isinstance(arg['v'],int) and arg['f'] > 0 and arg['v'] > 0 and isinstance(arg['e'],str) and arg['e'] != '' 

# teste

def unidades_iguais(u1,u2):
    '''
    unidades_iguais devolve True se u1 for igual a u2 e False caso contrario
    
    unidades_iguais: unidade x unidade -> booleano
    '''
    return u1 == u2

# transformadores

def unidade_para_char(unidade):
    '''
    unidade_para_char recebe uma unidade e devolve o primeiro caracter em maiuscula do exercito da unidade introduzida como argumento
    
    unidade_para_char: unidade -> str
    '''
    guarda_letra_exercito = unidade['e'][0] # variavel que armazena a primeira letra associada a chave 'e'
    
    return guarda_letra_exercito.upper()

def unidade_para_str(unidade):
    '''
    unidade_para_str recebe uma unidade e devolve uma cadeia de caracteres que representa a unidade
    do tipo '(primeira letra do exercito)(pontos de vida,pontos de ataque)@posicao'
    
    unidade_para_str: unidade -> str
    '''
    return unidade_para_char(unidade) + str([obter_vida(unidade),obter_forca(unidade)]) +'@' + str(obter_posicao(unidade))

# funcoes de alto nivel

def unidade_ataca(u1,u2): 
    '''
    unidade_ataca recebe duas unidades (uma que ataca e outra que eh atacada)
    retira aos pontos de vida de u2 a forca de ataque de u1
    se u2 for destruida, isto eh, pontos de vida de u2 <= 0 retorna True e False,caso contrario
    
    unidade_ataca: unidade x unidade -> booleano
    '''
    ataque_u1 = obter_forca(u1)
    
    remove_vida(u2,ataque_u1)
    
    if obter_vida(u2) <= 0: # a unidade eh destruida
        return True
    
    else:                   # a unidade nao eh destruida
        return False
    
def ordenar_unidades(tuplo_unidades): 
    '''
    ordenar_unidades recebe tuplo de unidades e ordena-as de acordo com a ordem de leitura do labirinto
    
    ordenar_unidades: tuplo unidades -> tuplo unidades
    '''
    # ordena-se primeiro as unidades que tem a coordenada x da posicao mais pequena
    ordena_por_x = tuple(sorted(tuplo_unidades, key = lambda unidade: (obter_pos_x(obter_posicao(unidade)))))
    
    # ordena-se as unidades (ordenadas por coordenada x da posicao menor) por coordenada y da posicao menor
    ordena_por_y = tuple(sorted (ordena_por_x, key = lambda unidade: (obter_pos_y(obter_posicao(unidade)))))
    
    return ordena_por_y
           
'''
TAD mapa

representa um labirinto e as unidades que se encontram nele

um mapa eh representado pelo dicionario com quatro chaves : 'dim' a qual se associa o tuplo de 2 elementos com as dimensoes Nx e Ny do labirinto, 'walls' a qual se associa o tuplo com as posisoes que correspodem a paredes que nao sao dos limites exteriores, 'e1' a qual se associa o tuplo com as unidades de um exercito e 'e2' a qual se associa o tuplo com as unidades do exercito inimigo de e1 
'''

# construtores

def cria_mapa(d,w,e1,e2):
    '''
     cria_mapa recebe: d =tuplo de 2 valores inteiros (Nx e Ny), w = tuplo de 0 ou mais posicoes (paredes que nao sao limites exteriores), e1 e e2 = tuplo de 1 ou mais unidades do mesmo exercito
     devolve dicionario que representa as caracteristicas do mapa
     
     cria_mapa: tuplo x tuplo x tuplo x tuplo -> mapa
    '''
    
   # d = tuplo de 2 valores inteiros (Nx e Ny), w = tuplo de 0 ou mais posicoes (paredes que nao sao limites exteriores), e1 e e2 = tuplo de 1 ou mais unidades do mesmo exercito
    
    if not isinstance(d,tuple) or not isinstance(w,tuple) or not isinstance(e1,tuple) or not isinstance(e2,tuple) or e1 == () or e2 == () or len(d) > 2 or not isinstance(d[0],int) or not isinstance(d[1],int) or d[0] < 3 or d[1] < 3:
        raise ValueError('cria_mapa: argumentos invalidos')
    
   # e1 e e2 sao tuplos um ou mais elementos
   # verifica se as unidades em e1 sao unidades possiveis
    
    for unidade in e1: 
        if eh_unidade(unidade) is False:
            raise ValueError('cria_mapa: argumentos invalidos')  
        
   # verifica se as unidades em e2 sao unidades possiveis
    for unidad in e2:
        if eh_unidade(unidad) is False:
            raise ValueError('cria_mapa: argumentos invalidos')       
    
   # verificar que se w nao eh tuplo vazio que as posicoes que tem sao validas
     
    if w!= ():
            for i in range(0,len(w)):
                if eh_posicao(w[i]) is False:
                    raise ValueError('cria_mapa: argumentos invalidos')
                # verificar que as posicoes de w e nao sao paredes dos limites exteriores e estao dentro dos mesmos
                elif obter_pos_x(w[i]) >= d[0] or obter_pos_x(w[i]) <= 0 or obter_pos_y(w[i]) >= d[1] or obter_pos_y(w[i]) <= 0:
                    raise ValueError('cria_mapa: argumentos invalidos')   

    return {'dim': d, 'walls': w ,'e1': e1, 'e2': e2}
            
def cria_copia_mapa(mapa):
    '''
    cria_copia_mapa recebe um mapa e devolve uma nova copia do mapa
    
    cria_copia_mapa: mapa -> mapa
    '''
    
    copia_unidades_e1 = ()
    copia_unidades_e2 = ()
    
    for i in range(len(mapa['e1'])):
        copia_unidades_e1 = copia_unidades_e1 + (cria_copia_unidade(mapa['e1'][i]),)
        
    for j in range(len(mapa['e2'])):
        copia_unidades_e2 = copia_unidades_e2 + (cria_copia_unidade(mapa['e2'][j]),)
        
    return cria_mapa(mapa['dim'],mapa['walls'],copia_unidades_e1,copia_unidades_e2)
    
    
# seletores

def obter_tamanho(mapa): 
    '''
    obter_tamanho recebe um mapa e devolve tuplo de dois valores inteiros (Nx e Ny), isto eh, as dimensoes do mapa
    
    obter_tamanho: mapa -> tuplo
    '''
    return mapa['dim']

def obter_nome_exercitos(mapa):
    '''
    obter_nome_exercitos recebe um mapa e devolve tuplo ordenado com as cadeias de caracteres que correspondem aos nomes dos exercitos do mapa
    
    obter_nome_exercitos: mapa -> tuplo
    '''
    
    # se um dos exercitos nao tiver unidades, retorna-se o apenas o nome do exercito que tem unidades
    
    if len(mapa['e1']) == 0:
        return obter_exercito(mapa['e2'][0])
    
    elif len(mapa['e2']) == 0:
        return obter_exercito(mapa['e1'][0])
    
    # ambos os exercitos tem unidades -> compara-se a primeira letra de cada exercito
    
    elif ord(unidade_para_char(mapa['e1'][0])) > ord(unidade_para_char(mapa['e2'][0])):
        return (obter_exercito(mapa['e2'][0]),obter_exercito(mapa['e1'][0]))
    
    else:
        return (obter_exercito(mapa['e1'][0]),obter_exercito(mapa['e2'][0]))

def obter_unidades_exercito(mapa,exercito): 
    '''
    obter_unidades_exercito devolve um tuplo com as unidades pertencentes ao exercito correspondente (cadeia de caracteres), unidades estao ordenadas pela ordem de leitura do labirinto
    
    obter_unidades_exercito: mapa x str -> tuplo unidades
    '''
    # o exercito que estamos a procura nao tem unidades
    
    if len(mapa['e1']) != 0 and exercito == obter_exercito(mapa['e1'][0]):
        return ordenar_unidades(mapa['e1'])
    
    elif len(mapa['e2']) != 0 and exercito == obter_exercito(mapa['e2'][0]):
        return ordenar_unidades(mapa['e2'])
    
    else:
        return ()

def obter_todas_unidades(mapa):
    '''
    obter_todas_unidades recebe um mapa e devolve um tuplo que contem todas as unidades do mapa, ordenadas de acordo com a ordem de leitura
    
    obter_todas_unidades: mapa -> tuplo
    '''
    todas_unidades = mapa['e1'] + mapa['e2'] 
    
    return ordenar_unidades(todas_unidades)

def obter_unidade(mapa,posicao):
    '''
    obter_unidade recebe um mapa e uma posicao e devolve a unidade do mapa que se encontra nessa posicao
    
    obter_unidade: mapa x posicao -> unidade
    '''
    todas_unidades = obter_todas_unidades(mapa)
    
    # procura-se a unidade que tem a posicao fornecida com argumento
    for indice in range(len(todas_unidades)):
        
        if posicao == obter_posicao(todas_unidades[indice]):
            
            return todas_unidades[indice]
    
# modificadores

def eliminar_unidade(mapa,unidade):
    '''
    eliminar_unidade recebe um mapa e uma unidade e elimina essa unidade do mapa, devolve o mapa
    
    eliminar_unidade: mapa x unidade -> mapa
    '''
    lista_e1 = list(mapa['e1'])
    lista_e2 = list(mapa['e2'])
    
    # verifica se a unidade fornecida esta em e1 e se estiver apaga-a
    for index in range(len(lista_e1)):
        
        if unidade == lista_e1[index]:
            
            del(lista_e1[index])
            
            mapa['e1'] = tuple(lista_e1)
            
            break
        
    # verifica se a unidade fornecida esta em e2 e se estiver apaga-a       
    for index2 in range(len(lista_e2)):
        
        if unidade == lista_e2[index2]:
            
            del(lista_e2[index2])    
            
            mapa['e2'] = tuple(lista_e2)
            
            break
            
    return mapa
            
def mover_unidade(mapa,unidade,posicao):
    '''
    mover_unidade recebe um mapa, uma unidade e uma posicao, alterando posicao de unidade para a posicao fornecida e deixando livre a posicao onde estava, retorna mapa
    
    mover_unidade: mapa x unidade x posicao -> mapa
    '''
    
    muda_posicao(unidade,posicao) # atualiza posicao da unidade
    
    return mapa

# reconhecedores

def eh_posicao_unidade(mapa,posicao):
    '''
    eh_posicao_unidade recebe um mapa e uma posicao, devolve True se posicao esta ocupada por unidade, False,caso contrario
    
    eh_posicao_unidade: mapa x posicao -> booleano
    '''
    # obtem todas as unidades do mapa verifica se existe alguma com a posicao igual a do argumento
    
    for n in range(len(obter_todas_unidades(mapa))):
        
        if posicao == obter_posicao(obter_todas_unidades(mapa)[n]):
            
            return True
    
    return False
    

def eh_posicao_corredor(mapa,posicao): 
    '''
    eh_posicao_corredor recebe um mapa e uma posicao e devolve True se a posicao p eh um corredor no labirinto(independentemente de estar ocupada por unidade)
    
    eh_posicao_corredor: mapa x posicao -> booleano
    '''
    dimX = mapa['dim'][0]
    
    dimY = mapa['dim'][1]
    
    # verifica se posicao esta dentro dos limites do labirinto, nunca podendo ser os proprios limites
    
    if obter_pos_x(posicao) > 0 and obter_pos_x(posicao) < dimX -1 and obter_pos_y(posicao) > 0 and obter_pos_y(posicao) < dimY - 1:
        
        # posicao nao e extremidade, verifica se posicao esta em walls
        if posicao not in mapa['walls']:
            return True       
        
    return False

def eh_posicao_parede(mapa,posicao):
    '''
    eh_posicao_parede recebe um mapa e uma posicao e devolve True se a posicao corresponde a uma parede do labirinto, False, caso contrario
    
    eh_posicao_parede: mapa x posicao -> booleano
    '''
    dimX = mapa['dim'][0]
    
    dimY = mapa['dim'][1]
    
    # verifica se posicao esta em walls (paredes que nao sao limites exteriores)
    
    for b in range(len(mapa['walls'])):
        
        if posicao == mapa['walls'][b]:
            
            return True
        
    # verifica se posicao eh um limite exterior (x,y de posicao tem de estar dentro das dimensoes do labirinto)
        
    if obter_pos_x(posicao) == 0 and obter_pos_y(posicao) < dimY:
        return True
    
    elif obter_pos_x(posicao) == dimX - 1 and obter_pos_y(posicao) < dimY:
        return True
    
    elif obter_pos_y(posicao) == 0 and obter_pos_x(posicao) < dimX:
        return True
    
    elif obter_pos_y(posicao) == dimY - 1 and obter_pos_x(posicao) < dimX:
        return True
        
    return False
    
# teste

def mapas_iguais(m1,m2): 
    '''
    mapas_iguais recebe dois mapas e devolve True se m1 e m2 forem iguais, caso contrario retorna False
    
    mapas_iguais: mapa x mapa -> booleano
    '''
    return m1 == m2
    
# transformador

def mapa_para_str(mapa): 
    '''
    mapa_para_str recebe um mapa e devolve a cadeia de caracteres que representa mapa
    
    mapa_para_str: mapa -> str
    '''
    dimX = mapa['dim'][0]
    dimY = mapa['dim'][1]
    
    todas_posicoes = [] # lista que armazena todas as posicoes do mapa
    
    str_mapa = ''
    
    # obter todas as posicoes, respeitando a ordem de leitura do labirinto (fixamos um y e incrementa-se o x)
    
    k = 0
    
    while k < dimY:
        
        y = k
        
        for x in range(dimX):
            
            todas_posicoes = todas_posicoes + [(x,y)]
            
        k += 1

    for p in range(len(todas_posicoes)):
        
        pos = todas_posicoes[p]
        
        # pos eh uma parede nas extremidades ou em corredores, so nao eh parede da coluna mais a direita
        if eh_posicao_parede(mapa,pos) is True and obter_pos_x(pos) != dimX - 1:
            
            str_mapa = str_mapa + '#'
        
        # pos eh uma parede na coluna mais a direita mas nao eh a ultima parede de todas
        elif eh_posicao_parede(mapa,pos) is True and obter_pos_x(pos) == dimX - 1 and obter_pos_y(pos) != dimY - 1:
            
            str_mapa = str_mapa + '#\n'
        
        # ultima parede do mapa
        elif eh_posicao_parede(mapa,pos) is True and obter_pos_x(pos) == dimX - 1 and obter_pos_y(pos) == dimY - 1:
            
            str_mapa = str_mapa + '#'
            
        # pos nao eh parede, nao eh unidade mas eh uma posicao no corredor
        elif eh_posicao_corredor(mapa,pos) is True and eh_posicao_unidade(mapa,pos) is False:
            
            str_mapa = str_mapa + '.'
        
        # pos eh unidade e esta em corredor
        elif eh_posicao_unidade(mapa,pos) is True and eh_posicao_corredor(mapa,pos) is True:
            
            str_mapa = str_mapa + unidade_para_char(obter_unidade(mapa,pos))
            
    return str_mapa
    
# funcoes de alto nivel

def obter_inimigos_adjacentes(mapa,unidade):
    '''
    obter_inimigos_adjacentes recebe um mapa e uma unidade e devolve um tuplo com as unidades inimigas (do exercito contrario) adjacentes a unidade (ordenadas de acordo com a ordem de leitura do movimento)
    
    obter_inimigos_adjacentes: mapa x unidade -> tuplo unidades
    '''
    
    exercito = obter_exercito(unidade) # variavel que guarda o nome do exercito de unidade
    
    pos_adj = obter_posicoes_adjacentes(obter_posicao(unidade)) # tuplo que tem as posicoes adjacentes a unidade
    
    inimigos_adj = ()
    
    for index in range(len(pos_adj)):
        
        # posicao tem unidade e exercito dessa unidade eh diferente do exercito da unidade fornecida como argumento?
        
        if eh_posicao_unidade(mapa,pos_adj[index]) is True and  obter_exercito(obter_unidade(mapa,pos_adj[index])) != exercito:
            
            inimigos_adj = inimigos_adj + (obter_unidade(mapa,pos_adj[index]),)
            
    return ordenar_unidades(inimigos_adj)
    
def obter_movimento(mapa, unit):
    '''
    A funcao obter_movimento devolve a posicao seguinte da unidade argumento
    de acordo com as regras de movimento das unidades no labirinto.

    obter_movimento: mapa x unidade -> posicao
    '''

    ######################
    # Funcoes auxiliares #
    ######################
    def pos_to_tuple(pos):
        return obter_pos_x(pos), obter_pos_y(pos)

    def tuple_to_pos(tup):
        return cria_posicao(tup[0], tup[1])

    def tira_repetidos(tup_posicoes):
        conj_tuplos = set(tuple(map(pos_to_tuple, tup_posicoes)))
        return tuple(map(tuple_to_pos, conj_tuplos))

    def obter_objetivos(source):
        enemy_side = tuple(filter(lambda u: u != obter_exercito(source), obter_nome_exercitos(mapa)))[0]
        target_units = obter_unidades_exercito(mapa, enemy_side)
        tup_com_repetidos = \
            tuple(adj
                  for other_unit in target_units
                  for adj in obter_posicoes_adjacentes(obter_posicao(other_unit))
                  if eh_posicao_corredor(mapa, adj) and not eh_posicao_unidade(mapa, adj))
        return tira_repetidos(tup_com_repetidos)

    def backtrack(target):
        result = ()
        while target is not None:
            result = (target,) + result
            target, _ = visited[target]
        return result

    ####################
    # Funcao principal #
    ####################
    # Nao mexer se ja esta' adjacente a inimigo
    if obter_inimigos_adjacentes(mapa, unit):
        return obter_posicao(unit)

    visited = {}
    # posicao a explorar, posicao anterior e distancia
    to_explore = [(pos_to_tuple(obter_posicao(unit)), None, 0)]
    # registro do numero de passos minimo ate primeira posicao objetivo
    min_dist = None
    # estrutura que guarda todas as posicoes objetivo a igual minima distancia
    min_dist_targets = []

    targets = tuple(pos_to_tuple(obj) for obj in obter_objetivos(unit))

    while to_explore:  # enquanto nao esteja vazio
        pos, previous, dist = to_explore.pop(0)

        if pos not in visited:  # posicao foi ja explorada?
            visited[pos] = (previous, dist)  # registro no conjunto de exploracao
            if pos in targets:  # se a posicao atual eh uma dos objetivos
                # se eh primeiro objetivo  ou se esta a  distancia minima
                if min_dist is None or dist == min_dist:
                    # acrescentor 'a lista de posicoes minimas
                    min_dist = dist
                    min_dist_targets.append(pos)
            else:  # nao 'e objetivo, acrescento adjacentes
                for adj in obter_posicoes_adjacentes(tuple_to_pos(pos)):
                    if eh_posicao_corredor(mapa, adj) and not eh_posicao_unidade(mapa, adj):
                        to_explore.append((pos_to_tuple(adj), pos, dist + 1))

        # Parar se estou a visitar posicoes mais distantes que o minimo,
        # ou se ja encontrei todos os objetivos
        if (min_dist is not None and dist > min_dist) or len(min_dist_targets) == len(targets):
            break

    # se encontrei pelo menos uma posicao objetivo, 
    # escolhe a de ordem de leitura menor e devolve o primeiro movimento
    if len(min_dist_targets) > 0:
        # primeiro dos objetivos em ordem de leitura
        tar = sorted(min_dist_targets, key=lambda x: (x[1], x[0]))[0]
        path = backtrack(tar)
        return tuple_to_pos(path[1])

    # Caso nenhuma posicao seja alcancavel
    return obter_posicao(unit)
       
# Funcoes adicionais
 
def calcula_pontos(mapa,exercito):
    '''
    calcula_pontos recebe um mapa e uma cadeia de caracteres, correspondente ao nome de um exercito do mapa, devolve a pontuacao do exercito
    
    calcula_pontos: mapa x str -> int
    '''
    # guarda tuplo com unidades do exercito fornecido
    unidades_exercito = obter_unidades_exercito(mapa,exercito)
    
    pontuacao = 0
    
    if unidades_exercito == ():
        
        return pontuacao
    
    for index in range(len(unidades_exercito)):
        
        pontuacao += obter_vida(unidades_exercito[index]) 
        
    return pontuacao


def simula_turno(mapa):
    '''
    simula_turno modifica o mapa, simulando um turno de batalha completo e devolve o proprio mapa, segue a ordem de leitura do labirinto, cada unidade (viva) realiza um unico movimento e eventualmente um ataque
    
    simula_turno: mapa -> mapa
    '''
    # variavel que guarda todas unidades do mapa
    todas_unidades = list(obter_todas_unidades(mapa)) 
    
    for i in range(len(todas_unidades)):
        
        unidade = todas_unidades[i]
        
        if obter_vida(unidade) != 0: # unidade que estamos a verificar esta viva?
            
            pos_seguinte = obter_movimento(mapa,unidade)
            
            mover_unidade(mapa,unidade,pos_seguinte) # unidade move-se
            
            inimigo = obter_inimigos_adjacentes(mapa,unidade)
            
            if inimigo !=():
                # inimigo[0] eh o primeiro pela ordem de leitura do labirinto
                if unidade_ataca(unidade,inimigo[0]) is True: # unidade eh destruida
                    eliminar_unidade(mapa,inimigo[0])
                    
                # (else) unidade eh atacada mas nao destruida -> ciclo continua
            
    return mapa

def simula_batalha(file,booleano):
    '''
    simula_batalha permite simular uma batalha completa, a batalha acaba quando um dos exercitos vence ou apos um turno mapa e unidades nao se alteram, recebe um valor booleano True (modo verboso) ou False (modo quiet), apresenta o nome do exercito que vence ou EMPATE caso nenhum exercito venca
    
    simula_batalha: str x booleano -> str
    '''
    
     # Funcao auxiliar #
     
    def show_scores(exercito1,exercito2):
        '''
        show_scores recebe dois nomes de exercitos diferentes e mostra os pontos de cada um, representacao do tipo [ exercito1: pontuacao1 exercito2: pontuacao2 ]
        '''
        
        score1 = exercito1 + ':' + str(calcula_pontos(mapa,exercito1))
        score2 = exercito2 + ':' + str(calcula_pontos(mapa,exercito2))
        return '[ ' + score1 + ' ' + score2 + ' ]'
    
    
    # abrir o ficheiro para obter informacao sobre mapa e exercitos
    config = open(file,'r')
    tamanho = eval(config.readline()) 
    info_e1 = eval(config.readline())
    info_e2 = eval(config.readline())
    walls = eval(config.readline())
    posicoes_e1 = eval(config.readline())
    posicoes_e2 = eval(config.readline()) 
    
    # formar primeiro exercito
    e1 = tuple(cria_unidade(cria_posicao(p[0], p[1]), info_e1[1], info_e1[2], info_e1[0]) for p in posicoes_e1)
    
    # formar segundo exercito
    e2 = tuple(cria_unidade(cria_posicao(p[0], p[1]), info_e2[1], info_e2[2], info_e2[0]) for p in posicoes_e2)
    
    # criar mapa com informacoes fornecidas no ficheiro
    mapa = cria_mapa(tamanho,walls,e1,e2)
    
    # str com os nomes de cada exercito
    exercito1 = obter_nome_exercitos(mapa)[0]

    exercito2 = obter_nome_exercitos(mapa)[1]
    
    # mostrar mapa e pontos iniciais
    print(mapa_para_str(mapa)) 
    print(show_scores(exercito1,exercito2))    
    
    mapa_anterior = cria_copia_mapa(mapa) # guarda mapa do turno anterior
    
    if booleano == True: # modo verboso
        
        print(mapa_para_str(simula_turno(mapa))) # garantir que o ciclo corre pelo menos uma vez
        print(show_scores(exercito1,exercito2))  
        
    elif booleano == False:
        
        simula_turno(mapa) # garantir que o ciclo corre pelo menos uma vez
    
    while mapas_iguais(mapa_anterior,mapa) is False: # enquanto houver alteracao a unidades e mapa
        
        mapa_anterior = cria_copia_mapa(mapa)
        
        if booleano == True:
            
            print(mapa_para_str(simula_turno(mapa))) #atualiza mapa
            print(show_scores(exercito1,exercito2))      
            
            if calcula_pontos(mapa,exercito1) == 0: # exercito2 vence
                
                return exercito2
            
            elif calcula_pontos(mapa,exercito2) == 0: # exercito1 vence
                
                return exercito1
            
        elif booleano == False:
            
            simula_turno(mapa)

            if calcula_pontos(mapa,exercito1) == 0: # exercito2 vence
                print(mapa_para_str(mapa)) # mostra o mapa do ultimo turno
                print(show_scores(exercito1,exercito2))                 
                return exercito2
            
            if calcula_pontos(mapa,exercito2) == 0: # exercito1 vence
                print(mapa_para_str(mapa)) # mostra o mapa do ultimo turno
                print(show_scores(exercito1,exercito2))                 
                return exercito1      
    
    if booleano == False:
        print(mapa_para_str(mapa)) # mostra o mapa do ultimo turno
        print(show_scores(exercito1,exercito2))
        
    return 'EMPATE'        