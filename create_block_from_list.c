/* $Revision: 1.2 $
 *		     CREATE BLOCK FROM LIST
 *
 *	This function will create a single slab of memory from the
 *	list provided to it. It will also walk any list of lists
 *	that it finds and it will put these lists together as one
 *	single list. It will also build a LEVEL_LNK list that will
 *	hold the dimensions of the list that it has just walked. It
 *	will also check that the list it is currently walking matches
 *	the dimension from the LEVEL_LNK list it got. This is of
 *	course if it is not the end of the level link list. Also the
 *	type of the data items in the list must match the type passed
 *	in.
 *
 *		Copyright (c) 1999 Peter Antoine.
 */

#include	<malloc.h>
#include	"cople.h"

char*	create_block_from_list(char* data,LIST* list,LEVEL_LNK* level,int* data_size,DATA_TYPE type)
{
	int	end_chain = 0,count = 0,old_size;
	int	*int_pointer;
	char	*char_pointer;
	float	*float_pointer;
	long int	*string_pointer;
	LEVEL_LNK	*new_level;
	LIST_ENTRY	*here;
	DATA_TYPE	data_type;

	
	/* must check to see if this is the end of the chain
	 * if so then set the end_chain boolean to 1.
	 */
	if (level->count == 0)
		end_chain = 1;
	
	/* Now lets walk the current list. It might as well
	 * start from the start. 
	 */
	here = list->list;

	while(here != NULL)
	{
		old_size = *data_size;
	
		switch(here->type)
		{
		case c_list:	data_type = ((LIST*) here->data.list)->type;
			if (end_chain)
			{
				new_level       = calloc(1,sizeof(LEVEL_LNK));
				level->next     = new_level;
				new_level->prev = level;
			};

			data = create_block_from_list(data,(LIST*) here->data.list,new_level,data_size,type);
			break;
		
		case c_string:	*data_size     += DATA_TYPE_SIZE[c_string];
			data            = realloc(data,*data_size);
			char_pointer = (char*) (data + old_size);
			conv_int_to_bigendian(char_pointer,here->data.string);
			break;
		
		case c_int:	*data_size  += DATA_TYPE_SIZE[c_int];
			data         = realloc(data,*data_size);
			char_pointer = (char*) (data + old_size);
			conv_int_to_bigendian(char_pointer,here->data.integer);
			break;

		case c_char:	*data_size   += DATA_TYPE_SIZE[c_char];
			data          = realloc(data,*data_size);
			char_pointer  = (char*) (data + old_size);
			*char_pointer = here->data.character;
			break;
		
		case c_boolean:	*data_size  += DATA_TYPE_SIZE[c_boolean];
			data         = realloc(data,*data_size);
			int_pointer  = (int*) (data + old_size);
			*int_pointer = here->data.boolean;
			break;

		case c_real:	*data_size    += DATA_TYPE_SIZE[c_real];
			data           = realloc(data,*data_size);
			float_pointer  = (float*) (data + old_size);
			*float_pointer = here->data.flt_point;
			break;

		default:	yyerror("WARNING: using a variable type not implemented in lists!");
		};

		/* only need to check the lowest level as the type is passed all the
		 * way from the top. This must check to see if the elements in the
		 * array match the definition type.
		 */
		if (here->type != c_list && here->type != type)
			yyerror("ERROR: invalild element to a constant array - type mismatch");

		count++;
		here = here->next;
	};

	/* check that all is ok, set up the level link element
	 * if nessary, and check that the array is not empty.
	 * then go home...
	 */

	if (count == 0)
		yyerror("ERROR: not allowed to have a zero length constant array");

	if (end_chain)
		level->count = count;
	else{
		if (level->count != count)
			yyerror("ERROR: unbalanced constant array found.");
	};

	return data;
}
