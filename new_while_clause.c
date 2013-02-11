/* $Revision: 1.2 $
 *		NEW WHILE CLAUSE
 *
 *	This function will create a while clause. This function
 *	does no checking, it will leave that to the code generation
 *	and parsing functions.
 *
 *		(c) 1999 Peter Antoine.
 */

#include	<malloc.h>
#include	"codetree.h"


CLAUSE*	new_while_clause(CLAUSE* condition,CLAUSE* action)
{
	CLAUSE	*code;

	code = calloc(1,sizeof(CLAUSE));

	code->c_type = ct_while;
	code->c_while.condition = (CLAUSE*) condition;
	code->c_while.action    = (CLAUSE*) action;

	return code;
}
