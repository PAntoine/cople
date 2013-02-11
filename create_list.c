/* $Revision: 1.2 $
		CREATE_LIST

	This function is part of the COPLE compiler. This function
	will create a list header and it will then return the address
	of the headers start point. This structure will be used to 
	create other constant structures.

		P.Antoine.
					*/

#include	<malloc.h>
#include	"cople.h"

LIST*	create_list(LIST_TYPE type)
{
	LIST	*result;

	result = (LIST *) calloc(1,sizeof(LIST));

	if(result != NULL)
		result->type = type;

	return result;
}









