/* $Revision: 1.2 $

		COPLE Header File

	This file is the definitions for the compiler functions that
	actually do the compilation. It also has the structures for
	the psudo operators. The code list is made up of either 
	instructions that have none, one or two operands.

		P.Antoine.
					*/

#ifndef	__COPLE_STRUC
#define	__COPLE_STRUC

/*	System defines.
 */

typedef enum	{INTEL_486} PROCESSOR;	/* need to define the fifferent processor types */

#define	__DEFAULT_PROC__	INTEL_486
#define 	__VERSION_NO__		"$Revision: 1.2 $"

/*	Standard DEFINES that are needed for the structures. These are not very
 *	scientific as the size of the list/blocks are not known as I dont know
 *	what the programs/functions will look like when code is being written
 *	in anger, so these are just guesses. Making the the blocks and lists
 *	bigger will speed up searches but use more mem, making them smaller will
 *	slow down searches.
 */

#define	HASH_NUM	13	/* number of hash blocks and the hashing number */
#define	NAME_NUM	40	/* number of names to a name block */
#define	HASH_BLOCK_SZ	40	/* number of entries in a hash block */
#define	INST_LIST_SZ	40	/* size of the code list in a local block */
#define	LOCAL_SYMB_SZ	40	/* number of local symbols per local block */

/*	ERROR_CODES
 *	This error codes are for the comp_error function and define the level of error
 *	that has occoured. If the the error is FATAL then the function exit and tidy up.
 */

#define	FATAL_ERROR	0
#define	WARNING	1
#define	COMMENT	2

/*	The following are the names of the standard exceptions that
 *	COPLE will trap with the whenever clause. When one of these
 *	errors occurs the "error_code" variable will have the code
 *	that will pin-point the program error.
 */

#define	NUM_EXCEPTIONS	8	/* the number of exceptions (max 32!) */

static char	*exception[] =	{"ok",		/* this cannot be trapped ! */
		"system_error",		/* a system error has occured */
		"file_error",		/* problem with a file */
		"permissions_error",	/* tripped the security system */
		"realm_error",		/* problem with the named realm */
		"object_error",		/* the called object does not exists */
		"instance_error",	/* the called instance was not found */
		"service_call_error"	/* problem with the called service */
	};	

/*	The following equates are the instructions that the
 *	COPLE system supports, and will have to have code macros
 *	for so that the code can be generated from.
 */

static char	*instr_name[] =	{"nop",
		"and",
		"or",
		"xor",
		"add",
		"sub",
		"inc",
		"not",
		"dec",
		"multi",
		"modulo",
		"divide",
		"load",
		"store",
		"push",
		"pop",
		"call",
		"ret",
		"subr",
		"endsubr",
		"set",
		"setgt",
		"setge",
		"setne",
		"seteq",
		"setlt",
		"setle",
		"loop",
		"branch",
		"branch_ne",
		"branch_eq",
		"branch_lt",
		"branch_le",
		"branch_gt",
		"branch_ge"}; 

typedef	enum{	null,
		and,		/* logical bitwise and */
		or,		/* logical bitwise or */
		xor,		/* logical bitwaise xor */
		add,		/* addition */
		sub,		/* subtract */
		inc,		/* increment */
		not,		/* logical not */
		dec,		/* decrement */
		multi,		/* multiply */
		modulo,		/* remainder of division */
		divide,		/* divide */
		load,		/* local load */
		store,		/* local store */
		push,		/* push onto the stack */
		pop,		/* pop the stack */
		call,		/* call function */
		ret,		/* return from function */
		subr,		/* sub routine */
		endsubr,		/* end subroutine call */
		test,		/* compare two values/variables */
		greater_than,		/* test instructions > */
		greater_equal,		/* test instructions >= */
		not_eq,		/* test instructions != */
		equal,		/* test instruction == */
		less_than,		/* test instruction < */
		less_equal,		/* test instruction <= */
		loop,		/* dec register the loop */
		branch,		/* program branch */
		branch_ne,		/* jump on result of a test */
		branch_eq,		/* jump on result of a test */
		branch_lt,		/* jump on less_than */
		branch_le,		/* jump on less_than equal to */
		branch_gt,		/* jump on greater_than */
		branch_ge		/* jump on greater_than equal to */
		} INST_TYPE;

