/* $Revision: 1.2 $
 *
 *		    Generate Statement Code
 *
 *	This function converts the clauses to the code for the statements. It uses
 *	gen_instr to actually generate the CASM instructions but this generates the
 *	order. The labels are genrrated by the functions in the xxx_label code.
 *
 *
 *		Copyright (c) 1999 Peter Antoine
 *
 */

#include	<stdlib.h>
#include	<stdio.h>
#include	"cople.h"
#include	"codetree.h"

OPERAND	gen_stat_code(CLAUSE *clause,LEVEL_LNK *stack,int regno,int addrno)
{
	LIST_ENTRY	*walk;
	OPERAND	label_1,label_2,op,op1,op2;
	int	count;
	char	*left_op;
	extern int	initial_size;
	extern FILE	*object_file;

	op.type  = o_null;	/* default return type - no_op (or no return) */
	op.reg   = regno;
	op1.type = o_null;
	op1.reg  = regno;

	switch(clause->c_type)
	{
	case ct_whenever:
		fprintf(object_file,"\tmove.d\ta30(#%d),#when_label_%d\n",
			clause->c_whenever.exception*4,
			clause->c_whenever.label);
		break;
	
	case ct_test:	label_1 = get_label();
		
		op2 = gen_stat_code(clause->c_test.condition,stack,regno,addrno);
		if (op2.type != o_reg)		/* just incase its a boolean var - solo */
		{
			op1.type = o_reg;
			gen_instr(load,op1,op2,addrno);
		};

		gen_instr(branch_ne,label_1,op1,addrno);
		gen_stat_code(clause->c_test.action,stack,regno,addrno);
		put_label(label_1);
		break;

	case ct_if:	label_1 = get_label();
		label_2 = get_label();
		
		op2 = gen_stat_code(clause->c_if.condition,stack,regno,addrno);
		if (op2.type != o_reg)		/* just incase its a boolean var - solo */
		{
			op1.type = o_reg;
			gen_instr(load,op1,op2,addrno);
		};

		gen_instr(branch_ne,label_1,op1,addrno);
		gen_stat_code(clause->c_if.action,stack,regno,addrno);
		gen_instr(branch,label_2,op1,addrno);
		put_label(label_1);
		gen_stat_code(clause->c_if.on_fail,stack,regno,addrno);
		put_label(label_2);
		break;

	case ct_while:	label_1 = get_label();
		label_2 = get_label();
		
		gen_instr(branch,label_2,op1,addrno);
		put_label(label_1);
		gen_stat_code(clause->c_while.action,stack,regno,addrno);
		put_label(label_2);
		op2 = gen_stat_code(clause->c_while.condition,stack,regno,addrno);
		if (op2.type != o_reg)		/* just incase its a boolean var - solo */
		{
			op1.type = o_reg;
			gen_instr(load,op1,op2,addrno);
		};
		gen_instr(branch_eq,label_1,op1,addrno);
		break;

	case ct_for:	label_1 = get_label();
		label_2 = get_label();

		gen_stat_code(clause->c_for.initial,stack,regno,addrno);
		gen_instr(branch,label_2,op1,addrno);

		gen_stat_code(clause->c_for.action,stack,regno,addrno);
		gen_stat_code(clause->c_for.step,stack,regno,addrno);

		put_label(label_2);
		op2 = gen_stat_code(clause->c_for.condition,stack,regno,addrno);
		if (op2.type != o_reg)		/* just incase its a boolean var - solo */
		{
			op1.type = o_reg;
			gen_instr(load,op1,op2,addrno);
		};
		gen_instr(branch_ne,label_1,op1,addrno);
		break;

	case ct_forall:	gen_forall_code(clause,stack,regno,addrno);
		break;

	case ct_bool: 	if (get_weight(clause->c_bool.left) < get_weight(clause->c_bool.left))
		{
			op1 = gen_stat_code(clause->c_bool.right,stack,regno,addrno);
			op  = gen_stat_code(clause->c_bool.left,stack,regno+1,addrno);
		}else{
			op  = gen_stat_code(clause->c_bool.left,stack,regno,addrno);
			op1 = gen_stat_code(clause->c_bool.right,stack,regno+1,addrno);
		};

		op = gen_instr(clause->c_bool.instr,op,op1,addrno);
		break;
		
	case ct_call:	if (clause->c_call.object == 0)
			gen_local_call(clause,stack,regno,addrno);
		else
			gen_remote_call(clause,stack,regno,addrno);
		break;
		
	case ct_array:	op.is_data = 0;
		if (clause->c_array.entry->c_type == ct_variable)
		{
			op.data = clause->c_array.entry->c_variable.value.data.integer;
			op.is_data = 1;
		}else
			op = gen_stat_code(clause->c_array.entry,stack,regno,addrno);
		
		op.type      = o_array_ref;
		op.offset    = clause->c_array.symbol->c_variable.value.data.symbol->offset;
		op.data_type = clause->c_array.symbol->c_variable.value.data.symbol->array_type;
		break;

	case ct_variable:
		if ((clause->c_variable.value.type == c_reference) 
		 || (clause->c_variable.value.type == c_addr))
		{
			if (clause->c_variable.value.data.symbol->access_type & a_param)
			{
				op.type   = o_parameter;
				op.offset = clause->c_variable.value.data.symbol->offset;
			
			} else if (clause->c_variable.value.data.symbol->access_type & a_global)
			{
				op.type = o_variable;
				op.offset = clause->c_variable.value.data.symbol->offset;
			} else {
				op.type = o_variable;
				op.offset = initial_size + clause->c_variable.value.data.symbol->offset;
			};

			/* if the reference value to a paramter is wanted then
			 * we need to know what type of variable it is.
			 */
			if (clause->c_variable.value.type == c_addr)
				if (clause->c_variable.value.data.symbol->access_type & a_ptr)
					op.type = o_ptr_ref;
				else
					op.type = o_var_ref;

			op.data_type = clause->c_variable.value.data.symbol->type;
		} else {
			op.type      = o_constant;
			op.offset    = 0;
			op.data_type = clause->c_variable.value.type;
			op.data      = (unsigned int) clause->c_variable.value.data.integer;
		};
		break;

	case ct_service:
		fprintf(object_file,"%s:\tmove.d\ta31,sp\n", clause->c_service.name->name);
		gen_block_code(clause->c_service.code,stack,regno,addrno);
		fprintf(object_file,"\tret\n");
		break;

	case ct_block:	gen_block_code(clause,stack,regno,addrno);
		break;

	default:	yyerror("SYSTEM ERROR: CLAUSE type invaild - internal parser error.");
	};

	return op;
}

