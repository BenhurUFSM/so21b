// montador para SO21b

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

// auxiliares

void erro_brabo(char *msg)
{
  fprintf(stderr, "ERRO FATAL: %s\n", msg);
  exit(1);
}

bool tem_numero(char *s, int *num)
{
  if (isdigit(*s) || (*s == '-' && isdigit(*(s+1)))) {
    *num = atoi(s);
    return true;
  }
  return false;
}

// memoria de saida

#define MEM_TAM 1000
int mem[MEM_TAM];
int mem_pos;

void mem_insere(int val)
{
  if (mem_pos >= MEM_TAM-1) {
    erro_brabo("programa muito grande!");
  }
  mem[mem_pos++] = val;
}

void mem_altera(int pos, int val)
{
  if (pos < 0 || pos >= mem_pos) {
    erro_brabo("erro interno, alteração de região não inicializada");
  }
  mem[pos] = val;
}

void mem_imprime(void)
{
  for (int i = 0; i < mem_pos; i+=10) {
    printf("    /* %03d */", i);
    for (int j = i; j < i+10 && j < mem_pos; j++) {
      printf(" %d,", mem[j]);
    }
    printf("\n");
  }
}

// simbolos

#define SIMB_TAM 1000
struct {
  char *nome;
  int endereco;
} simbolo[SIMB_TAM];
int simb_num;

int simb_endereco(char *nome)
{
  for (int i=0; i<simb_num; i++) {
    if (strcmp(nome, simbolo[i].nome) == 0) {
      return simbolo[i].endereco;
    }
  }
  return -1;
}

void simb_novo(char *nome, int endereco)
{
  if (nome == NULL) return;
  if (simb_endereco(nome) != -1) {
    fprintf(stderr, "ERRO: redefinicao do simbolo '%s'\n", nome);
    return;
  }
  if (simb_num >= SIMB_TAM) {
    erro_brabo("Excesso de símbolos");
  }
  simbolo[simb_num].nome = strdup(nome);
  simbolo[simb_num].endereco = endereco;
  simb_num++;
}


// referencias

#define REF_TAM 1000
struct {
  char *nome;
  int linha;
  int endereco;
} ref[REF_TAM];
int ref_num;

void ref_nova(char *nome, int linha, int endereco)
{
  if (nome == NULL) return;
  if (ref_num >= REF_TAM) {
    erro_brabo("excesso de referências");
  }
  ref[ref_num].nome = strdup(nome);
  ref[ref_num].linha = linha;
  ref[ref_num].endereco = endereco;
  ref_num++;
}

void ref_resolve(void)
{
  for (int i=0; i<ref_num; i++) {
    int endereco = simb_endereco(ref[i].nome);
    if (endereco == -1) {
      fprintf(stderr, 
              "ERRO: simbolo '%s' referenciado na linha %d não foi definido\n",
              ref[i].nome, ref[i].linha);
    }
    mem_altera(ref[i].endereco, endereco);
  }
}


// instrucoes

typedef enum {
  NOP,    PARA,   CARGI,  CARGM,  CARGX,  ARMM,   ARMX,   MVAX,
  MVXA,   INCX,   SOMA,   SUB,    MULT,   DIV,    RESTO,  NEG,
  DESV,   DESVZ,  DESVNZ, LE,     ESCR,   CONST,  ESPACO,
} opcode_t;
struct {
  char *nome;
  int num_args;
} instrucoes[] = {
  { "NOP",    0 },
  { "PARA",   0 },
  { "CARGI",  1 },
  { "CARGM",  1 },
  { "CARGX",  1 },
  { "ARMM",   1 },
  { "ARMX",   1 },
  { "MVAX",   0 },
  { "MVXA",   0 },
  { "INCX",   0 },
  { "SOMA",   1 },
  { "SUB",    1 },
  { "MULT",   1 },
  { "DIV",    1 },
  { "RESTO",  1 },
  { "NEG",    0 },
  { "DESV",   1 },
  { "DESVZ",  1 },
  { "DESVNZ", 1 },
  { "LE",     1 },
  { "ESCR",   1 },
  // pseudo-instrucoes
  { "CONST",  1 },
  { "ESPACO", 1 },
};
#define INSTR_NUM (sizeof(instrucoes)/sizeof(instrucoes[0]))

