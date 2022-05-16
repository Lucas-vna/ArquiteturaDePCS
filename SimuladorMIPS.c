// SIMULADOR MIPS

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define SIZE 256 // 1 kb

uint32_t R[32]; // reg ou registers
uint32_t ram[SIZE];
uint32_t opcode, rs, rt, rd, shamt, funct;

uint32_t pc = 0, ic = 0;

//---------------------------------------------------------------------
// FUNCOES LOGICA E ARITMETICAS
//---------------------------------------------------------------------

void add(uint8_t dest, uint8_t src_1, uint8_t src_2, int reg[])
{

  reg[dest] = reg[src_1] + reg[src_2];

} // function add

void addi(uint8_t dest, uint8_t src, int16_t imm, int reg[])
{

  if (dest == 29)
  {
    reg[dest] = reg[src] + imm / 4;
  } // if
  else
  {
    reg[dest] = reg[src] + reg[src];
  } // else

} // function addi

void sub(uint8_t dest, uint8_t src_1, uint8_t src_2, int reg[])
{

  reg[dest] = reg[src_1] + reg[src_2];

} // function sub

void mult(uint8_t src_1, uint8_t src_2, int reg[])
{

  reg[33] = reg[src_1] * reg[src_2];
  reg[32] = 0;

} // function mult

void div(uint8_t src_1, uint8_t src_2, int reg[])
{

  reg[32] = reg[src_1] % reg[src_2];
  reg[33] = reg[src_1] / reg[src_2];

} // function div

void and (uint8_t dest, uint8_t src_1, uint8_t src_2, int reg[])
{

  reg[dest] = reg[src_1] & reg[src_2];

} // function and

void or (uint8_t dest, uint8_t src_1, uint8_t src_2, int reg[])
{
  reg[dest] = reg[src_1] | reg[src_2];

} // function or

void xor (uint8_t dest, uint8_t src_1, uint8_t src_2, int reg[]) 
{
  reg[dest] = reg[src_1] ^ reg[src_2];
  
} // function xor

    void nor(uint8_t dest, uint8_t src_1, uint8_t src_2, int reg[])
{

  reg[dest] = !(reg[src_1] | reg[src_2]);

} // function nor

void slt(uint8_t dest, uint8_t src_1, uint8_t src_2, int reg[])
{

  reg[dest] = reg[src_1] < reg[src_2];

} // function slt

void sll(uint8_t dest, uint8_t src, uint8_t shamt, int reg[])
{

  reg[dest] = reg[src] << shamt;

} // function sll

void srl(uint8_t dest, uint8_t src, uint8_t shamt, int reg[])
{

  reg[dest] = reg[src] >> shamt;

} // function srl

void sra(uint8_t dest, uint8_t src, uint8_t shamt, int reg[])
{

  reg[dest] = reg[src] >> shamt;

} // function sra

void andi(uint8_t dest, uint8_t src, uint8_t imm, int reg[])
{

  reg[dest] = reg[src] & imm;

} // function andi

void ori(uint8_t dest, uint8_t src, uint8_t imm, int reg[])
{

  reg[dest] = reg[src] | imm;

} // function ori

void xori(uint8_t dest, uint8_t src, uint8_t imm, int reg[])
{

  reg[dest] = reg[src] ^ imm;

} // function xori

//---------------------------------------------------------------------
// MAIN
//---------------------------------------------------------------------

