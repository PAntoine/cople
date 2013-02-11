/* $Revision: 1.2 $
 *
 *		   CONV_CONST_LIST_TO_ARRAY 
 * 
 *	This function will create an array from a list of constants. It will
 *	check to see if the constant being evaluated matches the type of the
 *	array. If it does not matches it will raise an error.
 *
 *		(c) 1998 Peter Antoine.
 *
 *	Note: all arrays except a string are just offsets from the start of
 *	the array, and the first bytes are the array size. With the string
 *	the array is a list of pointers to the strings.
 */

#include	<malloc.h>
#include	"cople.h"

SYMBOL*	conv_const_list_to_array(LIST *const_list, char *sym_name,DATA_TYPE type,int a_type)
{
	int	*int_pointer;
	int	access_type,count,size,index_size,old_size;
	int	dim = 0,data_size = 0;
	char	*char_pointer;
	float	*float_pointer;
	char	*data_space;
	short int	*dim_list;
	long int	*index;
	LIST_ENTRY	*pointer;
	LEVEL_LNK	*level,*temp;
	SYMBOL	*new_symbol;
	
	extern int	level_at;
	extern int	initial_size;
	extern int	current_memory;
	extern char	*initial_space;

	
	/* Walk the data array space and find any elements that are not of
	 * the correct type. Also add the data to the data space.
	 */

	data_space = NULL;
	data_size  = 0;
	level = calloc(1,sizeof(LEVEL_LNK));	/* only needed for c_list */

	pointer = const_list->list;

	while (pointer != NULL)
	{
		old_size = data_size;
		
		if (pointer->type != type && pointer->type != c_list)
			yyerror("ERROR: element in constant array does not match the array type.");

		switch(pointer->type)
		{
		case c_list:	data_space = create_block_from_list(data_space,(LIST*)pointer->data.list,level,&data_size,type);
			break;

		case c_string:	 data_size   += DATA_TYPE_SIZE[d_string];
			data_space   = realloc(data_space,data_size);
			char_pointer  = (char*) (data_space + old_size);
			conv_int_to_bigendian(char_pointer,pointer->data.string);
			break;

		case c_int:	 data_size   += DATA_TYPE_SIZE[d_int];
			data_space   = realloc(data_space,data_size);
			char_pointer  = (char*) (data_space + old_size);
			conv_int_to_bigendian(char_pointer,pointer->data.integer);
			break;

		case c_char:	data_size    += DATA_TYPE_SIZE[d_char];
			data_space    = realloc(data_space,data_size);
			char_pointer  = (char*) (data_space + old_size);
			*char_pointer = pointer->data.character;
			break;
		
		case c_boolean:	data_size   += DATA_TYPE_SIZE[d_boolean];
			data_space   = realloc(data_space,data_size);
			int_pointer  = (int*) (data_space + old_size);
			*int_pointer = pointer->data.boolean;
			break;
		
		case c_real:	data_size     += DATA_TYPE_SIZE[d_real];
			data_space     = realloc(data_space,data_size);
			float_pointer  = (float*) (data_space + old_size);
			*float_pointer = pointer->data.flt_point;
			break;

		default:	yyerror("WARNING: using a variable type not implemented in lists!");
		};
		pointer = pointer->next;
	}

	/* for multi-dimensional arrays the dimensions of the lower
	 * dimensions need to be stored in the symbol. But, first 
	 * an array (int array) needs to be set up with the boundarys
	 * of the lower (inner) dimensions.
	 */

	if (const_list->list->type == c_list)
	{
		/* this to note that the list itself is a 
		 * dimension and needs to be added.
		 */
		dim++;
		dim_list = calloc(1,sizeof(short int));
		dim_list[0] = const_list->list_size;

		while(level != NULL)
		{
			dim_list = realloc(dim_list,(dim * sizeof(short int)));
			dim_list[dim] = (short int) level->count;
			temp  = level;
			level = level->next;
			free(temp);
			dim++;
		};

		/* now a multi-dimensional array */
		access_type = (a_type | a_multi);
	}else{
		dim = 1;
		access_type = a_type;
		dim_list    = calloc(1,sizeof(short int));
		dim_list[0] = const_list->list_size;
	};

	/* say goodbye to the list that has been used to create the array.
	 */

	free_list(const_list);

	/* all initialised variables must be in the intialised space
	 * so, lets put this variable there.
	 */

	initial_space = realloc(initial_space,initial_size + data_size);
	memcpy(initial_space + initial_size,data_space,data_size);

	/* Now need to create a new symbol then allocate the data
	 * structure that has just been created to it.
	 */
	
	new_symbol = add_symbol(sym_name);

	new_symbol->type	= d_array;
	new_symbol->array_type	= type;
	new_symbol->size	= data_size;
	new_symbol->num_dim	= dim;
	new_symbol->dimensions	= dim_list;
	new_symbol->offset	= current_memory;
	new_symbol->data	= initial_size;
	new_symbol->access_type	= (a_const | access_type);


	/* string and reference fixups */
	if ( type == d_string || (a_type & a_ptr))
		add_range_fixup(initial_size,data_size/4);

	/* tidy and leave */
	if (level_at != 1)
		current_memory += data_size;
	else{
		new_symbol->offset       = initial_size;
		new_symbol->access_type |= a_global;
	}

	initial_size   += data_size;
	
	free(data_space);

	return new_symbol;
}
