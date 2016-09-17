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
#define DATA_MEMORY_BASE_ADD 1024 /*Need to discuss*/
#define INSTR_MEMORY_BASE_ADD 256 /*Need to discuss*/
#define BOOT_MEMORY_BASE_ADD 0

#define STORE_OPCODE 3
#define LOAD_OPCODE 4

unsigned int memory[MEMORY_SIZE];
unsigned int reg[REG_COUNT];
unsigned int temp_reg[TEMP_REG_COUNT];


enum registers{A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q};
unsigned int src=0, dest=0;
unsigned int opcode = 0;
unsigned int offset=0;


/* Program Counter */
unsigned int pc = INSTR_MEMORY_BASE_ADD;

/* Stack Pointer */
unsigned int sp;
char *p1 = NULL;
int address;
/****FLAGS*****/


/******STACK MEMORY****/

void init_memory( )
{
	int i;
 	for(i=0;i<MEMORY_SIZE;i++)
 	{
 		memory[i] = 0;		
	}
	memory[730]=100;
}


int execute_load(){
	opcode = memory[pc];
	src = memory[pc+1];
	offset =  memory[pc+2];
	dest = memory[pc+3];
	address = reg[src] + offset;
	if(address >= INSTR_MEMORY_BASE_ADD && address < MEMORY_SIZE){
		reg[dest] = memory[address];
		pc = pc + 4;
		printf("LOAD Instruction executed successfully\n\n\n");
		return 0;
	}		
}


int print_values(){
	printf("**************Values of Registers****************\n");
	int x = 0;
	char reg_char;
	for(x =0; x<REG_COUNT; x=x+2){
		reg_char = 'A' + x;
		printf("Register %c: %d\t\t\t\tRegister %c: %d\n",reg_char,reg[x],reg_char+1,reg[x+1]);	
	}
	printf("*****************Value of Instruction Register******************\n");
	printf("Instruction Register:%s\n", p1);
	printf("*****************Value of MAR******************\n");
	printf("MAR:%u\n",address);
	printf("*****************Value of MDR******************\n");
	printf("MDR:%u\n",memory[address]);
	printf("*****************Current Memory location******************\n");
	printf("Memory Location:%u\n",address);
	printf("*****************Value of Program Counter******************\n");
	printf("Program Counter:%u\n",pc);
	printf("\n\n\n");
	return 0;
}


int main(){
	int i = 0; 
	char *p2 = NULL, *destreg = NULL, *srcreg = NULL;
	char instruction[20];
	int result;
	init_memory();
	reg[0] = 700;
	reg[1] = 700;	
	result = print_values();
	while(pc < MEMORY_SIZE){
		printf("Enter instructions LOAD/STORE/EXIT\n");
		printf("1. Format for LOAD operation: LOAD dest_reg,offset(src_reg) Eg: LOAD A,30(B) \n");
		printf("2 .Format for STORE opcode: STORE dest_reg,offset(src_reg) Eg: STORE A,30(B) \n");
		fgets(instruction,20,stdin);
		p1 = strtok(instruction, " ,()");
		if (strcasecmp(p1, "LOAD")==0) {
			opcode = LOAD_OPCODE;
			p2 = strtok(NULL," ");    
	      		destreg = strtok(p2,",");
			dest = destreg[0] - 'A';      /*Index for Destination register*/
			if(dest > Q){
				printf("Destination registers should be from Register A-Q \n");
				break;
			}	      
              		p2 = strtok(NULL,",");
	      		offset = atoi(strtok(p2,"("));
		        p2 = strtok(NULL,"(");
			if(p2 == NULL){
				offset = 0;
			}
			srcreg = strtok(p2,")");
			src = srcreg[0] - 'A';     /*Index for source register*/
			if(src > Q){
				printf("Source registers should be from Register A-Q \n");
				break;
			}
			memory[INSTR_MEMORY_BASE_ADD + i] = opcode;
			memory[INSTR_MEMORY_BASE_ADD + i+1] = src;
			memory[INSTR_MEMORY_BASE_ADD + i+2] = offset; 
			memory[INSTR_MEMORY_BASE_ADD + i+3] = dest; 
			i = i+4;
			result = execute_load();
			if (result !=0){
				printf("Error in executing Load Instruction \n");
				break;
			}
			result = print_values();
			if (result !=0){
				printf("Error in printing values \n");
				break;
			}	      	
		}
		else if (strcasecmp(p1, "STORE")==0) {
			
		}
		else {
			printf("Enter valid instructions \n");
			break;
		}
	}
return 0;
}
