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
#define DATA_MEMORY_BASE_ADD 1024	/* Base Address of Data Memory = 1024 */
#define INSTR_MEMORY_BASE_ADD 256	/* Base Address of Instruction Memory = 256 */
#define BOOT_MEMORY_BASE_ADD 0		/* Base Address of BIOS = 0 */

#define STORE_OPCODE 3		/* Opcode for STORE instruction */
#define LOAD_OPCODE 4		/* Opcode for LOAD instruction */

unsigned int memory[MEMORY_SIZE];	/* Total Memory size = 2048 bytes */
unsigned int reg[REG_COUNT];		/* Number of General Purpose Registers = 16 */
unsigned int temp_reg[TEMP_REG_COUNT];		/* Number of Temporary Registers = 16 */


enum registers{A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P};
unsigned int src=0, dest=0;	/* Index for source and destination registers */
unsigned int opcode = 0;	
unsigned int offset=0;


/* Program Counter */
unsigned int pc = INSTR_MEMORY_BASE_ADD;	/* PC initialized to starting address of instruction memory */

/* Stack Pointer */
unsigned int sp = MEMORY_SIZE-1S;
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

/****************Function for LOAD Instruction *********************************************/
/*	On executing LOAD destreg,offset(srcreg) instruction, the address or 
	value present in source register is added with offset which will result 
	into final address. The value present in final address is then loaded in 
	destination register.*/
/*******************************************************************************************/
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
	else{ 
		printf("Invalid location\n");
	}		
}

/****************Function for STORE Instruction *********************************************/
/*	On executing STORE destreg,offset(srcreg) instruction, the address or 
	value present in source register is added with offset which will result 
	into final address. The value present in destination register is then 
	loaded in memory's final address.*/
/*******************************************************************************************/
int execute_store(){
	opcode = memory[pc];
	src = memory[pc+1];
	offset =  memory[pc+2];
	dest = memory[pc+3];
	address = reg[src] + offset;
	if(address >= INSTR_MEMORY_BASE_ADD && address < MEMORY_SIZE){
		memory[address] = reg[dest];
		pc = pc + 4;
		printf("STORE Instruction executed successfully\n\n\n");
		return 0;
	}
	else{ 
		printf("Invalid location\n");
	}
}

/********Function for printing values of registers, memory, flags *********/
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
	printf("MAR(dec):%d\n",address);
	printf("MAR(HEX):%xH\n",address);
	printf("*****************Value of MDR******************\n");
	printf("MDR:%u\n",memory[address]);
	printf("*****************Current Memory location******************\n");
	printf("Memory Location(dec):%d\n",address);
	printf("Memory Location(HEX):%xH\n",address);
	printf("*****************Value of Program Counter******************\n");
	printf("Program Counter(dec):%d\n",pc);
	printf("Program Counter(HEX):%xH\n",pc);
	printf("*****************Value of Stack Pointer******************\n");
	printf("Stack Pointer(dec):%d\n",sp);
	printf("Stack Pointer(HEX):%xH\n",sp);
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
			if(dest >= P){
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
			if(src >= P){
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

			opcode = STORE_OPCODE;
			p2 = strtok(NULL," ");
	      		destreg = strtok(p2,",");
			dest = destreg[0] - 'A';      /*Index for Destination register*/
			if(dest >= P){
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
			if(src >= P){
				printf("Source registers should be from Register A-Q \n");
				break;
			}
			memory[INSTR_MEMORY_BASE_ADD + i] = opcode;
			memory[INSTR_MEMORY_BASE_ADD + i+1] = src;
			memory[INSTR_MEMORY_BASE_ADD + i+2] = offset;
			memory[INSTR_MEMORY_BASE_ADD + i+3] = dest;
			i = i+4;
			result = execute_store();
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
		else {
			printf("Enter valid instructions \n");
			break;
		}
	}
return 0;
}
