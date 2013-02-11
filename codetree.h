/* $Revision: 1.2 $
 *
 *		   CODE TREE Header File
 *
 *	This file contains the structures that are used in constructing
 *	the code tree from the code parser.
 *
 *		Copyright (c) P.Antoine 1999.
 *
 */

#ifndef	__COPLE_CODE_TREE__
#define	__COPLE_CODE_TREE__

#include	"cople.h"

/* Parameter definitions */

typedef struct	{
	DATA_TYPE	type;	/* the type of the paramter */
	char	*name;	/* the name of parameter */
	short int	offset;	/* the stack offset */
	CONSTANT	*def;	/* this the default value for the parmeter */	
	int	a_ref;	/* is this parameter a reference parameter */
	} PARAMETER;

typedef struct	{
	int	list_size;	/* number of parmeters in the list */
	PARAMETER	*list;	/* the actual parameter list */
	} PARAMETER_LIST;

/* Whenever changed list, this will hold a
 * pair of integers the first to hold the exception number
 * that had its exception changed - and the second to hold
 * the exception old label.
 */

 typedef struct {
 	int	exception_num;	/* the number of the exception */
	int	old_label;	/* the label for the old whenever */
	} WHEN_CHANGE;

/* Definitions for the code tree
 */

typedef enum	{
	ct_null,
	ct_whenever,
	ct_test,
	ct_if,
	ct_while,
	ct_for,
	ct_forall,
	ct_bool,
	ct_array,
	ct_unary,
	ct_branch,
	ct_call,
	ct_service,
	ct_block,
	ct_variable
	} CLAUSE_TYPE;

typedef union	CLS_DEF {
	CLAUSE_TYPE	c_type;		/* the type of the clause */
	struct	{
		CLAUSE_TYPE	type;	/* CLAUse type */
		int	exception;	/* the exception number - translated from exception name */
		int	label;	/* the whenever label to be set */
		union CLS_DEF	*code;	/* a pointer to the code */
	} c_whenever;
	struct	{
		CLAUSE_TYPE	type;	/* CLAUse type */
		union CLS_DEF	*condition;	/* a boolean condition */
		union CLS_DEF	*action;	/* the action to be carried out if the condition is true */	
	} c_test;
	struct	{
		CLAUSE_TYPE	type;	/* clause type */
		union CLS_DEF	*condition;	/* a boolean condition tree */
		union CLS_DEF	*action;	/* the action to  */
		union CLS_DEF	*on_fail;
	} c_if;
	struct	{
		CLAUSE_TYPE	type;	/* clause type */
		union CLS_DEF	*condition;	/* a boolean condition tree */
		union CLS_DEF	*action;	/* the action to  */
	} c_while;
	struct	{
		CLAUSE_TYPE	type;	/* clause type */
		union CLS_DEF	*condition;	/* the limiting code for the for loop */
		union CLS_DEF	*initial;	/* the set up code for the for loop */
		union CLS_DEF	*step;	/* the step code */
		union CLS_DEF	*action;	/* the action to be done */
	} c_for;
	struct	{
		CLAUSE_TYPE	type;	/* clause type */
		LIST	*lookup;	/* the lookup list */
		union CLS_DEF	*ref_var;	/* the reference variable for the for all */
		union CLS_DEF	*action;	/* the action to  */
	} c_forall;
	struct	{
		CLAUSE_TYPE	type;	/* CLAUse type */
		INST_TYPE	instr;	/* the instruction type */
		int	weight;	/* the weight of this branch */
		union CLS_DEF	*left;	/* the left operand */
		union CLS_DEF	*right;	/* the right operand */
	} c_bool;
	struct	{
		CLAUSE_TYPE	type;	/* CLAUse type */
		union CLS_DEF	*symbol;	/* a pointer to the arrays symbol */
		union CLS_DEF	*entry;	/* the calculated array entry (CONSTANT/CODE)*/
	} c_array; 
	struct	{
		CLAUSE_TYPE	type;	/* cluase type */
		INST_TYPE	instr;	/* the instruction type */
		union CLS_DEF	*operand;	/* the operand of the clause */
	} c_unary;
	struct	{
		CLAUSE_TYPE	type;	/* CLAUse type */
		INST_TYPE	instr;	/* the instruction type */
		int	label;	/* where to jump to */
	} c_branch;
	struct	{
		CLAUSE_TYPE	type;	/* cluase type */
		union CLS_DEF	*object;	/* the object that the service belongs to */
		CONSTANT	*service;	/* the name of the service called */
		LIST	*list;	/* the parameters for the call */
		DATA_TYPE	result;	/* the return type of the call */
	} c_call;
	struct	{
		CLAUSE_TYPE	type;	/* cluase type */
		union CLS_DEF	*owner;	/* the block that owns the service */
		SYMBOL	*name;	/* the function name */
		int	stack_size;	/* the stack size for the pointers */
		int	num_locals;	/* the number of locals in the block */
		SYMBOL	**locals;	/* local symbol list */
		union CLS_DEF	*code;	/* the code for the service */
		DATA_TYPE	result;	/* the return type of the service */
	} c_service;
	struct	{
		CLAUSE_TYPE	type;	/* cluase type */
		union CLS_DEF	*owner;	/* the privious block */
		int	local_size;	/* the size of the locals in data space */
		int	local_base;	/* the offset for the start of local memory */
		int	num_locals;	/* the number of locals in the block */
		SYMBOL	**locals;	/* local symbol list */
		int	num_when;	/* number of whenevers in the block */
		WHEN_CHANGE	*whenc_list;	/* the when change list */
		int	num_instr;	/* number of instructions in the block */
		union CLS_DEF	**instr_list;	/* instructions in the block */
	} c_block;
	struct	{
		CLAUSE_TYPE	type;	/* cluase type */
		CONSTANT	value;	/* a direct value */	
	} c_variable;			/* the clause can be a variable as well */

	} CLAUSE;

