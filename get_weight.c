/* $Revision: 1.2 $
 *		          GET_WEIGHT
 *
 *	This function will get weight of an operand. The weight is a rough guide
 *	to how many registers that the instruction will need. It is used to 
 *	order the evaluation of the binary instructions. This function is recursive.
 *
 *		Copyright (c) 1999 Peter Antoine.
 */

#include	"codetree.h"

int	get_weight(CLAUSE *operand)
{
	int weight;
	
	switch(operand->c_type)
	{
	case ct_variable:	if (operand->c_variable.value.type == c_reference)
				weight = 1;
			else
				weight = 0;
			break;

	case ct_array:		weight = get_weight(operand->c_array.entry) + 1;
			break;

	case ct_unary:		weight = get_weight(operand->c_unary.operand);
			break;
	
	case ct_bool:		weight = operand->c_bool.weight;
			break;

	case ct_call:		weight = -1;	/* max on function calls */
			break;

	default:		yyerror("ERROR: Invalid type for an operand to a binary clause");
			weight = 0;
			break;
	};

	return weight;
}
