/* $Revision: 1.2 $
 *		NEW FORALL CLAUSE
 *
 *	This function will create a forall clause. This function
 *	does no checking, it will leave that to the code generation
 *	and parsing functions.
 *
 *		(c) 1999 Peter Antoine.
 */

#include	<malloc.h>
#include	"codetree.h"


CLAUSE*	new_forall_clause(LIST* lookup, CLAUSE* variable, CLAUSE* action)
{
	CLAUSE	*code;

	code = calloc(1,sizeof(CLAUSE));

	code->c_type = ct_forall;
	code->c_forall.lookup  = lookup;
	code->c_forall.ref_var = variable;
	code->c_forall.action  = action;

	return code;
}
