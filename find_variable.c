/* $Revision: 1.2 $
 *
 *		        FIND_VARIABLE
 *
 *	This function will search the current local var list for the named
 *	symbol. If not found it will then search the block level tree, and
 *	the local var list for all blocks in the tree. If not found then
 *	the function will return a NULL pointer, else it will return the
 *	SYMBOL pointer. If the block is a service header block (ct_service)
 *	then the local table is searched and if the symbol is not found then
 *	the PARMETER_LIST that belongs to the service is then searched.
 * 
 *		Copyright (c) 1998 Peter Antoine.
 */

#include	<stdlib.h>
#include	"codetree.h"

SYMBOL*	find_variable( char* name )
{
	extern CLAUSE	*current_block;

	CLAUSE	*here,*owner;
	SYMBOL	**locals;
	SYMBOL	*result = NULL;
	int	count,size;

	here = current_block;

	if (here->c_type == ct_block)
	{
		locals = here->c_block.locals;
		size   = here->c_block.num_locals;
	} else {
		locals = here->c_service.locals;
		size   = here->c_service.num_locals;
	};

	/* need to search the whole tree untill it reaches the end
	 * or of course the name is found.
	 */

	while(result == NULL && here != NULL)
	{
		/* Quick note: not aligning the same variables in the CLAUSE
		 * union due to the fact that I keep changing them and I dont
		 * want the position to be depended on. Too hard to debug.
		 */
		
		if (here->c_type == ct_block)
		{	size   = here->c_block.num_locals;
			locals = here->c_block.locals;
			owner  = here->c_block.owner;
		}else {
			size   = here->c_service.num_locals;
			locals = here->c_service.locals;
			owner  = here->c_service.owner;
		};
		
		for(count=0;count< size && result == NULL; count++)
		{
			if (strcmp(name,locals[count]->name) == 0)
				result = locals[count];
		};

		here = owner;
	};

	return result;
}
