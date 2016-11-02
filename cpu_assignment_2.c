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
#define location_recursive_binary_search 500
#define LOOP_ADDRESS 900 /* Looping function */
#define STORE_OPCODE 3		/* Opcode for STORE instruction */
#define LOAD_OPCODE 4		/* Opcode for LOAD instruction */
#define ADD_OPCODE 5
#define SUB_OPCODE 6
#define DIV_OPCODE 7
#define MUL_OPCODE 8
#define MOD_OPCODE 9
#define CMPQ_OPCODE 10
#define ADDQ_OPCODE 11
#define SETE_OPCODE 12
#define SETS_OPCODE 13
#define SETNS_OPCODE 14
#define SETL_OPCODE 15
#define SETLE_OPCODE 16
#define SETG_OPCODE 17
#define JMP_OPCODE 18
#define PUSH_OPCODE 19
#define POP_OPCODE 20

/******add opcodes*****************************************************************************************************/


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
bool load_store_and_lea();
bool alu_operations();
bool cond_codes();
bool jump_instructions();
bool binary_search();
void init_memory();
int execute_load();
int execute_store();
int execute_lea();
int print_values();
int add(int inti, int src);
int sub(int inti, int src);
int mul(int inti, int src);
int division(int inti, int src);
int mod(int inti, int src);
int recursive_binary_search(int, int, int);
int rem =0;
int addq(int src,int dest);
int cmpq(int src,int dest);
int sete();
int sets();
int setns();
int setl();
int setle();
int setg();
void push(int);
int pop();
int looping(int dest,int count,int max);
bool loops();
int jump(int loc);
int loopingwhile(int dest,int max);

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
	printf("MDR:%d\n",memory[address]);
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
 /****************************************Jump Label*******************************************/
  int jump(int loc)
  {
  	pc = loc;
  	return pc;
  }


/********************************************************************************************
This is the main function for binary search where it asks user to enter elements in ascending 
order and the element to be searched. Later it calls recursive_binary_search function which 
returns the index for the searched element and displays the position og found element.
**********************************************************************************************/
bool binary_search(){
	printf("Enter number of elements for binary search\n");
	char element_char[5];
	int element_int,element;
	fgets(element_char,5,stdin);
	sscanf(element_char,"%d",&element_int);
	int c;
	int key,index;
	temp_reg[12] = 1900; 
	printf("Enter elements in ascending order. Press enter after each element!!\n");
	scanf("%d",&element);
	memory[temp_reg[12]] = element;
	temp_reg[12] = temp_reg[12] + 1;
	for(c=1;c<element_int;c++){
		scanf("%d",&element);
		memory[temp_reg[12]] = element;
		if(memory[temp_reg[12]-1]>memory[temp_reg[12]]){
			printf("Elements entered are not in ascending order!!\n");
			exit(0);
		}
		temp_reg[12] = temp_reg[12] + 1;
	}
	
        temp_reg[12] = 1900;
	printf("The Elements are \n");
	for(c=0;c<element_int;c++){
		printf("%d  ",memory[temp_reg[12]]);
		temp_reg[12] = temp_reg[12] + 1;
	}
	printf("\n");
	//printf("%d\n",temp_reg[12]);

	
	printf("Enter the element to be searched\n");
	scanf("%d",&key);
	push(pc);										//PUSH PC
	//pc = 500;		//RECURSIVE BINARY SEARCH FUNCTION IS AT 500th LOCATION
	//temp_reg[12] = temp_reg[12] - 1;
	jump(location_recursive_binary_search);							//JUMP 500
	temp_reg[16] = recursive_binary_search(1900,temp_reg[12],key);  			//CALL 
	temp_reg[16] = sub(temp_reg[16],1900);	
	if(temp_reg[16]<0){
		printf("****ELEMENT NOT FOUND****\n");
	}
	else{
		reg[7] = add(temp_reg[16],1);							//ADD R7,TR16,1 
		pc = pc +4; 		
		printf("ELEMENT FOUND AT POSITION %d\n",reg[7]);
	}	
	printf("\n\n");
	//print_values();
	pc = pop();										//POP PC
	print_values();
	return true;

}

void push(pc){
	memory[sp] = pc+4;
	sp = sub(sp,1);										//SUB SP,SP,1
	printf("stack in push %d\n", sp);		
}
int pop(){
	sp = add(sp,1);										//ADD SP,SP,1
	printf("stack in pop %d\n", sp);
	return memory[sp];

}


