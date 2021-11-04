#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void imprime_estado(cpu_estado_t *est)
{
  printf("PC=%04d A=%06d X=%06d\n", cpue_PC(est), cpue_A(est), cpue_X(est));
}

#define TAM 20 // o tamanho da memória

int main()
{
   // programa para executar na nossa CPU
   int progr[] = {
   #include "ex1.maq"
   };
                 
   // variáveis que representam o computador
   mem_t *mem = mem_cria(TAM);
   es_t *es = es_cria();
   cpu_t *cpu = cpu_cria();
   cpu_estado_t *estado = cpue_cria();
   
   // copia o programa para a memória
   for (int i = 0; i < TAM; i++) {
     if (mem_escreve(mem, i, progr[i]) != ERR_OK) {
       printf("Erro de memoria, endereco %d\n", i);
       exit(1);
     }
   }
   
   // inicializa a CPU com as variáveis criadas
   cpu_altera_estado(cpu, estado);
   cpu_altera_memoria(cpu, mem);
   cpu_altera_es(cpu, es);
   
   // executa uma instrução por vez até parar
   while (true) {
     imprime_estado(cpu_estado(cpu));
     err_t err = cpu_executa_1(cpu);
     if (err != ERR_OK) {
       printf("Erro na execução: %d\n", err);
       printf("Estado final:\n");
       imprime_estado(cpu_estado(cpu));
       break;
     }
   }
   
   // destroi todo mundo!
   return 0;
}
