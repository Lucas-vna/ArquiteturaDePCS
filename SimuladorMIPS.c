/*
-> adicionar mascara do i e j
-> incluir instruçoes tipo i e j
-> implementar testes
-> chamada de sistema
-> alegro


*/

// SIMULADOR MIPS

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define SIZE 256 // 1 kb

uint32_t R[32]; // reg ou R
uint32_t ram[SIZE];
uint32_t opcode, rs, rt, rd, shamt, funct;

uint32_t pc = 0, ic = 0;

//----------------------------------------------------
//----------------------TIPO R------------------------
//----------------------------------------------------

void add(uint8_t dest, uint8_t src_1, uint8_t src_2, int reg[])
{

    reg[dest] = reg[src_1] + reg[src_2];

} // function add

void sub(uint8_t dest, uint8_t src_1, uint8_t src_2, int reg[])
{

    reg[dest] = reg[src_1] - reg[src_2];

} // function sub

void mult(uint8_t dest, uint8_t src_1, uint8_t src_2, int reg[])
{

    reg[dest] = reg[src_1] * reg[src_2];    

} // function mult

void divv(uint8_t dest, uint8_t src_1, uint8_t src_2, int reg[])
{
    
    reg[dest] = reg[src_1] / reg[src_2];

} // function div

void and (uint8_t dest, uint8_t src_1, uint8_t src_2, int reg[])
{

    reg[dest] = reg[src_1] & reg[src_2];

} // function and

void or (uint8_t dest, uint8_t src_1, uint8_t src_2, int reg[])
{
    reg[dest] = reg[src_1] | reg[src_2];

} // function or

void xor (uint8_t dest, uint8_t src_1, uint8_t src_2, int reg[]) {
    reg[dest] = reg[src_1] ^ reg[src_2];
} // function xor

    void nor(uint8_t dest, uint8_t src_1, uint8_t src_2, int reg[])
{

    reg[dest] = !(reg[src_1] | reg[src_2]);

} // function nor

