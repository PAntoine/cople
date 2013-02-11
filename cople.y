/* $Revision: 1.2 $
 *
 *		COPLE Definition
 *
 *		  Version 1.0
 *
 *	This language is designed for the COBOS operating system. It will be
 * 	implemented in two ways. The first will be a system language for
 *	writting the COBOS OS in. The second will be a tokenised interpreted
 *	langauge for creating the COBOS objects.
 *
 *
 *		Author: 	Peter Antoine.
 *		Copyright:	1998-1999
 * 
 *	This file is designed for BISON.
 *
 */

%{	#include	"cople.h"
	#include	"codetree.h"
	#include	<stdio.h>

	#define	YYDEBUG  	1

	extern	CONSTANT	*tmp_const;
	extern	SYMBOL	*tmp_symbol;
	extern	PARAMETER	tp;
	extern	CLAUSE	*system_pnt;
	extern	CLAUSE	*user_pnt;
	extern	CLAUSE	*node_pnt;

	extern	CLAUSE	*tree_root;
	extern	CODE_TYPE	program_type;
	extern	char	*prog_name;

	int	yydebug = 0;
%}

%union{
	char	char_string[255];
	char	character;
	int	int_number;
	float	float_number;
	void*	complex_type;
	SYMBOL*	symbol_type;
	CONSTANT*	const_type;
	INST_TYPE	instr_type;
	CLAUSE*	clause;
	LIST*	list_type;
	PARAMETER	parameter;
	PARAMETER_LIST*	parameter_list;
	}

%token	<char_string>	STRING_CONST VALID_NAME
%token	<character>	CHARACTER
%token	<int_number>	INTEGER
%token	<char_string>	FLOAT
%token	<int_number>	COPLE, COPLE_APP, COPLE_OBJ, COPLE_SYS
%token		LT_OP, LE_OP, GT_OP, GE_OP, EQ_OP, NE_OP, ASSIGN_OP, AND_OP, OR_OP, NOT_OP, XOR_OP, MOD_OP
%token		IF, THEN, ELSE, ELSEIF, FI, WHILE, FORALL, FOR 
%token		BEGIN, END, WHENEVER
%token		REF, INT, CHAR, BOOLEAN, REAL, INSTANCE, STRING, VOID
%token		MANDATORY, STRUCT 
%token		NEW, DELETE, OF, WITH, TRUE, FALSE
%token	YYERROR_VERBOSE

%type	<int_number>	type_def
%type	<list_type>	int_list
%type	<list_type>	const_list
%type	<clause>	statement_list
%type	<list_type>	expression_list
%type	<list_type>	VALID_NAME_list
%type	<list_type>	mixed_arg_list
%type	<list_type>	pos_arg_list
%type	<list_type>	sd_list
%type	<list_type>	md_list
%type	<list_type>	lookup_list
%type	<parameter_list>	param_list
%type	<parameter_list>	param_def_list

%type	<clause>	obj_body
%type	<clause>	app_body
%type	<clause>	sys_body

%type	<symbol_type>	declaration

%type	<clause>	statement

%type	<const_type>	constant
%type	<int_number>	int_const
%type	<float_number>	float_const
%type	<character>	char_const
%type	<int_number>	boolean_const
%type	<const_type>	object_name
%type	<const_type>	memory_ref
%type	<const_type>	lookup_entry
%type	<const_type>	pos_argument
%type	<const_type>	name_argument
%type	<clause>	item

%type	<clause>	compound
%type	<clause>	for_loop
%type	<clause>	assignment
%type	<clause>	forall_loop
%type	<clause>	while_loop
%type	<clause>	if_statement
%type	<clause>	system_call
%type	<clause>	service_def
%type	<clause>	service_call
%type	<clause>	esif_statement
%type	<clause>	whenever_stat

%type	<clause>	term
%type	<clause>	logical_exp
%type	<clause>	boolean_exp
%type	<clause>	arith_exp
%type	<clause>	expression

%type	<instr_type>	boolean_op
%type	<instr_type>	logical_op
%type	<instr_type>	arith_op


%%	/* the Grammer */

/*--------------------------------------------*
 * There are three types of the COPLE language
 * one for applications, one for objects and
 * the final for defining and writing system
 * functions.
 *--------------------------------------------*/

langauge:	application
	| object
	| system
	;

application:	'@' COPLE ':' COPLE_APP '!' VALID_NAME app_body		{ tree_root = $7;
					program_type = p_application;
					prog_name = (char*) malloc(sizeof($6)+1);
					strcpy(prog_name,$6);}
	;

app_body:	compound	{}
	| error	{}
	;

object:	'@' COPLE ':' COPLE_OBJ '!' VALID_NAME obj_body		{ tree_root = $7;
					program_type = p_object;
					prog_name = (char*) malloc(sizeof($6)+1);
					strcpy(prog_name,$6);}
	;

