/* ******************************************************************************************************
    PROGRAM NAME: cpu_setup.c
    OBJECTIVE: Develop Best CPU.
    DESCRIPTION: Setup CPU architecture with LOAD and STORE data and ALU operations and FLAGS. 
    TEAM MEMBERS: Sanket Dhami, Karthik Sadanand, Ramyashree, Neha Rege
    DATE: 8th Oct, 2016
   ****************************************************************************************************** */
  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>

#define MEMORY_SIZE 2048     
#define REG_COUNT 16         
#define TEMP_REG_COUNT 16	
#define DATA_MEMORY_BASE_ADD 1024	/* Base Address of Data Memory = 1024 */
#define INSTR_MEMORY_BASE_ADD 256	/* Base Address of Instruction Memory = 256 */
#define BOOT_MEMORY_BASE_ADD 0		/* Base Address of BIOS = 0 */

#define STORE_OPCODE 3		/* Opcode for STORE instruction */
#define LOAD_OPCODE 4		/* Opcode for LOAD instruction */

 int memory[MEMORY_SIZE];	/* Total Memory size = 2048 bytes */
 int reg[REG_COUNT];		/* Number of General Purpose Registers = 16 */
 int temp_reg[TEMP_REG_COUNT];		/* Number of Temporary Registers = 16 */
enum registers{A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P};
 int src=0, dest=0,inti=0;	/* Index for source and destination registers */
 int opcode = 0;	
 int offset=0;
int i = 0;
char *destreg = NULL, *srcreg = NULL,*basereg = NULL, *indexreg = NULL,*intreg = NULL;
bool flags[4]={ false };	/* flag register for setting various flags*/
/*Carry Flag = flags[0] 
 * Zero Flag = flags[1]
 * Sign Flag = flags[2]
 * Overflow flag = flags[3] 
 *  */

/* Program Counter */
 int pc = INSTR_MEMORY_BASE_ADD;	/* PC initialized to starting address of instruction memory */

/* Stack Pointer */
 int sp = MEMORY_SIZE-1;
char *p1 = NULL;
int address,part_address,result;

/******Function Declarations*********/
bool load_and_store();
bool alu_operations();
void init_memory();
int execute_load();
int execute_store();
int print_values();
int add(int inti, int src);
int sub(int inti, int src);
int mul(int inti, int src);
int division(int inti, int src);
int mod(int inti, int src);
int rem =0;


/***************Function pointers*******/

int (*fun_ptr_add)(int, int) = &add;
int (*fun_ptr_sub)(int, int) = &sub;
int (*fun_ptr_mul)(int, int) = &mul;
int (*fun_ptr_division)(int, int) = &division;
int (*fun_ptr_mod)(int, int) = &mod;


void init_memory( )
{
	int i;
 	for(i=0;i<MEMORY_SIZE;i++)
 	{
 		memory[i] = 0;		
	}
	memory[1144]=4;/** Initialized 1144th Memory location to 4. For testing. */ 	
	memory[1148]=8;/* Initialized 1148th Memory location to 8. For testing. */ 
}


