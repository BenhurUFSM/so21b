#ifndef ERR_H
#define ERR_H

// os tipos de erro que podem acontecer ao tentar executar uma instrução
typedef enum {
  ERR_OK = 0,        // sem erro
  // erros de memória
  ERR_MEM_END_INV,   // acesso a endereço de memória inválido
  // erros de E/S
  ERR_ES_DISP_INV,   // acesso a dispositivo inexistente
  ERR_ES_OP_INV,     // operação inválida em dispositivo
  // erros de CPU
  ERR_CPU_PARADA,    // CPU executou instrução PARA
  ERR_CPU_INSTR_INV, // tentativa de execução de instrução inexistente
} err_t;

#endif // ERR_H
