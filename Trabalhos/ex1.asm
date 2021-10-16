; programa de exemplo para SO21b
     CARGI 0
     MVAX         ; x=0
     CARGI 10
     ARMM l       ; l=10
ali  MVXA
     ESCR 1       ; print x
     INCX         ; x++
     MVXA
     SUB l
     DESVNZ ali   ; if x != l goto ali
     PARA         ; stop
l    ESPACO 1
