/* $Revision: 1.2 $
 *		NEW call CLAUSE
 *
 *	This function will create a call clause. This function
 *	does no checking, it will leave that to the code generation
 *	and parsing functions.
 *
 *		(c) 1999 Peter Antoine.
 */

#include	<malloc.h>
#include	"codetree.h"


CLAUSE*	new_call_clause(CLAUSE* object, CONSTANT* name, LIST* list)
{
	CLAUSE	*code;

	code = calloc(1,sizeof(CLAUSE));

	code->c_type = ct_call;
	code->c_call.object  = object;
	code->c_call.service = name;
	code->c_call.list    = list;

	return code;
}