obj_body:	{new_level();}
	declaration_list madatory_def service_list		{ $$ = end_level();}
	;

madatory_def:	MANDATORY ':' VALID_NAME_list
	;

system:	'@' COPLE ':' COPLE_SYS '!' VALID_NAME sys_body 	{ tree_root = $7;
					program_type = p_system;
					prog_name = (char*) malloc(sizeof($6)+1);
					strcpy(prog_name,$6);}
	;

sys_body:	 compound		{}
	| error		{}
	;


/*--------------------------------------------*
 * lists.
 *
 * All lists return a pointer to the head
 * of the list that holds the type infomation.
 * The node types are based on the type created
 * in the create_list call.
 *--------------------------------------------*/

int_list:	int_list ',' INTEGER	{ $$ = $1; add_to_list($1, create_int_const($3));}
	| INTEGER		{ $$ = create_list(l_int); add_to_list($$,create_int_const($1));} 
	;

md_list:	md_list ',' sd_list 	{ $$ = $1; add_to_list($1,create_list_const($3));}
	| sd_list		{ $$ = create_list(l_list); add_to_list($$,create_list_const($1));}
	;

sd_list:	'{' const_list '}'	{ $$ = $2;}
	| '{' md_list '}'	{ $$ = $2;}
	;

const_list:	 const_list ',' constant 	{ $$ = $1; add_to_list($1, $3);}
	| constant		{ $$ = create_list(l_const); add_to_list($$,$1);}
	;

statement_list:	statement_list statement	{ add_clause($2);}
	| statement		{ add_clause($1);}
	;

declaration_list: declaration_list declaration	{ }	/* bison construct - for organisation */
	| declaration		{ }
	;

service_list:	service_list service_def 	{ add_clause($2);} 
	| service_def		{ add_clause($1);} 
	;

VALID_NAME_list: VALID_NAME_list ',' VALID_NAME { $$ = $1; add_to_list($1, create_sym_ref($3));}
	| VALID_NAME		{ $$ = create_list(l_name); add_to_list($$,create_sym_ref($1));}
	;

lookup_list:	lookup_list ',' lookup_entry	{ $$ = $1; add_to_list($1,$3);}
	| lookup_entry		{ $$ = create_list(l_lookup); add_to_list($$,$1);}
	;

lookup_entry:	VALID_NAME '=' term	{ $$ = create_lookup_const(create_str_const($1),$3);}
	;

expression_list: expression_list ',' expression	{ $$ = $1; add_to_list($$, create_expr_const($3));}
	| expression		{ $$ = create_list(l_expr); add_to_list($$,create_expr_const($1));}
	;

/*----------------------------------------------------------------------------*
 * Argument lists
 *
 * for calling services the paramters can be called by name, but for this the
 * parameters called by position must come first in the list. This is just
 * to make it possible to do this in remote calls.
 *
 *i.e.
 * realm:object.service(positional parameters,named parameters)
 *----------------------------------------------------------------------------*/

mixed_arg_list:	mixed_arg_list ',' name_argument	{ $$ = $1; add_to_list($1, $3);}
	| pos_arg_list ',' name_argument	{ $$ = $1; add_to_list($1, $3);}
	| name_argument			{ $$ = create_list(l_args); add_to_list($$,$1);}
	;

pos_arg_list:	pos_arg_list ',' pos_argument		{ $$ = $1; add_to_list($1, $3);}
	| pos_argument			{ $$ = create_list(l_args); add_to_list($$,$1);}
	;

name_argument:	VALID_NAME '=' expression		{ $$ = create_arg_const(get_type($3),create_str_const($1),$3);}
	;
	
pos_argument:	expression			{ $$ = create_arg_const(get_type($1),0,$1);}
	;

/*----------------------------------------*
 * declarations:
 *  struct:   struct name{ declartion ... }
 *  array:    type name [int,int,...]
 *  stand:    type name
 *  instance: instance name[int] of obj_name
 *
 *----------------------------------------*/

declaration:	struct_def		{}
	| instance_def		{}
	| array_def		{}
	| standard_def		{}
	| memory_def		{}
	;

struct_def:	STRUCT VALID_NAME '{' declaration '}'		{}
	;

array_def:	type_def VALID_NAME '[' ']' '=' sd_list 		{ conv_const_list_to_array($6,$2,$1,a_var);}
	| type_def VALID_NAME '[' int_list ']'		{ conv_list_to_array($4,$2,$1,a_var);}
	;

standard_def:	type_def VALID_NAME '=' constant		{ add_variable($2,$1,$4,a_var);}
	| type_def VALID_NAME			{ add_variable($2,$1,NULL,a_var);}
	;

