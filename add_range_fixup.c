/* $Revision: 1.2 $
 *
 *		           ADD RANGE FIXUP
 *
 *	This function will add an offset to a reference variable that needs
 *	to be fixed up at run time.
 *
 *		Copyright (c) 1999 Peter Antoine
 */

#include	<memory.h>
#include	"cople.h"

void	add_range_fixup(int offset,int count)
{
	RANGE_FIXUP		*last,*walk;
	extern RANGE_FIXUP	*range_list;

	if (range_list == NULL)
	{
		range_list = (RANGE_FIXUP*) calloc(1,sizeof(RANGE_FIXUP));
		range_list->base  = offset;
		range_list->count = count;
	}else{
		walk = range_list;
		
		while(walk != NULL)
		{
			last = walk;
			walk = walk->next;
		};

		last->next = (RANGE_FIXUP*) calloc(1,sizeof(RANGE_FIXUP));
		last->next->base  = offset;
		last->next->count = count;
		last->next->prev  = last;
	};
}
