; programa de exemplo para SO21b
; cria outros processos, com os programas 1 e 2
NP1     DEFINE 3     ; número de processos para executar o programa 1
NP2     DEFINE 2     ; número de processos para executar o programa 2
; laço para criar os processos com o programa 1
        CARGI 0
        MVAX         ; x=0
        CARGI NP1
        ARMM limite  ; limite=NP1
ali1    CRIA 1       ; cria processo para executar programa 1
        INCX         ; x++
        MVXA
        SUB limite
        DESVNZ ali1  ; if x != limite goto ali1
        PARA         ; stop
; laço para criar os processos com o programa 2
        CARGI 0
        MVAX         ; x=0
        CARGI NP2
        ARMM limite  ; limite=NP2
ali2    CRIA 2       ; cria processo para executar programa 2
        INCX         ; x++
        MVXA
        SUB limite
        DESVNZ ali2  ; if x != limite goto ali2
        PARA         ; stop
limite  ESPACO 1
