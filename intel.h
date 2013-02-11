/* $Revision: 1.2 $

		INTEL Header File

	This file holds the machine specific data for the intel code.

		P.Antoine.
					*/

#ifndef	__INTEL_DATA
#define	__INTEL_DATA

#include	"cople.h"
#include	"codetree.h"

#define	MAX_VIRTUAL_REGISTERS	100

typedef enum {eax,ebx,ecx,esi,edi,edx} REGS;
static char*	INTEL_SIZE[]	= {" ","byte","byte","dword","dword","dword","byte"};
static char*	INTEL_REG[]	= {"eax","ebx","ecx","esi","edi","edx"};
static char*	INTEL_SEG[]	= {"fs","gs","es"};

static char*	INTEL_INSTR[] =	{"nop",
		"and",
		"or",
		"xor",
		"add",
		"sub",
		"inc",
		"not",
		"dec",
		"imul",
		"idiv",
		"idiv",
		"mov",
		"mov",
		"push",
		"pop",
		"call",
		"ret",
		"call",
		"ret",
		"set",
		"g",
		"ge",
		"ne",
		"e",
		"l",
		"le",
		"loop",
		"jmp",
		"jne",
		"je",
		"jlt",
		"jle",
		"lg",
		"jge"}; 


typedef	struct{
	unsigned	dynamic:1,	/* if the operand is dymamic */
		immediate:1,	/* operand is just a value */
		pointer:1,	/* the operand is a pointer */
		use_reg:1,	/* the value is a register */
		label:1,	/* the operand is a label */
		parameter:1,	/* the operand is a parameter */
		reference:1,	/* the value is a reference to memeory */
		use_base:1,	/* the base is in use */
		null:1;	/* this operand is nothing */
	} STATE;

typedef	struct{
	int	last_used;	/* the last operand to access this register */
	int	reg_no;	/* the register number that has it */
	int	base;	/* the base of the reference */
	unsigned long	value;	/* the immidate value or label number */
	DATA_TYPE	type;	/* the datatype and INSTR_SIZE are in step */
	STATE	state;	/* the state of the operand */
	} INTEL_OPERAND;

typedef	struct{
	int	reg_no;	/* the real register in use */
	int	save_space;	/* the place the register is stored in */
	unsigned	saved:1,	/* if register then it has been pushed */
		inuse:1;	/* if the register is inuse */
	} VIRT_REGISTER;

typedef	struct{
	int	inuse;	/* is the real register allocated */
	int	last_used;	/* last operand accessed */
	int	virt_reg_no;	/* the virtual operand that has it */
	} REAL_REGISTER;

typedef	struct{
	int	last_used;	/* the last operand number that accessed this segment register */
	unsigned long	used_by;	/* the dynamic element that in the register */
	} SEG_REGISTER;  

/* Functions that generate the INTEL specific code: this code is based
 * on 486 code and instructions.
 */

void	generate_intel_code(CLAUSE*,LEVEL_LNK*);		/* controls the intel code generation */
void	gi_generate_data(void);			/* generate the dbs for the intialised data */
void	allocate_register(INTEL_OPERAND*);		/* will allocate a register to a virtual register */
void	free_register(INTEL_OPERAND*);			/* will release a virtual register and any reals its using */
int	push_register(int);			/* will save a register in the save space */
int	pop_register(int);			/* gets a register from the save space */
void	swap_registers(int,int);			/* swap two virtual registers */
void	find_register(int);			/* assign a real register to a virtual one */
int	load_seg_reg(unsigned long);			/* loads a segment register */
void	gi_block_code(CLAUSE*,LEVEL_LNK*,int);		/* generates the block level code */
INTEL_OPERAND	gi_bool_code(INST_TYPE,INTEL_OPERAND,INTEL_OPERAND,int);	/* generate the boolean instruction code */
INTEL_OPERAND	gi_stat_code(CLAUSE*,LEVEL_LNK*,int,int);		/* generates the statement code */
char*	gi_operand(INTEL_OPERAND*,char*);		/* returns a string with the operand in it */
void	gi_when_code(LEVEL_LNK*);			/* this generates the when code */

void	find_register(int regi_no);			/* finds a free real register */

#endif	