/********************************************************************************************
Here we have implemented Recursive Binary Search along with Stack functions.
This function searches an element from a given set of elements stored in memory using binary 
search and recursively calls the function till it finds an element. If found, it displays the 
index of the element present in array of element. If not found then, it will display element not
found. 
Stack operations are also performed like PUSH and POP functions.
**********************************************************************************************/
int recursive_binary_search(int start,int end,int key){
	temp_reg[13] = start;
	temp_reg[14] = end;
	temp_reg[15] = key;
	//printf("Start PC = %d\n",pc);
	cmpq(temp_reg[13],temp_reg[14]);							//CMPQ TR13,TR14
	//printf("%d>=%d\n",temp_reg[14],temp_reg[13]);
	pc = pc+4;
	//printf("PC before IF= %d\n",pc);
	if(flags[1] == 1 || flags[2] == 1){						
		//int temp_reg[16];
		temp_reg[16] = sub(temp_reg[14],temp_reg[13]);					//SUB TR16,TR14,TR13
		//printf("temp_reg[16] after sub =  %d\n",temp_reg[16]);
		pc = pc+4;
		//printf("PC = %d\n",pc);
		temp_reg[16] = division(temp_reg[16],2);					//DIV TR16,TR16,2
		//printf("temp_reg[16] after division = %d\n",temp_reg[16]);
		pc = pc+4;
		//printf("PC = %d\n",pc);
		temp_reg[16] = add(temp_reg[13],temp_reg[16]);					//ADD TR16,TR13,TR16
		//printf("temp_reg[16] after add = %d\n",temp_reg[16]);
		pc = pc+4;
		//printf("PC = %d\n",pc);
		cmpq(memory[temp_reg[16]],temp_reg[15]);					//CMPQ TR16,TR15
		pc = pc+4;
		//printf("PC after cmpq= %d\n",pc);
		if (flags[1]==1){
		//	printf("%d==%d\n",memory[temp_reg[16]],temp_reg[15]);
			return temp_reg[16];							//RET
		}
		else if(flags[2]==1){
			temp_reg[13] = add(temp_reg[16],1);					//ADD TR13,TR16,1
			pc = pc+4;
		//	printf("%d<%d\n",memory[temp_reg[16]],temp_reg[15]);
			printf("SP before push= %d\n",sp);
			push(pc);								//PUSH 
			jump(location_recursive_binary_search);					//JUMP 500
			pc = pop();								//POP
			printf("SP after pop= %d\n",sp);
			return recursive_binary_search(temp_reg[13],temp_reg[14],temp_reg[15]);	//CALL & RET 
			
		}
		else{
		//	printf("%d>%d\n",memory[temp_reg[16]],temp_reg[15]);
			temp_reg[14] = sub(temp_reg[16],1);					//SUB TR14,TR16,1
			pc = pc +4;
			push(pc);								//PUSH
			jump(location_recursive_binary_search);					//JUMP 500
			printf("SP before push= %d\n",sp);
			pc = pop();								//POP
			printf("SP after pop= %d\n",sp);
			return recursive_binary_search(temp_reg[13],temp_reg[14],temp_reg[15]); //CALL & RET
			
		}
		
	}
	return -1;
}



