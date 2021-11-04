#ifndef CPU_E_H
#define CPU_E_H

#include "err.h"

typedef struct cpu_estado_t cpu_estado_t;  // estrutura opaca

cpu_estado_t *cpue_cria(void);
void cpue_destroi(cpu_estado_t *self);

int cpue_PC(cpu_estado_t *self);
int cpue_A(cpu_estado_t *self);
int cpue_X(cpu_estado_t *self);
err_t cpue_erro(cpu_estado_t *self);
int cpue_complemento(cpu_estado_t *self);

void cpue_muda_PC(cpu_estado_t *self, int val);
void cpue_muda_A(cpu_estado_t *self, int val);
void cpue_muda_X(cpu_estado_t *self, int val);
void cpue_muda_erro(cpu_estado_t *self, err_t err, int complemento);

#endif // CPU_E_H