instance_def:	INSTANCE VALID_NAME OF object_name		{ add_variable($2,d_instance,$4,a_inst);}
	;

memory_def:	REF type_def VALID_NAME			{ add_variable($3,$2,NULL,a_ptr);}
	| REF type_def VALID_NAME '=' memory_ref		{ add_variable($3,$2,$5,a_ptr);}
	| REF type_def VALID_NAME '[' int_list ']'		{ conv_list_to_array($5,$3,$2,a_ptr);}
	;

memory_ref:	'&' VALID_NAME				{ $$ = create_sym_ref($2);}
	| '&' INTEGER				{ $$ = create_ref_const($2);}
	;
	
/*-----------------------------------------*
 * Definitions of the parts of the language
 * that are used by all definitions. The 
 * The num_instr is used as a label and it
 * points to the instruction number in the
 * instruction list.
 *-----------------------------------------*/

statement: 	compound
	| for_loop
	| assignment
	| forall_loop
	| while_loop
	| if_statement
	| system_call
	| service_call
	| whenever_stat
	;

compound:	BEGIN		{ new_level(); }
	declaration_list
	statement_list		
	END		{ $$ = end_level();}
	| BEGIN		{ new_level(); }
	  statement_list
	  END		{ $$ = end_level();}
	;

/*-------------------------------------------*
 * loop control programming constructs.
 *-------------------------------------------*/

whenever_stat:	WHENEVER VALID_NAME THEN statement		{ $$ = new_when_clause($2,$4);}
	;

for_loop:	FOR '(' assignment ';' expression ';' expression ')' compound 	{ $$ = new_for_clause($3,$5,$7,$9);}
	| FOR '(' ';' expression ';' expression ')' compound 	{ $$ = new_for_clause(NULL,$4,$6,$8);}
	| FOR '(' ';' expression ';' ')' compound 		{ $$ = new_for_clause(NULL,$4,NULL,$7);}
	| FOR '(' assignment ';' expression ';' ')' compound 	{ $$ = new_for_clause($3,$5,NULL,$8);}
	;

assignment: 	item ASSIGN_OP expression 		{$$ = new_binary_clause(store,$1,$3);}
	| '$' VALID_NAME ASSIGN_OP object_name	{$$ = new_binary_clause(store,find_operand($2,d_instance),new_const_clause($4));}
	| '$' VALID_NAME ASSIGN_OP '$' VALID_NAME	{$$ = new_binary_clause(store,find_operand($2,d_instance),find_operand($5,d_instance));}
 	| item ASSIGN_OP memory_ref		{$$ = new_binary_clause(store,$1,new_const_clause($3));}
 	| memory_ref ASSIGN_OP expression	{$$ = new_binary_clause(store,new_const_clause($1),$3);}
	;

forall_loop:	FORALL VALID_NAME compound		{ $$ = new_forall_clause(NULL,find_operand($2,d_instance),$3);}
	| FORALL VALID_NAME WITH lookup_list compound	{ $$ = new_forall_clause($4,find_operand($2,d_instance),$5);}
	;

while_loop:	WHILE '(' expression ')' compound	{ $$ = new_while_clause($3,$5);}
	;

if_statement:	IF expression THEN statement esif_statement FI		{ $$ = new_if_clause($2,$4,$5);}
	| IF expression THEN statement ELSE statement FI		{ $$ = new_if_clause($2,$4,$6);}
	| IF expression THEN statement FI		{ $$ = new_test_clause($2,$4);}
	;

esif_statement: ELSEIF expression THEN statement		{ $$ = new_test_clause($2,$4); }
	| ELSEIF expression THEN statement ELSE statement	{ $$ = new_if_clause($2,$4,$6); }
	| ELSEIF expression THEN statement esif_statement	{ $$ = new_if_clause($2,$4,$5); }
	;

/*--------------------------------------------*
 * basic programming constructs.
 *--------------------------------------------*/

expression:	boolean_exp
	| logical_exp
	| arith_exp
	| term
	;

boolean_exp:	term boolean_op term		{ $$ = new_binary_clause($2,$1,$3);}
	| NOT_OP term			{ $$ = new_unary_clause(not,$2);}
	;

arith_exp:	term arith_op term		{ $$ = new_binary_clause($2,$1,$3);}
	;

logical_exp:	term logical_op term		{ $$ = new_binary_clause($2,$1,$3);}
	;

term:	'(' expression ')'		{ $$ = $2;}	
	| item			{ $$ = $1;}
	| constant			{ $$ = new_const_clause($1); }
	;	

/*--------------------------------------------*
 * data type and names definitions.
 *--------------------------------------------*/

boolean_op:	AND_OP			{ $$ = and; }
	| OR_OP			{ $$ = or;  }
	| XOR_OP			{ $$ = xor; }
	;
	
