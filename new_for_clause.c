/* $Revision: 1.2 $
 *		NEW FOR CLAUSE
 *
 *	This function will create a for clause. This function
 *	does no checking, it will leave that to the code generation
 *	and parsing functions.
 *
 *		(c) 1999 Peter Antoine.
 */

#include	<malloc.h>
#include	"codetree.h"


CLAUSE*	new_for_clause(CLAUSE* initial,CLAUSE* condition,CLAUSE* step,CLAUSE* action)
{
	CLAUSE	*code;

	code = calloc(1,sizeof(CLAUSE));

	code->c_type = ct_for;
	code->c_for.condition = condition;
	code->c_for.initial   = initial;
	code->c_for.step      = step;
	code->c_for.action    = action;

	return code;
}
