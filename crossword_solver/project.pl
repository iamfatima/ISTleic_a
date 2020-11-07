% Maria Campos 95629 %

:- [codigo_comum].

% OBTEM_LETRAS_PALAVRAS/2 -> predicado que recebe uma Lista de Palavras e devolve uma lista %
% ordenada, sendo que os elementos sao listas com as letras de cada palavra da Lista de Palavras %

obtem_letras_palavras(Lst_Pals,Letras) :-  
    sort(Lst_Pals, Aux), 
    obtem_letras_palavras_aux(Aux,Letras).

obtem_letras_palavras_aux([],[]).
obtem_letras_palavras_aux([H|T],[P|R]) :- 
    atom_chars(H, P), 
    obtem_letras_palavras_aux(T,R).


% ESPACO_FILA/2 -> predicado que recebe uma Fila e retorna os possiveis Espacos nela, de acordo %
% com as regras %

espaco_fila(Fila, Esp) :- 
    append([Pref,Esp,Suf], Fila),
    verifica_suf(Suf),
    verifica_pref(Pref),
    length(Esp,Comp), 
    Comp >= 3, 
    nao_membro(#,Esp).

verifica_pref(Pref) :- last(Pref,U), U == '#'. 

verifica_pref([]).

verifica_suf([]). 

verifica_suf([P|_]) :- P == '#'. 

% nao_membro -> predicado auxiliar que recebe um elemento e uma lista e tem sucesso se o elemento nao pertencer a lista, sem unificar %

nao_membro(_, []).
nao_membro(E, [I|L]) :- E \== I, nao_membro(E, L).


% ESPACOS_FILA/2 -> predicado que coleciona as respostas a espacos_fila e retorna-as numa lista %

espacos_fila(Fila, Espacos) :- bagof(Esp,espaco_fila(Fila,Esp),Espacos),!.

% Se nao for um espaco, adiciona lista vazia %
espacos_fila(Fila,[]) :- \+ espaco_fila(Fila,_). 


% ESPACOS_PUZZLE/2 ->  predicado que recebe um grelha e devolve a lista de espacos validos nesse puzzle %

espacos_puzzle(Grelha, Espacos) :- 
    mat_transposta(Grelha,Trans),
    append(Grelha,Trans,Linhas_Colunas),                 % Soma da grelha com a Transposta %
    espacos_puzzle_aux(Linhas_Colunas,Espacos,[]),!.

espacos_puzzle_aux([],Esps,Esps).

% Verifca se os elementos da grelha sao espacos %
espacos_puzzle_aux([P|R],Espacos,Res) :- espacos_fila(P,Esp), append(Res, Esp, Esps), espacos_puzzle_aux(R,Espacos,Esps).


% ESPACOS_COM_POSICOES_COMUNS/3 -> predicado que recebe um espaco e uma lista de espacos e devolve a lista de espacos com variaveis em comum %
% com espaco, excetuando ele mesmo %

espacos_com_posicoes_comuns(Espacos,Esp,Esps_Com) :- espacos_com_posicoes_comuns_aux(Espacos,Esp,Esps_Com,[]),!.

espacos_com_posicoes_comuns_aux([],_,Acc,Acc).

espacos_com_posicoes_comuns_aux([P|R],Esp,Esps_Com,Acc) :- 
    P == Esp, 
    espacos_com_posicoes_comuns_aux(R,Esp,Esps_Com,Acc).

espacos_com_posicoes_comuns_aux([P|R],Esp,Esps_Com,Acc) :- 
    P \== Esp, 
    el_comum(Esp,P),
    append(Acc,[P],Acc1),
    espacos_com_posicoes_comuns_aux(R,Esp,Esps_Com,Acc1).

% Espacos nao tem elementos em comum, continua com recursao %
espacos_com_posicoes_comuns_aux([P|R],Esp,Esps_Com,Acc) :- 
    P \== Esp, 
    P \== [], 
    \+ el_comum(Esp,P),
    espacos_com_posicoes_comuns_aux(R,Esp,Esps_Com,Acc).

% el_comum -> predicado que verifica se listas tem pelos menos um elemento em comum %
el_comum([],_) :- fail.
el_comum([H|_],Lst) :- membro(H,Lst).
el_comum([H|T],Lst) :- \+ membro(H,Lst), el_comum(T,Lst).

% membro -> predicado que recebe um elemento e uma lista e tem sucesso se esse elemento pertence a lista %
membro(X, [Y | _]) :- X == Y,!.
membro(X, [P | R]) :- X \== P, membro(X, R).


% PALAVRA_POSSIVEL_ESP/4 -> predicado que recebe uma palavra, um espaco e uma lista de espacos e verifica se %
%  uma dada palavra eh possivel para colocar nesse espaco %

% Verifica se comprimento Pal e Esp sao iguais e se Pal respeita letras ja atribuidas a Esp %

palavra_possivel_esp(Pal,Esp,Espacos,Letras) :-
    membro(Esp,Espacos), 
    respeita_esp(Esp,Pal), 
    Esp = Pal,
    espacos_com_posicoes_comuns(Espacos,Esp,Comuns), 
    copia_lista(Comuns,Comuns_Copia), itera_pelos_espacos(Comuns_Copia,Letras),!.

itera_pelos_espacos([],_) :-!.

itera_pelos_espacos([P|R],Letras) :- 
    itera_pelas_letras(P,Letras), 
    itera_pelos_espacos(R,Letras).

itera_pelos_espacos([P|_],Letras) :- 
    \+ itera_pelas_letras(P,Letras),
    fail.

itera_pelas_letras(_,[]) :- fail.

% O espaco unificou com a palavra %

itera_pelas_letras(P,[H|_]) :- 
    length(P,C1),
    length(H,C2), 
    C1 == C2, 
    respeita_esp(P,H),!. 

% O espaco nao unifcou com a palavra %

itera_pelas_letras(P,[H|T]) :- 
    \+ respeita_esp(P,H),
    itera_pelas_letras(P,T). 

% copia_lista e copia elemento, criam uma copia da lista dada %
copia_lista(Lista,Copia) :- maplist(copia_elemento,Lista,Copia).

copia_elemento(El,_) :- var(El), !.
copia_elemento(El,El).

% respeita_esp verifica se a palavra e uma copia do espaco unificam %
respeita_esp(Esp,Pal) :- copia_lista(Esp,Copia), Copia = Pal.


% PALAVRAS_POSSIVEIS_ESP/4 -> predicado que encontra todas as palavras possiveis para um dado espaco %

palavras_possiveis_esp(Letras,Espacos,Esp,Pals_Possiveis) :- 
    findall(Pal,(member(Pal, Letras), palavra_possivel_esp(Pal,Esp,Espacos,Letras)), Pals_Possiveis).


% PALAVRAS_POSSIVEIS/3 -> predicado que lista todas as palavras possiveis para cada espaco  %

palavras_possiveis(Letras, Espacos, Pals_Possiveis) :- 
    bagof([Esp|[Possivel]],(member(Esp,Espacos),palavras_possiveis_esp(Letras,Espacos,Esp,Possivel)),Pals_Possiveis).


% LETRAS_COMUNS/2 -> predicado que retorna uma lista de pares que representam as letras em comum e as posicoes delas, dada uma lista de palavras %

letras_comuns(Lst_Pals, Letras_comuns) :- setof((Pos,Letra),(letras_comuns_aux(Lst_Pals,Letra,Pos)),Letras_comuns).

letras_comuns(Lst_Pals,[]) :- \+ letras_comuns_aux(Lst_Pals,_,_). % Caso nao existam letras em comum, lista vazia %

% Para cada da primeira palavra da lista de palavras, numa dada posicao verifica se todas as letras das palavras restantes sao %
% as mesmas %

letras_comuns_aux([P|R], Letra, Pos) :- 
    member(Letra,P),nth1(Pos, P, Letra), 
    R \== [], 
    letras_comuns_aux2(R,Letra,Pos).

letras_comuns_aux([P|R], Letra, Pos) :- 
    member(Letra,P),
    nth1(Pos, P, Letra),
    R == [].

% Verifica o resto das letras na mesma posicao %
letras_comuns_aux2([],_,_).

letras_comuns_aux2([H|T],Letra,Pos) :- 
    nth1(Pos, H, Letra), 
    letras_comuns_aux2(T,Letra,Pos).


% ATRIBUI_COMUNS/1 -> predicado que recebe uma lista de palavras possiveis e atualiza-a atribuindo a cada espaco as letras comuns a %
% todas as palavras possiveis para esse espaco %

atribui_comuns([]).

atribui_comuns([P|R]) :- 
    atribui_comuns_aux(P), 
    atribui_comuns(R).

% Para cada elemento de palavras possiveis, obtem se as letras comuns e substitui-se as mesmas em espaco %

atribui_comuns_aux([Esp,Pal]) :- 
    letras_comuns(Pal,Letras_comuns), 
    atribui_comuns_aux2(Esp,Letras_comuns).

% Substituicao das letras comuns as palavras em espaco %

atribui_comuns_aux2(_,[]).

atribui_comuns_aux2(Esp,[(A,B)|R]) :- 
    nth1(A,Esp,B), 
    atribui_comuns_aux2(Esp,R).


% RETIRA_IMPOSSIVEIS/2 -> predicado que recebe uma lista de palavras possiveis e %
% retira palavras que nao ja nao unificam com espaco, sendo o resultado a nova lista %

retira_impossiveis(Pals_Possiveis,Novas_Pals_Possiveis) :- retira_impossiveis_aux(Pals_Possiveis,[],Novas_Pals_Possiveis).

% retira_impossiveis_aux -> constroi as Novas_Pals_Possiveis atraves de um acumulador %

retira_impossiveis_aux([],Acumula_Pals,Acumula_Pals).

retira_impossiveis_aux([H|T],Acumula_Pals,Novas_Pals_Possiveis) :- 
    nth1(1, H, Esp), 
    nth1(2, H, Palavras), 
    unifica_esp(Esp,Palavras,[],Pals_Novas),
    append(Acumula_Pals, [[Esp,Pals_Novas]], Acumula_Pals1), 
    retira_impossiveis_aux(T,Acumula_Pals1,Novas_Pals_Possiveis).

% unifica_esp -> predicado que verifica se eh possivel unificar espaco com as palavras possiveis para ele, acumula as que unificam numa lista %

unifica_esp(_,[],Acc,Acc).

unifica_esp(Esp,[P|R],Acc,Pals_Novas) :- 
    respeita_esp(Esp,P),!,
    append(Acc,[P],Acc1),
    unifica_esp(Esp,R,Acc1,Pals_Novas).

unifica_esp(Esp,[_|R],Acc,Pals_Novas) :- % Ignora palavras que nao respeitem Esp %
    unifica_esp(Esp,R,Acc,Pals_Novas).

% OBTEM_UNICAS/2 -> predicado que recebe um lista de palavras possiveis e retorna uma lista %
% constituida pelas palavras possiveis que sao unicas para um dado espaco %

obtem_unicas(Pals_Possiveis,Unicas) :- obtem_unicas_aux(Pals_Possiveis,Unicas,[]).

obtem_unicas_aux([],Acc,Acc).

obtem_unicas_aux([P|R],Unicas,Acc) :- 
    nth1(2,P,Pal), 
    length(Pal,Comp), 
    Comp == 1,!, 
    append(Acc,Pal,Acc1),             
    obtem_unicas_aux(R,Unicas,Acc1).

obtem_unicas_aux([_|R],Unicas,Acc) :- obtem_unicas_aux(R,Unicas,Acc). % Ignora palavras que tem comprimento maior que 1 %


% RETIRA_UNICAS/2 -> predicado que recebe uma lista de palavras possiveis e retira palavras %
% que sejam a unica hipotese para um espaco %

retira_unicas(Pals_Possiveis,Novas_Pals_Possiveis) :- 
    obtem_unicas(Pals_Possiveis,Unicas),
    retira_unicas_aux(Pals_Possiveis,[],Novas_Pals_Possiveis,Unicas).

retira_unicas_aux([],Acumula_Pals,Acumula_Pals,_).

% Lista de palavras eh reconstruida %

retira_unicas_aux([H|T],Acumula_Pals,Novas_Pals_Possiveis,Unicas) :-  
    nth1(1,H, Esp), 
    nth1(2, H, Palavras),
    length(Palavras,Comp), 
    Comp > 1 ,!,
    membro_unicas(Palavras,[],Pals_Novas,Unicas),
    append(Acumula_Pals, [[Esp,Pals_Novas]], Acumula_Pals1), 
    retira_unicas_aux(T,Acumula_Pals1,Novas_Pals_Possiveis,Unicas).

% Se a lista de palavras for unitaria, apenas junta ao resultado %

retira_unicas_aux([H|T],Acumula_Pals,Novas_Pals_Possiveis,Unicas) :-  
    nth1(1,H, Esp), 
    nth1(2, H, Palavras),
    append(Acumula_Pals, [[Esp,Palavras]], Acumula_Pals1), 
    retira_unicas_aux(T,Acumula_Pals1,Novas_Pals_Possiveis,Unicas).

% membro_unicas/4 -> elimina as unitarias de cada elemento de pals_possiveis %

membro_unicas([],Acc,Acc,_).

membro_unicas([P|R],Acc,Pals_Novas,Unicas) :- 
    \+ membro(P,Unicas),
    append(Acc,[P],Acc1), 
    membro_unicas(R,Acc1,Pals_Novas,Unicas).

membro_unicas([P|R],Acc,Pals_Novas,Unicas) :-  
    membro(P,Unicas), 
    membro_unicas(R,Acc,Pals_Novas, Unicas).


% SIMPLIFICA/2 -> predicado que recebe uma lista de palavras possiveis e  aplica sucessivamente os 
% predicados atribui_comuns, retira_impossiveis, retira_unicas ate a lista que recebe nao sofra alteracoes %

simplifica(Pals_Possiveis, Novas_Pals_Possiveis) :- simplifica_aux(Pals_Possiveis,_,Novas_Pals_Possiveis),!.


simplifica_aux(Pals_Possiveis,Res_Anterior,Novas_Pals_Possiveis) :- 
    Pals_Possiveis \== Res_Anterior,!,                              % Comparar com lista anterior %                          
    atribui_comuns(Pals_Possiveis), 
    retira_impossiveis(Pals_Possiveis,Pals_Possiveis1),
    retira_unicas(Pals_Possiveis1,Res_Atual),
    simplifica_aux(Res_Atual,Pals_Possiveis,Novas_Pals_Possiveis).   % Guarda-se lista anterior e avanca-se com a modificada %

simplifica_aux(Pals_Possiveis,_,Pals_Possiveis) :- !.

% INICIALIZA/2 -> predicado que recebe um puzzle, inicializa-o, obtendo a lista de letras de cada palavra, os espacos %
% disponiveis na grelha, a lista de palavras possiveis para cada espaco e simplifica a lista %

inicializa(Puz, Pals_Possiveis) :- 
    nth1(1,Puz,Lst_Pals), 
    obtem_letras_palavras(Lst_Pals,Letras),
    nth1(2,Puz,Grelha), 
    espacos_puzzle(Grelha,Espacos), 
    palavras_possiveis(Letras,Espacos,Pals_Pos),
    simplifica(Pals_Pos,Pals_Possiveis).

% ESCOLHE_MENOS_ALTERNATIVAS/2 -> predicado que recebe uma lista de palavras possiveis e  escolhe o espaco com menos %
% alternativas para ser preenchido %

escolhe_menos_alternativas(Pals_Possiveis, Escolha) :- escolhe_menos_alternativas_aux(Pals_Possiveis,[],Escolha).

escolhe_menos_alternativas_aux([],Guarda_Escolha,Guarda_Escolha) :- Guarda_Escolha \== [].

escolhe_menos_alternativas_aux([P|R],Guarda_Escolha,Escolha) :- 
    nth1(2,P, Lst_Pals), 
    length(Lst_Pals,Comp), 
    Comp > 1, 
    Guarda_Escolha == [],                        % Inicializa Guarda_Escolha com o primeiro elemento de Pals_Possiveis %
    escolhe_menos_alternativas_aux(R,P,Escolha).

escolhe_menos_alternativas_aux([P|R],Guarda_Escolha,Escolha) :- 
    nth1(2,P, Lst_Pals),
    length(Lst_Pals,Comp), 
    Comp == 1, 
    escolhe_menos_alternativas_aux(R,Guarda_Escolha,Escolha).  % Se a lst_pals eh unitaria, ignora %

escolhe_menos_alternativas_aux([P|R],Guarda_Escolha,Escolha) :- 
    nth1(2,P, Lst_Pals), 
    length(Lst_Pals,Comp), 
    Comp > 1, 
    nth1(2,Guarda_Escolha, Lst_Pals1), 
    length(Lst_Pals1,Comp_Esc), 
    Comp < Comp_Esc,
    escolhe_menos_alternativas_aux(R,P,Escolha).  % Encontra-se a nova lista mais pequena que passa a ser Guarda_Escolha %

escolhe_menos_alternativas_aux([P|R],Guarda_Escolha,Escolha) :- 
    nth1(2,P, Lst_Pals), 
    length(Lst_Pals,Comp), 
    Comp > 1, 
    Guarda_Escolha \== [] ,
    nth1(2,Guarda_Escolha, Lst_Pals1), 
    length(Lst_Pals1,Comp_Esc), 
    Comp >= Comp_Esc, 
    escolhe_menos_alternativas_aux(R,Guarda_Escolha,Escolha). % Ignora comprimento maiores do que Guarda_Escolha %


% EXPERIMENTA_PAL/3 -> predicado que recebe um escolha e uma lista de palavras possiveis e substitui em Pals_Possiveis o elemento Escolha %
% por [Esp,[Pal]], sendo estes membros de Escolha %

experimenta_pal(Escolha,Pals_Possiveis,Novas_Pals_Possiveis) :-  experimenta_pal_aux(Pals_Possiveis,Escolha,[],Novas_Pals_Possiveis).

experimenta_pal_aux([],_,Acc,Acc).

experimenta_pal_aux([P|R],Escolha,Acc,Novas_Pals_Possiveis) :- 
    P \== Escolha, 
    append(Acc, [P], Acc1),           
    experimenta_pal_aux(R,Escolha,Acc1,Novas_Pals_Possiveis).

% A lista de palavras passa a ser apenas a de escolha %

experimenta_pal_aux([P|R],Escolha,Acc,Novas_Pals_Possiveis) :- 
    P == Escolha, 
    nth1(1,Escolha, Esp), 
    nth1(2, Escolha, Lst_Pals), 
    member(Pal,Lst_Pals), 
    Esp = Pal, 
    append(Acc, [[Esp,[Pal]]], Acc1),                     % Encontrou-se a escolha, juntar ao acumulador, a palavra passa a ser unica na Lst_Pals %                       
    experimenta_pal_aux(R,Escolha,Acc1,Novas_Pals_Possiveis).


% RESOLVE_AUX/2 -> predicado que recebe uma lista de palavras possiveis e  simplifica-a ate as palavras serem todas unitarias %

resolve_aux(Pals_Possiveis, Pals_Possiveis) :- unitarias(Pals_Possiveis),!.

resolve_aux(Pals_Possiveis, Novas_Pals_Possiveis) :-  
    \+ unitarias(Pals_Possiveis), 
    escolhe_menos_alternativas(Pals_Possiveis,Escolha),
    experimenta_pal(Escolha,Pals_Possiveis,Pals_Possiveis1), 
    simplifica(Pals_Possiveis1,Pals_Possiveis2),
    resolve_aux(Pals_Possiveis2,Novas_Pals_Possiveis),!.

% unitarias/1 -> predicado auxiliar que tem sucesso se todas as palavras na lista forem unitarias %

unitarias([]).
unitarias([P|R]) :- 
    nth1(2, P, Lst_Pals), 
    length(Lst_Pals,Comp), 
    Comp == 1, 
    unitarias(R).


% RESOLVE/1 -> predicado que recebe um puzzle inicializa e resolve o mesmo %

resolve(Puz) :- 
    inicializa(Puz, Pals_Possiveis), 
    resolve_aux(Pals_Possiveis,_).
