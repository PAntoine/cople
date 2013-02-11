/* $Revision: 1.2 $
 *
 *		        NEW_LEVEL
 * 
 *	This function will create a new scope level in the block tree for the
 *	code. The block holds the local scope of the program, which begins 
 *	with a BEGIN. All variables (and constants) that are created in the
 *	block are added to the local var list that is created by this call.
 *
 *		(c) 1998 Peter Antoine.
 */

#include	<malloc.h>
#include	"cople.h"
#include	"codetree.h"

void	new_level(void)
{
	extern int		level_at;
	extern int		current_memory;
	extern LEVEL_LNK	*last_mem_lnk;
	extern CLAUSE		*current_block;
	
	CLAUSE	*new_block;
	
	/* change the block tree */
	
	new_block = calloc(1,sizeof(CLAUSE));

	new_block->c_type = ct_block;
	new_block->c_block.owner      = current_block;
	new_block->c_block.local_base = current_memory;
	new_block->c_block.locals     = (SYMBOL**) calloc(1,sizeof(SYMBOL*));
	new_block->c_block.instr_list = (CLAUSE**) calloc(1,sizeof(CLAUSE*));
	
	current_block = new_block;	
	level_at++;

	/* now change the memory list */

	last_mem_lnk->count      = current_memory;
	last_mem_lnk->next       = (LEVEL_LNK*) calloc(1,sizeof(LEVEL_LNK));
	last_mem_lnk->next->prev = last_mem_lnk;
	last_mem_lnk	         = last_mem_lnk->next;
}
