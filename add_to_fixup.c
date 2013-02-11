/* $Revision: 1.2 $
 *
 *		           ADD to FIXUP
 *
 *	This function will add an offset to a reference variable that needs
 *	to be fixed up at run time.
 *
 *		Copyright (c) 1999 Peter Antoine
 */

#include	<memory.h>
#include	"cople.h"

void	add_to_fixup(int offset)
{
	LEVEL_LNK		*last,*walk;
	extern LEVEL_LNK	*fix_up_list;

	if (fix_up_list == NULL)
	{
		fix_up_list = (LEVEL_LNK*) calloc(1,sizeof(LEVEL_LNK));
		fix_up_list->count = offset;
	}else{
		walk = fix_up_list;
		
		while(walk != NULL)
		{
			last = walk;
			walk = walk->next;
		};

		last->next = (LEVEL_LNK*) calloc(1,sizeof(LEVEL_LNK));
		last->next->count = offset;
		last->next->prev  = last;
	};
}