/********************************************************************************************
Below are set of condition codes where it sets the register to 1 based on the flag.
For Eg. SETS A  will set the register to 1 if Signed Flag is 1. 
**********************************************************************************************/
bool cond_codes(){
	printf("1. Format for SETS operation: SETS dest Eg: SETS A\n");
	printf("2. Format for SETNS operation: SETNS dest Eg: SETNS A\n");
	printf("3. Format for SETL operation: SETL dest Eg: SETL A\n");	
	printf("4. Format for SETLE operation: SETLE dest Eg: SETLE A\n");
	printf("5. Format for SETG operation: SETG dest Eg: SETG A\n");
	printf("6. Format for SETE operation: SETE dest Eg: SETE A\n");		
	char instruction[20];
	int len;
	char *p2 = NULL;
	fgets(instruction,20,stdin);
	len = strlen(instruction);
	instruction[len-1] = '\0';
	p1 = strtok(instruction, " ");
	//printf("%s\n",p1);
	p2 = strtok(NULL," ");
	destreg = strtok(p2,",");
	dest = destreg[0]-'A';
	if(dest > P){
			printf("Destination register should be from Register A-P \n");
			return false;
		}
	//printf("Dest = %s\n",destreg);
/**********************************************************************************************/
	if (strcasecmp(p1, "SETE")==0) {
		reg[dest] = sete();
		//printf("Result %d\n", reg[dest]);
		pc = pc + 4;
		print_values();	
		return true;		
	}
	else if (strcasecmp(p1, "SETS")==0) {
		reg[dest] = sets();
		//printf("Result %d\n", reg[dest]);
		pc = pc + 4;
		print_values();	
		return true;		
	}
	else if (strcasecmp(p1, "SETNS")==0) {
		reg[dest] = setns();
		//printf("Result %d\n", reg[dest]);
		pc = pc + 4;
		print_values();
		return true;			
	}
	else if (strcasecmp(p1, "SETL")==0) {
		reg[dest] = setl();
		//printf("Result %d\n", reg[dest]);
		pc = pc + 4;
		print_values();	
		return true;		
	}	
	else if (strcasecmp(p1, "SETLE")==0) {
		reg[dest] = setle();
		//printf("Result %d\n", reg[dest]);
		pc = pc + 4;
		print_values();		
		return true;	
	}
	else if (strcasecmp(p1, "SETG")==0) {
		reg[dest] = setg();
		//printf("Result %d\n", reg[dest]);
		pc = pc + 4;
		print_values();
		return true;			
	}
/*	else if (strcasecmp(p1, "COMPQ")==0) {
		reg[dest] = compq();
		//printf("Result %d\n", reg[dest]);
		pc = pc + 4;
		print_values();
		return true;			
	}*/
	else{
		return false;
	}
}


int sete(){
	if(flags[1] == true){

		return 1;
	}
	else{
		return 0;
	}
}

int sets(){
	printf("Sign Flag = %d\n",flags[2]);
	if(flags[2] == true){
		return 1;
	}
	else{
		return 0;
	}
}

int setns(){
	printf("Sign Flag = %d\n",flags[2]);
	if(flags[2] == false){
		return 1;
	}
	else{
		return 0;
	}
}

int setl(){
	if(flags[2]!=flags[3]){
		return 1;
	}
	else{
		return 0;
	}
}

int setg(){
	if((!(flags[2]!=flags[3]))&&(!flags[1])){
		return 1;
	}
	else{
		return 0;
	}
}

int setle(){
	if((flags[2]!=flags[3])||(flags[1])){
		return 1;
	}
	else{
		return 0;
	}
}
/********************************************************************************************
Conditional codes with two operands.
/********************************************************************************************/
bool condcode_two(){
printf("1. Format for ADDQ operation: ADDQ src,dest Eg: ADDQ A,B\n");
printf("2. Format for CMPQ operation: CMPQ src,dest Eg: CMPQ A,B\n");
char instruction[20];
	int len;
	char *p2 = NULL;
	fgets(instruction,20,stdin);
	len = strlen(instruction);
	instruction[len-1] = '\0';
	p1 = strtok(instruction, " ");
	p2 = strtok(NULL," ");
	srcreg = strtok(p2,",");
	src = srcreg[0]-'A';
	if(src > P){
			printf("Source register should be from Register A-P \n");
			return false;
		}
	p2 = strtok(NULL," ");
	destreg = strtok(p2,",");
	dest = destreg[0]-'A';
	if(dest > P){
			printf("Destination register should be from Register A-P \n");
			return false;
		}
	printf("Dest = %s\t Src reg = %s\n",destreg,srcreg);
	/*****************************ADDITION**************************/
		if (strcasecmp(p1, "ADDQ")==0) {
			reg[dest] = addq(reg[src],reg[dest]);
			printf("Result %d\n", reg[dest]);
			pc = pc + 4;
			print_values();	
			return true;		
		}
		else if (strcasecmp(p1, "CMPQ")==0) {
			reg[dest] = cmpq(reg[src],reg[dest]);
			printf("Result %d\n", reg[dest]);
			pc = pc + 4;
			print_values();	
			return true;		
		}
		else
		{
		return false;
		}
}
/**************************************ADDQ*******************************************************/
int addq(int src, int dest){
	temp_reg[9] = src;
	temp_reg[10] = dest;
	temp_reg[11] = add(temp_reg[9],temp_reg[10]);
	if(temp_reg[11] == 0)
	{
	printf("Zero Flag is set\n");
		flags[1] = 1;
	}
	if(temp_reg[11] < 0)
	{
	printf("Sign Flag is set\n");
	flags[2] = 1;
	}
	if((temp_reg[9]>0 && temp_reg[10]>0 && temp_reg[11]<0)||(temp_reg[9]<0 && temp_reg[10]<0) && temp_reg[10]>=0)
	{
	printf("Overflow Flag is set\n");
		flags[3] = 1;
	}
	return temp_reg[11];
}

