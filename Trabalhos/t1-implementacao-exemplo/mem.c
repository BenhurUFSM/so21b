#include "mem.h"
#include <stdlib.h>

// tipo de dados opaco para representar uma região de memória
struct mem_t {
  int tam;
  int *conteudo;
};

// tipos de acesso à memória
typedef enum { leitura, escrita } acesso_t;

mem_t *mem_cria(int tam)
{
  mem_t *self;
  self = malloc(sizeof(*self));
  if (self != NULL) {
    self->tam = tam;
    self->conteudo = malloc(tam * sizeof(*(self->conteudo)));
    if (self->conteudo == NULL) {
      free(self);
      self = NULL;
    }
  }
  return self;
}

void mem_destroi(mem_t *self)
{
  if (self != NULL) {
    if (self->conteudo != NULL) {
      free(self->conteudo);
    }
    free(self);
  }
}

int mem_tam(mem_t *self)
{
  return self->tam;
}

// função auxiliar, verifica se dá para realizar um acesso do tipo indicado
//   a esse endereço?
static err_t verif_permissao(mem_t *self, int endereco, acesso_t tipo)
{
  // todos os enderecos são válidos para todos os tipos de acesso
  if (endereco >= 0 && endereco < self->tam) {
    return ERR_OK;
  }
  return ERR_MEM_END_INV;
}

err_t mem_le(mem_t *self, int endereco, int *pvalor)
{
  err_t err = verif_permissao(self, endereco, leitura);
  if (err == ERR_OK) {
    *pvalor = self->conteudo[endereco];
  }
  return err;
}

err_t mem_escreve(mem_t *self, int endereco, int valor)
{
  err_t err = verif_permissao(self, endereco, escrita);
  if (err == ERR_OK) {
    self->conteudo[endereco] = valor;
  }
  return err;
}