int instr_opcode(char *nome)
{
  for (int i=0; i<INSTR_NUM; i++) {
    if (strcmp(instrucoes[i].nome, nome) == 0) {
      return i;
    }
  }
  return -1;
}

// montagem

void monta_instrucao(int linha, int opcode, char *arg)
{
  // trata pseudo-opcodes antes
  int argn;
  
  if (opcode == ESPACO) {
    if (!tem_numero(arg, &argn) || argn < 1) {
      fprintf(stderr, "ERRO: linha %d 'ESPACO' deve ter valor positivo\n",
              linha);
      return;
    }
    for (int i = 0; i < argn; i++) {
      mem_insere(0);
    }
    return;
  } else if (opcode == CONST) {
    // nao faz nada, vai inserir o valor definido em arg
  } else {
    mem_insere(opcode);
  }
  if (instrucoes[opcode].num_args == 0) {
    return;
  }
  if (tem_numero(arg, &argn)) {
    mem_insere(argn);
  } else {
    // não é número, põe um 0 e insere uma referência para alterar depois
    ref_nova(arg, linha, mem_pos);
    mem_insere(0);
  }
}

void monta_linha(int linha, char *label, char *instrucao, char *arg)
{
  if (label != NULL) {
    simb_novo(label, mem_pos);
  }
  if (instrucao == NULL) return;
  int num_instr = instr_opcode(instrucao);
  if (num_instr == -1) {
    fprintf(stderr, "ERRO: linha %d: instrucao '%s' desconhecida\n",
                    linha, instrucao);
    return;
  }
  if (instrucoes[num_instr].num_args == 0 && arg != NULL) {
    fprintf(stderr, "ERRO: linha %d: instrucao '%s' não tem argumento\n",
                    linha, instrucao);
    return;
  }
  if (instrucoes[num_instr].num_args == 1 && arg == NULL) {
    fprintf(stderr, "ERRO: linha %d: instrucao '%s' necessita argumento\n",
                    linha, instrucao);
    return;
  }
  monta_instrucao(linha, num_instr, arg);
}

char *pula_ate_espaco(char *s)
{
  while (*s != ' ' && *s != '\0') {
    s++;
  }
  return s;
}

char *detona_espacos(char *s)
{
  while (*s == ' ') {
    *s = '\0';
    s++;
  }
  return s;
}

void tira_comentario(char *s)
{
  // um comentário começa com ';' -- faz a string terminar aí.
  while(*s != '\0' && *s != ';') {
    s++;
  }
  *s = '\0';
}

void monta_string(int linha, char *str)
{
  char *label = NULL;
  char *instrucao = NULL;
  char *arg = NULL;
  tira_comentario(str);
  if (*str == '\0') return;
  if (*str != ' ') {
    label = str;
    str = pula_ate_espaco(str);
  }
  str = detona_espacos(str);
  if (*str != '\0') {
    instrucao = str;
    str = pula_ate_espaco(str);
  }
  str = detona_espacos(str);
  if (*str != '\0') {
    arg = str;
    str = pula_ate_espaco(str);
  }
  str = detona_espacos(str);
  if (*str != '\0') {
    fprintf(stderr, "linha %d: ignorando '%s'\n", linha, str);
  }
  if (label != NULL || instrucao != NULL) {
    monta_linha(linha, label, instrucao, arg);
  }
}

void monta_arquivo(char *nome)
{
  int linha = 1;
  FILE *arq;
  arq = fopen(nome, "r");
  if (arq == NULL) {
    fprintf(stderr, "Não foipossível abrir o arquivo '%s'\n", nome);
    return;
  }
  while (true) {
    char lin[500];
    if (fgets(lin, 500, arq) == NULL) break;
    int n = strlen(lin) - 1;
    if (lin[n] == '\n') lin[n] = '\0';
    monta_string(linha, lin);
    linha++;
  }
  fclose(arq);
  ref_resolve();
}

int main(int argc, char *argv[argc])
{
  if (argc != 2) {
    fprintf(stderr, "ERRO: chame como '%s nome_do_arquivo'\n", argv[0]);
    return 1;
  }
  monta_arquivo(argv[1]);
  mem_imprime();
  return 0;
}
