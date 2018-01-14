#include "mips.h"

//Allocate the sim structure
//Allocate instruction structure
//update instruction's registers etc.
//update Data Memory
//TODO: SP Registers

simPT simAllocate (unsigned int dataMemSize) 
{
   simPT simP           = (simPT)calloc(1, sizeof(simT));

   simP->dataMemory  = (uchar*)calloc(dataMemSize, sizeof(uchar));
   simP->dataMemSize = dataMemSize;

   for(int i = 0; i < dataMemSize; i++) {
      simP->dataMemory[i] = 0xFF; 
   }
   return simP;
}

////returns value of the specified general purpose register
uint get_gp_register(simPT simP, uint reg)
{
   return simP->gprFile[reg];
}

opcodeT strToEnum (char* str) {
   if(!strcmp(str, "XOR"))
      return XOR;
   if(!strcmp(str, "ADD")) 
      return ADD;
   if(!strcmp(str, "SUB")) 
      return SUB;
   if(!strcmp(str, "LW")) 
      return LW;
   if(!strcmp(str, "SW")) 
      return SW;
   if(!strcmp(str, "BNEZ"))
      return BNEZ;
   if(!strcmp(str, "BLTZ"))
      return BLTZ;
   if(!strcmp(str, "EOP")) 
      return EOP;
   if(!strcmp(str, "ADDI"))
      return ADDI;
   if(!strcmp(str, "SUBI"))
      return SUBI;
}


//loads the assembly program in file "filename" in instruction memory at the specified address
void load_program(simPT simP, const char *filename, unsigned base_address) {
   instructPT instructP = (instructPT)calloc(1, sizeof(instructT));

}

uint alu (uint value1, uint value2, opcodeT opcode){
   if(opcode == ADD || opcode == ADDI)
      return value1 + value2; 
   else if(opcode == SUB || opcode == SUBI)
      return value1 - value2;
   else 
      return value1 ^ value2;
}

uint agen (simPT simP, instructPT instructP) {
   return (instructP->opr1 + get_gp_register(simP, instructP->opr2));// % simP->dataMemSize;
}

int processInstr(simPT simP){

   //fetch stage
   instructPT instructP = simP->instMemory[simP->sprFile[PC]];

   //Decode, execute, memory, writeback stage
   simP->sprFile[PC]++;
   switch(instructP->opcode) {
      case LW:{
                 simP->gprFile[instructP->opr0] = simP->dataMemory[agen(simP, instructP)];
                 break;
              }

      case SW:{
                 simP->dataMemory[agen(simP, instructP)] = get_gp_register(simP, instructP->opr0);
                 break;
              }

      case ADD:
      case SUB:
      case XOR:{
                  simP->gprFile[instructP->opr0] = alu(get_gp_register(simP, instructP->opr1), 
                        get_gp_register(simP, instructP->opr2), 
                        instructP->opcode);
                  break;
               }

      case ADDI:
      case SUBI:{
                   int imm = instructP->opr2;
                   imm     = (imm << 16) >> 16;
                   simP->gprFile[instructP->opr0] = alu(get_gp_register(simP, instructP->opr1),
                         imm, instructP->opcode);
                   break;
                }

      case BLTZ:{
                   if(get_gp_register(simP, instructP->opr0) < 0) 
                      simP->sprFile[PC] = instructP->opr1;
                   break;
                }

      case BNEZ:{
                   if(get_gp_register(simP, instructP->opr0) != 0) 
                      simP->sprFile[PC] = instructP->opr1;
                   break;
                }
      case EOP: {
                   return 1;
                }
   }
   return 0;
}

//runs the simulator for "cycles" clock cycles (run the program to completion if cycles=0) 
void run(simPT simP, uint cycles) {
   int eop = 0;
   do {
      eop = processInstr(simP);
      simP->instCount++;
   } while((simP->instCount < cycles && cycles != 0) || (cycles == 0 && !eop) );
}

////resets the state of the simulator
///* Note: registers should be reset to UNDEFINED value */
//void reset();
//
////returns value of the specified special purpose register
///* Note: you are allowed to use a custom format for the IR register.Therefore
//   Therefore, the test cases won't check the value of IR using this method. 
//   You can add an extra method to retrieve the content of IR */
//unsigned get_sp_register(sp_register_t reg);
//
//
////returns the IPC
//float get_IPC();
//
////returns the number of instructions fully executed
//unsigned get_instructions_executed();
//
////prints the content of the data memory within the specified address range
//void print_memory(unsigned start_address, unsigned end_address);
//
//// writes an integer value to data memory at the specified address (use little-endian format: https://en.wikipedia.org/wiki/Endianness)
//void write_memory(unsigned address, unsigned value);
//
////prints the values of the registers 
//void print_registers();



