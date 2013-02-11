/* $Revision: 1.2 $
 *
 *		        ADD CLAUSE
 * 
 *	This function will add a clause to the current block. It gets the address
 *	of the current block from the global varaiable current block. The clause list
 *	for this block is extended by one, before the now clause pointer is added.
 *
 *		Copyright (c) 1999 Peter Antoine.
 */

#include	<malloc.h>
#include	"cople.h"
#include	"codetree.h"

void	add_clause(CLAUSE* clause)
{
	extern CLAUSE	*current_block;
	
	current_block->c_block.instr_list = realloc(current_block->c_block.instr_list,
				(sizeof(CLAUSE*)*(current_block->c_block.num_instr+1)));
	current_block->c_block.instr_list[current_block->c_block.num_instr] = clause;
	current_block->c_block.num_instr++;
}
