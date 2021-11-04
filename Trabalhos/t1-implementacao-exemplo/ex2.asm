; programa de exemplo para SO21b
video   DEFINE 1
teclado DEFINE 0
        CARGI 0
        MVAX         ; x=0
        LE teclado
        ARMM l       ; l=read()
ali     MVXA
        ESCR video   ; print x
        INCX         ; x++
        MVXA
        SUB l
        DESVNZ ali   ; if x != l goto ali
        PARA         ; stop
l       ESPACO 1
