/* $Revision: 1.2 $
 *
 *		        END_LEVEL
 * 
 *	This function simply makes the current_level equal the owner of
 *	the current-current_level.
 *
 *		(c) 1998 Peter Antoine.
 */

#include	<malloc.h>
#include	"cople.h"
#include	"codetree.h"

CLAUSE*	end_level(void)
{
	int		count;
	CLAUSE		*result;
	extern int 		level_at;
	extern int		current_when[];
	extern int		current_memory;
	extern int		max_memory_size;
	extern CLAUSE		*current_block;
	extern LEVEL_LNK	*last_mem_lnk;

	
	result        = current_block;

	/* put back any whenevers that have lost there scope */
	for(count = 0; count < current_block->c_block.num_when;count++)
		current_when[current_block->c_block.whenc_list[count].exception_num] = 
				current_block->c_block.whenc_list[count].old_label;

	/* calc the locals space */
	result->c_block.local_size = current_memory - last_mem_lnk->prev->count;
	
	/* walk back up the block tree */
	if (result != NULL)
		current_block = result->c_block.owner;
	else
		current_block = NULL;

	level_at--;

	/* now do the memory list thing */

	if (current_memory > max_memory_size)
		max_memory_size = current_memory;

	last_mem_lnk   = last_mem_lnk->prev;
	current_memory = last_mem_lnk->count;
	
	free(last_mem_lnk->next);

	return result;
}
