/* ******************************************************************************************************
    PROGRAM NAME: cpu_setup.c
    OBJECTIVE: Develop Best CPU.
    DESCRIPTION: Setup CPU architecture with LOAD and STORE data functionality. 
    TEAM MEMBERS: Sanket Dhami, Karthik Sadanand, Ramyashree, Neha Rege
    DATE: 15th Sep, 2016
   ****************************************************************************************************** */
  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#define MEMORY_SIZE 2048
#define REG_COUNT 16
#define TEMP_REG_COUNT 16
#define DATA_MEMORY_BASE_ADD 768 /*Need to discuss*/
#define INSTR_MEMORY_BASE_ADD 256 /*Need to discuss*/
#define BOOT_MEMORY_BASE_ADD 0

#define STORE_OPCODE 3
#define LOAD_OPCODE 4

unsigned int memory[MEMORY_SIZE];
unsigned int reg[REG_COUNT];
unsigned int temp_reg[TEMP_REG_COUNT];


enum registers{A, B , C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q};


/* Program Counter */
unsigned int pc = INSTR_MEMORY_BASE_ADD;

/* Stack Pointer */
unsigned int sp;

/****FLAGS*****/
/***Flag will be CHAR ARRAY****/

/******STACK MEMORY****/

void init_memory( )
{
	int i;
 	for(i=0;i<MEMORY_SIZE;i++)
 	{
 		memory[i] = 0;		
	}
	memory[120]=100;
}

void init_reg( )
{
	int i;
 	for(i=0;i<REG_COUNT;i++)
 	{
 		reg[i] = 0;		
	}
 		
}

void init_temp_reg( )
{
	int i;
 	for(i=0;i<TEMP_REG_COUNT;i++)
 	{
 		temp_reg[i] = 0;		
	}
 		
}

int execute_load(int dest, int src, int offset){
	int address = reg[src] + offset;
	if(address >= INSTR_MEMORY_BASE_ADD && address < MEMORY_SIZE){
		reg[dest] = memory[address];
		pc = pc + 4;
		return 0;
	}	
	
}


int main(){
	int  temp = 0,  value = 0, i = 0, count = 0;
	unsigned int opcode = 0;
	unsigned int src=0, dest=0;
	unsigned int offset;
	char *p1 = NULL; 
	char *p2 = NULL, *destreg = NULL, *srcreg = NULL;
	char instruction[20];
	int regtr1, regtr2;
	int result;	
	while(pc < MEMORY_SIZE){
		printf("\nFormat for entering opcode: LOAD dest_reg,offset (src_reg) Eg: LOAD A,30(B) \n");
		printf("Format for entering opcode: STORE dest_reg,offset (src_reg) Eg: STORE A,30(B) \n");
		printf("Enter instructions LOAD/STORE/PRINT/EXIT\n");
		fgets(instruction,20,stdin);
		p1 = strtok(instruction, " ,()");
		if (strcasecmp(p1, "LOAD")==0) {
			opcode = LOAD_OPCODE;
			p2 = strtok(NULL," ");    
	      		destreg = strtok(p2,",");
			dest = destreg[0] - 'A';      /*Index for Destination register*/
			if(dest > Q){
				printf("Destination registers should be for Register A-Q\n");
				break;
			}	      
              		p2 = strtok(NULL,",");
	      		offset = atoi(strtok(p2,"("));
		        p2 = strtok(NULL,"(");
			srcreg = strtok(p2,")");
			src = srcreg[0] - 'A';     /*Index for source register*/
			if(src > Q){
				printf("Source registers should be for Register A-Q\n");
				break;
			}
			result = execute_load(dest, src, offset);
			if (result !=0){
				printf("Error in executing Load Instruction\n");
			}	      
	
		}
		else if (strcasecmp(p1, "STORE")==0) {


		}
		else if(strcasecmp(p1, "PRINT")==0){
		
		}
		else{
			break;
		}
	}
}
