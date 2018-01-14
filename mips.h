#ifndef SIM_H_
#define SIM_H_

#include<stdio.h>
#include<string.h>
#include<malloc.h>
#include<stdlib.h>


#define UNDEFINED 0xFFFFFFFF //constant used to initialize registers
#define NUM_SPR 9
#define NUM_GPR 32
#define NUM_OPCODES 10

typedef enum {PC, NPC, IR, A, B, IMM, COND, ALU_OUTPUT, LMD} spRegT;

typedef enum {LW, SW, ADD, ADDI, SUB, SUBI, XOR, BLTZ, BNEZ, EOP} opcodeT;

typedef struct _instructT* instructPT;
typedef struct _simT*      simPT;

typedef unsigned char uchar;
typedef unsigned int  uint;

typedef struct _instructT{
   opcodeT           opcode;
   uint              opr0;
   uint              opr1;
   uint              opr2;

}instructT;

typedef struct _simT{
   int               cycleCount;
   int               instCount;

   uint              gprFile[NUM_GPR];
   uint              sprFile[NUM_SPR];

   uchar*            dataMemory;
   instructPT        *instMemory;
   uint              dataMemSize;



}simT;

simPT simAllocate (unsigned int dataMemSize); 
opcodeT strToEnum (char* str);
void run(simPT simP, uint cycles);

#endif /*SIM_H_*/
