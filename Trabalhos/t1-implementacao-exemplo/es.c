#include "es.h"
#include <stdio.h>
#include <stdlib.h>

// implementação simplória, para suporta aos 2 dispositivos.
// se (quando) tiver mais dispositivos, provavelmente tenha que reestruturar


// define a estrutura opaca
struct es_t {
  // não tem muito o que colocar aqui, os dispositivos estão hard-coded abaixo
};

es_t *es_cria(void)
{
  es_t *self;
  self = malloc(sizeof(*self));
  return self;
}

void es_destroi(es_t *self)
{
  free(self);
}

typedef enum { leitura, escrita } acesso_t;
typedef enum { teclado=0, video=1 } dispositivo_t;

// função auxiliar, para verificar se tal acesso a tal dispositivo é ok
static err_t verif_acesso(es_t *self, int dispositivo, acesso_t tipo)
{
  // só tem 2 dispositivos, teclado e video
  if (dispositivo != teclado && dispositivo != video) return ERR_ES_DISP_INV;
  // do teclado só dá para ler
  if (dispositivo == teclado && tipo != leitura) return ERR_ES_OP_INV;
  // o video é só para escrita
  if (dispositivo == video && tipo != escrita) return ERR_ES_OP_INV;
  return ERR_OK;
}

err_t es_le(es_t *self, int dispositivo, int *pvalor)
{
  err_t err = verif_acesso(self, dispositivo, leitura);
  if (err == ERR_OK) {
    // só tem dispositivo teclado por enquanto
    printf("Digite um valor inteiro: ");
    scanf("%d", pvalor);
  }
  return err;
}

err_t es_escreve(es_t *self, int dispositivo, int valor)
{
  err_t err = verif_acesso(self, dispositivo, escrita);
  if (err == ERR_OK) {
    // só tem dispositivo video por enquanto
    printf("[%d]\n", valor);
  }
  return err;
}
