/* $Revision: 1.2 $
 *
 *		   CONV_LIST_TO_ARRAY 
 * 
 *	This function will convert an int list to an array. The
 *	format of the array is as follows:
 *
 *		[dimensions][d0,d1,...,dn][ data space ]
 *
 *	where n = dimensions. The size of data space is the 
 *	(product of d0 to dn) * data_type size. This structure
 *	will allow for in flight boundary checking for programs
 *	that use variables to access the array.
 *
 *		(c) 1998 Peter Antoine.
 */

#include	<malloc.h>
#include	"cople.h"

SYMBOL*	conv_list_to_array(LIST *int_list, char *sym_name,DATA_TYPE type,int a_type)
{
	int	dim,access_type,product,size;
	short int	*dim_list;
	LIST_ENTRY	*pointer;
	SYMBOL	*new_symbol;
	extern int	level_at;
	extern int	initial_size;
	extern char	*initial_space;
	extern int	current_memory;
	
	if (int_list->type != l_int)
	{
		printf("ERROR: trying to define an array not from an int_list \n");
		return NULL;
	};


	if (int_list->list == NULL)
	{
		printf("ERROR: not allowed to have zero dimensioned array. \n");
		return NULL;
	};

	/* set up the muliple dimensions array */


	/* for multi-dimensional arrays the dimensions of the lower
	 * dimensions need to be stored in the symbol. But, first 
	 * an array (int array) needs to be set up with the boundarys
	 * of the lower (inner) dimensions.
	 */

	dim = 0;
	product  = 1;
	pointer  = int_list->list;
	dim_list = calloc(int_list->list_size,sizeof(short int));

	while(pointer != NULL)
	{
		dim_list[dim++] = (short int) pointer->data.integer;
		product *= pointer->data.integer;
		pointer  = pointer->next;
	};

	size = DATA_TYPE_SIZE[type] * product;

	/* now a multi-dimensional array */
	access_type = (a_type | a_multi);

	/* Now need to create a new symbol then allocate the data
	 * structure that has just been created to it.
	 */
	
	new_symbol = add_symbol(sym_name);

	new_symbol->type	= d_array;
	new_symbol->array_type	= type;
	new_symbol->size	= size;
	new_symbol->num_dim	= int_list->list_size;
	new_symbol->dimensions	= dim_list;
	new_symbol->data	= 0;
	new_symbol->access_type	= (a_var | access_type);

	/* global or local ? */
	if (level_at == 1)
		new_symbol->offset = initial_size;
	else
		new_symbol->offset = current_memory;


	/* Need to check to see if the variable is a global - if so space
	 * must be allocated in the initialised space.
	 */

	if (level_at == 1)
	{
		initial_space = realloc(initial_space,initial_size + size);
		memset(initial_space+initial_size,'\0',size);
		initial_size += size;
		new_symbol->access_type |= a_global;
	} else
		current_memory += new_symbol->size;

	/* say goodbye to the list that has been used to create the array.
	 * and take the final bow....
	 */

	free_list(int_list);
	return new_symbol;
}
