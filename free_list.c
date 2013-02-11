/* $Revision: 1.2 $
		    FREE LIST

	This function will free a list. It will free all the alocated
	memory that exists on the list.

		(c) 1998 P.Antoine.
					*/

#include	<malloc.h>
#include	"cople.h"

void	free_list( LIST* list)
{
	LIST_ENTRY	*entry,*next;

	/* start from the list to the end, removing all the
	 * emements in-between.
	 */

	entry = list->list;
	
	while(entry != NULL)
	{
		next  = entry->next;

		if (list->type == c_list)
			free_list(entry->data.list);

		free(entry);

		entry = next;
	}

	/* now clear the list head */
	free(list);
}

