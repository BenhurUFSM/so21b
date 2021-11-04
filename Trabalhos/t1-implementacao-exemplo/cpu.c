#include "cpu.h"
#include <stdbool.h>
#include <stdlib.h>

// uma CPU tem estado, memoria, controlador de ES
struct cpu_t {
  cpu_estado_t *estado;
  mem_t *mem;
  es_t *es;
};

cpu_t *cpu_cria(void)
{
  cpu_t *self;
  self = malloc(sizeof(*self));
  if (self != NULL) {
    self->estado = NULL;
    self->mem = NULL;
    self->es = NULL;
  }
  return self;
}

void cpu_destroi(cpu_t *self)
{
  // eu nao criei memoria, es, etc, quem criou que destrua!
  free(self);
}

cpu_estado_t *cpu_estado(cpu_t *self)
{
  return self->estado;
}

void cpu_altera_estado(cpu_t *self, cpu_estado_t *estado)
{
  self->estado = estado;
}

void cpu_altera_memoria(cpu_t *self, mem_t *mem)
{
  self->mem = mem;
}

void cpu_altera_es(cpu_t *self, es_t *es)
{
  self->es = es;
}



// ---------------------------------------------------------------------
// funções auxiliares para usar durante a execução das instruções
// alteram o estado da CPU caso ocorra erro

// lê um valor da memória
static bool pega_mem(cpu_t *self, int endereco, int *pval)
{
  err_t err = mem_le(self->mem, endereco, pval);
  if (err != ERR_OK) {
    cpue_muda_erro(self->estado, err, endereco);
  }
  return err == ERR_OK;
}

// lê o opcode da instrução no PC
static bool pega_opcode(cpu_t *self, int *popc)
{
  return pega_mem(self, cpue_PC(self->estado), popc);
}

// lê o argumento 1 da instrução no PC
static bool pega_A1(cpu_t *self, int *pA1)
{
  return pega_mem(self, cpue_PC(self->estado) + 1, pA1);
}

static void incrementa_PC(cpu_t *self)
{
   cpue_muda_PC(self->estado, cpue_PC(self->estado) + 1);
}

static void incrementa_PC2(cpu_t *self)
{
   cpue_muda_PC(self->estado, cpue_PC(self->estado) + 2);
}

// escreve um valor na memória
static bool poe_mem(cpu_t *self, int endereco, int val)
{
  err_t err = mem_escreve(self->mem, endereco, val);
  if (err != ERR_OK) {
    cpue_muda_erro(self->estado, err, endereco);
  }
  return err == ERR_OK;
}

// lê um valor da E/S
static bool pega_es(cpu_t *self, int dispositivo, int *pval)
{
  err_t err = es_le(self->es, dispositivo, pval);
  if (err != ERR_OK) {
    cpue_muda_erro(self->estado, err, dispositivo);
  }
  return err == ERR_OK;
}

// escreve um valor na E/S
static bool poe_es(cpu_t *self, int dispositivo, int val)
{
  err_t err = es_escreve(self->es, dispositivo, val);
  if (err != ERR_OK) {
    cpue_muda_erro(self->estado, err, dispositivo);
  }
  return err == ERR_OK;
}

// ---------------------------------------------------------------------
// funções auxiliares para implementação de cada instrução

static void op_NOP(cpu_t *self) // não faz nada
{
  incrementa_PC(self);
}

static void op_PARA(cpu_t *self) // para a CPU
{
  cpue_muda_erro(self->estado, ERR_CPU_PARADA, 0);
}

static void op_CARGI(cpu_t *self) // carrega imediato
{
  int A1;
  if (pega_A1(self, &A1)) {
    cpue_muda_A(self->estado, A1);
    incrementa_PC2(self);
  }
}

static void op_CARGM(cpu_t *self) // carrega da memória
{
  int A1, mA1;
  if (pega_A1(self, &A1) && pega_mem(self, A1, &mA1)) {
    cpue_muda_A(self->estado, mA1);
    incrementa_PC2(self);
  }
}

static void op_CARGX(cpu_t *self) // carrega indexado
{
  int A1, mA1mX;
  int X = cpue_X(self->estado);
  if (pega_A1(self, &A1) && pega_mem(self, A1+X, &mA1mX)) {
    cpue_muda_A(self->estado, mA1mX);
    incrementa_PC2(self);
  }
}

static void op_ARMM(cpu_t *self) // armazena na memória
{
  int A1;
  if (pega_A1(self, &A1) && poe_mem(self, A1, cpue_A(self->estado))) {
    incrementa_PC2(self);
  }
}

static void op_ARMX(cpu_t *self) // armazena indexado
{
  int A1;
  int X = cpue_X(self->estado);
  if (pega_A1(self, &A1) && poe_mem(self, A1+X, cpue_A(self->estado))) {
    incrementa_PC2(self);
  }
}

