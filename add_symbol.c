/* $Revision: 1.2 $
		ADD SYMBOL

	This function will add a symbol to the symbol table, and return
	a pointer to the symbol created. If the return pointer is NULL
	then the symbol was invaild, that is the name was allready in
	use in the local block.

		(c) 1998 P.Antoine.
					*/

#include	<malloc.h>
#include	"codetree.h"

SYMBOL*	add_symbol(char *name)
{
	extern HASH_TABLE	symbol_table;
	extern CLAUSE		*current_block;

	SYMBOL		*result;
	SYMBOL		**locals;
	HASH_BLOCK		*hash_block;
	int		found,finished,got_it,count,size;
	long		hash;
	char		*name_ptr = NULL;

	/* see if the name is in the local list of names that
	 * have been created in the local block. Remeber if the
	 * name is allready local then it cannot be used again,
	 * well have none of that variable shadowing here!
	 */
	
	if (current_block->c_type == ct_block){
		locals = current_block->c_block.locals;
		size   = current_block->c_block.num_locals;
	}else{
		locals = current_block->c_service.locals;
		size   = current_block->c_service.num_locals;
	};
	found = 0;
	
	for( count = 0; count < size  && !found ; count++)
	{
		if (strcmp(name,locals[count]->name) == 0)
		{
			yyerror("ERROR: symbol has been defined inside the local block");
			result = NULL;
			found = 1;
		}
	}

	/* hash into the symbol table and find if the name has
	 * been used before. If so get the pointer for the 
	 * name, else create a new name.
	 */

	if (!found)
	{
		/* set up the hash value and then find the first entry of
		 * the hash table that referers to the hash chain.
		 */
		finished = 0;
		got_it   = 0;
		
		hash = (long) name[0];	/* use the first 4 bytes as the hash code */
		hash = hash / HASH_NUM;
		
		hash_block = symbol_table.block[hash];
		if (hash_block == NULL)
		{
			symbol_table.block[hash] = calloc(1,sizeof(HASH_BLOCK));
			hash_block = symbol_table.block[hash];
		}
		
		/* now search the hash block chain, there are no
		 * empty slots in the chain, so the first empty
		 * slot is the end of the chain.
		 */
		
		while(!finished)
		{
			for(count = 0; count < HASH_BLOCK_SZ && !finished; count++)
			{
				if ( hash_block->index[count].name == NULL )
				{	finished = 1;
					found    = 1;
					got_it   = count;
				} 
				else if (strcmp(name,hash_block->index[count].name) == 0)
					name_ptr = hash_block->index[count].name;
			}

			/* if not found then get the next block
			 * in the chain.
			 */
			if (!finished && hash_block->next != NULL)
				hash_block = hash_block->next;
			else
				finished = 1;
		}			

		if (!found)
		{	hash_block->next = calloc(1,sizeof(HASH_BLOCK));
			hash_block = hash_block->next;
		}

		/* copy the name the entry in the hash table then the
		 * address needs to be returned. To save some memory
		 * if there is a name in use - might as well reuse it.
		 */

		hash_block->index[got_it].block = current_block;
	
		if (name_ptr != NULL)
			hash_block->index[got_it].name = name_ptr;
		else{
			hash_block->index[got_it].name = malloc(strlen(name)+1);
			strcpy(hash_block->index[got_it].name,name);
		};

		/* Add the symbol to the local symbols table */
		if (current_block->c_type == ct_block)
		{
			current_block->c_block.locals = realloc(current_block->c_block.locals,
					(sizeof(SYMBOL*)*(current_block->c_block.num_locals+1)));
			current_block->c_block.locals[current_block->c_block.num_locals] = &hash_block->index[got_it];
			current_block->c_block.num_locals++;
		}else{
			current_block->c_service.locals = realloc(current_block->c_service.locals,
					(sizeof(SYMBOL*)*(current_block->c_service.num_locals+1)));
			current_block->c_service.locals[current_block->c_service.num_locals] = &hash_block->index[got_it];
			current_block->c_service.num_locals++;
		};
		result = &hash_block->index[got_it];
	}

	return result;
}
