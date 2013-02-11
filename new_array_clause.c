/* $Revision: 1.2 $
 *		NEW ARRAY CLAUSE
 *
 *	This function will create a array clause. This function
 *	does no checking, it will leave that to the code generation
 *	and parsing functions.
 *
 *		(c) 1999 Peter Antoine.
 */

#include	<malloc.h>
#include	"codetree.h"


CLAUSE*	new_array_clause(CLAUSE* symbol, LIST* calc)
{
	CLAUSE	*code;

	code = calloc(1,sizeof(CLAUSE));

	code->c_type = ct_array;
	code->c_array.symbol = symbol;

	if (symbol->c_variable.value.data.symbol == NULL)
		code->c_array.entry = NULL;
	else
		code->c_array.entry = calculate_array_entry(symbol->c_variable.value.data.symbol,calc);

	return code;
}
