#include "cpu_estado.h"
#include <stdlib.h>

struct cpu_estado_t {
  int PC;
  int A;
  int X;
  err_t erro;
  int complemento;
};

cpu_estado_t *cpue_cria(void)
{
  cpu_estado_t *self;
  self = malloc(sizeof(*self));
  if (self != NULL) {
    self->PC = 0;
    self->A = 0;
    self->X = 0;
    self->erro = ERR_OK;
    self->complemento = 0;
  }
  return self;
}

void cpue_destroi(cpu_estado_t *self)
{
  free(self);
}

int cpue_PC(cpu_estado_t *self)
{
  return self->PC;
}

int cpue_A(cpu_estado_t *self)
{
  return self->A;
}

int cpue_X(cpu_estado_t *self)
{
  return self->X;
}

err_t cpue_erro(cpu_estado_t *self)
{
  return self->erro;
}

int cpue_complemento(cpu_estado_t *self)
{
  return self->complemento;
}

void cpue_muda_PC(cpu_estado_t *self, int val)
{
  self->PC = val;
}

void cpue_muda_A(cpu_estado_t *self, int val)
{
  self->A = val;
}

void cpue_muda_X(cpu_estado_t *self, int val)
{
  self->X = val;
}

void cpue_muda_erro(cpu_estado_t *self, err_t err, int complemento)
{
  self->erro = err;
  self->complemento = complemento;
}

