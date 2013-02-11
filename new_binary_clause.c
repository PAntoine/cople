/* $Revision: 1.2 $
 *		NEW BINARY CLAUSE
 *
 *	This function will create a binary clause. This function
 *	does no checking, it will leave that to the code generation
 *	and parsing functions.
 *
 *		(c) 1999 Peter Antoine.
 */

#include	<malloc.h>
#include	"codetree.h"


CLAUSE*	new_binary_clause(INST_TYPE type,CLAUSE* left,CLAUSE* right)
{
	CLAUSE	*code;
	DATA_TYPE	left_type,right_type;
	int	left_weight,right_weight,weight;

	code = calloc(1,sizeof(CLAUSE));

	code->c_type = ct_bool;
	code->c_bool.instr = type;
	
	/* need to type check the two paramters that are passed
	 * to the clause. They must be the same.
	 */

	right_type = get_type(right);
	left_type  = get_type(left);

	/* If either of the operands are calls then the type checking
	 * needs to be later when all the locals services are defined.
	 * Or, for object calls these are done at run-time. The type
	 * for runtime calls cannot be set until the whole expression
	 * tree can be walked.
	 */
	
	if (right_type == left_type || right_type == d_call || left_type == d_call)
	{
		code->c_bool.left  = (CLAUSE*) left;
		code->c_bool.right = (CLAUSE*) right;

		/* If left is greater than right then the weight is the left
		 * weight. If the the right is greater then that is the weight
		 * if there are both the same, then the weight needs to incresse
		 * as the result of the first to be evaluated needs to be held.
		 */
	
		left_weight = get_weight(left);
		right_weight = get_weight(right);

		if ( left_weight >  right_weight)
			weight = left_weight;
		else if (right_weight < left_weight)
			weight = right_weight;
		else
			weight = left_weight+1;

		code->c_bool.weight = right_weight+1;
	} else {
		yyerror("ERROR: type mismatch.");
		free(code);
		code = NULL;
	};
	return code;
}