/* Whenerver list - this is a list of the whenever clauses
 * that have been defined.
 */

typedef struct	WHEN_CLS{
	CLAUSE	*when_code;	/* the code for the whenever clause */
	struct WHEN_CLS	*next;	/* next whenever clause */
	} WHENEVER_LIST;


/* Function definition.
 * These are the functions that are used to build the 
 * code tree. This tree is used later for create the
 * program from.
 */

void	new_level(void);			/* creates a new level block */
CLAUSE*	end_level();				/* this closes the current block and goes up a level */
void  	new_service_entry(SYMBOL*,PARAMETER_LIST*);		/* the creates a service level */
CLAUSE*	end_service_level(CLAUSE*);			/* this ends the service level */
CLAUSE*	find_operand(char*,DATA_TYPE);			/* find a symbol and make a CLAUSE of it */

CLAUSE*	new_when_clause(char*,CLAUSE*);			/* create a new whenever clause */
CLAUSE*	new_test_clause(CLAUSE*,CLAUSE*);		/* create a new test clause */
CLAUSE*	new_if_clause(CLAUSE*,CLAUSE*,CLAUSE*);		/* creates a new if clause */
CLAUSE*	new_while_clause(CLAUSE*,CLAUSE*);		/* creates a new while clause */
CLAUSE*	new_for_clause(CLAUSE*,CLAUSE*,CLAUSE*,CLAUSE*);	/* creates a new for clause */
CLAUSE*	new_forall_clause(LIST*,CLAUSE*,CLAUSE*);		/* creates a forall call */
CLAUSE*	new_binary_clause(INST_TYPE,CLAUSE*,CLAUSE*);		/* create boolean clause */
CLAUSE*	new_array_clause(CLAUSE*,LIST*);		/* create array reference clause */
CLAUSE*	new_unary_clause(INST_TYPE,CLAUSE*);		/* unary clause */
CLAUSE*	new_branch_clause(INST_TYPE,int);		/* branch clause */
CLAUSE*	new_call_clause(CLAUSE*,CONSTANT*,LIST*);		/* create a service/function call */
CLAUSE*	new_service_clause(SYMBOL*,PARAMETER_LIST*,DATA_TYPE,CLAUSE*);	/* create a service definition */
CLAUSE*	new_const_clause(CONSTANT*);			/* creates a clause from a constant */

CLAUSE*	calculate_array_entry(SYMBOL*,LIST*);		/* creates the codetree to access an element of an array */

/* Other functions that need codetree elements as paramters
 * Note: the header files will be merged at a later stage.
 */

PARAMETER_LIST*	new_param_list(void);			/* create a new parameter list */
void	add_parameter(PARAMETER_LIST*,DATA_TYPE,char*,CONSTANT*,int);	/* add parameter to the parameter list */


/* functions that generate the CASM assembler code. 
 */

void	generate_code(CLAUSE*);			/* function that holds it all together */
void	gen_block_code(CLAUSE*,LEVEL_LNK*,int,int);		/* generates the block level code */
OPERAND	gen_stat_code(CLAUSE*,LEVEL_LNK*,int,int);		/* generates the statement code */
OPERAND	gen_instr(INST_TYPE,OPERAND,OPERAND,int);		/* the meat of the whole thing - generates the instructions */
char*	get_op_string(OPERAND,int,int*,char*);		/* this will generate an operand string from OPERAND */
void	gen_local_call(CLAUSE*,LEVEL_LNK*,int,int);		/* the function will create a local call with fixups */

OPERAND	get_label(void);			/* this gets a new label */
void	put_label(OPERAND);			/* this tells the system the put a label before the next call */
void	gen_labels(void);			/* creates all outstanding labels */


/* Function to define a CONSTANT but as it uses a clause it must
 * be here.
 */

CONSTANT*	create_lookup_const(CONSTANT*,CLAUSE*);		/* creates a lookup constant */
CONSTANT*	create_expr_const(CLAUSE*);			/* creates a expression constant */
CONSTANT*	create_arg_const(DATA_TYPE,CONSTANT*,CLAUSE*);		/* creates a argument constant */


/* Macro definitions.
 * These are the macros for the code tree.
 */

#define	clause_num()		(PROGRAM.current_block->num_clauses)
#define	next_clause()		(PROGRAM.current_block->num_clauses + 1)
#define	new_null_clause()	((CLAUSE*) calloc(1,sizeof(CLAUSE)))
#endif
