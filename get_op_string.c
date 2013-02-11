/* $revision$
 *
 *		     Get Operand String
 *
 *	This is a simple function that returns a string for the operand
 *	it will also load the array base if needed.
 *
 *		Copyright (c) 1999 Peter Antoine
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "codetree.h"
#include "cople.h"

char*	get_op_string(OPERAND op,int address,int *new_address,char *op_string)
{
	int	temp,so_temp;
	extern int	level_at;
	extern FILE	*object_file;

	temp = address;

	switch(op.type)
	{
	case o_array_ref:
		/* need to load the array base */
		fprintf(object_file,"\tmove.d\ta%d,a30\n",temp);
		if (op.offset != 0)
			fprintf(object_file,"\tadd.d\ta%d,#%d\n",temp,op.offset);
	
		/* must now reference the array*/
		if (!op.is_data)
			sprintf(op_string,"a%d(d%d)",temp++,op.reg);
		else
			sprintf(op_string,"a%d(#%d)",temp++,op.data);
	
		break;
	case o_parameter:
		sprintf(op_string,"a31(#%d)",op.offset);
		break;
	case o_variable:
		sprintf(op_string,"a30(#%d)",op.offset);
		break;
	case o_reg:	
		sprintf(op_string,"d%d",op.reg);
		break;
	case o_constant:
		sprintf(op_string,"#%d",op.data);
		break;
	case o_var_ref:
		fprintf(object_file,"\tmove.d\td%d,a30\n",op.reg);
		fprintf(object_file,"\tadd.d\td%d,#%d\n",op.reg,op.offset);
		sprintf(op_string,"d%d",op.reg);
		break;

	case o_ptr_ref:	fprintf(object_file,"\tmove.d\ta%d,a30(#%d)\n",temp,op.offset);
		sprintf(op_string,"a%d(#0)",temp++);
		break;
	
	default:
		yyerror("invalid operand - internal failure");
	};

	*new_address = temp;
	return op_string;
}