static void op_MVAX(cpu_t *self) // inicializa X
{
  cpue_muda_X(self->estado, cpue_A(self->estado));
  incrementa_PC(self);
}

static void op_MVXA(cpu_t *self) // recupera X
{
  cpue_muda_A(self->estado, cpue_X(self->estado));
  incrementa_PC(self);
}

static void op_INCX(cpu_t *self) // incrementa X
{
  cpue_muda_X(self->estado, cpue_X(self->estado)+1);
  incrementa_PC(self);
}

static void op_SOMA(cpu_t *self) // soma
{
  int A1, mA1;
  if (pega_A1(self, &A1) && pega_mem(self, A1, &mA1)) {
    cpue_muda_A(self->estado, cpue_A(self->estado) + mA1);
    incrementa_PC2(self);
  }
}

static void op_SUB(cpu_t *self) // subtração
{
  int A1, mA1;
  if (pega_A1(self, &A1) && pega_mem(self, A1, &mA1)) {
    cpue_muda_A(self->estado, cpue_A(self->estado) - mA1);
    incrementa_PC2(self);
  }
}

static void op_MULT(cpu_t *self) // multiplicação
{
  int A1, mA1;
  if (pega_A1(self, &A1) && pega_mem(self, A1, &mA1)) {
    cpue_muda_A(self->estado, cpue_A(self->estado) * mA1);
    incrementa_PC2(self);
  }
}

static void op_DIV(cpu_t *self) // divisão
{
  int A1, mA1;
  if (pega_A1(self, &A1) && pega_mem(self, A1, &mA1)) {
    cpue_muda_A(self->estado, cpue_A(self->estado) / mA1);
    incrementa_PC2(self);
  }
}

static void op_RESTO(cpu_t *self) // resto
{
  int A1, mA1;
  if (pega_A1(self, &A1) && pega_mem(self, A1, &mA1)) {
    cpue_muda_A(self->estado, cpue_A(self->estado) % mA1);
    incrementa_PC2(self);
  }
}

static void op_NEG(cpu_t *self) // inverte sinal
{
  cpue_muda_A(self->estado, -cpue_A(self->estado));
  incrementa_PC(self);
}

static void op_DESV(cpu_t *self) // desvio incondicional
{
  int A1;
  if (pega_A1(self, &A1)) {
    cpue_muda_PC(self->estado, A1);
  }
}

static void op_DESVZ(cpu_t *self) // desvio condicional
{
  int A1;
  if (cpue_A(self->estado) == 0) {
    if (pega_A1(self, &A1)) {
      cpue_muda_PC(self->estado, A1);
    }
  } else {
    incrementa_PC2(self);
  }
}

static void op_DESVNZ(cpu_t *self) // desvio condicional
{
  int A1;
  if (cpue_A(self->estado) != 0) {
    if (pega_A1(self, &A1)) {
      cpue_muda_PC(self->estado, A1);
    }
  } else {
    incrementa_PC2(self);
  }
}

static void op_LE(cpu_t *self) // leitura de E/S
{
  int A1, dado;
  if (pega_A1(self, &A1) && pega_es(self, A1, &dado)) {
    cpue_muda_A(self->estado, dado);
    incrementa_PC2(self);
  }
}

static void op_ESCR(cpu_t *self) // escrita de E/S
{
  int A1;
  if (pega_A1(self, &A1) && poe_es(self, A1, cpue_A(self->estado))) {
    incrementa_PC2(self);
  }
}


err_t cpu_executa_1(cpu_t *self)
{
  // não executa se CPU já estiver em erro
  if (cpue_erro(self->estado) == ERR_OK) {
    int opcode;
    if (pega_opcode(self, &opcode)) {
      switch (opcode) {
        case  0: op_NOP(self);    break;
        case  1: op_PARA(self);   break;
        case  2: op_CARGI(self);  break;
        case  3: op_CARGM(self);  break;
        case  4: op_CARGX(self);  break;
        case  5: op_ARMM(self);   break;
        case  6: op_ARMX(self);   break;
        case  7: op_MVAX(self);   break;
        case  8: op_MVXA(self);   break;
        case  9: op_INCX(self);   break;
        case 10: op_SOMA(self);   break;
        case 11: op_SUB(self);    break;
        case 12: op_MULT(self);   break;
        case 13: op_DIV(self);    break;
        case 14: op_RESTO(self);  break;
        case 15: op_NEG(self);    break;
        case 16: op_DESV(self);   break;
        case 17: op_DESVZ(self);  break;
        case 18: op_DESVNZ(self); break;
        case 19: op_LE(self);     break;
        case 20: op_ESCR(self);   break;
        default: cpue_muda_erro(self->estado, ERR_CPU_INSTR_INV, 0);
      }
    }
  }
  return cpue_erro(self->estado);
}