void slt(uint8_t dest, uint8_t src_1, uint8_t src_2, int reg[])
{

    reg[dest] = (reg[src_1] < reg[src_2]);    

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

//----------------------------------------------------
//----------------------TIPO I------------------------
//----------------------------------------------------

void addi(uint8_t dest, uint8_t src, int16_t imm, int reg[])
{
    reg[dest] = reg[src] + imm;

} // function addi

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

//----------------------------------------------------
//----------------------TIPO J------------------------
//----------------------------------------------------

int j(uint32_t target)
{
    return (int)((target << 2)/* - 0x400000*/) / 4;
}

int jr(uint8_t src, int reg[])
{
    reg[src] = reg[31];
    return reg[src];
}

int jal(int pc, uint32_t target, int reg[])
{
    reg[31] = pc;
    return (int)((target << 2)/* - 0x400000*/) / 4;
}

//----------------------------------------------------
//----------------------LW E SW-----------------------
//----------------------------------------------------

bool lw(uint8_t dest, int16_t offset, uint8_t src, int reg[], int pilha[])
{
    if (offset / 4 + reg[src] >= SIZE)
        return false;

    reg[dest] = pilha[offset / 4 + reg[src]];
    return true;
}

bool sw(uint8_t dest, int16_t offset, uint8_t src, int reg[], int pilha[])
{
    // offset é o imediado para o armazenamento da pilha
    if (offset / 4 + reg[src] >= SIZE)
        return false;

    pilha[offset / 4 + reg[src]] = reg[dest];
    return true;
}

//----------------------------------------------------
//----------------------SYSCALL-----------------------
//----------------------------------------------------

int syscall(int pc, int reg[], char string_data[])
{
    uint8_t v0 = reg[2];
    uint8_t a0 = reg[4];

    switch (v0)
    {
    case 0x1:
        printf("Printando um valor inteiro...\n");
        printf("%d\n\n", a0);
        break;

    case 0xA:
        printf("Saindo do programa...\n\n");
        return ram[SIZE];

    default:
        printf("Chamada de sistema recebeu um código inválido");
    }

    return pc;
}



//----------------------------------------------------
//----------------------MAIN--------------------------
//----------------------------------------------------
int main(int argc, char *argv[])
{

    uint32_t *ptr;

    int result;

    // Abre um arquivo BINÁRIO para LEITURA
    FILE *arq = fopen(/*argv[]*/ "testetipor.bin", "r+b");

    if (arq == NULL) // Se houve erro na abertura
    {
        printf("Problemas na abertura do arquivo\n");
        return 0;
    }

    result = fread(ram, 4, 100, arq); // (buffer, numero de bytes, qnde de dados que vai tentar ler, arquivo)
    printf("Numero de elementos lidos: %d\n", result);

    for (int i = 0; i < result; i++)
    {
        printf("%01x \n", (unsigned int)ram[i]);
    }

    uint32_t instruct[result];
    int aux = 0;

    for (int i = 0; i < result; i++)
    {
        pc = i;
        ic = (uint32_t)ram[pc];

        instruct[i] = ((ic & 0xFF000000) >> 24 | (ic & 0x00FF0000) >> 8 | (ic & 0x0000FF00) << 8 | (ic & 0x000000FF) << 24);

        ic = instruct[i];
    }

    int pilha[SIZE];
    for (int i = 0; i < SIZE; i++)
    {
        pilha[i] = 0;
    }

    //----------------------------------------------------
    //----------------------TIPO R------------------------
    //----------------------------------------------------
    int pc = 0;
    int valor = 0;

    do
    {

        opcode = (instruct[pc] & 0xFC000000) >> 26; // separando os operadores do opcode

        if (opcode == 0x00)
        {
            rs = (instruct[pc] & 0x3E00000) >> 21;
            rt = (instruct[pc] & 0x1F0000) >> 16;
            rd = (instruct[pc] & 0xF800) >> 11;
            shamt = (instruct[pc] & 0x7C0) >> 6;
            funct = (instruct[pc] & 0x3F);
            // funct = (ic & 0x3F);
            /*
                    0000 0010 0011 0010 1000 0000 0010 0000
                      0    2   3   2    8     0    2    0

                      02 32 80 20

                    rs = 17;
                    rt = 18;
                    rd = 16;
                    shamt = 0;
                    funct = 32;
            */

            printf("\n opcode %01x | rs %01x | rt %01x | rd %01x | shamt %01x | funct %01x\n", opcode, rs, rt, rd, shamt, funct);
            // printf("%01x ", instruct);

            R[rs] = 10;
            R[rt] = 5;
            // R[rd] = 10;

            switch (funct)
            {

            case 0x20: // CASE ADD
                add(rd, rs, rt, R);
                printf(" Adicao: registrador destino  %d\n\n", R[rd]);
                break;

            case 0x22: // CASE SUB
                sub(rd, rs, rt, R);
                printf("  Subtracao: registrador destino  %d\n\n", R[rd]);
                break;

            case 0x18: // CASE MUL
                mult(rd, rs, rt, R);
                printf("  Multiplicacao: registrador destino  %d\n\n", R[rd]);
                break;

            case 0x1A: // CASE DIV
                divv(rd, rs, rt, R);
                printf("  Divisao: registrador destino  %d\n\n", R[rd]);
                break;

            case 0x24: // CASE AND
                and(rd, rs, rt, R);
                printf("  Caso AND: registrador destino  %d\n\n", R[rd]);
                break;

            case 0x25: // CASE OR
                or (rd, rs, rt, R);
                printf("  Caso OR: registrador destino  %d\n\n", R[rd]);
                break;

            case 0x26: // CASE XOR
                xor(rd, rs, rt, R);
                printf("  Caso XOR: registrador destino  %d\n\n", R[rd]);
                break;

            case 0x27: // CASE NOR
                nor(rd, rs, rt, R);
                printf("  Caso NOR: registrador destino  %d\n\n", R[rd]);
                break;

            case 0x2A: // CASE SLT
                slt(rd, rs, rt, R);
                printf("  Caso SLT: registrador destino  %d\n\n", R[rd]);
                break;

            case 0x00: // CASE SLL
                sll (rd, rt, shamt, R);
                printf("  Caso SLL: registrador destino  %d\n\n", R[rd]);
                break;

            case 0x2: // CASE SRL
                srl (rd, rt, shamt, R);
                printf("  Caso SRL: registrador destino  %d\n\n", R[rd]);
                break;

            case 0x3: // CASE SRA
                sra (rd, rt, shamt, R);
                printf("  Caso SRA: registrador destino  %d\n\n", R[rd]);
                break;

            case 0x8: // CASE JR
                aux = jr(rs, R);
                if (aux < result && aux >= 0)
                {
                    pc = aux;
                    printf("Conteudo dos registradores:\n  Registrador %d: %d\n  Novo $pc: %d\n\n", rs, R[rs], pc);
                }
                else {
                    printf("Nao foi possivel realizar o salto -> valor de $pc fora do escopo\n");
                    exit(3);
                }                

                break;

            } // switch interno

        } // if opcode 0

        // MASCARA DO TIPO J

        else if (opcode == 0x2 || opcode == 0x3)
        {
            uint32_t target = instruct[pc] & 0x3FFFFFF;

            switch (opcode)
            {
            case 0x2: // J
                printf("$pc = %d\nOperacao: j 0x%08x\n", pc, target);
                aux = j(target);
                printf("AUX: %d\nOperacao: j 0x%08x\n", aux, target);
                if (aux < result && aux >= 0)
                {
                    pc = aux - 1;
                    printf("Conteudo dos registradores:\n  Novo $pc: %d\n\n", pc + 1);
                }
                else
                {
                    printf("Nao foi possivel realizar o salto -> valor de $pc fora do escopo\n");
                    exit(3);
                }
                break;

            case 0x3: // Jal
                printf("$pc = %d\nOperacao: jal 0x%08x\n", pc, target);
                aux = jal(pc, target, R);
                if (aux < result && aux >= 0)
                {
                    pc = aux - 1;
                    printf("Conteudo dos registradores:\n  Registrador 31: %d\n  Novo $pc: %d\n\n", R[31], pc + 1);
                }
                else
                {
                    printf("Nao foi possivel realizar o salto -> valor de $pc fora do escopo\n");                    
                    exit(3);
                }
                break;

            default:
                break;
            }
        }

        // MASCARA DO TIPO I
        else
        {
            rs = (instruct[pc] & 0x3E00000) >> 21;
            rd = (instruct[pc] & 0x1F0000) >> 16;
            int16_t immediate = instruct[pc] & 0xFFFF;

            R[rd] = 0;
            R[rs] = 10;

            printf("PC: %d", pc);
            printf("\n opcode %01x | rd %01x | rs %01x | imetiato: %01x\n", opcode, rd, rs, immediate);

            switch (opcode)
            {
            case 0x8: // ADDI
                addi(rd, rs, immediate, R);
                printf("  Adicao imediata: registrador destino  %d\n\n", R[rd]);
                break;

            case 0xC: // ANDI
                andi(rd, rs, immediate, R);
                printf("  Caso ANDI: registrador destino  %d\n\n", R[rd]);
                break;

            case 0xD: // ORI
                ori(rd, rs, immediate, R);
                printf("  Caso ORI: registrador destino  %d\n\n", R[rd]);
                break;

            case 0xE: // XORI
                xori(rd, rs, immediate, R);
                printf("  Caso XORI: registrador destino  %d\n\n", R[rd]);
                break;

            case 0x23: // LW
                if (lw(rd, immediate, rs, R, pilha))
                {
                    printf("Conteudo dos registradores:\n  Registrador %d: %d\n  Registrador %d: %d\n\n", rd, R[rd], rs, R[rs]);
                }
                else
                {
                    printf("Erro ao realizar a operacao -> Nao foi reservado espaço na memoria\n");
                    exit(3);
                }

                break;

            case 0x2B: // SW
                if (sw(rd, immediate, rs, R, pilha))
                {
                    printf("Conteudo dos registradores:\n  Registrador %d: %d\n  Registrador %d: %d\n", rd, R[rd], rs, R[rs]);
                    printf("Conteudo da memoria:\n  RAM[%d]: %d\n\n", R[rs] + immediate / 4, pilha[immediate / 4 + R[rs]]);
                }
                else
                {
                    printf("Erro ao realizar a operacao -> Nao foi reservado espaço na memoria\n");
                    exit(3);
                }
                break;

            default:
                break;
            } // SWITCH
        }     // ELSE
        pc++;
    } while (pc < result);
    return 0;
} // MAIN