int main(int argc, char *argv[])
{

  uint32_t *ptr;

  int result;

  // Abre um arquivo BINÁRIO para LEITURA
  FILE *arq = fopen(argv[1], "r+b");

  if (arq == NULL) // Se houve erro na abertura
  {
    printf("Problemas na abertura do arquivo\n");
    return 0;
  }

  result = fread(ram, 4, 100, arq); // (buffer, numero de bytes, qnde de dados que vai tentar ler, arquivo)
  printf("Nro de elementos lidos: %d\n", result);

  for (int i = 0; i < result; i++)
  {
    printf("%01x \n", (unsigned int)ram[i]);
  }

  uint32_t instruct;

  for (int i = 0; i < result; i++)
  {

    pc = i;

    ic = (uint32_t)ram[pc];

    instruct = ((ic & 0xFF000000) >> 24 | (ic & 0x00FF0000) >> 8 | (ic & 0x0000FF00) << 8 | (ic & 0x000000FF) << 24);

    ic = instruct;

    opcode = (ic & 0xFC000000) >> 26; // separando os operadores do opcode
    rs = (ic & 0x3E00000) >> 21;
    rt = (ic & 0x1F0000) >> 16;
    rd = (ic & 0xF800) >> 11;
    shamt = (ic & 0x7C0) >> 6;
    funct = (ic & 0x3F);

    printf("\n opcode %01x | rs %01x | rt %01x | rd %01x | shamt %01x | funct %01x\n", opcode, rs, rt, rd, shamt, funct);
    // printf("%01x ", instruct);

    R[rs] = 10;
    R[rt] = 5;

    switch (opcode)
    {

    case 0x00:

      switch (funct)
      {

      case 0x20: // CASE ADD
        add(rd, rs, rt, R);
        printf("  Alguma adição: registrador destino  %d\n\n", R[rd]);
        break;

      case 0x8: // CASE ADDI
        addi(rd, rs, rt, R);
        printf("  Alguma adição: registrador destino  %d\n\n", R[rd]);
        break;

      case 0x22: // CASE SUB
        sub(rd, rs, rt, R);
        printf("  Alguma subtração: registrador destino  %d\n\n", R[rd]);
        break;

      case 0x18: // CASE MULT
        mult(rs, rt, R);
        printf("  Alguma adição: registrador destino  %d\n\n", R[rd]);
        break;

      case 0x1A: // CASE DIV
        div(rs, rt, R);
        printf("  Alguma adição: registrador destino  %d\n\n", R[rd]);
        break;

      case 0x24: // CASE AND
        and(rd, rs, rt, R);
        printf("  Alguma adição: registrador destino  %d\n\n", R[rd]);
        break;

      case 0x25: // CASE OR
        or(rd, rs, rt, R);
        printf("  Alguma adição: registrador destino  %d\n\n", R[rd]);
        break;

      case 0x26: // CASE XOR
        xor(rd, rs, rt, R);
        printf("  Alguma adição: registrador destino  %d\n\n", R[rd]);
        break;

      case 0x27: // CASE NOR
        nor(rd, rs, rt, R);
        printf("  Alguma adição: registrador destino  %d\n\n", R[rd]);
        break;

      case 0x2A: // CASE SLT
        slt(rd, rs, rt, R);
        printf("  Alguma adição: registrador destino  %d\n\n", R[rd]);
        break;

      case 0x00: // CASE SLL
        sll(rd, rs, rt, R);
        printf("  Alguma adição: registrador destino  %d\n\n", R[rd]);
        break;

      case 0x2: // CASE SRL
        srl(rd, rs, rt, R);
        printf("  Alguma adição: registrador destino  %d\n\n", R[rd]);
        break;

      case 0x3: // CASE SRA
        sra(rd, rs, rt, R);
        printf("  Alguma adição: registrador destino  %d\n\n", R[rd]);
        break;

      case 0xC: // CASE ANDI
        andi(rd, rs, rt, R);
        printf("  Alguma adição: registrador destino  %d\n\n", R[rd]);
        break;

      case 0xD: // CASE ORI
        ori(rd, rs, rt, R);
        printf("  Alguma adição: registrador destino  %d\n\n", R[rd]);
        break;

      case 0xE: // CASE XORI
        xori(rd, rs, rt, R);
        printf("  Alguma adição: registrador destino  %d\n\n", R[rd]);
        break;

      }//switch interno
    
    }//switch externo

  }//for

  return 0;
}

// luiz amor da minha vida
