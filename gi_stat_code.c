/* $Revision: 1.2 $ (generate intel)
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
#include	"intel.h"
#include	"cople.h"
#include	"codetree.h"

INTEL_OPERAND	gi_stat_code(CLAUSE *clause,LEVEL_LNK *stack,int alloc_reg,int is_if)
{
	LIST_ENTRY		*walk;
	INTEL_OPERAND		op = {0,0,0,0,d_null,{0,0,0,0,0,0,0,0,0}};
	INTEL_OPERAND		op1 = {0,0,0,0,d_null,{0,0,0,0,0,0,0,0,0}};
	int		count,label_1,label_2;
	int		da_bool;
	char		*left_op;
	char		op_str1[50],op_str2[50];
	extern int		initial_size;
	extern FILE		*object_file;
	extern int		max_save_space;
	extern int		current_saved;
	extern int		current_label;
	extern long		*save_space;
	extern VIRT_REGISTER	*virt_reg;
	extern REAL_REGISTER	*real_reg;

	switch(clause->c_type)
	{
	case ct_whenever:
		fprintf(object_file,"\tmov\tdword [%d],when_label_%d\n",
			clause->c_whenever.exception*4,
			clause->c_whenever.label);
		op.state.null = 0;
		break;
	
	case ct_test:
		/* if x then y fi */
		if (is_if == 0)
			label_1 = ++current_label;
		else
			label_1 = is_if;
			
		op = gi_stat_code(clause->c_test.condition,stack,0,label_1);
		free_register(&op);
		op = gi_stat_code(clause->c_test.action,stack,0,1);
		
		if (is_if == 0)
			fprintf(object_file,"label_%d:\n",label_1);
		
		op.state.null = 1;
		break;
	
	case ct_if:	/* if x then y else z fi */
		label_1 = ++current_label;
		
		if(is_if == 0)
			label_2 = ++current_label;
		else
			label_2 = is_if;

		/* if x */
		op = gi_stat_code(clause->c_if.condition,stack,0,label_1);
		free_register(&op);
		
		/* else z*/
		op = gi_stat_code(clause->c_if.on_fail,stack,0,label_2);
		fprintf(object_file,"\tjmp\tlabel_%d\n", label_2);
		free_register(&op);
		
		/* then y */
		fprintf(object_file,"label_%d:\n",label_1);
		op = gi_stat_code(clause->c_if.action,stack,0,0);
		
		/* fi and no return from an if */
		if (is_if == 0)
			fprintf(object_file,"label_%d:\n",label_2);
		op.state.null = 1;
		break;	
	
	case ct_while:	
		label_1 = ++current_label;
		label_2 = ++current_label;
		/* jump to the conditional */
		fprintf(object_file,"\tjmp\tlabel_%d\n", label_2);
		
		/* now the code */
		fprintf(object_file,"label_%d:\n", label_1);
		op = gi_stat_code(clause->c_while.action,stack,0,0);
		free_register(&op);

		/* now the conditional */
		fprintf(object_file,"label_%d:\n", label_2);
		op = gi_stat_code(clause->c_while.condition,stack,0,label_1);
		
		op.state.null = 1;
		break;
		
	case ct_for:
		label_1 = ++current_label;
		label_2 = ++current_label;

		if (clause->c_for.initial != NULL){
			op = gi_stat_code(clause->c_for.initial,stack,0,0);
			free_register(&op);
		};
		
		fprintf(object_file,"label_%d:\n", label_1);
		op = gi_stat_code(clause->c_for.condition,stack,0,label_2);
		free_register(&op);
		
		op = gi_stat_code(clause->c_for.action,stack,0,0);
		free_register(&op);
		
		if (clause->c_for.step != NULL){
			op = gi_stat_code(clause->c_for.step,stack,0,0);
			op.state.null = 1;
		};

		fprintf(object_file,"\tjmp\tlabel_%d\n", label_1);
		fprintf(object_file,"label_%d:\n", label_2);
		break;
		
	case ct_forall: gi_forall_code(clause,stack);
		op.state.null = 1;
		break;

	case ct_unary:	/* only - unary instruction is a not - at the moment */
		op1 = gi_stat_code(clause->c_unary.operand,stack,0,0);
		
		if (op1.state.use_reg)
		{	/* if register it can be changed and returned */
			gi_operand(&op1,op_str1);
			fprintf(object_file,"\tnot\t%s\n",op_str1);
			op = op1;
		} else {
			/* this is a memory reference cannot be changed
			 * will need to get a register to change and return.
			 */
			allocate_register(&op);
			gi_operand(&op1,op_str1);
			gi_operand(&op,op_str2);

			if (INTEL_SIZE[op1.type] != "dword")
				fprintf(object_file,"\tmovzx\t%s,%s %s\n",op_str2,INTEL_SIZE[op1.type],op_str1);
			else
				fprintf(object_file,"\tmov\tdword %s,%s\n",op_str2,op_str1);
			
			fprintf(object_file,"\tnot\t%s\n",op_str2);
			free_register(&op1);
		};

		if (is_if)
		{
			/* is if generate a cmp and jump. The register is
			 * also no longer needed so throw that away.
			 */
			fprintf(object_file,"\tcmp\t%s %s, 0\n",INTEL_SIZE[op1.type],op_str2);
			fprintf(object_file,"\tje\tlabel_%d\n",is_if);
			free_register(&op);
		};
			
		break;
	case ct_bool:
		if (get_weight(clause->c_bool.left) < get_weight(clause->c_bool.right))
		{
			op1 = gi_stat_code(clause->c_bool.right,stack,0,0);
			op  = gi_stat_code(clause->c_bool.left,stack,1,0);
		} else {
			op  = gi_stat_code(clause->c_bool.left,stack,0,0);
			op1 = gi_stat_code(clause->c_bool.right,stack,1,0);
		};
		
		op = gi_bool_code(clause->c_bool.instr,op,op1,is_if);
		op1.state.null = 1;
		break;
		
	case ct_call:	gi_remote_call(clause,stack,0,0);
		op.state.null = 1;
		break;
	case ct_array:
		/* the entry point */
		if (clause->c_array.entry->c_type == ct_variable)
		{
			/* if its a constant value set it */
			free_register(&op);
			op.value = clause->c_array.entry->c_variable.value.data.integer;
		
			op.state.use_reg = 0;
			op.state.immediate = 1;
		} else 
			op  = gi_stat_code(clause->c_array.entry,stack,1,0);
		
		/* now sort out the base offset */
		op.base  = clause->c_array.symbol->c_variable.value.data.symbol->offset;
			
		/* if not global then its in the variable space */
		if (((clause->c_variable.value.type == c_reference) || (clause->c_variable.value.type == c_addr)) &&
		   (!(clause->c_variable.value.data.symbol->access_type & a_global)))
			op.base += initial_size;
			
		op.type  = clause->c_array.symbol->c_variable.value.data.symbol->array_type;
		op.state.use_base = 1;
		op.state.reference = 1;
		break;

	case ct_variable:
		if (is_if != 0)
		{
			da_bool = 0;
			if (clause->c_variable.value.type == c_reference)
				if( clause->c_variable.value.data.symbol->type == d_boolean)
					da_bool = 1;
			else
				if( clause->c_variable.value.type == d_boolean)
					da_bool = 1;
		
			/* if the type is bool and is_if is set then this is
			 * a boolean condition. This needs to check the value
			 * against 0 and jump if equal.
			 */
			if (da_bool)
			{
				op = gi_stat_code(clause,stack,0,0);	/* its all here allready */
				gi_operand(&op,op_str1);
				fprintf(object_file,"\tcmp\t%s %s,0\n",
					INTEL_SIZE[d_boolean], op_str1);
				fprintf(object_file,"\tje\tnear label_%d\n",is_if);
				free_register(&op);
			} else 
				comp_error(WARNING,"variable only clause - passed with an if label - not a boolean");
		}
		else if ((clause->c_variable.value.type == c_reference) || (clause->c_variable.value.type == c_addr))
		{
			op.value = clause->c_variable.value.data.symbol->offset;
			op.state.immediate = 1;	/* always use the offset */
			
			/* is the variable a parameter that is being accessed */
			if (clause->c_variable.value.data.symbol->access_type & a_param)
				op.state.parameter = 1;
			
			/* if not global then its in the variable space */
			if (!(clause->c_variable.value.data.symbol->access_type & a_global))
				op.value += initial_size;
			
			/* is it a pointer ? */
			if (clause->c_variable.value.data.symbol->access_type & a_ptr)
				op.state.pointer = 1;

			/* is it a address or reference */
			if (clause->c_variable.value.type == c_reference)
				op.state.reference = 1;
			
			/* string are always pointers - default look at the content */
			if (clause->c_variable.value.data.symbol->type == d_string)
			{
				if (clause->c_variable.value.type == c_reference)
					op.state.pointer = 1;
					
				op.state.reference = 1;
			};


			/* is it a simple variable reference */
			if ((clause->c_variable.value.data.symbol->access_type == 0) ||
				(clause->c_variable.value.data.symbol->access_type == a_global))
			{
				op.state.reference = 1;
			}
		}else {
			op.state.immediate = 1;
			op.value = clause->c_variable.value.data.integer;
		};

		/* be wise to set the data size (type) */
		if (clause->c_variable.value.type == c_reference)
			op.type = clause->c_variable.value.data.symbol->type;
		else
			op.type = clause->c_variable.value.type;
		break;
		
	case ct_service:
		fprintf(object_file,"%s:\n", clause->c_service.name->name);
		gi_block_code(clause->c_service.code,stack,0);
		fprintf(object_file,"\tret\n");
		op.state.null = 1;
		break;

	case ct_block:	gi_block_code(clause,stack,0);
		op.state.null = 1;
		break;

	default:	yyerror("SYSTEM ERROR: CLAUSE type invaild - internal parser error.");
	};

	/* Free any registers that are allocated and are no longer needed.
	 * allways free op1 as only op can be passed on. If op is null
	 * then free that.
	 */

	if(op.state.null)
		free_register(&op1);
	
	if (op.state.null){
		free_register(&op);
		op.state.use_reg = 0;
	};
	
	return op;
}