/*	The following types are the data and constant types that are
 *	can be definied by the programs.
 */

typedef	enum{	p_null,		/* un-defined program type */
		p_object,		/* the program is an object */
		p_system,		/* the program defines a system function */
		p_application		/* the program definies an application */
		} CODE_TYPE;

static char	*type_name[]	= {"NULL",
		"char",
		"bollean",
		"real",
		"integer",
		"string",
		"void"};

static char	INSTR_SIZE[]	= {' ','b','b','d','d','d','b'};

typedef	enum{	c_null,
		c_char,		/* char constant type */
		c_boolean,		/* boolean */
		c_real,		/* IEEE 64 bit floating point number */
		c_int,		/* 64 bit integer */
		c_string,		/* string constant */
		c_void,		/* non-type - only used for services */
		c_list,		/* a sub-array */
		c_memory,		/* a memory refrence */
		c_instance,		/* instance pointer to an object */
		c_reference,		/* reference to a symbol */
		c_lookup,		/* only used in forall clauses */	
		c_arg,		/* an argument for a service call */
		c_expr,		/* an expression used in array references */
		c_addr		/* an addr reference constant */
		} CONST_TYPE;

typedef	enum{	d_null,
		d_char,		/* char variable */
		d_boolean,		/* boolean */
		d_real,		/* real */
		d_int,		/* integer 64 bit */
		d_string,		/* string */
		d_array,		/* array of information */
		d_struct,		/* data structure */
		d_memory,		/* a reference to memeory */
		d_instance,		/* instance pointer to an object */
		d_call,		/* not a real data-type but used in type checking */
		d_service		/* its a service dear boy... */
		} DATA_TYPE;

typedef	enum{	l_null,
		l_int,		/* integer list */
		l_const,		/* all constant types allowed */
		l_param,		/* constant and valid names only */
		l_state,		/* list of instructions (statements)*/
		l_serv,		/* list of services */
		l_name,		/* valid names only */
		l_args,		/* VALID names and constants only */
		l_struct,		/* structure element */
		l_list,		/* list of lists (muti-dimensional array */
		l_lookup,		/* used to pass parameters to forall */
		l_expr		/* used for array refrences */
		} LIST_TYPE;	

typedef	enum{	o_null,		/* gone pear shaped */
		o_reg,		/* the operand is an register */
		o_parameter,		/* the operand in stack reference */
		o_variable,		/* the register is from variable space */
		o_constant,		/* the operand is a constant value */
		o_array_ref,		/* the value is address register relative */
		o_label,		/* a label for a branch */
		o_ptr_ref,		/* a refrence to the contents of the pointer reference */
		o_var_ref		/* a refrence to the address of a variable */
		} OPER_TYPE;

/* Definitions of the access type of a symbol. Note that the
 * parameter type is scoped within a funciton.
 */

#define	a_var	0x00		/* variable - default */
#define	a_reference	0x80		/* a refrence to a prameter variable */
#define	a_global	0x40		/* a global/object variable */
#define	a_service	0x20		/* a service name in the symbol table */
#define	a_multi	0x10		/* a multi-dimensional array */
#define	a_param	0x08		/* variable is a parameter */
#define	a_ptr	0x04		/* variable is a pointer */
#define	a_inst	0x02		/* " is a instance reference */
#define	a_const	0x01		/* " is a constant symbol */
#define	a_c_ptr	(a_ptr | a_const)	/* " a constant pointer */
#define	a_c_inst	(a_inst | a_const)	/* " a constant instance */

/*	Constant array for the data type data sizes, this is just 
 *	to make coding easier.
 */

static	int DATA_TYPE_SIZE[] = { 	0,	/* d_null */
			1,	/* d_char */
			1,	/* d_boolean */
			4,	/* d_real */
			4,	/* d_int */
			4,	/* d_string */
			0,	/* d_array */
			0,	/* d_struct */
			4,	/* d_memory */
			68,	/* d_instance */
			0,	/* d_call */
			0	/* d_service */
		};

