#ifndef MEM_H
#define MEM_H

// simulador da memória principal
// é um vetor de inteiros

#include "err.h"

typedef struct mem_t mem_t; // para ter um tipo opaco

mem_t *mem_cria(int tam);
int mem_tam(mem_t *m);

err_t mem_le(mem_t *m, int endereco, int *pvalor);
err_t mem_escreve(mem_t *m, int endereco, int valor);
void mem_destroi(mem_t *m);

#endif // MEM_H
