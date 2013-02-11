/* $Revision: 1.2 $
 *		NEW CONSTANT CLAUSE
 *
 *	This function will create a constant clause. This function
 *	simply wraps a constant in the CLAUSE structure, this 
 *	makes life easier in the later stages on compilation.
 *
 *		(c) 1999 Peter Antoine.
 *
 */

#include	<malloc.h>
#include	"codetree.h"

CLAUSE*	new_const_clause(CONSTANT *constant)
{
	CLAUSE	*code;

	code = calloc(1,sizeof(CLAUSE));

	code->c_type           = ct_variable;
	memcpy(&code->c_variable.value,constant,sizeof(CONSTANT));	/* not c_variable is not a pointer */

	return code;
}