/**************************************CMPQ*******************************************************/
int cmpq(int src, int dest){
	temp_reg[9] = src;
	temp_reg[10] = dest;
	temp_reg[11] = sub(temp_reg[9],temp_reg[10]);
	if(temp_reg[9] == temp_reg[10])
	{
	//printf("Zero Flag is set\n");
		flags[1] =1;
	}
	if(temp_reg[11] < 0)
	{
	//printf("Sign Flag is set\n");
		flags[2] = 1;
	}
	if((temp_reg[9]>0 && temp_reg[10]<0 && temp_reg[11]<0)||(temp_reg[9]<0 && temp_reg[10]>0) && temp_reg[10]>0)
	{
	//printf("Overflow Flag is set");
		flags[3] = 1;
	}
	return temp_reg[11];
}

/*********************************************************************************************
Simple Function which calls the respective instruction. A function pointer is implemented 
to point the functions like add, sub, mul, division and mod.
/********************************************************************************************/

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
			return true;		
		}
		/************************SUBTRACTION***************************/
		else if(strcasecmp(p1, "SUB")==0)
		{		
			reg[dest] = (*fun_ptr_sub)(reg[inti],reg[src]);
			printf("Result %d\n", reg[dest]);
			pc = pc + 4;
			print_values();
			return true;		
		}
		/************************MULTIPLICATION***************************/
		else if(strcasecmp(p1, "MUL")==0)
		{
			reg[dest] = (*fun_ptr_mul)(reg[inti],reg[src]);
			printf("Result %d\n", reg[dest]);
			pc = pc + 4;
			print_values();
			return true;
		}
		/************************DIVISION***************************/
		else if(strcasecmp(p1, "DIV")==0)
		{	
			reg[dest] = (*fun_ptr_division)(reg[inti],reg[src]);
			printf("Result %d\n", reg[dest]);
			pc = pc + 4;
			print_values();
			return true;
		}
		/************************MOD***************************/
		else if(strcasecmp(p1, "MOD")==0)
		{
			reg[dest] = (*fun_ptr_mod)(reg[inti],reg[src]);
			printf("Result %d\n", reg[dest]);
			pc = pc + 4;
			print_values();
			return true;
		}
		else{
		return false;
		}
	return true;
}