/********Function for printing values of registers, memory, flags *********/
int print_values(){
	printf("**************Values of Registers****************\n");
	int x = 0;
	char reg_char;
	for(x =0; x<REG_COUNT; x=x+2){
		reg_char = 'A' + x;
		printf("Register %c: %d\t\t\t\t Register %c: %d\n",reg_char,reg[x],reg_char+1,reg[x+1]);	
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

	printf("*****************Value of Flag Register******************\n");
	printf("Carry Flag = %d\t",flags[0]);
	printf("Zero Flag = %d\t",flags[1]);
	printf("Sign Flag = %d\t",flags[2]);
	printf("Overflow Flag = %d\n",flags[3]);
	if(flags[0] == true){
		printf("WARNING: Carry Flag is generated\n");
	}
	if(flags[3] == true){
		printf("WARNING: Overflow flag is generated\n");
	}

	printf("*****************Value of Stack Pointer******************\n");
	printf("Stack Pointer(dec):%d\n",sp);
	printf("Stack Pointer(HEX):%xH\n",sp);
	printf("\n\n\n");
	return 0;
}

bool alu_operations(){
	printf("1. Format for ADD operation: ADD dest_reg,src_reg1,src_reg2 Eg: ADD rd,r1,r2 \n");
	printf("2. Format for SUB operation: SUB dest_reg,src_reg1,src_reg2 Eg: SUB rd,r1,r2 \n");
	printf("3. Format for MUL operation: MUL dest_reg,src_reg1,src_reg2 Eg: MUL rd,r1,r2 \n");
	printf("4. Format for DIV operation: DIV dest_reg,src_reg1,src_reg2 Eg: DIV rd,r1,r2 \n");
	printf("5. Format for MOD operation: MOD dest_reg,src_reg1,src_reg2 Eg: MOD rd,r1,r2 \n");
	char instruction[20];
	int len;
	char *p2 = NULL;
	fgets(instruction,20,stdin);
	len = strlen(instruction);
	instruction[len-1] = '\0';
	p1 = strtok(instruction, " ");
	p2 = strtok(NULL," ");
	destreg = strtok(p2,",");
	dest = destreg[0]-'A';
	if(dest > P){
			printf("Destination register should be from Register A-P \n");
			return false;
		}
	p2 = strtok(NULL," ");
	intreg = strtok(p2,",");
	inti = intreg[0]-'A';
	if(inti > P){
			printf("Intermediate register should be from Register A-P \n");
			return false;
		}
	p2 = strtok(NULL," ");
	srcreg = strtok(p2,",");
	src = srcreg[0]-'A';
	if(src > P){
			printf("Source register should be from Register A-P \n");
			return false;
		}
	printf("Dest = %s\tIntr Reg = %s\tSrc reg = %s\n",destreg,intreg,srcreg);
	/*****************************ADDITION**************************/
	if (strcasecmp(p1, "ADD")==0) {
		reg[dest] = (*fun_ptr_add)(reg[inti],reg[src]);
		printf("Result %d\n", reg[dest]);
		pc = pc + 4;
		print_values();			
	}
		/************************SUBTRACTION***************************/
		else if(strcasecmp(p1, "SUB")==0)
		{		
		reg[dest] = (*fun_ptr_sub)(reg[inti],reg[src]);
		printf("Result %d\n", reg[dest]);
		pc = pc + 4;
		 print_values();		
		}
		/************************MULTIPLICATION***************************/
		else if(strcasecmp(p1, "MUL")==0)
		{
		reg[dest] = (*fun_ptr_mul)(reg[inti],reg[src]);
		printf("Result %d\n", reg[dest]);
		pc = pc + 4;
		print_values();
		}
		/************************DIVISION***************************/
		else if(strcasecmp(p1, "DIV")==0)
		{	
		reg[dest] = (*fun_ptr_division)(reg[inti],reg[src]);
		printf("Result %d\n", reg[dest]);
		pc = pc + 4;
		print_values();
		}
		/************************MOD***************************/
		else if(strcasecmp(p1, "MOD")==0)
		{
		reg[dest] = (*fun_ptr_mod)(reg[inti],reg[src]);
		printf("Result %d\n", reg[dest]);
		pc = pc + 4;
		print_values();
	}
	return true;
}
/*****************************************ADDITION FUNCTION*****************************************/
int add(int inti, int src){
	//opcode = memory[pc];
	//dest = memory[pc+1];
	//inti =  memory[pc+2];
	//src = memory[pc+3];
	int carry;
	temp_reg[0] = src;
	temp_reg[1] = inti;
	while(temp_reg[0]!=0){
		carry = temp_reg[1] & temp_reg[0];
		temp_reg[1] = temp_reg[1] ^ temp_reg[0];
		temp_reg[0] = carry << 1;	
	}
	
	if((src>=0 && inti>=0 && temp_reg[1]<0) || (src<0 && inti<0 && temp_reg[1]>0)){  //Overflow criteria
		flags[3] = true; 
	}
	else{
		flags[3] = false;
	}
	if(carry == 1){      //Carry Criteria
		flags[0] = true;
	}
	else{
		flags[0] = false;
	}
	if(temp_reg[1]<0){    //Sign Flag criteria
		flags[2] = true;
	}
	else{
		flags[2] = false;
	}
	if(temp_reg[1]==0){    //Zero Flag criteria
		flags[1] = true;
	}
	else{
		flags[1] = false;
	}
	return temp_reg[1];
}
/*****************************************SUBTRACTION FUNCTION*****************************************/
int sub(int inti, int src){
	printf("Performing Subtraction \n");	
	temp_reg[4] = src;
	temp_reg[5] = inti; //2'S complement
	temp_reg[4] = (*fun_ptr_add)(~temp_reg[4], 1);
	temp_reg[4] = (*fun_ptr_add)(temp_reg[4], temp_reg[5]);
	return temp_reg[4];
}

/*****************************************MULTIPLICATION FUNCTION*****************************************/
int mul(int inti, int src){
	printf("Performing Multiplication Operation\n");
	int result = 0;
	temp_reg[2] = src;
	temp_reg[3] = inti;
	while (temp_reg[2] != 0)
	{
		if ((temp_reg[2] & 01) != 0)
		{
		result = (*fun_ptr_add)(result,temp_reg[3]);
		}
	temp_reg[3] <<= 1;
	temp_reg[2] >>= 1;
	}
	return result;
}

/*****************************************DIVISION FUNCTION*****************************************/
int division(int inti, int src){
	//printf("Performing Division");
	temp_reg[5] = inti;
	temp_reg[6] = src;
	int   result = 0 ,sign = 0;        
  if (temp_reg[5]< 0 )
  {          
    temp_reg[5]=-temp_reg[5];          
    sign ^= 1;      
  }             
  if (temp_reg[6]< 0 )
  {          
    temp_reg[6]=-temp_reg[6];          
    sign ^= 1;      
  }        
  if (temp_reg[6]!= 0 )
  {   
    printf("dividing");             
    while (temp_reg[5]>=temp_reg[6])
	{              
      temp_reg[5] = (*fun_ptr_sub)(temp_reg[5],temp_reg[6]);  	    
	  printf("temp_reg[5] afteR sub %d",temp_reg[5]); 
      result++;          
    }      
  }     
   
  if (sign)
  {         
    result=-result;      
    
  }
  	if(result < 0){    //Sign Flag criteria
		flags[2] = true;
	}
	else{
		flags[2] = false;
	}
	if(result == 0){    //Zero Flag criteria
		flags[1] = true;
	}
	else{
		flags[1] = false;
	}
  printf(" rem = %d \n",temp_reg[0]);   
  return   result; 
}

/*****************************************MOD FUNCTION*****************************************/
int mod(int inti, int src){
temp_reg[7] = inti;
temp_reg[8] = src;
int   c= 0 ,sign = 0;        
  if (temp_reg[7]< 0 )
  {          
    temp_reg[7]=-temp_reg[7];          
    sign ^= 1;      
  }             
  if (temp_reg[8]< 0 )
  {          
    temp_reg[8]=-temp_reg[8];          
    sign ^= 1;      
  }        
  if (temp_reg[8]!= 0 )
  {   
              
    while (temp_reg[7]>=temp_reg[8])
	{              
    temp_reg[7] = (*fun_ptr_sub)(temp_reg[7],temp_reg[8]);  	    
	  
      c++;          
    }      
  }     
   
  if (sign)
  {         
    c=-c;      
    
  }
  return temp_reg[7];
  }


/*****************************************LOAD AND STORE FUNCTION*****************************************/
bool load_and_store(){
	printf("1. Format for LOAD operation: LOAD dest_reg,offset(src_reg) Eg: LOAD A,30(RB,RI,4) \n");
	printf("2 .Format for STORE opcode: STORE dest_reg,offset(src_reg) Eg: STORE A,30(B) \n"); 
	char *p2 = NULL,*p3 = NULL,*p4 = NULL;
	char instruction[20];
	int s,base,index;
	fgets(instruction,20,stdin);
	p1 = strtok(instruction, " ");
	if (strcasecmp(p1, "LOAD")==0) {
		opcode = LOAD_OPCODE;
		p2 = strtok(NULL," ");
//			printf("p2 = %s\n",p2);          		
		destreg = strtok(p2,",");
//			printf("DESTreg = %s\n",destreg);
		dest = destreg[0] - 'A';      /*Index for Destination register*/
		if(dest > P){
			printf("Destination registers should be from Register A-P \n");
			return false;
		}	      
              	p2 = strtok(NULL,"");
//			printf("p2 = %s\n",p2);
	      		
		p2 = strtok(p2,"(");
//			printf("p2 = %s\n",p2); 			
		if(p2==NULL){
			offset = 0;
		}
		else{
			offset = atoi(p2);
		}
//			printf("offset = %d\n",offset);

		p3 = strtok(NULL,"(");
//			printf("p3 BEFORE = %s\n",p3);
		if(p3 == NULL){
			if(p2==NULL){

			}
			else{
				p3=p2;
			}
		}
//			printf("p3 AFTER= %s\n",p3);
		p4 =strtok(p3,",)");
//			printf("%s\n",p4);
		if(p4 == NULL){
			basereg = NULL;
		}
		else{
			basereg = p4;
			base = basereg[0] - 'A';     /*Index for source register*/
			if(base > P){
				printf("Base registers should be from Register A-P \n");
				return false;	
			}
		}			
//			printf("basereg = %s\n",basereg);
		p2 = strtok(NULL,"");
//			printf("p2 = %s\n",p2);
		indexreg =  strtok(p2,",)");
//			printf("indexreg = %s\n",indexreg);
		if(indexreg == NULL){
				
		}
		else{
			index = indexreg[0] - 'A';     /*Index for source register*/
			if(index > P){
				printf("Index registers should be from Register A-P \n");
				return false;
			}
		}			
		p2 = strtok(NULL,")");
//			printf("p2 = %s\n",p2);
		if(p2==NULL){
			s = 0;
		}
		else{
			s = atoi(strtok(p2,")"));
		}
		//printf("s = %d\n",s);
		//printf("reg[base] = %d\treg[index] = %d\ts = %d\n",reg[base],reg[index],s);
		part_address = reg[base]+s*reg[index];
		memory[INSTR_MEMORY_BASE_ADD + i] = opcode;
		memory[INSTR_MEMORY_BASE_ADD + i+1] = dest;
		memory[INSTR_MEMORY_BASE_ADD + i+2] = offset; 
		memory[INSTR_MEMORY_BASE_ADD + i+3] = part_address; 
		i = i+4;
		result = execute_load();
		if (result !=0){
			printf("Error in executing Load Instruction \n");
			return false;
		}
		result = print_values();
		if (result !=0){
			printf("Error in printing values \n");
			return false;
		}	      	
	}
	else if (strcasecmp(p1, "STORE")==0) {
		opcode = STORE_OPCODE;
		p2 = strtok(NULL," ");
	     	srcreg = strtok(p2,",");
		src = srcreg[0] - 'A';      /*Index for Destination register*/
		if(src > P){
			printf("Destination registers should be from Register A-Q \n");
			return false;
		}
		p2 = strtok(NULL,"");
//			printf("p2 = %s\n",p2);
	      		
		p2 = strtok(p2,"(");
//		printf("p2 = %s\n",p2); 			
		if(p2==NULL){
			offset = 0;
		}
		else{
			offset = atoi(p2);
		}
//			printf("offset = %d\n",offset);

	        p3 = strtok(NULL,"(");
//			printf("p3 BEFORE = %s\n",p3);
		if(p3 == NULL){
			if(p2==NULL){

			}
			else{
				p3=p2;
			}
		}
//			printf("p3 AFTER= %s\n",p3);
		p4 =strtok(p3,",)");
//			printf("%s\n",p4);
		if(p4 == NULL){
			basereg = NULL;
		}
		else{
			basereg = p4;
			base = basereg[0] - 'A';     /*Index for source register*/
			if(base > P){
				printf("Base registers should be from Register A-P \n");
				return false;	
			}
		}			
//			printf("basereg = %s\n",basereg);
		p2 = strtok(NULL,"");
//			printf("p2 = %s\n",p2);
		indexreg =  strtok(p2,",)");
//			printf("indexreg = %s\n",indexreg);
		if(indexreg == NULL){
				
		}
		else{
			index = indexreg[0] - 'A';     /*Index for source register*/
			if(index > P){
				printf("Index registers should be from Register A-P \n");
				return false;
			}
		}			
		p2 = strtok(NULL,")");
//			printf("p2 = %s\n",p2);
		if(p2==NULL){
			s = 0;
		}
		else{
			s = atoi(strtok(p2,")"));
		}
//			printf("s = %d\n",s);
//			printf("reg[base] = %d\treg[index] = %d\ts = %d\n",reg[base],reg[index],s);
		part_address = reg[base]+s*reg[index];
		memory[INSTR_MEMORY_BASE_ADD + i] = opcode;
		memory[INSTR_MEMORY_BASE_ADD + i+1] = src;
		memory[INSTR_MEMORY_BASE_ADD + i+2] = offset; 
		memory[INSTR_MEMORY_BASE_ADD + i+3] = part_address; 
		i = i+4;              		
		result = execute_store();
		if (result !=0){
			printf("Error in executing Store Instruction \n");
			return false;
		}
		result = print_values();
		if (result !=0){
			printf("Error in printing values \n");
			return false;
		}
		return true;
	}
	else {
		printf("Enter valid instructions \n");
		return false;
	}	
	return true;
}

/****************Function for LOAD Instruction *********************************************/
/*	On executing LOAD destreg,offset(srcreg) instruction, the address or 
	value present in source register is added with offset which will result 
	into final address. The value present in final address is then loaded in 
	destination register.*/
/*******************************************************************************************/
int execute_load(){
	opcode = memory[pc];
	dest = memory[pc+1];
	offset =  memory[pc+2];
	part_address = memory[pc+3];
	address = part_address + offset;
//	printf("part_address = %d\t full address = %d\n",part_address,address);
	if(address >= DATA_MEMORY_BASE_ADD && address < MEMORY_SIZE){
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
	part_address = memory[pc+3];
	address = part_address + offset;
//	printf("part_address = %d\t full address = %d\n",part_address,address);
	if(address >= DATA_MEMORY_BASE_ADD && address < MEMORY_SIZE){
		memory[address] = reg[dest];
		pc = pc + 4;
		printf("STORE Instruction executed successfully\n\n\n");
		return 0;
	}
	else{ 
		printf("Invalid location\n");
	}
}


int main(){
	bool res = true;
	int option;
	char char_option[5];
	init_memory();
	reg[0] = 1024;	/*Initialized Reg A = 1024 */
	reg[1] = 30;	/*Initialized Reg B = 1024 */
	reg[2] =-103;
	reg[3] =-69;
	reg[4] = 2147483647;
	reg[5] = 100;
	result = print_values();
	while(pc < MEMORY_SIZE){
		while(res == true){
			printf("Enter instructions number from the menu\n");
			printf("****************Instructions Menu****************\n");
			printf("1. Load/Store Instruction\n");
			printf("2. ALU operations - ADD/SUB/MUL/DIV/MOD\n");
			printf("3. EXIT\n");
			fgets(char_option,5,stdin);
			sscanf(char_option,"%d",&option);
			//option = option - '0';
			//printf(" %d\n",option);
			switch(option){
				case 1:
					res = load_and_store();
					printf("\nDone\n");
					break;
				case 2:
					res = alu_operations();
					break;
				case 3:
					res = false;
					break;
			}

		}

	//	printf("\nDone\n");	
	}
}