/*	Operand - this structure is used in the final code 
 *	generation step.
 */

typedef struct	{
	OPER_TYPE	type;		/* the operand type */
	DATA_TYPE	data_type;		/* the data type for the operand */
	int	is_data;		/* if the operand holds data or register value(only for array refs) */
	unsigned int	data;		/* if a constant the data for the constant */
	int	reg;		/* register number */
	int	offset;		/* the offset into the data space */
	} OPERAND;

/*	Some structures that will be used in the final code
 *	that is built.
 */

typedef union	{
	short int	size;
	char	name[32 + 2];
	} NAME_STRN;

/*	Structures that are used to hold the parsed code before
 *	the object code is generated.
 */

typedef struct {
	char	*name;		/* name of the symbol used - if constant null */
	DATA_TYPE	type;		/* data type */
	DATA_TYPE	array_type;		/* if type = d_array then array_type is the data type of the array */
	int	offset;		/* offset into the variable space */
	int	size;		/* total memory size */
	int	num_dim;		/* the number of dimensions in the array - 0 not an array */
	short int	*dimensions;		/* an array of the size of each dimension - starting with the top */
	int	access_type;		/* 001 - instance, 010 - constant, 100 - ptr (or any legal comb) */
	int	data;		/* this holds type specific data */
	void	*block;		/* the node that owns the entry */
	} SYMBOL;

typedef struct HASH_B{
	SYMBOL	index[HASH_BLOCK_SZ];	/* block of 40 names */
	struct HASH_B	*next;		/* overflow block */
	} HASH_BLOCK;

typedef	struct {
	HASH_BLOCK	*block[HASH_NUM];	/* HASH_NUM blocks of 40 names */
	HASH_BLOCK	*last[HASH_NUM];	/* the last block in each chain */
	int	last_entry[HASH_NUM];	/* last entry in the block */
	} HASH_TABLE;

/*	List strucures, this structure is temporary and is used
 *	in some of the definitions to hold lists of different
 *	types of data. This data is coverted into different
 *	structures depending on use.
 */

typedef union	{
	short int	size;
	char	*data;
	} STRING_UN;

typedef struct	{
	NAME_STRN	name;		/* name of the structure entry XXXX.name */
	int	offset;		/* offset into the structure data space of XXXX */
	DATA_TYPE	type;		/* type of variable for entry */
	} STRUCT_ENTRY;

typedef struct	{
	int	segment;
	int	offset;
	} MEMORY_REF;

typedef struct	{
	int	size;		/* the number of fields in the structure */
	STRUCT_ENTRY	*array;		/* the array of field descriptions */
	char	*data;		/* data space (if any) for the structure */
	} STRUCTURE;

typedef struct	{
	void	*name;		/* this is the name of the element to be looked up (offset in common) */
	void	*clause;		/* this is the "value" to be checked against */
	} LOOKUP;

typedef struct	{
	DATA_TYPE	type;		/* the data type */
	void	*name;		/* a constant string that holds the name of the variable */
	void	*value;		/* is a reference to a clause */
	int	a_type;		/* the access type of the argument */
	} ARGUMENT;

typedef struct	L_ENT{
	CONST_TYPE	type;		/* type of data in the list - c_null = symbol */
	union	{		/* this union holds the data for the list entry */
		SYMBOL	*symbol;
		int	integer;
		char	character;
		int	string;	/* offset into common where the string const is stored */
		int	boolean;
		float	flt_point;
		MEMORY_REF	memory;
		STRUCT_ENTRY	element;
		void	*list;
		LOOKUP	lookup;
		ARGUMENT	argument;
		void	*expr;
	} 	data;
	struct L_ENT	*next;		/* next list element - NULL = the end */
	} LIST_ENTRY;