logical_op:	LE_OP			{ $$ = less_equal; }
	| LT_OP			{ $$ = less_than; }
	| GE_OP			{ $$ = greater_equal; }
	| GT_OP			{ $$ = greater_than; }
	| NE_OP			{ $$ = not_eq; }
	| EQ_OP			{ $$ = equal; }
	;

arith_op:	'*'			{ $$ = multi; }
	| '%'			{ $$ = modulo; }
	| '/'			{ $$ = divide; }
	| '+'			{ $$ = add; }
	| '-'			{ $$ = sub; }
	;

item:	VALID_NAME			{ $$ = find_operand($1,d_null);}
	| VALID_NAME '[' expression_list ']'	{ $$ = new_array_clause(find_operand($1,d_array),$3);}
	;	

constant:	int_const			{ $$ = create_int_const($1); }
	| char_const			{ $$ = create_char_const($1); }
	| float_const			{ $$ = create_float_const($1); }
	| boolean_const			{ $$ = create_bool_const($1); }
	| STRING_CONST			{ $$ = create_str_const($1); }
	;

int_const:	'-' INTEGER			{ $$ = 0 - $2; }
	| INTEGER			{ $$ = $1; }
	;

float_const:	INTEGER '.' INTEGER		{ $$ = $1; $$ += (1 / $3);}	/* this is cheap will do it prop - later */

char_const:	CHARACTER 			{ $$ = $1; }

boolean_const:	TRUE			{ $$ = 1; }
	| FALSE			{ $$ = 0; }
	;

type_def:	CHAR			{ $$ = d_char; }
	| BOOLEAN			{ $$ = d_boolean; }
	| REAL			{ $$ = d_real; }
	| INT			{ $$ = d_int; }
	| STRING			{ $$ = d_string; }
	; 

object_name:	VALID_NAME			{ $$ = create_inst_const(NULL,$1,0);}
	| VALID_NAME '#' INTEGER 		{ $$ = create_inst_const(NULL,$1,$3);}
	| VALID_NAME ':' VALID_NAME		{ $$ = create_inst_const($1,$3,0);}
	| VALID_NAME ':' VALID_NAME '#' INTEGER 	{ $$ = create_inst_const($1,$3,$5);}
	;

/*--------------------------------------------------*
 * call structures. Note: inline code has a number!
 *--------------------------------------------------*/

system_call:	COPLE_SYS ':' VALID_NAME '(' mixed_arg_list ')'		{ $$ = new_call_clause(system_pnt,create_str_const($3),$5);}
system_call:	COPLE_SYS ':' VALID_NAME '(' pos_arg_list ')'		{ $$ = new_call_clause(system_pnt,create_str_const($3),$5);}
	;

service_call:	object_name '.' VALID_NAME '(' mixed_arg_list ')'	{ $$ = new_call_clause(new_const_clause($1),create_str_const($3),$5);}
	| object_name '.' VALID_NAME '(' pos_arg_list ')'	{ $$ = new_call_clause(new_const_clause($1),create_str_const($3),$5);}
	| '$' VALID_NAME '.' VALID_NAME '(' mixed_arg_list ')'	{ $$ = new_call_clause(find_operand($2,d_instance),create_str_const($4),$6);}
	| '$' VALID_NAME '.' VALID_NAME '(' pos_arg_list ')'	{ $$ = new_call_clause(find_operand($2,d_instance),create_str_const($4),$6);}
	| VALID_NAME '(' mixed_arg_list ')'		{ $$ = new_call_clause(NULL,create_str_const($1),$3);}
	| VALID_NAME '(' pos_arg_list ')'		{ $$ = new_call_clause(NULL,create_str_const($1),$3);}
	;

service_def:	service_hdr compound			{ $$ = end_service_level($2);}
	;

service_hdr:	VALID_NAME '(' param_list ')'			{ new_service_entry(add_variable($1,d_service,NULL,a_service),$3);}
	;

param_list:	VOID				{ $$ = NULL;}
	| param_def_list			{ $$ = $1;}
	;

param_def_list:	param_def_list ',' type_def VALID_NAME		{ $$ = $1; add_parameter($1,$3,$4,NULL,0);}
	| param_def_list ',' REF type_def VALID_NAME		{ $$ = $1; add_parameter($1,$4,$5,NULL,1);}
	| param_def_list ',' type_def VALID_NAME '=' constant	{ $$ = $1; add_parameter($1,$3,$4,$6,0);}
	| type_def VALID_NAME 			{ $$ = new_param_list();add_parameter($$,$1,$2,NULL,0);}
	| REF type_def VALID_NAME 			{ $$ = new_param_list();add_parameter($$,$2,$3,NULL,1);}
	| type_def VALID_NAME '=' constant 		{ $$ = new_param_list();add_parameter($$,$1,$2,$4,0);}
	;
