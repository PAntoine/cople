/* $Revision: 1.2 $
 *		NEW BRANCH CLAUSE
 *
 *	This function will create a branch clause. This function
 *	does no checking, it will leave that to the code generation
 *	and parsing functions.
 *
 *		(c) 1999 Peter Antoine.
 */

#include	<malloc.h>
#include	"codetree.h"

CLAUSE*	new_branch_clause(INST_TYPE type, int label)
{
	CLAUSE	*code;

	code = calloc(1,sizeof(CLAUSE));

	code->c_type = ct_branch;
	code->c_branch.instr = type;
	code->c_branch.label = label;

	return code;
}
