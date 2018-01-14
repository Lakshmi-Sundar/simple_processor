#include "mips.h"

void parser (simPT simP, char* filename) {
   FILE* trace;
   char buff[1024], label[495];
   int a, b, c, lineNo = 0;

   trace  = fopen(filename, "r");

   do {
      simP->instMemory         = (instructPT*) realloc(simP->instMemory, (lineNo + 1)*sizeof(instructPT));
      instructPT instructP     = (instructPT) calloc(1, sizeof(instructT));
      simP->instMemory[lineNo] = instructP;
      fscanf(trace, "%s ", buff);
      if((!strcmp(buff, "XOR") || !strcmp(buff, "ADD") || !strcmp(buff, "SUB"))) {
         fscanf(trace, "R%d R%d R%d", &a, &b, &c);
         instructP->opcode     = strToEnum(buff);
         instructP->opr0       = a;
         instructP->opr1       = b;
         instructP->opr2       = c;
         
      }
      else if((!strcmp(buff, "BLTZ")) || (!strcmp(buff, "BNEZ"))) {
         fscanf(trace, "R%d %s", &a, label);
         FILE* temp = fopen(filename, "r");
         int line = 0;
         do{
            char str[25];
            fscanf(temp, "%s", str);
            if(str[strlen(str)-1] == ':'){
               str[strlen(str)-1] = '\0';
               if(!strcmp(str, label)){
                  b = line;
                  break;
               }
            }
            if( !strcmp(str, "XOR")  || 
                !strcmp(str, "ADD")  || 
                !strcmp(str, "SUB")  ||
                !strcmp(str, "LW")   ||
                !strcmp(str, "SW")   ||
                !strcmp(str, "BNEZ") ||
                !strcmp(str, "BLTZ") ||
                !strcmp(str, "EOP")  ||
                !strcmp(str, "ADDI") ||
                !strcmp(str, "SUBI") )
               line++;
         }while(!feof(temp));
         fclose(temp);
         instructP->opcode     = strToEnum(buff);
         instructP->opr0       = a;
         instructP->opr1       = b;
      }
      else if((!strcmp(buff, "ADDI")) || (!strcmp(buff, "SUBI"))) {
         fscanf(trace, "R%d R%d %x", &a, &b, &c);
         instructP->opcode     = strToEnum(buff);
         instructP->opr0       = a;
         instructP->opr1       = b;
         instructP->opr2       = c;
      }
      else if((!strcmp(buff, "LW")) || (!strcmp(buff, "SW"))) {
         fscanf(trace, "R%d %d(R%d)", &a, &b, &c);
         instructP->opcode     = strToEnum(buff);
         instructP->opr0       = a;
         instructP->opr1       = b;
         instructP->opr2       = c;
      }
      else if((!strcmp(buff, "EOP"))) {
         instructP->opcode     = strToEnum(buff);
      }
      else{
         if(buff[strlen(buff)-1] != ':')
            printf("Instruction not supported\n");
      }
      lineNo++;
   }while(!feof(trace));
}

int main (int argc, char**argv) {

   simPT simP = simAllocate(1024*1024);
   parser(simP, argv[1]);
   run(simP, 0);
}