typedef struct	{
	int	list_size;		/* number of elements in the list */
	LIST_TYPE	type;		/* type of list */
	int	data_size;		/* the total memory used by elems in the list */
	LIST_ENTRY	*list;		/* the start of the list */
	LIST_ENTRY	*last_entry;		/* the last entry in the list */
	} LIST;

typedef struct	{
	int	offset;		/* the offset into the parameter space */
	LIST_ENTRY	*op;		/* the variable to pass the value back to */
	} REF_LIST;

typedef struct	{			/* this structure is used by create constant */
	CONST_TYPE	type;
	union	{		/* this union holds the data for the list entry */
		SYMBOL	*symbol;
		int	integer;
		char	character;
		int	string;	/* offset into common of where the string const is stored */
		int	boolean;
		float	flt_point;
		MEMORY_REF	memory;
		STRUCT_ENTRY	element;
		LIST	*list;
		LOOKUP	lookup;
		ARGUMENT	argument;
		void	*expr;
	} 	data;
	} CONSTANT;

/* This structure is used when walking trees to hold some of the level data that
 * has been accumulated during the walk.
 */

typedef struct	L_LNK{
	struct L_LNK	*prev;
	int	count;
	struct L_LNK	*next;
	} LEVEL_LNK;

typedef struct	R_FU{
	struct R_FU	*prev;
	int	base;
	int	count;
	struct R_FU	*next;
	} RANGE_FIXUP;

/*	The following are the functional definition of the functions
 *	that are used by the parser to create the tables that the
 *	code can be generated from.
 */

LIST*	create_list(LIST_TYPE);		/* this function will create a list */
void	add_to_list(LIST*, CONSTANT*);		/* this function will add an element to a list */
SYMBOL*	conv_const_list_to_array(LIST*,char*,DATA_TYPE,int);	/* create an initialised array */
char*	create_block_from_list(char*,LIST*,LEVEL_LNK*,int*,DATA_TYPE); /* create the arrays data block from a list of lists */
SYMBOL*	conv_list_to_array(LIST*,char*,DATA_TYPE,int);	/* create an array from an int list */	
int	list_product(LIST*);		/* this will take a integer list and generate a product */


CONSTANT*	create_int_const(int);		/* This creates a single constant of type int */
CONSTANT*	create_char_const(char);		/* for a char */
CONSTANT*	create_float_const(float);		/* IEE floating poimt */
CONSTANT*	create_bool_const(int);		/* boolean constant */
CONSTANT*	create_ref_const(int);		/* a memory reference */
CONSTANT*	create_str_const(char*);		/* a string constant */
CONSTANT*	create_sym_ref(char*);		/* makes a constant with a symbol reference (bollocks!) */
CONSTANT*	create_array_ref(char*,int);		/* reference into the array */
CONSTANT*	create_far_const(int,long);		/* Far pointer constant */
CONSTANT*	create_near_const(long);		/* Near pointer constant */
CONSTANT*	create_list_const(LIST*);		/* List constant */
CONSTANT*	create_inst_const(char*,char*,int);	/* INSTANCE constant */

void	free_const(CONSTANT*);		/* this releases the memory used by a constant */

SYMBOL*	add_symbol(char*);		/* add a symbol to the program */
SYMBOL*	add_variable(char*,DATA_TYPE,CONSTANT*,int);	/* create new variable */
SYMBOL*	find_variable(char*);		/* this does a tree walk - to find if var is in scope */

/*	string conversion functions 
 *	These will swap between the cobos style strings and the
 *	unix styles.
 *	
 *	size(big-endian) 1 word (short int).
 *	| data
 *	|_|_______...
 *	ssdddd.......
 */

char*	conv_cobos_to_unix_str(char*);
void	conv_unix_to_cobos_str(char*,char*,int);
void	copy_cobos_str(char*,char*);

/* 	Tidy up functions. Used to return memory.
 */

void	free_link(LEVEL_LNK*);		/* this function will free a linked list */


/*	The following are the macros that are used by the parser
 *	to create the tables that the code can be generated from.
 */

#define	dimension_check(symbol,list)	( (symbol->num_dim == list->list_size))
#endif	
