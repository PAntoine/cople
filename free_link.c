/* $Revision: 1.2 $
 *		        FREE LINK
 *
 *	This function will free a level link list. This just walks
 *	down the list a returns the memeory.
 *
 *		Copyright (c) 1999 Peter Antoine.
 */

#include	<stdlib.h>
#include	"cople.h"

void	free_link(LEVEL_LNK *list)
{
	LEVEL_LNK	*walk,*hold;

	walk = list;

	while( walk != NULL)
	{
		hold = walk;
		walk = list->next;

		free(hold);
	};
}
