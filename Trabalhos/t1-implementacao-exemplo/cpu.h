#ifndef CPU_H
#define CPU_H

// simulador do executor de instruções de uma CPU
// executa a instrução no PC se possível, ou retorna erro

// tem acesso a
// - controlador de ES para as instruções de ES
// - memória, onde está o programa e os dados -- alterável pelas instruções
// - estado da CPU, onde estão os registradores -- alterável pelas instruções

#include "err.h"
#include "mem.h"
#include "es.h"
#include "cpu_estado.h"

typedef struct cpu_t cpu_t; // tipo opaco

cpu_t *cpu_cria(void);
void cpu_destroi(cpu_t *cpu);

cpu_estado_t *cpu_estado(cpu_t *cpu);
void cpu_altera_estado(cpu_t *cpu, cpu_estado_t *estado);
void cpu_altera_memoria(cpu_t *cpu, mem_t *mem);
void cpu_altera_es(cpu_t *cpu, es_t *es);

err_t cpu_executa_1(cpu_t *cpu);

#endif // CPU_H