/***********************************************************************************************************
Loops
For loop Increments till the maximum number is reached by the counter.
dowhile loop Increments if the source register is less than than the Comparing register or equal if it is
greater nothing is done.
whiledo loop increments if the source register is less than than the Comparing register if it is  equal and
greater nothing is done.
*********************************************************************************************************/
bool loops()
{
	printf("1. Format for For operation: to increment a number by one till the max is reached For Eg: for B,number\n");
	printf("2. Format for Do..While operation: it increments once without checking condition then it checks the condition to increment for Eg: dowhile B,number \n");
	printf("3. Format for While..do operation: it increments after the condition is true for Eg: whiledo B,number\n");
	char instruction[20];
	int len;
	char *p2 = NULL;
	fgets(instruction,20,stdin);
	len = strlen(instruction);
	instruction[len-1] = '\0';
	p1 = strtok(instruction, " ");
	p2 = strtok(NULL," ");
	srcreg = strtok(p2,",");
	src = srcreg[0]-'A';
	if(src > P){
			printf("Source register should be from Register A-P \n");
			return false;
		}
	p2 = strtok(NULL," ");
	destreg = strtok(p2,",");
	dest = destreg[0]-'A';
	if(dest > P){
			printf("Destination register should be from Register A-P \n");
			return false;
		}
	printf("Dest = %s\t Src destreg = %s\n",destreg,srcreg);
	if (strcasecmp(p1, "for")==0) {
			int count = 0;
			printf("dest register %d \n",reg[dest]);
			cmpq(count,reg[dest]);	
			pc = pc + 4;
			if(flags[2])
			{
				jump(LOOP_ADDRESS);	//jumps to particular location
				looping(reg[src],count,reg[dest]);
				printf("Result %d\n", reg[src]);
				print_values();	
				return true;
			}		
			else if(flags[1])
			{
				printf("Cannot increment as the value Max is equal to the count\n");
				print_values();	
				return true;
			}
		}
		else if (strcasecmp(p1, "dowhile") == 0)
		{
			cmpq(reg[src],reg[dest]);	
			pc = pc + 4;
			if(flags[1])
			{
				reg[src] = add(reg[src],1); //incrementing 
				pc = pc + 4;
				printf("Result %d\n", reg[src]);
				print_values();	
				return true;				
			}
			else if(flags[2])
			{
				jump(LOOP_ADDRESS);	//jumps to particular location
			 	loopingwhile(reg[src],reg[dest]);
				printf("Result %d\n", reg[src]);
				print_values();
				return true;
			}
			else if (!flags[1])
			{
				reg[src] = add(reg[src],1); //incrementing 
				pc = pc + 4;
				printf("Result %d\n", reg[src]);
				print_values();	
				return true;	
			}
		}
		else if (strcasecmp(p1,"whiledo") == 0)
		{
			int count = 0;
			cmpq(reg[src],reg[dest]); 
			pc = pc + 4;
			if(flags[2])
			{
				jump(LOOP_ADDRESS);	//jumps to particular location
				loopingwhile(reg[src],reg[dest]);
				printf("Result %d\n", reg[src]);
				print_values();	
				return true;
			}	
			else if(flags[1])
			{
				printf("Values are equal so cannot increment \n");
				print_values();	
				return true;
			}
			else if (!flags[1])
			{
				printf("Cannot increment as the src is greater than the given number so cannot increment\n");
				print_values();	
				return true;
			}	
			
		}
	else {
		return false;
	}
}
/********************************************Looping*********************************************
Increments till the count reaches the max. this is for "for loop"
************************************************************************************************/
int looping(int dest,int count,int max)
{
	temp_reg[16] = dest;
	temp_reg[11] = 1;
	temp_reg[12] = count;
	temp_reg[13] = max;
	printf("max value %d",max);
	temp_reg[16]= add(temp_reg[16],temp_reg[11]); // ADD R3,R1,R2
	pc = pc + 4;
	temp_reg[12] = add(temp_reg[12],temp_reg[11]); // ADD R3,R1,R2
	pc = pc + 4;
	cmpq(temp_reg[12],temp_reg[13]); 
	pc = pc + 4;
	if(flags[2])
	{
		pc = pc + 4;
		jump(900);
		looping(temp_reg[16],temp_reg[12],temp_reg[13]);		
	}
	reg[src] = temp_reg[16];
	return 0;
}
/********************************************LoopingWhile********************************************
Increments till the source equal to the destination. this is for "do while" and "while do"
************************************************************************************************/
int loopingwhile(int dest,int max)
{
	temp_reg[16] = dest;
	temp_reg[11] = 1;
	temp_reg[13] = max;
	printf("max value %d",max);
	temp_reg[16]= add(temp_reg[16],temp_reg[11]);	// ADD R3,R1,R2
	pc = pc + 4;
	cmpq(temp_reg[16],temp_reg[13]);	
	pc = pc + 4;
	if(flags[2]) //compare check sign
	{
		pc = pc + 4;
		jump(900);
		loopingwhile(temp_reg[16],temp_reg[13]);		
	}
	reg[src] = temp_reg[16];
	return 0;
}



/*****************************************ADDITION FUNCTION*****************************************
This function adds the content of inti and src and returns the value. The returned value is stored in 
destination register. All operations in the add function are bitwise operations.Thus making it a 3 
operand instruction. Also, flags like sign flag, carry flag, overflow flag and zero flag is implemented. 
/**************************************************************************************************/

