/* $Revision: 1.2 $
 *
 *		LIST PRODUCT
 *
 *	This function walks an integer list and then generates a product
 *	of the contents. This is used to access arrays.
 *
 *		(c) 1999 Peter Antoine.
 */

#include	<malloc.h>
#include	"cople.h"

int	list_product(LIST *int_list)
{
	int	*dimensions;
	int	count = 0;
	int	product = 1;
	LIST_ENTRY	*pointer;
	
	if (int_list->type != l_int)
	{
		printf("ERROR: trying to make an array not from an int_list \n");
		return 0;
	};

	if (int_list->list == NULL)
	{
		printf("ERROR: not allowed to have zero dimensioned array. \n");
		return 0;
	};

	/* set up the muliple dimensions array */

	pointer = int_list->list;
	while (pointer != NULL)
	{
		product *= pointer->data.integer;
		
		pointer = pointer->next;
	};

	return product;
}
