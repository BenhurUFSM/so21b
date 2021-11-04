#ifndef ES_H
#define ES_H

// controlador de dispositivos de entrada e saida
//
// por enquanto só tem dois dispositivos:
// 0 - teclado, só tem operação de entrada (de 1 inteiro)
// 1 - video, só tem operação de saída, de 1 inteiro

#include "err.h"

typedef struct es_t es_t; // declara o tipo como sendo uma estrutura opaca

es_t *es_cria(void);
void es_destroi(es_t *es);

err_t es_le(es_t *es, int dispositivo, int *pvalor);
err_t es_escreve(es_t *es, int dispositivo, int valor);

#endif // ES_H
