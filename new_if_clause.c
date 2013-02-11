/* $Revision: 1.2 $
 *		NEW IF CLAUSE
 *
 *	This function will create a IF clause. This function
 *	does no checking, it will leave that to the code generation
 *	and parsing functions.
 *
 *		(c) 1999 Peter Antoine.
 */

#include	<malloc.h>
#include	"codetree.h"

CLAUSE*	new_if_clause(CLAUSE* condition,CLAUSE* action, CLAUSE *on_fail)
{
	CLAUSE	*code;

	code = calloc(1,sizeof(CLAUSE));

	code->c_type = ct_if;
	code->c_if.condition = condition;
	code->c_if.action    = action;
	code->c_if.on_fail   = on_fail;

	return code;
}
