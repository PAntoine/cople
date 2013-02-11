/* $Revision: 1.2 $
 *		ADD_TO_LIST
 *
 *	This function will add an element to the list that is passed
 *	into the function. The type of the list defines what kind of
 *	data is allowed to be passed in to the function. This does
 *	not check the validity of the types passed in, as this is 
 *	the job of the parser.
 *
 *	     Copyright (c) 1998-1999 P.Antoine.
 *
 *	NOTE: COBOS strings are [size]{data} format, and do not end
 *	with a 0 byte.
 */

#include	<malloc.h>
#include	"cople.h"

void	add_to_list( LIST *list, CONSTANT *data)
{
	int	count = 0;
	LIST_ENTRY	*temp;

	if ((temp = calloc(1,sizeof(LIST_ENTRY))))
	{
		/* set the data type on the new entry block and add the block to 
		 * the list chain. If the list chain is enpty then add the block to the
		 * start of the list (list->list).
		 */

		temp->type = data->type;
		
		if (list->list == NULL){
			list->list_size = 1;
			list->list = temp;
			list->last_entry = temp;
		} else {
			list->last_entry->next = temp;
			list->last_entry = temp;	/* move the last entry pointer */
			list->list_size++;
		};

		/* copy the data to the new list entry item.
		 */
	
		switch(data->type)
		{
		case c_reference:
		case c_null:	temp->data.symbol = data->data.symbol;
			list->data_size += DATA_TYPE_SIZE[c_null];
			break;

		case c_char:	temp->data.character = data->data.character;
			list->data_size += DATA_TYPE_SIZE[c_char];
			break;

		case c_boolean:	temp->data.boolean = data->data.boolean;
			list->data_size += DATA_TYPE_SIZE[c_boolean];
			break;

		case c_real:	temp->data.flt_point = data->data.flt_point;
			list->data_size += DATA_TYPE_SIZE[c_real];
			break;

		case c_int:	temp->data.integer = data->data.integer;
			list->data_size += DATA_TYPE_SIZE[c_int];
			break;

		case c_string:	temp->data.string =  data->data.string;
			list->data_size += DATA_TYPE_SIZE[c_string];
			break;
		
		case c_list:	temp->data.list = data->data.list;	/* whatever the list size is */
			list->data_size += data->data.list->data_size;
			break;

		case c_lookup:	temp->data.lookup.clause = data->data.lookup.clause;
			temp->data.lookup.name   = data->data.lookup.name;
			break;

		case c_arg: temp->data.argument.type   = data->data.argument.type;
			temp->data.argument.name   = data->data.argument.name;
			temp->data.argument.value  = data->data.argument.value;
			temp->data.argument.a_type = data->data.argument.a_type;
			break;

		case c_expr:	temp->data.expr = data->data.expr;
			break;
		};
	}else
		printf("ERROR: cant allocate memory for list element\n");

	free_const(data);
}
