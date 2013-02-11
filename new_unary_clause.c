/* $Revision: 1.2 $
 *		NEW UNARY CLAUSE
 *
 *	This function will create a unary clause. This function
 *	does no checking, it will leave that to the code generation
 *	and parsing functions.
 *
 *		(c) 1999 Peter Antoine.
 */

#include	<malloc.h>
#include	"codetree.h"


CLAUSE*	new_unary_clause(INST_TYPE instr, CLAUSE* operand)
{
	CLAUSE	*code;

	code = calloc(1,sizeof(CLAUSE));

	code->c_type = ct_unary;
	code->c_unary.instr   = instr;
	code->c_unary.operand = operand;

	return code;
}
