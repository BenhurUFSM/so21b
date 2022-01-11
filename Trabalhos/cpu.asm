; programa de exemplo para SO21b
; programa limitado por CPU, só faz uma leitura com o número de vezes a repetir o laço
video   DEFINE 1
teclado DEFINE 0
        CARGI 0
        MVAX         ; x=0
        LE teclado
        ARMM l       ; l=read()
ali     MVXA
        INCX         ; x++
        MVXA
        SUB l
        DESVNZ ali   ; if x != l goto ali
        PARA         ; stop
l       ESPACO 1
