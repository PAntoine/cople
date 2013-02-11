/* $Revision: 1.2 $
 *		          GET_TYPE
 *
 *	This function will get the operand type of the operand. This
 *	is needed for type checking the binary type functions. This
 *	function does recursively call itself, this is for cases where
 *	the operand has operands in itself.
 *
 *	if #ptr then the data type is the type of the item being pointed
 *	at. If #var then the data type is d_memory. if just ptr then 
 *	the data type is also d_memory.
 *
 *		Copyright (c) 1999 Peter Antoine.
 */

#include	<stdlib.h>
#include	"codetree.h"

DATA_TYPE	get_type(CLAUSE	*operand)
{
	DATA_TYPE	operand_type;

	if (operand == NULL)
	{
		return d_null;
	};

	switch(operand->c_type)
	{
	case ct_variable:	if ((operand->c_variable.value.type == c_reference))
				if (operand->c_variable.value.data.symbol->access_type & a_ptr)
					operand_type = d_memory;
				else
					operand_type = operand->c_variable.value.data.symbol->type;
			
			else if (operand->c_variable.value.type == c_addr)
				if (operand->c_variable.value.data.symbol->access_type & a_ptr)
					operand_type = operand->c_variable.value.data.symbol->type;
				else
					operand_type = d_memory;
			else
				operand_type = operand->c_variable.value.type;
			break;

	case ct_array:		operand_type = operand->c_array.symbol->c_variable.value.data.symbol->array_type;
			break;

	case ct_unary:		operand_type = get_type(operand->c_unary.operand);
			break;
	
	case ct_bool:		operand_type = get_type(operand->c_bool.right);
			break;

	case ct_call:		operand_type = d_call;
			break;

	default:		yyerror("ERROR: Invalid type for an operand to a binary clause");
			operand_type = d_null;
			break;
	};

	return operand_type;
}