int add(int inti, int src){
	int carry;
	temp_reg[0] = src;
	temp_reg[1] = inti;
	while(temp_reg[0]!=0){
		carry = temp_reg[1] & temp_reg[0];
		temp_reg[1] = temp_reg[1] ^ temp_reg[0];
		temp_reg[0] = carry << 1;	
	}
	
	if((src>=0 && inti>=0 && temp_reg[1]<0) || (src<0 && inti<0 && temp_reg[1]>=0)){  //Overflow criteria
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



/*****************************************SUBTRACTION FUNCTION*****************************************
This substracts the content of inti and src and returns the value. The returned value is stored in 
destination register. Thus making it a 3 operand instruction. Basically it performs 2's complement 
of the negative number and calls the addition function.
/******************************************************************************************************/
int sub(int inti, int src){
	//printf("Performing Subtraction \n");	
	temp_reg[4] = src;
	temp_reg[5] = inti; 
	temp_reg[4] = (*fun_ptr_add)(~temp_reg[4], 1);
	temp_reg[4] = (*fun_ptr_add)(temp_reg[4], temp_reg[5]);
	return temp_reg[4];
}



/*****************************************MULTIPLICATION FUNCTION*****************************************
This function multiplies the content of inti and src and returns the value. The returned value is stored in 
destination register. Thus making it a 3 operand instruction. Primarily, with the help of logical left shift, 
multiplication instruction is carried out.
/*********************************************************************************************************/
int mul(int inti, int src){
	//printf("Performing Multiplication Operation\n");
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

/*****************************************DIVISION FUNCTION*****************************************
This function divides the content of inti with src and returns the value. The returned value is stored in 
destination register. Thus making it a 3 operand instruction. 

/***************************************************************************************************/
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
   // printf("dividing");             
    while (temp_reg[5]>=temp_reg[6])
	{              
      temp_reg[5] = (*fun_ptr_sub)(temp_reg[5],temp_reg[6]);  	    
//	  printf("temp_reg[5] afteR sub %d",temp_reg[5]); 
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
 // printf(" rem = %d \n",temp_reg[0]);   
  return   result; 
}

/*****************************************MOD FUNCTION*****************************************
This function performs modulus operation on the content of inti with src and returns the value. 
The returned value is stored in destination register. Thus making it a 3 operand instruction. 

/**********************************************************************************************/
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


/*****************************************LOAD,STORE and LEA FUNCTION*****************************************/
bool load_store_and_lea(){
	printf("1. Format for LOAD operation: LOAD dest_reg,offset(src_reg) Eg: LOAD A,30(RB,RI,4) \n");
	printf("2 .Format for STORE opcode: STORE dest_reg,offset(src_reg) Eg: STORE A,30(B) \n"); 
	printf("3. Format for LEA operation: LEA dest_reg,offset(src_reg) EG: LEA A,30(RB,RI,4) \n");
	char *p2 = NULL,*p3 = NULL,*p4 = NULL;
	char instruction[20];
	int s,base,index;
	fgets(instruction,20,stdin);
	p1 = strtok(instruction, " ");
	if (strcasecmp(p1, "LOAD")==0) {
		opcode = LOAD_OPCODE;
		p2 = strtok(NULL," ");
       		
		destreg = strtok(p2,",");

		dest = destreg[0] - 'A';      /*Index for Destination register*/
		if(dest > P){
			printf("Destination registers should be from Register A-P \n");
			return false;
		}	      
              	p2 = strtok(NULL,"");

	      		
		p2 = strtok(p2,"(");
			
		if(p2==NULL){
			offset = 0;
		}
		else{
			offset = atoi(p2);
		}


		p3 = strtok(NULL,"(");

		if(p3 == NULL){
			if(p2==NULL){

			}
			else{
				p3=p2;
			}
		}

		p4 =strtok(p3,",)");

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

		p2 = strtok(NULL,"");

		indexreg =  strtok(p2,",)");
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

		if(p2==NULL){
			s = 0;
		}
		else{
			s = atoi(strtok(p2,")"));
		}
		printf("%d\n",s);
		if(s==1 || s==2 || s==4 || s==8){
		}
		else{
			printf("S should be 1,2,4 or 8\n");
			exit(0);
		}

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
	else if (strcasecmp(p1, "LEA")==0) {
		p2 = strtok(NULL,"");

		destreg = strtok(p2,",");

		dest = destreg[0] - 'A';

		if(dest > P) {
			printf("Destination registers should be from Register A-P \n");
			return false;
		}

        p2 = strtok(NULL,"");

        p2 = strtok(p2,"(");
			
		if(p2==NULL){
			offset = 0;
		}
		else{
			offset = atoi(p2);
		}

		p3 = strtok(NULL,"(");

		if(p3 == NULL){
			if(p2==NULL){

			}
			else{
				p3=p2;
			}
		}

		p4 =strtok(p3,",)");

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

		p2 = strtok(NULL,"");

		indexreg =  strtok(p2,",)");

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

		if(p2==NULL){
			s = 0;
		}
		else{
			s = atoi(strtok(p2,")"));
		}
		if(s==0 || s==1 || s==2 || s==4 || s==8){
		}
		else{
			printf("S should be 1,2,4 or 8\n");
			exit(0);
		}

		part_address = reg[base]+s*reg[index];

		memory[INSTR_MEMORY_BASE_ADD + i] = opcode;
		memory[INSTR_MEMORY_BASE_ADD + i+1] = dest;
		memory[INSTR_MEMORY_BASE_ADD + i+2] = offset; 
		memory[INSTR_MEMORY_BASE_ADD + i+3] = part_address; 
		i = i+4;

		result = execute_lea();
		if (result !=0){
			printf("Error in executing Lea Instruction \n");
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

	      		
		p2 = strtok(p2,"(");
			
		if(p2==NULL){
			offset = 0;
		}
		else{
			offset = atoi(p2);
		}


	        p3 = strtok(NULL,"(");

		if(p3 == NULL){
			if(p2==NULL){

			}
			else{
				p3=p2;
			}
		}

		p4 =strtok(p3,",)");

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

		p2 = strtok(NULL,"");

		indexreg =  strtok(p2,",)");

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

		if(p2==NULL){
			s = 0;
		}
		else{
			s = atoi(strtok(p2,")"));
		}
		if(s==1 || s==2 || s==4 || s==8){
		}
		else{
			printf("S should be 1,2,4 or 8\n");
			exit(0);
		}

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

/****************Function for LEA Instruction *********************************************/
/*	On executing LEA destreg,offset(srcreg) instruction, the address or 
	value present in source register is added with offset which will result 
	into final address. The final address is then loaded directly into the 
	destination register.*/
/*******************************************************************************************/
int execute_lea() {

	opcode = memory[pc];
	dest = memory[pc+1];
	offset = memory[pc+2];

	part_address = memory[pc+3];
	address = part_address + offset;

	if(address >= INSTR_MEMORY_BASE_ADD && address < MEMORY_SIZE){
	reg[dest] = address;
	pc = pc+4;
	printf("LEA Instruction executed successfully\n\n\n");
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
	printf("part_address = %d\t full address = %d\n",part_address,address);
	if(address >= DATA_MEMORY_BASE_ADD && address < MEMORY_SIZE){
		memory[address] = reg[src];
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
			printf("1. Load/Store/LEA Instruction\n");
			printf("2. ALU operations - ADD/SUB/MUL/DIV/MOD\n");
			printf("3. Condition Codes - setne,sete,setle,setg,sets,setns,compq\n");
			printf("4. Condition Codes - compq,addq\n");
			printf("5. Perform Recursive Binary Search with Stack operations\n");
			printf("6. Loops, for, do..while, while..do implemented using jump instructions\n");
			printf("7. EXIT\n");
			fgets(char_option,5,stdin);
			sscanf(char_option,"%d",&option);
			//option = option - '0';
			//printf(" %d\n",option);
			switch(option){
				case 1:
					res = load_store_and_lea();
					if(res == false){
						printf("Wrong Instruction\n");
						exit(-1);
					}
					break;
				case 2:
					res = alu_operations();
					if(res == false){
						printf("Wrong Instruction\n");
						exit(-1);
					}
					break;			
				case 3: 
					res = cond_codes();
					if(res == false){
						printf("Wrong Instruction\n");
						exit(-1);
					}
					break;
				case 4:
					res = condcode_two();
					if(res == false){
						printf("Wrong Instruction\n");
						exit(-1);
					}
					break;
				
				case 5:
					res = binary_search();
					if(res == false){
						printf("Wrong Instruction\n");
						exit(-1);
					}
					exit(0);
				case 6:
					res = loops();
					if(res == false){
						printf("Wrong Instruction\n");
						exit(-1);
					}
					break;
					
				case 7:
					res = false;
					exit(-1);
				default:
					break;
			}

		}	
	}
}

