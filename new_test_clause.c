/* $Revision: 1.2 $
 *		NEW TEST CLAUSE
 *
 *	This function will create a test clause. This function
 *	does no checking, it will leave that to the code generation
 *	and parsing functions.
 *
 *		(c) 1999 Peter Antoine.
 */

#include	<malloc.h>
#include	"codetree.h"


CLAUSE*	new_test_clause(CLAUSE* condition,CLAUSE* action)
{
	CLAUSE	*code;

	code = calloc(1,sizeof(CLAUSE));

	code->c_type = ct_test;
	code->c_test.condition  = (CLAUSE*) condition;
	code->c_test.action     = (CLAUSE*) action;

	return code;
}
