/* $Revision: 1.2 $
 *		NEW WHEN CLAUSE
 *
 *	This function will create a whenever clause. This function
 *	does no checking, it will leave that to the code generation
 *	and parsing functions.
 *
 *		(c) 1999 Peter Antoine.
 */

#include	<malloc.h>
#include	"cople.h"
#include	"codetree.h"

CLAUSE*	new_when_clause(char* name,CLAUSE* action)
{
	int		count,when_num,found=0;
	CLAUSE		*code;
	WHENEVER_LIST		*end_when,*walk_when;
	extern int		when_label;
	extern int		current_when[];
	extern CLAUSE		*current_block;
	extern WHENEVER_LIST	*when_list;

	for (count=1; count < NUM_EXCEPTIONS && !found;count++)
	{
		if (strcmp(name,exception[count]) == 0)
			found = count;
	};
		
	code = calloc(1,sizeof(CLAUSE));

	if (found)
	{
		/* amend the when label list */
		when_num = current_block->c_block.num_when;
		current_block->c_block.whenc_list = realloc(current_block->c_block.whenc_list, ((when_num+1)*sizeof(WHEN_CHANGE)));
		current_block->c_block.whenc_list[when_num].exception_num = found;
		current_block->c_block.whenc_list[when_num].old_label     = current_when[found];

		current_block->c_block.num_when++;
		
		/* add the new clause to the whenever list */
		if (when_list == NULL){
			when_list = calloc(1,sizeof(WHENEVER_LIST));
			end_when = when_list;
		} else {
			walk_when = when_list;
		
			while (walk_when != NULL)
			{
				end_when = walk_when;
				walk_when = walk_when->next;
			};
		
			end_when->next = calloc(1,sizeof(WHENEVER_LIST));
			end_when = end_when->next;
		};
		end_when->when_code = action;

		/* finally set up the clause */	
		code->c_type = ct_whenever;
		code->c_whenever.exception  = found;
		code->c_whenever.label      = ++when_label;
		code->c_whenever.code       = action;

		/* update current when */
		current_when[found] = when_label;
	}

	return code;
}
